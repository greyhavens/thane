/* -*- Mode: C++; c-basic-offset: 4; indent-tabs-mode: t; tab-width: 4 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is [Open Source Virtual Machine.].
 *
 * The Initial Developer of the Original Code is
 * Adobe System Incorporated.
 * Portions created by the Initial Developer are Copyright (C) 2004-2006
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Adobe AS3 Team
 *   leon.sha@sun.com
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#include "MMgc.h"
#include "StaticAssert.h"

#ifdef MMGC_HAVE_STDARG_H
// for va_list
#include <stdarg.h>
#endif

#ifdef HAVE_STDARG
// for va_list
#include <stdarg.h>
#endif

// get alloca for CleanStack
#ifdef WIN32
	#include <malloc.h>
#endif

#ifdef MMGC_HAVE_ALLOCA_H
	#include <alloca.h>
#endif

#ifdef UNIX
	#include <sys/time.h>
#endif // UNIX

#ifdef MMGC_HAVE_PTHREAD_H
#include <pthread.h>
#endif // HAVE_PTHREAD_H

#ifdef MMGC_HAVE_PTHREAD_NP_H
#include <pthread_np.h>
#endif // HAVE_PTHREAD_NP_H

#ifdef AVMPLUS_SAMPLER
 //sampling support
#include "avmplus.h"
#else
#define SAMPLE_FRAME(_x, _s) 
#define SAMPLE_CHECK() 
#endif

#ifdef SOLARIS
#include <ucontext.h>
#include <sys/frame.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stack.h>
extern "C" greg_t _getsp(void);
#endif

#ifdef MMGC_SPARC
extern "C" {
void sparc_clean_windows()
{
	asm(
		"  cmp %i7, %o7 \n"
		"  be 2f\n"
		"  clr %o0\n"
		"  cmp %o7, 0\n"
		"  be 1f\n"
		"  clr %o1\n"
		"  call sparc_clean_windows\n"
		"  nop\n"
		"1:clr %o2\n"
		"  clr %o3\n"
		"  clr %o4\n"
		"  clr %o5\n"
		"  clr %o7\n"
		"  clr %l0\n"
		"  clr %l1\n"
		"  clr %l2\n"
		"  clr %l3\n"
		"  clr %l4\n"
		"  clr %l5\n"
		"  clr %l6\n"
		"  clr %l7\n"
		"2:nop\n"
		);
}
}
#endif


// Werner mode is a back pointer chain facility for Relase mode
//#define WERNER_MODE

#ifdef WERNER_MODE
	#include <typeinfo>
	#include <stdio.h>
	void *shouldGo;
	char statusBuffer[1024];
#endif

namespace MMgc
{
	// how many objects trigger a reap, should be high
	// enough that the stack scan time is noise but low enough
	// so that objects to away in a timely manner
	const int ZCT::ZCT_REAP_THRESHOLD = 512;

	// size of table in pages
	const int ZCT::ZCT_START_SIZE = 1;

#ifdef MMGC_64BIT
	const uintptr_t MAX_UINTPTR = 0xFFFFFFFFFFFFFFFF;
#else
	const uintptr_t MAX_UINTPTR = 0xFFFFFFFF;
#endif	

#ifdef MMGC_MEMORY_PROFILER
	// get detailed info on each size class allocators
	const bool dumpSizeClassState = false;
#endif

	/**
	 * Free Space Divisor.  This value may be tuned for optimum
 	 * performance.  The FSD is based on the Boehm collector.
	 */
	const static int kFreeSpaceDivisor = 4;

	// max amount we'll grow by when 
	const static int kMaxIncrement = 256;

	/**
	 * delay between GC incremental marks
	 */
	const static uint64_t kIncrementalMarkDelayTicks = int(10 * VMPI_getPerformanceFrequency() / 1000);

	const static uint64_t kMarkSweepBurstTicks = 1515909; // 200 ms on a 2ghz machine

	// Size classes for our GC.  From 8 to 128, size classes are spaced
	// evenly 8 bytes apart.  The finest granularity we can achieve is
	// 8 bytes, as pointers must be 8-byte aligned thanks to our use
	// of the bottom 3 bits of 32-bit atoms for Special Purposes.
	// Above that, the size classes have been chosen to maximize the
	// number of items that fit in a 4096-byte block, given our block
	// header information.
	const int16_t GC::kSizeClasses[kNumSizeClasses] = {
		8, 16, 24, 32, 40, 48, 56, 64, 72, 80, //0-9
		88, 96, 104, 112, 120, 128,	144, 160, 168, 176,  //10-19
		184, 192, 200, 216, 224, 240, 256, 280, 296, 328, //20-29
		352, 392, 432, 488, 560, 656, 784, 984, 1312, 1968 //30-39
	};

	/* kSizeClassIndex[] generated with this code:
	    kSizeClassIndex[0] = 0;
	    for (var i:int = 1; i < kNumSizeClasses; i++)
			for (var j:int = (kSizeClasses[i-1]>>3), n=(kSizeClasses[i]>>3); j < n; j++)
				kSizeClassIndex[j] = i;
	*/

	// index'd by size>>3 - 1
	const uint8_t GC::kSizeClassIndex[246] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		10, 11, 12, 13, 14, 15, 16, 16, 17, 17,
		18, 19, 20, 21, 22, 23, 23, 24, 25, 25,
		26, 26, 27, 27, 27, 28, 28, 29, 29, 29,
		29, 30, 30, 30, 31, 31, 31, 31, 31, 32,
		32, 32, 32, 32, 33, 33, 33, 33, 33, 33,
		33, 34, 34, 34, 34, 34, 34, 34, 34, 34,
		35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
		35, 35, 36, 36, 36, 36, 36, 36, 36, 36,
		36, 36, 36, 36, 36, 36, 36, 36, 37, 37,
		37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
		37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
		37, 37, 37, 38, 38, 38, 38, 38, 38, 38,
		38, 38, 38, 38, 38, 38, 38, 38, 38, 38,
		38, 38, 38, 38, 38, 38, 38, 38, 38, 38,
		38, 38, 38, 38, 38, 38, 38, 38, 38, 38,
		38, 38, 38, 38, 39, 39, 39, 39, 39, 39,
		39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
		39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
		39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
		39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
		39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
		39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
		39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
		39, 39, 39, 39, 39, 39	
	};
	const size_t kLargestAlloc = 1968;

#ifdef MMGC_THREADSAFE
#define USING_CALLBACK_LIST(gc)  GCAcquireSpinlock _cblock((gc)->m_callbackListLock)
#define USING_PAGE_MAP()         GCAcquireSpinlock _pmlock(pageMapLock)
#else
#define USING_CALLBACK_LIST(gc)  ((gc)->CheckThread())
#define USING_PAGE_MAP()         ((void) 0)
#endif

	GC::GC(GCHeap *gcheap)
		:
		greedy(false),
		nogc(false),
		findUnmarkedPointers(false),
		validateDefRef(false),
		keepDRCHistory(false),
		collectThreshold(256),
		dontAddToZCTDuringCollection(false),
		incrementalValidation(false),
#ifdef _DEBUG
		// check for missing write barriers at every Alloc
		incrementalValidationPedantic(false),
#endif
		incremental(true),
#ifdef MMGC_RCROOT_SUPPORT
		rcRootSegments(NULL),
#endif
		t0(VMPI_getPerformanceCounter()),
		bytesMarked(0),
		markTicks(0),
		lastStartMarkIncrementCount(0),
		markIncrements(0),
		marks(0),
		sweeps(0),
		numObjects(0),
		sweepStart(0),
		hitZeroObjects(false),
		emptyWeakRef(0),
		emptyWeakRefRoot(0),
		destroying(false),
		heapSizeAtLastAlloc(gcheap->GetTotalHeapSize()),
		stackCleaned(true),
		rememberedStackTop(0),
		stackEnter(0),
#ifndef MMGC_THREADSAFE
		disableThreadCheck(false),
#endif
		marking(false),
		lastMarkTicks(0),
		lastSweepTicks(0),
		memStart(MAX_UINTPTR),
		memEnd(0),
		totalGCPages(0),
		heap(gcheap),
		allocsSinceCollect(0),
		collecting(false),
		finalizedValue(true),
		smallEmptyPageList(NULL),
		largeEmptyPageList(NULL),
		m_roots(0),
	    m_callbacks(0),
		zct()
		// Expand, don't collect, until we hit this threshold
#if MMGC_THREADSAFE
		, m_exclusiveGCThread(NULL)
		, m_gcRunning(false)
		, m_condDone(m_lock)
		, m_requestCount(0)
		, m_condNoRequests(m_lock)
#endif
	{
		// sanity check for all our types
		MMGC_STATIC_ASSERT(sizeof(int8_t) == 1);
		MMGC_STATIC_ASSERT(sizeof(uint8_t) == 1);		
		MMGC_STATIC_ASSERT(sizeof(int16_t) == 2);
		MMGC_STATIC_ASSERT(sizeof(uint16_t) == 2);
		MMGC_STATIC_ASSERT(sizeof(int32_t) == 4);
		MMGC_STATIC_ASSERT(sizeof(uint32_t) == 4);
		MMGC_STATIC_ASSERT(sizeof(int64_t) == 8);
		MMGC_STATIC_ASSERT(sizeof(uint64_t) == 8);
		MMGC_STATIC_ASSERT(sizeof(intptr_t) == sizeof(void *));
		MMGC_STATIC_ASSERT(sizeof(uintptr_t) == sizeof(void *));
		#ifdef MMGC_64BIT
		MMGC_STATIC_ASSERT(sizeof(intptr_t) == 8);
		MMGC_STATIC_ASSERT(sizeof(uintptr_t) == 8);	
		#else	
		MMGC_STATIC_ASSERT(sizeof(intptr_t) == 4);
		MMGC_STATIC_ASSERT(sizeof(uintptr_t) == 4);	
		#endif		
	
		zct.SetGC(this);

		// Create all the allocators up front (not lazy)
		// so that we don't have to check the pointers for
		// NULL on every allocation.
		for (int i=0; i<kNumSizeClasses; i++) {			
			containsPointersAllocs[i] = new GCAlloc(this, kSizeClasses[i], true, false, i);
			containsPointersRCAllocs[i] = new GCAlloc(this, kSizeClasses[i], true, true, i);
			noPointersAllocs[i] = new GCAlloc(this, kSizeClasses[i], false, false, i);
		}

	#ifdef MMGC_THREADSAFE
		pageMapLock = VMPI_lockCreate();
		GCAssert(pageMapLock != NULL);
		m_callbackListLock = VMPI_lockCreate();
		GCAssert(m_callbackListLock != NULL);
	#endif //MMGC_THREADSAFE
	#ifdef MMGC_LOCKING
		m_rootListLock = VMPI_lockCreate();
		GCAssert(m_rootListLock != NULL);
	#endif //MMGC_LOCKING
		
		largeAlloc = new GCLargeAlloc(this);

		pageMap = (unsigned char*) heapAlloc(1);

		VMPI_memset(m_bitsFreelists, 0, sizeof(uint32_t*) * kNumSizeClasses);
		m_bitsNext = (uint32_t*)heapAlloc(1);

		// precondition for emptyPageList 
		GCAssert(offsetof(GCLargeAlloc::LargeBlock, next) == offsetof(GCAlloc::GCBlock, next));

		
		for(int i=0; i<GCV_COUNT; i++)
		{
			SetGCContextVariable(i, NULL);
		}

#ifdef _DEBUG
#ifdef WIN32
		m_gcThread = GetCurrentThreadId();
#endif
#endif

		// keep GC::Size honest
		GCAssert(offsetof(GCLargeAlloc::LargeBlock, usableSize) == offsetof(GCAlloc::GCBlock, size));

		emptyWeakRefRoot = new GCRoot(this, &this->emptyWeakRef, sizeof(this->emptyWeakRef)),
		emptyWeakRef = new (this) GCWeakRef(NULL);

		gcheap->AddGC(this);
 		allocaInit();
	}

	GC::~GC()
	{
 		allocaShutdown();
		heap->RemoveGC(this);

		// Force all objects to be destroyed
		destroying = true;
		ClearMarks();
		ForceSweep();

		// Go through m_bitsFreelist and collect list of all pointers
		// that are on page boundaries into new list, pageList
		void **pageList = NULL;
		for(int i=0, n=kNumSizeClasses; i<n; i++) {
			uint32_t* bitsFreelist = m_bitsFreelists[i];
			while(bitsFreelist) {
				uint32_t *next = *(uint32_t**)bitsFreelist;
				if(((uintptr_t)bitsFreelist & 0xfff) == 0) {
					*((void**)bitsFreelist) = pageList;
					pageList = (void**)bitsFreelist;
				}
				bitsFreelist = next;
			} 
		}
		
		// Go through page list and free all pages on it
		while (pageList) {
			void **next = (void**) *pageList;
			heap->Free((void*)pageList);
			pageList = next;
		}

		for (int i=0; i < kNumSizeClasses; i++) {
			delete containsPointersAllocs[i];
			delete containsPointersRCAllocs[i];
			delete noPointersAllocs[i];
		}

		if (largeAlloc) {
			delete largeAlloc;
		}

		// dtors for each GCAlloc will use this
		pageList = NULL;

		heap->Free(pageMap);

#ifndef MMGC_THREADSAFE
		CheckThread();
#endif
		delete emptyWeakRefRoot;
		GCAssert(!m_roots);
		GCAssert(!m_callbacks);

		// apparently the player can't be made to clean up so keep it from crashing at least
		while(m_roots) {
			m_roots->Destroy();
		}

		while(m_callbacks) {
			m_callbacks->Destroy();			
		}

	#ifdef MMGC_LOCKING
		VMPI_lockDestroy(m_rootListLock);
	#endif //MMGC_LOCKING
	#ifdef MMGC_THREADSAFE
		VMPI_lockDestroy(m_callbackListLock);
		VMPI_lockDestroy(pageMapLock);
	#endif //MMGC_THREADSAFE
	}

	void GC::Collect()
	{
		CollectWithBookkeeping(false, false);
	}

	void GC::CollectWithBookkeeping(bool callerHoldsLock,
									bool callerHasActiveRequest)
	{
#ifdef MMGC_THREADSAFE
		GCAssert(callerHoldsLock == m_lock.IsHeld());
#ifdef _DEBUG
		GCAssert(callerHasActiveRequest == GCThread::GetCurrentThread()->IsInActiveRequest());
#endif
#else
		(void) callerHoldsLock;
		(void) callerHasActiveRequest;
		CheckThread();
#endif

#ifdef MMGC_THREADSAFE
		if (!callerHoldsLock)
			m_lock.Acquire();

		if (nogc || m_gcRunning || zct.reaping) {
			if (!callerHoldsLock)
				m_lock.Release();
			return;
		}
#else
		if (nogc || collecting || zct.reaping) {
			return;
		}
#endif

#ifdef MMGC_THREADSAFE
		GCThread *thisThread = GCThread::GetCurrentThread();
		if (m_exclusiveGCThread != NULL) {
			// Someone is already collecting or waiting to collect.
			//
			// If it's the current thread, then we're being called
			// recursively.  This maybe shouldn't happen, but Collect can be
			// called indirectly from a finalizer.  Don't recurse, just ignore
			// it.
			//
			// If it's some other thread, wait for that thread to finish.
			//
			if (m_exclusiveGCThread != thisThread) {
				GCRoot *stackRoot;
				void *stack;
				size_t stackSize;

				// Call this macro here, not under "if
				// (callerHasActiveRequest)", because it introduces local
				// variables that must survive for the whole lifetime of
				// stackRoot.
				MMGC_GET_STACK_EXTENTS(this, stack, stackSize);

				// Before waiting, this thread must suspend any active
				// requests.  This avoids a deadlock where m_exclusiveGCThread
				// is waiting for m_requestCount to go to zero while we're
				// waiting for m_exclusiveGCThread to be done.
				//
				// When we do this, we root the calling thread's stack; this
				// way each collection scans the stack of every thread that is
				// in a suspended request.
				//
				if (callerHasActiveRequest) {
					stackRoot = new GCRoot(this, stack, stackSize);

					OnLeaveRequestAlreadyLocked();
				}

				while (m_exclusiveGCThread != NULL)
					m_condDone.Wait();

				if (callerHasActiveRequest) {
					// Resume previously suspended request.
					delete stackRoot;
					OnEnterRequestAlreadyLocked();
				}
			}

			if (!callerHoldsLock)
				m_lock.Release();
			return;
		}

		// If we get here, this thread will collect.
		m_exclusiveGCThread = thisThread;
		if (callerHasActiveRequest) {
			// We must suspend any active requests on this thread.
			OnLeaveRequestAlreadyLocked();
		}
		// Wait for other threads to suspend or end their active requests,
		// if any.
		while (m_requestCount > 0)
			m_condNoRequests.Wait();

		// These should not have changed while we were waiting, even though we
		// released the lock.
		GCAssert(m_requestCount == 0);
		GCAssert(m_exclusiveGCThread == thisThread);

		// This thread has acquired exclusiveGC.
		m_gcRunning = true;
#endif

		{
			USING_CALLBACK_LIST(this);
			for (GCCallback *cb = m_callbacks; cb; cb = cb->nextCB)
				cb->enterExclusiveGC();
		}

#ifdef MMGC_THREADSAFE
		m_lock.Release();
#endif

		{
			USING_CALLBACK_LIST(this);
			for (GCCallback *cb = m_callbacks; cb; cb = cb->nextCB)
				cb->enterExclusiveGCNoLock();
		}

		CollectImpl();

#ifdef MMGC_THREADSAFE
		m_lock.Acquire();

		// These should not have changed during collection,
		// even though we released the lock.
		GCAssert(m_requestCount == 0);
		GCAssert(m_exclusiveGCThread == thisThread);
#endif

		{
			USING_CALLBACK_LIST(this);
			for (GCCallback *cb = m_callbacks; cb; cb = cb->nextCB)
				cb->leaveExclusiveGC();
		}

#ifdef MMGC_THREADSAFE
		// This thread is relinquishing exclusiveGC.
		m_gcRunning = false;

		m_exclusiveGCThread = NULL;
		if (callerHasActiveRequest)
			OnEnterRequestAlreadyLocked();

		m_condDone.NotifyAll();

		if (!callerHoldsLock)
			m_lock.Release();
#endif
	}

	void GC::CollectImpl()
	{
#ifndef MMGC_THREADSAFE
		ReapZCT();
#endif

		if(!marking)
			StartIncrementalMark();
		if(marking)
			FinishIncrementalMark();

#ifdef _DEBUG
		// Dumping the stack trace at GC time can be very helpful with stack walk bugs
		// where something was created, stored away in untraced, unmanaged memory and not 
		// reachable by the conservative stack walk
		//DumpStackTrace();
		FindUnmarkedPointers();
#endif
	}

#ifdef _DEBUG
	void GC::Trace(const void *stackStart/*=NULL*/, uint32_t stackSize/*=0*/)
	{
		MMGC_ASSERT_EXCLUSIVE_GC(this);

		SAMPLE_FRAME("[mark]", core());

		// Clear all mark bits.
		ClearMarks();

		SAMPLE_CHECK();

		GCStack<GCWorkItem> work;
		{
			MMGC_LOCK(m_rootListLock);
			GCRoot *r = m_roots;
			while(r) {
				GCWorkItem item = r->GetWorkItem();
				MarkItem(item, work);
				r = r->next;
			}
		}

		SAMPLE_CHECK();

		if(stackStart == NULL) {
			MarkQueueAndStack(work);
		} else {
			GCWorkItem item(stackStart, stackSize, false);
			PushWorkItem(work, item);
			Mark(work);
		}
		
		SAMPLE_CHECK();

		ClearMarks();
	}
#endif

#ifdef MMGC_RCROOT_SUPPORT

	GC::RCRootSegment::RCRootSegment(GC* gc, void* mem, size_t size) 
		: GCRoot(gc, mem, size)
		, mem(mem)
		, size(size)
		, prev(NULL)
		, next(NULL)
	{}

	void* GC::AllocRCRoot(size_t size)
	{
		const int hdr_size = (sizeof(void*) + 7) & ~7;
		char* block = new char[size + hdr_size];
		// FIXME: should allocate with zeroing, probably.
		VMPI_memset(block, 0, size + hdr_size);
		void* mem = (void*)(block + hdr_size);
		RCRootSegment *segment = new RCRootSegment(this, mem, size);
		*(uintptr_t*)block = (uintptr_t)segment;
		segment->next = rcRootSegments;
		if (rcRootSegments)
			rcRootSegments->prev = segment;
		rcRootSegments = segment;
		return mem;
	}
	
	void GC::FreeRCRoot(void* mem)
	{
		const int hdr_size = (sizeof(void*) + 7) & ~7;
		char* block = (char*)mem - hdr_size;
		RCRootSegment* segment = (RCRootSegment*)*(uintptr_t*)block;
		if (segment->next != NULL)
			segment->next->prev = segment->prev;
		if (segment->prev != NULL)
			segment->prev->next = segment->next;
		else
			rcRootSegments = segment->next;
		delete segment;
		delete block;
	}

#endif // MMGC_RCROOT_SUPPORT

	void *GC::Alloc(size_t size, int flags/*0*/)
	{
#ifdef MMGC_THREADSAFE
		GCAutoLock _lock(m_lock);
		GCAssert(!m_gcRunning);
#else
		CheckThread();
#endif
		return AllocAlreadyLocked(size, flags);
	}

	void *GC::AllocAlreadyLocked(size_t size, int flags/*0*/)
	{
#ifdef AVMPLUS_SAMPLER
		avmplus::AvmCore *core = (avmplus::AvmCore*)GetGCContextVariable(GCV_AVMCORE);
		if(core)
			core->sampleCheck();
#endif
		GCAssertMsg(size > 0, "cannot allocate a 0 sized block\n");

#ifdef _DEBUG
		GCAssert(size + 7 > size);
		if (GC::greedy) {
			CollectWithBookkeeping(true, true);
		}
		// always be marking in pedantic mode
		if(incrementalValidationPedantic) {
			if(!marking)
				StartIncrementalMark();
		}
#endif

		// overflow detection
		if(size+7 < size)
			return NULL;

		size = (size+7)&~7; // round up to multiple of 8

		size += DebugSize();

		GCAssertMsg(size > 0, "debug overflow, adding space for Debug stuff overflowed size_t\n");
	
		GCAlloc **allocs = noPointersAllocs;

		if(flags & kRCObject) {
			allocs = containsPointersRCAllocs;
		} else if(flags & kContainsPointers) {
			allocs = containsPointersAllocs;
		}

		void *item;
		// Buckets up to 128 are spaced evenly at 8 bytes.
		if (size <= 128) {
			GCAlloc *b = (GCAlloc*)allocs[(size >> 3) - 1];
			GCAssert(size <= b->GetItemSize());
			item = b->Alloc(size, flags);
		} else if (size <= kLargestAlloc) {				
			// This is the fast lookup table implementation to
			// find the right allocator.
			unsigned index = kSizeClassIndex[(size>>3)-1];

			// assert that I fit 
			GCAssert(size <= allocs[index]->GetItemSize());

			// assert that I don't fit (makes sure we don't waste space)
			GCAssert(size > allocs[index-1]->GetItemSize());

			item = allocs[index]->Alloc(size, flags);
		} else {
			item = largeAlloc->Alloc(size, flags);
		}

		item = GetUserPointer(item);

		if(heap->HooksEnabled()) {
			heap->AllocHook(item, Size(item));
		}

#ifdef _DEBUG
		bool shouldZero = (flags & kZero) || incrementalValidationPedantic;
        
		// in debug mode memory is poisoned so we have to clear it here
		// in release builds memory is zero'd to start and on free/sweep
		// in pedantic mode uninitialized data can trip the write barrier 
		// detector, only do it for pedantic because otherwise we want the
		// mutator to get the poisoned data so it crashes if it relies on 
		// uninitialized values
		if((item) && (shouldZero)) {
			VMPI_memset(item, 0, Size(item));
		}
#endif

		return item;
	}

	void GC::MaybeGC(bool callerHasActiveRequest)
	{
		if (greedy) {
			CollectWithBookkeeping(false, callerHasActiveRequest);
		} else if (marking) {
			IncrementalMark();
		} else {
#ifdef MMGC_THREADSAFE
			GCAutoLock _lock(m_lock);
#endif

			// Burst logic to prevent collections from happening back to back.
			uint64_t now = VMPI_getPerformanceFrequency();
			if (now - lastSweepTicks <= kMarkSweepBurstTicks)
				return;

			// Definitely start GC if the heap expanded due to FixedMalloc
			// allocations.  The same heuristic applies to incremental and
			// non-incremental.
			bool force = (heapSizeAtLastAlloc > collectThreshold &&
						  heapSizeAtLastAlloc < heap->GetTotalHeapSize());

			if (incremental) {
				if (force || (totalGCPages > collectThreshold &&
							  allocsSinceCollect * kFreeSpaceDivisor >= totalGCPages)) {
					StartIncrementalMark();
				}
			} else {
				// Collect only if the heap is completely full (a conservative
				// heuristic).
				if (force || heap->GetFreeHeapSize() == 0)
					CollectWithBookkeeping(true, callerHasActiveRequest);
			}
		}
	}

	void *GC::Calloc(size_t num, size_t elsize, int flags)
	{
		uint64_t size = (uint64_t)num * (uint64_t)elsize;
		if(size > 0xfffffff0) 
		{
			GCAssertMsg(false, "Attempted allocation overflows size_t\n");
			return NULL;
		}
		return Alloc(num * elsize, flags);
	}

	void GC::Free(const void *item)
	{
#ifdef MMGC_THREADSAFE
		GCAutoLock _lock(m_lock);
		GCAssert(!m_gcRunning);
#else
		CheckThread();
#endif

		if(item == NULL) {
			return;
		}

		bool isLarge;

		// we can't allow free'ing something during Sweeping, otherwise alloc counters
		// get decremented twice and destructors will be called twice.
		if(collecting) {
			goto bail;
		}

		isLarge = GCLargeAlloc::IsLargeBlock(GetRealPointer(item));

		if (marking) {
			// if its on the work queue don't delete it, if this item is
			// really garbage we're just leaving it for the next sweep
			if(IsQueued(item)) 
				goto bail;
		}

#ifdef _DEBUG

		// RCObject have constract that they must clean themselves, since they 
		// have to scan themselves to decrement other RCObjects they might as well
		// clean themselves too, better than suffering a memset later
		if(isLarge ? GCLargeAlloc::IsRCObject(item) : GCAlloc::IsRCObject(item))
		{
			 RCObjectZeroCheck((RCObject*)item);
		}
#endif

		if(heap->HooksEnabled()) {
			heap->FinalizeHook(item, GC::Size(item));
			heap->FreeHook(item, GC::Size(item), 0xca);
		}
	
		if (isLarge) {
			largeAlloc->Free(GetRealPointer(item));
		} else {
			GCAlloc::Free(GetRealPointer(item));
		}
		return;

bail:

		// this presumes we got here via delete, maybe we should have
		// delete call something other than the public Free to distinguish
		if(IsFinalized(item))
			ClearFinalized(item);
		if(HasWeakRef(item))
			ClearWeakRef(item);
	}

	/*static*/ int GC::GetMark(const void *item)
	{
		item = GetRealPointer(item);
		if (GCLargeAlloc::IsLargeBlock(item)) {
			return GCLargeAlloc::GetMark(item);
		} else {
			return GCAlloc::GetMark(item);
		}
	}

	void GC::ClearMarks()
	{
		MMGC_ASSERT_EXCLUSIVE_GC(this);

		for (int i=0; i < kNumSizeClasses; i++) {
			containsPointersRCAllocs[i]->ClearMarks();
			containsPointersAllocs[i]->ClearMarks();
			noPointersAllocs[i]->ClearMarks();
		}
		largeAlloc->ClearMarks();
	}

	void GC::Finalize()
	{
		MMGC_ASSERT_EXCLUSIVE_GC(this);

		for(int i=0; i < kNumSizeClasses; i++) {
			containsPointersRCAllocs[i]->Finalize();
			containsPointersAllocs[i]->Finalize();
			noPointersAllocs[i]->Finalize();
		}
		largeAlloc->Finalize();
		finalizedValue = !finalizedValue;

		
		for(int i=0; i < kNumSizeClasses; i++) {
			containsPointersRCAllocs[i]->m_finalized = false;
			containsPointersAllocs[i]->m_finalized = false;
			noPointersAllocs[i]->m_finalized = false;
		}
	}

	void GC::Sweep(bool force)
	{	
		MMGC_ASSERT_EXCLUSIVE_GC(this);

		// collecting must be true because it indicates allocations should
		// start out marked, we can't rely on write barriers below since 
		// presweep could write a new GC object to a root
		collecting = true;

		SAMPLE_FRAME("[sweep]", core());
		sweeps++;

		size_t heapSize = heap->GetUsedHeapSize();

#ifdef MMGC_MEMORY_PROFILER
		if(heap->Config().autoGCStats) {
			GCLog("Before sweep memory info:\n");
			DumpMemoryInfo();
		}
#endif
		

		// invoke presweep on all callbacks
		{
			USING_CALLBACK_LIST(this);
			GCCallback *cb = m_callbacks;
			while(cb) {
				cb->presweep();
				cb = cb->nextCB;
			}
		}

		SAMPLE_CHECK();

		// if force is true we're being called from ~GC and this isn't necessary
		if(!force) {
			// we just executed mutator code which could have fired some WB's
			Mark(m_incrementalWork);
		}

		Finalize();

		// if force is true we're being called from ~GC and this isn't necessary
		if(!force) {
			// we just executed mutator code which could have fired some WB's
			Mark(m_incrementalWork);
		}
	
		SAMPLE_CHECK();
		// ISSUE: this could be done lazily at the expense other GC's potentially expanding
		// unnecessarily, not sure its worth it as this should be pretty fast
		GCAlloc::GCBlock *b = smallEmptyPageList;
		while(b) {
			GCAlloc::GCBlock *next = b->next;
#ifdef _DEBUG
			b->alloc->SweepGuts(b);
#endif
			b->alloc->FreeChunk(b);
			b = next;
		}
		smallEmptyPageList = NULL;

		SAMPLE_CHECK();

		GCLargeAlloc::LargeBlock *lb = largeEmptyPageList;		
		while(lb) {
			GCLargeAlloc::LargeBlock *next = lb->next;
			if(heap->HooksEnabled())
				heap->FreeHook(GetUserPointer(lb+1), lb->usableSize - DebugSize(), 0xba);
			FreeBlock(lb, lb->GetNumBlocks());
			lb = next;
		}
		largeEmptyPageList = NULL;
		
		// we potentially freed a lot of memory, tell heap to regulate
		heap->Decommit();

		SAMPLE_CHECK();

#ifdef MMGC_MEMORY_PROFILER
		if(heap->Config().autoGCStats) {			
			GCLog("After sweep memory info:\n");
			DumpMemoryInfo();
		}
#endif

		// don't want postsweep to fire WB's
		marking = false;
		collecting = false;

		// invoke postsweep callback
		{
			USING_CALLBACK_LIST(this);
			GCCallback *cb = m_callbacks;
			while(cb) {
				cb->postsweep();
				cb = cb->nextCB;
			}
		}

		SAMPLE_CHECK();

		allocsSinceCollect = 0;
		lastSweepTicks = VMPI_getPerformanceCounter();

		if(heap->Config().gcstats) {
			int sweepResults = 0;
			GCAlloc::GCBlock *bb = smallEmptyPageList;
			while(bb) {
				sweepResults++;
				bb = bb->next;
			}
			
			GCLargeAlloc::LargeBlock *lbb = largeEmptyPageList;
			while(lbb) {
				sweepResults += lbb->GetNumBlocks();
				lbb = lbb->next;
			}
			// include large pages given back
			sweepResults += int(heapSize - heap->GetUsedHeapSize());
			double millis = duration(sweepStart);
			gclog("[mem] sweep(%d) reclaimed %d whole pages (%d kb) in %.2f millis (%.4f s)\n", 
				sweeps, sweepResults, sweepResults*GCHeap::kBlockSize>>10, millis,
				duration(t0)/1000);
		}
	}

	void* GC::AllocBlock(int size, int pageType, bool zero)
	{
		MMGC_ASSERT_GC_LOCK(this);
		GCAssert(size > 0);
	
		// perform gc if heap expanded due to fixed memory allocations
		// utilize burst logic to prevent this from happening back to back
		// this logic is here to apply to incremental and non-incremental
		uint64_t now = VMPI_getPerformanceCounter();
		if(!marking && !collecting &&
			heapSizeAtLastAlloc > collectThreshold &&
			now - lastSweepTicks > kMarkSweepBurstTicks && 
			heapSizeAtLastAlloc < heap->GetTotalHeapSize()) 
		{
			if(incremental && !nogc)
				StartIncrementalMark();
			else
				CollectWithBookkeeping(true, true);
		}

		void *item;

		if(incremental && !nogc)
			item = AllocBlockIncremental(size, zero);
		else
			item = AllocBlockNonIncremental(size, zero);

		if(!item) {				
			item = heap->Alloc(size, true, zero);
		}

		GCAssert(item != NULL);
		if (item != NULL)
		{
			allocsSinceCollect += size;

			// mark GC pages in page map, small pages get marked one,
			// the first page of large pages is 3 and the rest are 2
			MarkGCPages(item, 1, pageType);
			if(pageType == kGCLargeAllocPageFirst) {
				MarkGCPages((char*)item+GCHeap::kBlockSize, size - 1, kGCLargeAllocPageRest);
			}
			totalGCPages += size;
		}

		// do this after any heap expansions from GC
		heapSizeAtLastAlloc = heap->GetTotalHeapSize();

		return item;
	}

	void* GC::AllocBlockIncremental(int size, bool zero)
	{
		MMGC_ASSERT_GC_LOCK(this);

		if(!collecting || incrementalValidation) {
			uint64_t now = VMPI_getPerformanceCounter();
			if (marking) {		
				if(now - lastMarkTicks > kIncrementalMarkDelayTicks) {
					IncrementalMark();
				}
			} else if (incrementalValidation ||
				(totalGCPages > collectThreshold &&
				allocsSinceCollect * kFreeSpaceDivisor >= totalGCPages &&
				// burst detection
				(now - lastSweepTicks > kMarkSweepBurstTicks))) {
					StartIncrementalMark();
			}
		}

		void *item = heap->Alloc(size, false, zero);
		if(!item && !collecting) {
			if(marking) {
				GCAssert(!nogc);
				FinishIncrementalMark();
				item = heap->Alloc(size, false, zero);
			}
		}
		return item;
	}

	void* GC::AllocBlockNonIncremental(int size, bool zero)
	{
		MMGC_ASSERT_GC_LOCK(this);

		void *item = heap->Alloc(size, false, zero);
		if (!item) {
			if (heap->GetTotalHeapSize() >= collectThreshold &&
				allocsSinceCollect >= totalGCPages / kFreeSpaceDivisor) 
			{
				CollectWithBookkeeping(true, true);
				item = heap->Alloc(size, false, zero);
			}
		}
		return item;
	}

	void GC::FreeBlock(void *ptr, uint32_t size)
	{
#ifdef MMGC_THREADSAFE
		GCAssert(m_lock.IsHeld() || destroying
				 || (m_gcRunning
					 && m_exclusiveGCThread == GCThread::GetCurrentThread()));
#endif
		if(!collecting) {
			allocsSinceCollect -= size;
		}
		totalGCPages -= size;
		heap->Free(ptr);
		UnmarkGCPages(ptr, size);
	}

	int GC::GetPageMapValueAlreadyLocked(uintptr_t addr) const
	{
		uintptr_t index = (addr-memStart) >> 12;

#ifdef MMGC_64BIT
		GCAssert((index >> 2) < uintptr_t(64*65536) * uintptr_t(GCHeap::kBlockSize));
#else
		GCAssert(index >> 2 < 64 * GCHeap::kBlockSize);
#endif
		// shift amount to determine position in the byte (times 2 b/c 2 bits per page)
		uint32_t shiftAmount = (index&0x3) * 2;
		// 3 ... is mask for 2 bits, shifted to the left by shiftAmount
		// finally shift back by shift amount to get the value 0, 1 or 3
		//return (pageMap[addr >> 2] & (3<<shiftAmount)) >> shiftAmount;
		return (pageMap[index >> 2] >> shiftAmount) & 3;
	}

	void GC::SetPageMapValue(uintptr_t addr, int val)
	{
		uintptr_t index = (addr-memStart) >> 12;
#ifdef MMGC_64BIT
		GCAssert((index >> 2) < uintptr_t(64*65536) * uintptr_t(GCHeap::kBlockSize));
#else
		GCAssert(index >> 2 < 64 * GCHeap::kBlockSize);
#endif
		pageMap[index >> 2] |= (val<<((index&0x3)*2));
	}	

	void GC::ClearPageMapValue(uintptr_t addr)
	{
		uintptr_t index = (addr-memStart) >> 12;
#ifdef MMGC_64BIT
		GCAssert((index >> 2) < uintptr_t(64*65536) * uintptr_t(GCHeap::kBlockSize));
#else
		GCAssert((index >> 2) < 64 * GCHeap::kBlockSize);
#endif
		pageMap[index >> 2] &= ~(3<<((index&0x3)*2));
	}	

	void GC::Mark(GCStack<GCWorkItem> &work)
	{
		MMGC_ASSERT_EXCLUSIVE_GC(this);
		while(work.Count()) {
			MarkItem(work);
		}
	}

	void GC::MarkGCPages(void *item, uint32_t numPages, int to)
	{
		USING_PAGE_MAP();
		uintptr_t addr = (uintptr_t)item;
		size_t shiftAmount=0;
		unsigned char *dst = pageMap;

		// save the current live range in case we need to move/copy
		size_t numBytesToCopy = (memEnd-memStart)>>14;

		if(addr < memStart) {
			// round down to nearest 16k boundary, makes shift logic work cause it works
			// in bytes, ie 16k chunks
			addr &= ~0x3fff;
			// marking earlier pages
			if(memStart != MAX_UINTPTR) {
				shiftAmount = (memStart - addr) >> 14;
			}
			memStart = addr;
		} 
		
		if(addr + (numPages+1)*GCHeap::kBlockSize > memEnd) {
			// marking later pages
			memEnd = addr + (numPages+1)*GCHeap::kBlockSize;
			// round up to 16k 
			memEnd = (memEnd+0x3fff)&~0x3fff;
		}

        uint32_t numPagesNeeded = (uint32_t)(((memEnd-memStart)>>14)/GCHeap::kBlockSize + 1);
		if(numPagesNeeded > heap->Size(pageMap)) {
			dst = (unsigned char*)heap->Alloc(numPagesNeeded);
		}

		if(shiftAmount || dst != pageMap) {
			VMPI_memmove(dst + shiftAmount, pageMap, numBytesToCopy);
			if ( shiftAmount ) {
				VMPI_memset(dst, 0, shiftAmount);
			}
			if(dst != pageMap) {
				heap->Free(pageMap);
				pageMap = dst;
			}
		}

		addr = (uintptr_t)item;
		while(numPages--)
		{
			GCAssert(GetPageMapValueAlreadyLocked(addr) == 0);
			SetPageMapValue(addr, to);
			addr += GCHeap::kBlockSize;
		}
	}

	void GC::UnmarkGCPages(void *item, uint32_t numpages)
	{
		uintptr_t addr = (uintptr_t) item;

		USING_PAGE_MAP();
		while(numpages--)
		{
			ClearPageMapValue(addr);
			addr += GCHeap::kBlockSize;
		}
	}

	void GC::CleanStack(bool force)
	{
#if defined(_MSC_VER) && defined(_DEBUG)
		// debug builds poison the stack already
		(void)force;
		return;
#else
		if(!force && (stackCleaned || rememberedStackTop == 0))
			return;

		stackCleaned = true;
		
		register void *stackP = 0;

#if defined MMGC_IA32
		#ifdef WIN32
		__asm {
			mov stackP,esp
		}
		#elif defined SOLARIS
		stackP = (void *) _getsp();
		#else
		asm("movl %%esp,%0" : "=r" (stackP));
		#endif
#endif

#if defined MMGC_AMD64
	#ifdef WIN32
		int foo;
		stackP = &foo;
	#else
		asm("mov %%rsp,%0" : "=r" (stackP));
	#endif
#endif

#if defined MMGC_SPARC
		stackP = (void *) _getsp();
#endif

#if defined MMGC_PPC
		// save off sp
	#ifdef _MAC
			asm("mr %0,r1" : "=r" (stackP));
	#else // _MAC
			asm("mr %0,%%r1" : "=r" (stackP));
	#endif // _MAC
#endif // MMGC_PPC

#if defined MMGC_ARM
    #ifdef _MSC_VER
        // no inline asm available
        int foo;
        stackP = &foo;
    #else
        asm("mov %0,sp" : "=r" (stackP));
    #endif
#endif //MMGC_ARM

		if( ((char*) stackP > (char*)rememberedStackTop) && ((char *)stackEnter > (char*)stackP)) {
			size_t amount = (char*) stackP - (char*)rememberedStackTop;
			void *stack = alloca(amount);
			if(stack) {
				VMPI_memset(stack, 0, amount);
#ifdef MMGC_SPARC
				sparc_clean_windows();
#endif
			}
		}
#endif // __MSC_VER && _DEBUG
	}

	#if defined(MMGC_PPC) && defined(__GNUC__)
	__attribute__((noinline)) 
	#endif
	void GC::MarkQueueAndStack(GCStack<GCWorkItem>& work)
	{
		GCWorkItem item;

		MMGC_GET_STACK_EXTENTS(this, item.ptr, item._size);

		// this is where we will clear to when CleanStack is called
		if(rememberedStackTop == 0 || rememberedStackTop > item.ptr) {
			rememberedStackTop = item.ptr;
		}

		PushWorkItem(work, item);
		Mark(work);
	}

	void GCRoot::init(GC* _gc, const void *_object, size_t _size)
	{
#ifndef _DEBUG
		// only do the memset on GCRoot subclasses
		if(_object == this) {
			size_t s = _gc->GetGCHeap()->GetFixedMalloc()->Size(this);
			// being a GCRoot its important we are clean
			VMPI_memset(this, 0, s);
		}
#endif
		gc = _gc;
		object = _object;
		size = _size;
		GCAssert(size % 2 == 0);
		gc->AddRoot(this);
	}

	GCRoot::GCRoot(GC * _gc)
	{
		init(_gc, this, _gc->GetGCHeap()->GetFixedMalloc()->Size(this));
	}

	GCRoot::GCRoot(GC * _gc, const void * _object, size_t _size)
	{
		init(_gc, _object, _size);
	}

	GCRoot::~GCRoot()
	{
		if(gc) {
			gc->RemoveRoot(this);
		}
	}

	void GCRoot::Set(const void * _object, size_t _size)
	{
		this->object = _object;
		this->size = _size;
	}

	void GCRoot::Destroy()
	{
		Set(NULL, 0);
		if(gc) {
			gc->RemoveRoot(this);
		}
		gc = NULL;
	}

	GCCallback::GCCallback(GC * _gc) : gc(_gc)
	{
		gc->AddCallback(this);
	}

	GCCallback::~GCCallback()
	{
		if(gc) {
			gc->RemoveCallback(this);
		}
	}

	void GCCallback::Destroy()
	{
		if(gc) {
			gc->RemoveCallback(this);
		}
		gc = NULL;
	}

#ifndef MMGC_THREADSAFE
	void GC::CheckThread()
	{
#ifdef _DEBUG
#ifdef WIN32
		GCAssertMsg(disableThreadCheck || m_gcThread == GetCurrentThreadId(), "Unsafe access to GC from wrong thread!");
#endif
#endif
	}
#endif

	bool GC::IsPointerToGCPage(const void *item)
	{
		USING_PAGE_MAP();
		if((uintptr_t)item >= memStart && (uintptr_t)item < memEnd)
			return GetPageMapValueAlreadyLocked((uintptr_t) item) != 0;
		return false;
	}

  ZCT::ZCT()
	{
		zctSize = ZCT_START_SIZE;
		zctFreelist = NULL;
		reaping = false;
		gc = NULL;
		count = 0;
		zctReapThreshold = ZCT_REAP_THRESHOLD;
	}

	ZCT::~ZCT()
	{
		gc->heapFree(zct, zctSize);
	}

	void ZCT::SetGC(GC *gc)
	{
		this->gc = gc;
		zct = (RCObject**) gc->heapAlloc(zctSize);
		zctNext = zct;
	}

	void ZCT::Add(RCObject *obj)
	{
		if(gc->collecting)
		{
			// this is a vestige from FP8 to fix bug 165100, it has the affect of delaying 
			// the deletion of some objects which causes the site to work
			if(gc->dontAddToZCTDuringCollection)
				return;

			// unmarked objects are gonna get swept anyways
			if(!GC::GetMark(obj))
				return;
		}

#if 0
		// note if we add things while reaping we could delete the object
		// here if we had a way to monitor our stack usage
		if(reaping && PLENTY_OF_STACK()) {
			{
				USING_CALLBACK_LIST(gc);
				GCCallback *cb = gc->m_callbacks;
				while(cb) {
					cb->prereap(obj);
					cb = cb->nextCB;
				}
			}
			if(gc->IsFinalized(obj))
				((GCFinalizable*)obj)->~GCFinalizable();
			gc->Free(obj);
			return;
		}
#endif

		if(zctFreelist) {
			RCObject **nextFree = (RCObject**)*zctFreelist;
			*zctFreelist = obj;
			obj->setZCTIndex((int)(zctFreelist - zct));
			zctFreelist = nextFree;			
		} else if(reaping && zctIndex > nextPinnedIndex) {
			// we're going over the list, insert this guy at the front if possible
			zctIndex--;
			GCAssert(zct[zctIndex] == NULL);
			obj->setZCTIndex(zctIndex);
			zct[zctIndex] = obj;
		} else if((uint32_t)(zctNext - zct) <= (RCObject::ZCT_INDEX>>8)) {
			*zctNext = obj;
			obj->setZCTIndex((int)(zctNext - zct));
			zctNext++;
		} else {
			// zct is full, do nothing, mark/sweep will have to handle it
			return;
		}

		count++;

		if(!reaping) {
			// object that were pinned last reap should be unpinned
			if(obj->IsPinned())
				obj->Unpin();
			if(!gc->collecting && zctNext >= zct+zctReapThreshold)
				Reap();
		}

		if(zctNext >= zct + zctSize*4096/sizeof(void *)) {
			// grow 
			RCObject **newZCT = (RCObject**) gc->heap->Alloc(zctSize*2);
			VMPI_memcpy(newZCT, zct, zctSize*GCHeap::kBlockSize);
			gc->heap->Free(zct);
			zctNext = newZCT + (zctNext-zct);
			zct = newZCT;	
			zctSize *= 2;
			GCAssert(!zctFreelist);
		}
	}

	void ZCT::Remove(RCObject *obj)
	{
		int index = obj->getZCTIndex();
		GCAssert(zct[index] == obj);

		if(reaping)
		{
			// freelist doesn't exist during reaping, simplifies things, holes will
			// be compacted next go around if index < nextPinnedIndex.  the freelist
			// allows incoming object to be added behind zctIndex which would mean
			// the reap process wouldn't cascade like its supposed to. 
			zct[index] = NULL;
		}
		else
		{
			zct[index] = (RCObject*) zctFreelist;
			zctFreelist = &zct[index];
		}
		obj->ClearZCTFlag();
		count--;
	}

	void ZCT::PinStackObjects(const void *start, size_t len)
	{
		RCObject **p = (RCObject**)start;
		RCObject **end = p + len/sizeof(RCObject*);

		const void *_memStart = (const void*)gc->memStart;
		const void *_memEnd = (const void*)gc->memEnd;

		while(p < end)
		{
			const void *val = GC::Pointer(*p++);	
			
			if(val < _memStart || val >= _memEnd)
				continue;

			int bits = gc->GetPageMapValue((uintptr_t)val); 
			bool doit = false;
			if (bits == GC::kGCAllocPage) {
				doit = GCAlloc::IsRCObject(val) && GCAlloc::FindBeginning(val) == GetRealPointer(val);
			} else if(bits == GC::kGCLargeAllocPageFirst) {
				doit = GCLargeAlloc::IsRCObject(val) && GCLargeAlloc::FindBeginning(val) == GetRealPointer(val);
			}

			if(doit) {
				RCObject *obj = (RCObject*)val;
				obj->Pin();
			}
		}
	}
			
#if 0
	// this is a release ready tool for hunting down freelist corruption
	void GC::CheckFreelists()
	{
		GCAlloc **allocs = containsPointersAllocs;
		for (int l=0; l < GC::kNumSizeClasses; l++) {
			GCAlloc *a = allocs[l];
			GCAlloc::GCBlock *_b = a->m_firstBlock;	
			while(_b) {
				void *fitem = _b->firstFree;
				void *prev = 0;
				while(fitem) {
					if((uintptr(fitem) & 7) != 0) {
						_asm int 3;
						break;
					}
					if((uintptr(fitem) & ~0xfff) != uintptr(_b))
						_asm int 3;
					prev = fitem;
					fitem = *(void**)fitem;
				}		
				_b = _b->next;
			}
		}
	}
#endif

#ifdef _DEBUG

	void GC::RCObjectZeroCheck(RCObject *item)
	{
		size_t size = Size(item)/sizeof(int);
		uint32_t *p = (uint32_t*)item;
		// skip vtable, first 4 bytes are cleared in Alloc
		p++;
#ifdef MMGC_64BIT
		p++; // vtable is 8-bytes
		size--; 
#endif		
		// in incrementalValidation mode manually deleted items
		// aren't put on the freelist so skip them
		if(incrementalValidation) {
			if(*p == 0xcacacaca)
				return;
		}
		for(int i=1; i<(int)size; i++,p++)
		{
			if(*p)
			{
#ifdef MMGC_MEMORY_INFO
				PrintStackTrace(item);
#endif
				GCAssertMsg(false, "RCObject didn't clean up itself.");
			}
		}	
	}

	// used in DRC validation
	void CleanEntireStack()
	{
#if defined(MMGC_IA32) && defined(WIN32)
		// wipe clean the entire stack below esp.
		register void *stackP;
		__asm {
			mov stackP,esp
		} 
		MEMORY_BASIC_INFORMATION mib;
		// find the top of the stack
		VirtualQuery(stackP, &mib, sizeof(MEMORY_BASIC_INFORMATION));
		// go down whilst pages are committed
		char *stackPeak = (char*) mib.BaseAddress;
		while(true)
		{
			VirtualQuery(stackPeak - 4096, &mib, sizeof(MEMORY_BASIC_INFORMATION));
			// break when we hit the stack growth guard page or an uncommitted page
			// guard pages are committed so we need both checks (really the commit check
			// is just an extra precaution)
			if((mib.Protect & PAGE_GUARD) == PAGE_GUARD || mib.State != MEM_COMMIT)
				break;
			stackPeak -= 4096;
		}
		size_t amount = (char*) stackP - stackPeak - 128;
		{
			void *stack = alloca(amount);
			if(stack) {
				VMPI_memset(stack, 0, amount);
			}
		}
#endif // _MMGC_IA32
	}
#endif // _DEBUG

	void ZCT::Reap()
	{
		if(gc->collecting || reaping || count == 0)
			return;

		uint64_t start = 0;
		if(gc->heap->Config().gcstats) {
			start = VMPI_getPerformanceCounter();
		}
		uint32_t pagesStart = (uint32_t)gc->totalGCPages;
		uint32_t numObjects=0;
		size_t objSize=0;

		reaping = true;
		
		SAMPLE_FRAME("[reap]", gc->core());

		// start by pinning live stack objects
		GCWorkItem item;
		MMGC_GET_STACK_EXTENTS(gc, item.ptr, item._size);
		PinStackObjects(item.ptr, item._size);

#ifdef MMGC_RCROOT_SUPPORT
		
		GC::RCRootSegment* segment = gc->rcRootSegments;
		while(segment)
		{
			PinStackObjects(segment->mem, segment->size);
			segment = segment->next;
		}
		
#endif // MMGC_RCROOT_SUPPORT

		// important to do this before calling prereap
		// use index to iterate in case it grows, as we go through the list we
		// unpin pinned objects and pack them at the front of the list, that
		// way the ZCT is in a good state at the end
		zctIndex = 0;
		nextPinnedIndex = 0;

		// invoke prereap on all callbacks
		{
			USING_CALLBACK_LIST(gc);
			GCCallback *cb = gc->m_callbacks;
			while(cb) {
				cb->prereap();
				cb = cb->nextCB;
			}
		}

#ifdef _DEBUG
		if(gc->validateDefRef) {
			// kill incremental mark since we're gonna wipe the marks
			gc->marking = false;
			gc->m_incrementalWork.Keep(0);

			CleanEntireStack();
			gc->Trace(item.ptr, item._size);
		}
#endif

		// first nuke the freelist, we won't have one when we're done reaping
		// and we don't want secondary objects put on the freelist (otherwise
		// reaping couldn't be a linear scan)
		while(zctFreelist) {
			RCObject **next = (RCObject**)*zctFreelist;
			*zctFreelist = 0;
            zctFreelist = next;
		}
		
		while(zct+zctIndex < zctNext) {
			SAMPLE_CHECK();
			RCObject *rcobj = zct[zctIndex++];
			if(rcobj && !rcobj->IsPinned()) {
				rcobj->ClearZCTFlag();
				zct[zctIndex-1] = NULL;
				count--;
#ifdef _DEBUG
				if(gc->validateDefRef) {
					if(gc->GetMark(rcobj)) {	
#ifdef MMGC_MEMORY_INFO
						rcobj->DumpHistory();
						GCDebugMsg(false, "Back pointer chain:");
						gc->DumpBackPointerChain(rcobj);
#endif
						GCAssertMsg(false, "Zero count object reachable, ref counts not correct!");
					}
				}
#endif
				// invoke prereap on all callbacks
				{
					USING_CALLBACK_LIST(gc);
					GCCallback *cbb = gc->m_callbacks;
					while(cbb) {
						cbb->prereap(rcobj);
						cbb = cbb->nextCB;
					}
				}

				GCAssert(*(intptr_t*)rcobj != 0);
				GCAssert(gc->IsFinalized(rcobj));
				((GCFinalizable*)rcobj)->~GCFinalizable();
				numObjects++;
				objSize += GC::Size(rcobj);
				gc->Free(rcobj);

				GCAssert(gc->weakRefs.get(rcobj) == NULL);
			} else if(rcobj) {
				// move it to front
				rcobj->Unpin();
				if(nextPinnedIndex != zctIndex-1) {
					rcobj->setZCTIndex(nextPinnedIndex);
					GCAssert(zct[nextPinnedIndex] == NULL);
					zct[nextPinnedIndex] = rcobj;
					zct[zctIndex-1] = NULL;
				}
				nextPinnedIndex++;				
			}
		}

		zctNext = zct + nextPinnedIndex;
		zctReapThreshold = count + ZCT_REAP_THRESHOLD;
		if(zctReapThreshold > int(zctSize*GCHeap::kBlockSize/sizeof(RCObject*)))
			zctReapThreshold = zctSize*GCHeap::kBlockSize/sizeof(RCObject*);
		GCAssert(nextPinnedIndex == count);
		zctIndex = nextPinnedIndex = 0;

		// invoke postreap on all callbacks
		{
			USING_CALLBACK_LIST(gc);
			GCCallback *cb = gc->m_callbacks;
			while(cb) {
				cb->postreap();
				cb = cb->nextCB;
			}
		}
		if(gc->heap->Config().gcstats && numObjects) {
			gc->gclog("[mem] DRC reaped %d objects (%d kb) freeing %d pages (%d kb) in %.2f millis (%.4f s)\n", 
				numObjects, objSize>>10, pagesStart - gc->totalGCPages, gc->totalGCPages*GCHeap::kBlockSize >> 10, 
				GC::duration(start), gc->duration(gc->t0)/1000);
		}
		reaping = false;

#ifdef _DEBUG
		if(gc->validateDefRef) {
			gc->Sweep();
		}
#endif
	}

	void GC::gclog(const char *format, ...)
	{
		static bool ingclog=false;
		
		(void)format;
		char buf[4096];
		va_list argptr;

		va_start(argptr, format);
		vsprintf(buf, format, argptr);
		va_end(argptr);

		GCAssert(VMPI_strlen(buf) < 4096);

		{
			USING_CALLBACK_LIST(this);
			GCCallback *cb = m_callbacks;
				if(cb) {
				cb->log(buf);
				cb = cb->nextCB;
			}
		}
		// log gross stats any time anything interesting happens
		if(!ingclog) {
			ingclog = true;
			heap->DumpMemoryInfo();
			ingclog = false;
		}
	}

#ifdef MMGC_MEMORY_INFO
	void GC::DumpBackPointerChain(void *o, pDumpBackCallbackProc proc, void *context)
	{
		int *p = (int*)GetRealPointer ( o ) ;
		int size = *p++;
		*p++; // skip old trace index slot
		//if(*(p+1) == 0xcacacaca || *(p+1) == 0xbabababa) {
			// bail, object was deleted
		//	return;
		//}
		const char *typeName = GetAllocationName(o);

		// strip "class "
		if (!VMPI_strncmp(typeName, "class ", 6))
			typeName += 6;
		GCDebugMsg(false, "Object: (%s *)0x%x\n", typeName, o);
		if (proc)
			proc(context, o, typeName);
		PrintStackTrace(o);
		GCDebugMsg(false, "---\n");
		// skip data + endMarker
		p += 1 + (size>>2);
		void *container = (void*)(*(void**)p);
		if(container && IsPointerToGCPage(container))
			DumpBackPointerChain(container, proc, context);
		else 
		{
			GCDebugMsg(false, "GCRoot object: 0x%x\n", container);
			if((uintptr_t)container >= memStart && (uintptr_t)container < memEnd)
				PrintStackTrace(container);
		}
	}

	void GC::WriteBackPointer(const void *item, const void *container, size_t itemSize)
	{
		GCAssert(container != NULL);
		uint32_t *p = (uint32_t*) item;
		uint32_t size = *p++;
		if(size && size <= itemSize) {
			// skip traceIndex + data + endMarker
			p += (2 + (size>>2));
			GCAssert(sizeof(uintptr_t) == sizeof(void*));
			*(uintptr_t*)p = (uintptr_t) container;
		}
	}

#endif

	bool GC::IsRCObject(const void *item)
	{
		int bits;
		{
			USING_PAGE_MAP();

			if ((uintptr_t)item < memStart || (uintptr_t)item >= memEnd || ((uintptr_t)item&0xfff) == 0)
				return false;
			bits = GetPageMapValueAlreadyLocked((uintptr_t)item);
		}

		item = GetRealPointer(item);
		switch(bits)
		{
		case kGCAllocPage:
			if((char*)item < ((GCAlloc::GCBlock*)((uintptr_t)item&~0xfff))->items)
				return false;
			return GCAlloc::IsRCObject(item);
		case kGCLargeAllocPageFirst:
			return GCLargeAlloc::IsRCObject(item);
		default:
			return false;
		}
	}

	int GC::DumpAlloc(GCAlloc *a)
	{
		int inUse =  a->GetNumAlloc() * a->GetItemSize();
		int maxAlloc =  a->GetMaxAlloc()* a->GetItemSize();
		int efficiency = maxAlloc > 0 ? inUse * 100 / maxAlloc : 100;
		if(inUse) {
			fprintf(heap->GetSpyFile(), "Allocator(%d): %d%% efficiency %d kb out of %d kb\n", a->GetItemSize(), efficiency, inUse>>10, maxAlloc>>10);
		}
		return maxAlloc-inUse;
	}

	void GC::DumpMemoryInfo()
	{
		size_t total = totalGCPages * GCHeap::kBlockSize;
		heap->log_percentage("[mem] \tmanaged fragmentation ", total-GetBytesInUse(), total);
		if(ticksToMillis(markTicks) != 0 && bytesMarked != 0) {
			uint32_t markRate = (uint32_t) (bytesMarked / (1024 * ticksToMillis(markTicks))); // kb/ms == mb/s
			fprintf(heap->GetSpyFile(),"[mem] \tmark rate %u mb/s\n", markRate);
		}
		fprintf(heap->GetSpyFile(),"[mem] \tmark increments %d\n", marks);
		fprintf(heap->GetSpyFile(),"[mem] \tsweeps %d mb/s\n", sweeps);

		int waste=0;
		for(int i=0; i < kNumSizeClasses; i++)
		{
			waste += DumpAlloc(containsPointersAllocs[i]);
			waste += DumpAlloc(containsPointersRCAllocs[i]);
			waste += DumpAlloc(noPointersAllocs[i]);
		}
		fprintf(heap->GetSpyFile(),"Wasted %d kb\n", waste>>10);
	}

#ifdef _DEBUG

	void GC::CheckFreelist(GCAlloc *gca)
	{	
		GCAlloc::GCBlock *b = gca->m_firstFree;
		while(b)
		{
			void *freelist = b->firstFree;
			while(freelist)
			{			
				// b->firstFree should be either 0 end of free list or a pointer into b, otherwise, someone
				// wrote to freed memory and hosed our freelist
				GCAssert(freelist == 0 || ((uintptr_t) freelist >= (uintptr_t) b->items && (uintptr_t) freelist < (uintptr_t) b + GCHeap::kBlockSize));
				freelist = *((void**)freelist);
			}
			b = b->nextFree;
		}
	}

	void GC::CheckFreelists()
	{
		for(int i=0; i < kNumSizeClasses; i++)
		{
			CheckFreelist(containsPointersAllocs[i]);
			CheckFreelist(noPointersAllocs[i]);
		}
	}

	void GC::UnmarkedScan(const void *mem, size_t size)
	{
		uintptr_t lowerBound = memStart;
		uintptr_t upperBound = memEnd;
		
		uintptr_t *p = (uintptr_t *) mem;
		uintptr_t *end = p + (size / sizeof(void*));

		while(p < end)
		{
			uintptr_t val = *p++;
			
			if(val < lowerBound || val >= upperBound)
				continue;
			
			// normalize and divide by 4K to get index
			int bits = GetPageMapValueAlreadyLocked(val);
			switch(bits)
			{
			case 0:
				continue;
				break;
			case kGCAllocPage:
				GCAssert(GCAlloc::ConservativeGetMark((const void*) (val&~7), true));
				break;
			case kGCLargeAllocPageFirst:
				GCAssert(GCLargeAlloc::ConservativeGetMark((const void*) (val&~7), true));
				break;
			default:
				GCAssertMsg(false, "Invalid pageMap value");
				break;
			}
		}
	}

	void GC::FindUnmarkedPointers()
	{
		MMGC_ASSERT_EXCLUSIVE_GC(this);

		if(findUnmarkedPointers)
		{
			uintptr_t m = memStart;

			while(m < memEnd)
			{
				// divide by 4K to get index
				int bits = GetPageMapValue(m);
				if(bits == kNonGC) {
					UnmarkedScan((const void*)m, GCHeap::kBlockSize);
					m += GCHeap::kBlockSize;
				} else if(bits == kGCLargeAllocPageFirst) {
					GCLargeAlloc::LargeBlock *lb = (GCLargeAlloc::LargeBlock*)m;
					const void *item = GetUserPointer((const void*)(lb+1));
					if(GCLargeAlloc::GetMark(item) && GCLargeAlloc::ContainsPointers(GetRealPointer(item))) {
						UnmarkedScan(item, GC::Size(item));
					}
					m += lb->GetNumBlocks() * GCHeap::kBlockSize;
				} else if(bits == kGCAllocPage) {
					// go through all marked objects
					GCAlloc::GCBlock *b = (GCAlloc::GCBlock *) m;
                    for (int i=0; i < b->alloc->m_itemsPerBlock; i++) {
                        // If the mark is 0, delete it.
                        int marked = GCAlloc::GetBit(b, i, GCAlloc::kMark);
                        if (!marked) {
                            void* item = (char*)b->items + b->alloc->m_itemSize*i;
                            if(GCAlloc::ContainsPointers(item)) {
								UnmarkedScan(GetUserPointer(item), b->alloc->m_itemSize - DebugSize());
							}
						}
					}
					
					m += GCHeap::kBlockSize;
				}				 
			}
		}
	}

/* macro to stack allocate a string containing 3*i (indent) spaces */
#define ALLOCA_AND_FILL_WITH_SPACES(b, i) \
	{ b = (char*)alloca((3*(i))+1); \
	int n = 0; \
	for(; n<3*(i); n++) b[n] = ' '; \
	b[n] = '\0'; }

	void GC::ProbeForMatch(const void *mem, size_t size, uintptr_t value, int recurseDepth, int currentDepth)
	{
		uintptr_t lowerBound = memStart;
		uintptr_t upperBound = memEnd;
		
		uintptr_t *p = (uintptr_t *) mem;
		uintptr_t *end = p + (size / sizeof(void*));

		int bits = GetPageMapValue((uintptr_t)mem);

		while(p < end)
		{
			uintptr_t val = *p++;
			
			if(val < lowerBound || val >= upperBound)
				continue;

			// did we hit ?
			if (val == value)
			{
				// ok so let's see where we are 
				uintptr_t* where = p-1;
				GCHeap::HeapBlock* block = heap->AddrToBlock(where);
				//GCAssertMsg(block->inUse(), "Not sure how we got here if the block is not in use?");
				GCAssertMsg(block->committed, "Means we are probing uncommitted memory. not good");
				int* ptr;			  

				switch(bits)
				{
				case kNonGC:
					{
						if (block->size == 1)
						{
							// fixed sized entries find out the size of the block
							FixedAlloc::FixedBlock* fixed = (FixedAlloc::FixedBlock*) block->baseAddr;
							int fixedsize = fixed->size;

							// now compute which element we are 
							uintptr_t startAt = (uintptr_t) &(fixed->items[0]);
							uintptr_t item = ((uintptr_t)where-startAt) / fixedsize;

							ptr = (int*) ( startAt + (item*fixedsize) );
						}
						else
						{
							// fixed large allocs ; start is after the block 
							ptr = (int*) block->baseAddr;
						}
					}
					break;

				default:
					ptr = ((int*)FindBeginning(where)) - 2;
					break;
				}

				int  taggedSize = *ptr;
				int  traceIndex = *(ptr+1);
				int* real = (ptr+2);

				char* buffer = 0;
				ALLOCA_AND_FILL_WITH_SPACES(buffer, currentDepth);

				if (buffer) GCDebugMsg(false, buffer);
				GCDebugMsg(false, "Location: 0x%08x  Object: 0x%08x (size %d)\n", where, real, taggedSize);
				if (buffer) GCDebugMsg(false, buffer);
#if 0
				PrintStackTraceByIndex(traceIndex);
#else
				(void)traceIndex;
#endif

				if (recurseDepth > 0)
					WhosPointingAtMe(real, recurseDepth-1, currentDepth+1);
			}
		}
	}

	/**
	 * This routine searches through all of GC memory looking for references to 'me' 
	 * It ignores already claimed memory thus locating active references only.
	 * recurseDepth can be set to a +ve value in order to follow the chain of 
	 * pointers arbitrarily deep.  Watch out when setting it since you may see
	 * an exponential blow-up (usu. 1 or 2 is enough).	'currentDepth' is for
	 * indenting purposes and should be left alone.
	 */
	void GC::WhosPointingAtMe(void* me, int recurseDepth, int currentDepth)
	{
#ifdef MMGC_THREADSAFE
		if (currentDepth == 0)
			pageMapLock.Acquire();
#endif

		uintptr_t val = (uintptr_t)me;
		uintptr_t m = memStart;

		char* buffer = 0;
		ALLOCA_AND_FILL_WITH_SPACES(buffer, currentDepth);

		if (buffer) GCDebugMsg(false, buffer);
		GCDebugMsg(false, "[%d] Probing for pointers to : 0x%08x\n", currentDepth, me);
		while(m < memEnd)
		{
			// divide by 4K to get index
			int bits = GetPageMapValueAlreadyLocked(m);
			if(bits == kNonGC) 
			{
				ProbeForMatch((const void*)m, GCHeap::kBlockSize, val, recurseDepth, currentDepth);
				m += GCHeap::kBlockSize;
			} 
			else if(bits == kGCLargeAllocPageFirst) 
			{
				GCLargeAlloc::LargeBlock *lb = (GCLargeAlloc::LargeBlock*)m;
				const void *item = GetUserPointer((const void*)(lb+1));
				bool marked = GCLargeAlloc::GetMark(item);
				if (marked)
				{
					if(GCLargeAlloc::ContainsPointers(GetRealPointer(item))) 
					{
						ProbeForMatch(item, GC::Size(item), val, recurseDepth, currentDepth);
					}
				}
				m += lb->GetNumBlocks() * GCHeap::kBlockSize;
			} 
			else if(bits == kGCAllocPage) 
			{
				// go through all marked objects
				GCAlloc::GCBlock *b = (GCAlloc::GCBlock *) m;
                for (int i=0; i < b->alloc->m_itemsPerBlock; i++) 
				{
                    int marked = GCAlloc::GetBit(b, i, GCAlloc::kMark);
                    if (marked) 
					{
                        void* item = (char*)b->items + b->alloc->m_itemSize*i;
                        if(GCAlloc::ContainsPointers(item)) 
						{
							ProbeForMatch(GetUserPointer(item), b->alloc->m_itemSize - DebugSize(), val, recurseDepth, currentDepth);
						}
					}
				}				
				m += GCHeap::kBlockSize;
			}
			else
			{
				GCAssertMsg(false, "Oh seems we missed a case...Tom any ideas here?");
			
			}
		}

#ifdef MMGC_THREADSAFE
		if (currentDepth == 0)
			pageMapLock.Release();
#endif
	}
#undef ALLOCA_AND_FILL_WITH_SPACES
#endif


	void GC::StartIncrementalMark()
	{
		GCAssert(!marking);
		GCAssert(!collecting);

		lastStartMarkIncrementCount = markIncrements;

		// set the stack cleaning trigger
		stackCleaned = false;

		marking = true;

		GCAssert(m_incrementalWork.Count() == 0);
	
		uint64_t start = VMPI_getPerformanceCounter();

		// clean up any pages that need sweeping
		for(int i=0; i < kNumSizeClasses; i++) {
			containsPointersRCAllocs[i]->SweepNeedsSweeping();
			containsPointersAllocs[i]->SweepNeedsSweeping();
			noPointersAllocs[i]->SweepNeedsSweeping();
		}

		// at this point every object should have no marks or be marked kFreelist
#ifdef _DEBUG		
		for(int i=0; i < kNumSizeClasses; i++) {
			containsPointersRCAllocs[i]->CheckMarks();
			containsPointersAllocs[i]->CheckMarks();
			noPointersAllocs[i]->CheckMarks();
		}
#endif
	
		{
			MMGC_LOCK(m_rootListLock);
			GCRoot *r = m_roots;
			while(r) {
				GCWorkItem item = r->GetWorkItem();
				if(item.ptr)
					MarkItem(item, m_incrementalWork);
				r = r->next;
			}
		}
		markTicks += VMPI_getPerformanceCounter() - start;
		IncrementalMark();
	}

#if 0
	// TODO: SSE2 version
	void GC::MarkItem_MMX(const void *ptr, size_t size, GCStack<GCWorkItem> &work)
	{
		 uintptr *p = (uintptr*) ptr;
		// deleted things are removed from the queue by setting them to null
		if(!p)
			return;

		bytesMarked += size;
		marks++;

		uintptr *end = p + (size / sizeof(void*));
		uintptr thisPage = (uintptr)p & ~0xfff;

		// since MarkItem recurses we have to do this before entering the loop
		if(IsPointerToGCPage(ptr)) 
		{
			int b = SetMark(ptr);
#if defined(_DEBUG) && !defined(AVMPLUS_SAMPLER) // sampler does some marking which triggers this
			// def ref validation does a Trace which can 
			// cause things on the work queue to be already marked
			// in incremental GC
			if(!validateDefRef) {
				GCAssert(!b);
			}
#endif
		}

		_asm {
			// load memStart and End into mm0
			movq mm0,memStart			
		}

		while(p < end) 
		{		
			_asm {
				mov       ebx, [p]
				mov       ecx, [count]
				sar       ecx, 1
				mov       eax, dword ptr [lowerBound]
				dec       eax
				movd      mm1, eax
				movd      mm2, dword ptr [upperBound]
				punpckldq mm1, mm1
				punpckldq mm2, mm2
				mov		  eax, 3
				movd	  mm5, eax
				punpckldq mm5, mm5				
			  MarkLoop:
				movq      mm0, qword ptr [ebx]
				movq      mm3, mm0
				pcmpgtd   mm3, mm1
				movq      mm4, mm2
				pcmpgtd   mm4, mm0
				pand      mm3, mm4
				packssdw  mm3, mm3
				movd      eax, mm3
				or        eax, eax
				jz        Advance

				// normalize and divide by 4K to get index
				psubd	  mm0, mm1
				psrld     mm0, 12

				// shift amount to determine position in the byte (times 2 b/c 2 bits per page)
				movq      mm6, mm0
				pand      mm6, mm5
				pslld     mm6, 1
				packssdw  mm6, mm6

				// index = index >> 2 for pageMap index
				psrld     mm0, 2
				packssdw  mm0, mm0

				// check 
				push      ecx

				

				push	  [workAddr]
				movd	  edx, mm6
				push      edx // packShiftAmount
				movd	  edx, mm0
				push	  edx // packIndex4
				push	  eax // packTest
				push	  dword ptr [ebx+4] // val2
				push	  dword ptr [ebx] // val1
				mov		  ecx, [this]
				call	  ConservativeMarkMMXHelper
					
				pop		  ecx

			Advance:
				add       ebx, 8
				loop      MarkLoop
				mov       dword ptr [p], ebx				
			}
		}
	}

#endif

#ifdef MMGC_INTERIOR_PTRS
	inline bool IsLargeAllocPage(int bits) {
		return (bits == GC::kGCLargeAllocPageFirst
				|| bits == GC::kGCLargeAllocPageRest);
	}
#else
	inline bool IsLargeAllocPage(int bits) {
		return bits == GC::kGCLargeAllocPageFirst;
	}
#endif

#ifdef WERNER_MODE
#define RECURSIVE_MARK

	class MarkList
	{
	public:
		static MarkList *current;
		static int offset;
		MarkList(GCWorkItem &item) : prev(current), wi(item), off(offset)
		{
			current = this;
		}
		~MarkList() { current = prev; }
		MarkList *prev;
		GCWorkItem &wi;
		int off;
	};
	MarkList *MarkList::current = NULL;
	int MarkList::offset = -1;
#endif

	void GC::MarkItem(GCWorkItem &wi, GCStack<GCWorkItem> &work)
	{
		size_t size = wi.GetSize();
		uintptr_t *p = (uintptr_t*) wi.ptr;

#ifdef WERNER_MODE
		MarkList me(wi);
		
		if(p == shouldGo) {
			MarkList *wl = MarkList::current;
			while(wl) {
				const char *name = "";
// To enable RTTI, you must change all your projects to use RTTI first.  
#if 0
				static bool tryit = true;
				if (tryit)
				{
					try {
						const std::type_info *ti = &typeid(*(MMgc::GCFinalizedObject*)wl->wi.ptr);
						if (ti->name() && (int(ti->name()) > 0x10000))
							name = ti->name();
					} catch(...) {
						name = "unknown";
					}
				}
#endif

				if(wl->prev)
					VMPI_sprintf(statusBuffer, "0x%x+%d -> 0x%x size=%d (%s)\n",  (unsigned int)wl->prev->wi.ptr, wl->off, (unsigned int)wl->wi.ptr, wl->wi.GetSize(), name);
				else
					VMPI_sprintf(statusBuffer, "0x%x : %d (%s)\n", (unsigned int)wl->wi.ptr, wl->wi.GetSize(), name);
				wl = wl->prev;
				OutputDebugString(statusBuffer);
			}
			VMPI_sprintf(statusBuffer, "\n");
			OutputDebugString(statusBuffer);
			//shouldGo = NULL;
		}
#endif
		bytesMarked += size;
		marks++;

		uintptr_t *end = p + (size / sizeof(void*));
		uintptr_t thisPage = (uintptr_t)p & ~0xfff;

		// set the mark bits on this guy
		if(wi.IsGCItem())
		{
			int b = SetMark(wi.ptr);
			(void)b;
#ifdef _DEBUG
			// def ref validation does a Trace which can 
			// cause things on the work queue to be already marked
			// in incremental GC
			if(!validateDefRef) {
				//GCAssert(!b);
			}
#endif			
		}
		else
		{
			GCAssert(!IsPointerToGCPage(wi.ptr));
		}

		uintptr_t _memStart = memStart;
		uintptr_t _memEnd = memEnd;
		
		numObjects++;
		objSize += size;

		while(p < end) 
		{
#ifdef WERNER_MODE
			MarkList::offset = (int)p - (int)wi.ptr;
#endif

			uintptr_t val = *p++;  
			
			if(val < _memStart || val >= _memEnd)
				continue;

			// normalize and divide by 4K to get index
			int bits = GetPageMapValue(val); 
						
			if (bits == kGCAllocPage)
			{
				const void *item;
				GCAlloc::GCBlock *block = (GCAlloc::GCBlock*) (val & ~0xFFF);

#ifdef MMGC_INTERIOR_PTRS
				item = (void*) val;
#else
				// back up to real beginning
				item = GetRealPointer((const void*) (val & ~7));
#endif

				// guard against bogus pointers to the block header
				if(item < block->items)
					continue;

				int itemNum = GCAlloc::GetIndex(block, item);
#ifdef MMGC_INTERIOR_PTRS
				// adjust |item| to the beginning of the allocation
				item = block->items + itemNum * block->size;
#else
				// if |item| doesn't point to the beginning of an allocation,
				// it's not considered a pointer.
				if (block->items + itemNum * block->size != item)
				{
#ifdef MMGC_64BIT
// Doubly-inherited classes have two vtables so are offset 8 more bytes than normal. 
// Handle that here (shows up with PlayerScriptBufferImpl object in the Flash player)
					if ((block->items + itemNum * block->size + sizeof(void *)) == item)
						item = block->items + itemNum * block->size;
					else
#endif // MMGC_64BIT
						continue;
				}
#endif

				// inline IsWhite/SetBit
				// FIXME: see if using 32 bit values is faster
				uint32_t *pbits = &block->GetBits()[itemNum>>3];
				int shift = (itemNum&0x7)<<2;
				int bits2 = *pbits;
				//if(GCAlloc::IsWhite(block, itemNum)) 
				if((bits2 & ((GCAlloc::kMark|GCAlloc::kQueued)<<shift)) == 0)
				{
					if(block->alloc->ContainsPointers())
					{
						const void *realItem = item;
						uint32_t itemSize = block->size;
						#ifdef MMGC_MEMORY_INFO
						realItem = GetUserPointer(realItem);
						itemSize -= (uint32_t)DebugSize();
						#endif
						if(((uintptr_t)realItem & ~0xfff) != thisPage)
						{							
							*pbits = bits2 | (GCAlloc::kQueued << shift);
							block->gc->PushWorkItem(work, GCWorkItem(realItem, itemSize, true));
						}
						else
						{
							// clear queued bit
							*pbits = bits2 & ~(GCAlloc::kQueued << shift);
							// skip stack for same page items, this recursion is naturally limited by
							// how many item can appear on a page, worst case is 8 byte linked list or
							// 512
							GCWorkItem newItem(realItem, itemSize, true);
							MarkItem(newItem, work);
						}
					}
					else
					{
						//GCAlloc::SetBit(block, itemNum, GCAlloc::kMark);
						*pbits = bits2 | (GCAlloc::kMark << shift);
					}
					#if defined(MMGC_MEMORY_INFO)
					GC::WriteBackPointer(item, (end==(void*)0x130000) ? p-1 : wi.ptr, block->size);
					#endif
				}
			}
			else if (IsLargeAllocPage(bits))
			{
				//largeAlloc->ConservativeMark(work, (void*) (val&~7), workitem.ptr);
				const void* item;

#ifdef MMGC_INTERIOR_PTRS
				if (bits == kGCLargeAllocPageFirst)
				{
					// guard against bogus pointers to the block header
					if ((val & 0xffff) < sizeof(GCLargeAlloc::LargeBlock))
						continue;

					item = (void *) ((val & ~0xfff) |
									 sizeof(GCLargeAlloc::LargeBlock));
				}
				else
				{
					item = GetRealPointer(FindBeginning((void *) val));
				}
#else
				// back up to real beginning
				item = GetRealPointer((const void*) (val & ~7));

				// If |item| doesn't point to the start of the page, it's not
				// really a pointer.
				if(((uintptr_t) item & 0xfff) != sizeof(GCLargeAlloc::LargeBlock))
					continue;
#endif

				GCLargeAlloc::LargeBlock *b = GCLargeAlloc::GetBlockHeader(item);
				if((b->flags & (GCLargeAlloc::kQueuedFlag|GCLargeAlloc::kMarkFlag)) == 0) 
				{
					uint32_t usize = b->usableSize;
					if((b->flags & GCLargeAlloc::kContainsPointers) != 0) 
					{
						b->flags |= GCLargeAlloc::kQueuedFlag;
						const void *realItem = item;
						#ifdef MMGC_MEMORY_INFO
						realItem = GetUserPointer(item);
						usize -= (uint32_t)DebugSize();
						#endif
						b->gc->PushWorkItem(work, GCWorkItem(realItem, usize, true));
					} 
					else
					{
						// doesn't need marking go right to black
						b->flags |= GCLargeAlloc::kMarkFlag;
					}
					#if defined(MMGC_MEMORY_INFO)
					GC::WriteBackPointer(item, end==(void*)0x130000 ? p-1 : wi.ptr, usize);
					#endif
				}
			}
		}
	}

	void GC::IncrementalMark()
	{
		MMGC_ASSERT_EXCLUSIVE_GC(this);
		uint32_t time = incrementalValidation ? 1 : 5;
#ifdef _DEBUG
		time = 1;
#endif

		SAMPLE_FRAME("[mark]", core());
		if(m_incrementalWork.Count() == 0 || hitZeroObjects) {
			FinishIncrementalMark();
			return;
		} 
		
		markIncrements++;
		// FIXME: tune this so that getPerformanceCounter() overhead is noise
		static unsigned int checkTimeIncrements = 100;
		uint64_t start = VMPI_getPerformanceCounter();

		numObjects=0;
		objSize=0;

		uint64_t ticks = start + time * VMPI_getPerformanceFrequency() / 1000;
		do {
			unsigned int count = m_incrementalWork.Count();
			if (count == 0) {
				hitZeroObjects = true;
				break;
			}
			if (count > checkTimeIncrements) {
				count = checkTimeIncrements;
			}
			for(unsigned int i=0; i<count; i++) 
			{
 				MarkItem(m_incrementalWork);
			}
			SAMPLE_CHECK();
		} while(VMPI_getPerformanceCounter() < ticks);

		lastMarkTicks = VMPI_getPerformanceCounter();
		markTicks += lastMarkTicks - start;

		if(heap->Config().gcstats) {
			double millis = duration(start);
			size_t kb = objSize>>10;
			gclog("[mem] mark(%d) %d objects (%d kb %d mb/s) in %.2f millis (%.4f s)\n", 
				markIncrements-lastStartMarkIncrementCount, numObjects, kb, 
				uint32_t(double(kb)/millis), millis, duration(t0)/1000);
		}
	}

	void GC::FinishIncrementalMark()
	{
		MMGC_ASSERT_EXCLUSIVE_GC(this);

		// Don't finish an incremental mark (i.e., sweep) if we
		// are in the midst of a ZCT reap.
		if (zct.reaping)
		{
			return;
		}
		
		hitZeroObjects = false;

		// finished in Sweep
		sweepStart = VMPI_getPerformanceCounter();
		
		// mark roots again, could have changed (alternative is to put WB's on the roots
		// which we may need to do if we find FinishIncrementalMark taking too long)
		
		{
			MMGC_LOCK(m_rootListLock);
			GCRoot *r = m_roots;
			while(r) {					
				GCWorkItem item = r->GetWorkItem();
				// need to do this while holding the root lock so we don't end 
				// up trying to scan a deleted item later, another reason to keep
				// the root set small
				if(item.ptr) {
					MarkItem(item, m_incrementalWork);
				}
				r = r->next;
			}
		}
		MarkQueueAndStack(m_incrementalWork);

#ifdef _DEBUG
		// need to traverse all marked objects and make sure they don't contain
		// pointers to unmarked objects
		FindMissingWriteBarriers();
#endif

		GCAssert(!collecting);
		collecting = true;
		GCAssert(m_incrementalWork.Count() == 0);
		Sweep();
		GCAssert(m_incrementalWork.Count() == 0);
		collecting = false;
		marking = false;
	}

	int GC::IsWhite(const void *item)
	{
		// back up to real beginning
		item = GetRealPointer((const void*) item);

		// normalize and divide by 4K to get index
		if(!IsPointerToGCPage(item))
			return false;
		int bits = GetPageMapValue((uintptr_t)item);	
		switch(bits) {
		case 1:
			return GCAlloc::IsWhite(item);
		case 3:
			// FIXME: we only want pointers to the first page for large items, fix
			// this by marking the first page and subsequent pages of large items differently
			// in the page map (ie use 2).
			return GCLargeAlloc::IsWhite(item);
		}
		return false;
	}
	
	// TODO: fix headers so this can be declared there and inlined
	void GC::WriteBarrierWrite(const void *address, const void *value)
	{
		GCAssert(!IsRCObject(value));
		*(uintptr_t*)address = (uintptr_t) value;
	}

	// optimized version with no RC checks or pointer swizzling
	void GC::writeBarrierRC(const void *container, const void *address, const void *value)
	{
		GCAssert(IsPointerToGCPage(container));
		GCAssert(((uintptr_t)container & 3) == 0);
		GCAssert(((uintptr_t)address & 2) == 0);
		GCAssert(address >= container);
		GCAssert(address < (char*)container + Size(container));

		WriteBarrierNoSubstitute(container, value);
		WriteBarrierWriteRC(address, value);
	}

	// TODO: fix headers so this can be declared there and inlined
	void GC::WriteBarrierWriteRC(const void *address, const void *value)
	{
			RCObject *rc = (RCObject*)Pointer(*(RCObject**)address);
			if(rc != NULL) {
				GCAssert(rc == FindBeginning(rc));
				GCAssert(IsRCObject(rc));
				rc->DecrementRef();
			}
		*(uintptr_t*)address = (uintptr_t) value;
			rc = (RCObject*)Pointer(value);
			if(rc != NULL) {
				GCAssert(IsRCObject(rc));
				GCAssert(rc == FindBeginning(value));
				rc->IncrementRef();
			}
	}

	void GC::WriteBarrier(const void *address, const void *value)
	{
		GC* gc = GC::GetGC(address);
		if(Pointer(value) != NULL && gc->marking) {
			void *container = gc->FindBeginning(address);
			gc->WriteBarrierNoSubstitute(container, value);
		}
		gc->WriteBarrierWrite(address, value);
	}

	void GC::WriteBarrierNoSub(const void *address, const void *value)
	{
		GC *gc = NULL;
		if(value != NULL && (gc = GC::GetGC(address))->marking) {
			void *container = gc->FindBeginning(address);
			gc->WriteBarrierNoSubstitute(container, value);		
		}
	}

	void GC::TrapWrite(const void *black, const void *white)
	{
		// assert fast path preconditions
		(void)black;
		GCAssert(marking);
		GCAssert(GetMark(black));
		GCAssert(IsWhite(white));
		// currently using the simplest finest grained implementation,
		// which could result in huge work queues.  should try the
		// more granular approach of moving the black object to grey
		// (smaller work queue, less frequent wb slow path) but if the
		// black object is big we end up doing useless redundant
		// marking.  optimal approach from lit is card marking (mark a
		// region of the black object as needing to be re-marked)
		if(ContainsPointers(white)) {
			SetQueued(white);
			PushWorkItem(m_incrementalWork, GCWorkItem(white, (uint32_t)Size(white), true));
		} else {
			SetMark(white);
		}
	}

	bool GC::ContainsPointers(const void *item)
	{
		item = GetRealPointer(item);
		if (GCLargeAlloc::IsLargeBlock(item)) {
			return GCLargeAlloc::ContainsPointers(item);
		} else {
			return GCAlloc::ContainsPointers(item);
		}
	}

	bool GC::IsGCMemory (const void *item)
	{
		int bits = GetPageMapValue((uintptr_t)item);
		return (bits != 0);
	}

	bool GC::IsQueued(const void *item)
	{
		return !GetMark(item) && !IsWhite(item);
	}

	uint32_t *GC::GetBits(int numBytes, int sizeClass)
	{
		uint32_t *bits;

		MMGC_ASSERT_GC_LOCK(this);
		GCAssert(numBytes % 4 == 0);

		#ifdef MMGC_64BIT // we use first 8-byte slot for the free list
		if (numBytes == 4)
			numBytes = 8;
		#endif

		// hit freelists first
		if(m_bitsFreelists[sizeClass]) {
			bits = m_bitsFreelists[sizeClass];
			m_bitsFreelists[sizeClass] = *(uint32_t**)bits;
			VMPI_memset(bits, 0, sizeof(uint32_t*));
			return bits;
		}

		if(!m_bitsNext)
			m_bitsNext = (uint32_t*)heap->Alloc(1);

		int leftOver = GCHeap::kBlockSize - ((uintptr_t)m_bitsNext & 0xfff);
		if(leftOver >= numBytes) {
			bits = m_bitsNext;
			if(leftOver == numBytes) 
				m_bitsNext = 0;
			else 
				m_bitsNext += numBytes/sizeof(uint32_t);
		} else {
			if(leftOver>=int(sizeof(void*))) {
				// put waste in freelist
				for(int i=0, n=kNumSizeClasses; i<n; i++) {
					GCAlloc *a = noPointersAllocs[i];
					if(!a->m_bitsInPage && a->m_numBitmapBytes <= leftOver) {
						FreeBits(m_bitsNext, a->m_sizeClassIndex);
						break;
					}
				}
			}
			m_bitsNext = 0;
			// recurse rather than duplicating code
			return GetBits(numBytes, sizeClass);
		}
		return bits;
	}

	void GC::AddRoot(GCRoot *root)
	{
		MMGC_LOCK(m_rootListLock);
		root->prev = NULL;
		root->next = m_roots;
		if(m_roots)
			m_roots->prev = root;
		m_roots = root;
	}

	void GC::RemoveRoot(GCRoot *root)
	{	
		MMGC_LOCK(m_rootListLock);
		if( m_roots == root )
			m_roots = root->next;
		else
			root->prev->next = root->next;

		if(root->next)
			root->next->prev = root->prev;
	}
	
	void GC::AddCallback(GCCallback *cb)
	{
		USING_CALLBACK_LIST(this);

		cb->prevCB = NULL;
		cb->nextCB = m_callbacks;
		if(m_callbacks)
			m_callbacks->prevCB = cb;
		m_callbacks = cb;
	}

	void GC::RemoveCallback(GCCallback *cb)
	{
		USING_CALLBACK_LIST(this);

		if( m_callbacks == cb )
			m_callbacks = cb->nextCB;
		else
			cb->prevCB->nextCB = cb->nextCB;

		if(cb->nextCB)
			cb->nextCB->prevCB = cb->prevCB;
	}

	void GC::PushWorkItem(GCStack<GCWorkItem> &stack, GCWorkItem item)
	{
#ifdef RECURSIVE_MARK
		MarkItem(item, stack);
#else
		if(item.ptr) {
			stack.Push(item);
		}
#endif
	}

	GCWeakRef* GC::GetWeakRef(const void *item) 
	{
		GC *gc = GetGC(item);
#ifdef MMGC_THREADSAFE
		GCAutoLock _lock(gc->m_lock);
#endif
		GCWeakRef *ref = (GCWeakRef*) gc->weakRefs.get(item);

		if(ref == NULL) {
			ref = new (gc) GCWeakRef(item);
			gc->weakRefs.put(item, ref);
			item = GetRealPointer(item);
			if (GCLargeAlloc::IsLargeBlock(item)) {
				GCLargeAlloc::SetHasWeakRef(item, true);
			} else {
				GCAlloc::SetHasWeakRef(item, true);
			}
		} else {
			GCAssert(ref->get() == item);
		}
		return ref;
	}

	void GC::ClearWeakRef(const void *item)
	{
		GCWeakRef *ref = (GCWeakRef*) weakRefs.remove(item);
		GCAssert(weakRefs.get(item) == NULL);
		GCAssert(ref != NULL);
		GCAssert(ref->get() == item || ref->get() == NULL);
		if(ref) {
			ref->m_obj = NULL;
			item = GetRealPointer(item);
			if (GCLargeAlloc::IsLargeBlock(item)) {
				GCLargeAlloc::SetHasWeakRef(item, false);
			} else {
				GCAlloc::SetHasWeakRef(item, false);
			}
		}
	}

#ifdef _DEBUG
	
	void GC::WhitePointerScan(const void *mem, size_t size)
	{		
		uintptr_t *p = (uintptr_t *) mem;
		// the minus 8 skips the deadbeef and back pointer 
		uintptr_t *end = p + ((size) / sizeof(void*));

		while(p < end)
		{
			uintptr_t val = *p;		
			if(val == 0xdeadbeef)
				break;
			if(IsWhite((const void*) (val&~7)) && 
			   *(((int32_t*)(val&~7))+1) != (int32_t)0xcacacaca && // Free'd
			   *(((int32_t*)(val&~7))+1) != (int32_t)0xbabababa) // Swept
			{
#ifdef MMGC_MEMORY_INFO
				GCDebugMsg(false, "Object 0x%x allocated here:\n", mem);
				PrintStackTrace(mem);
				GCDebugMsg(false, "Didn't mark pointer at 0x%x, object 0x%x allocated here:\n", p, val);
				PrintStackTrace((const void*)(val&~7));
#endif
				GCAssert(false);
			}
			p++;
		}
	}

	void GC::FindMissingWriteBarriers()
	{
		MMGC_ASSERT_EXCLUSIVE_GC(this);

		if(!incrementalValidation)
			return;

		uintptr_t m = memStart;
		while(m < memEnd)
		{
			// divide by 4K to get index
			int bits = GetPageMapValue(m);
			switch(bits)
			{
			case 0:
				m += GCHeap::kBlockSize;
				break;
			case 3:
				{
					GCLargeAlloc::LargeBlock *lb = (GCLargeAlloc::LargeBlock*)m;
					const void *item = GetUserPointer((const void*)(lb+1));
					if(GCLargeAlloc::GetMark(item) && GCLargeAlloc::ContainsPointers(item)) {
						WhitePointerScan(item, lb->usableSize - DebugSize());
					}
					m += lb->GetNumBlocks() * GCHeap::kBlockSize;
				}
				break;
			case 1:
				{
					// go through all marked objects in this page
					GCAlloc::GCBlock *b = (GCAlloc::GCBlock *) m;
                    for (int i=0; i< b->alloc->m_itemsPerBlock; i++) {
                        // find all marked objects and search them
                        if(!GCAlloc::GetBit(b, i, GCAlloc::kMark))
                            continue;

						if(b->alloc->ContainsPointers()) {
	                        void* item = (char*)b->items + b->alloc->m_itemSize*i;
							WhitePointerScan(GetUserPointer(item), b->alloc->m_itemSize - DebugSize());
						}
					}
					m += GCHeap::kBlockSize;
				}
				break;
			default:
				GCAssert(false);
				break;
			}
		}
	}
#endif

	void *GC::heapAlloc(size_t siz, bool expand, bool zero)
	{
		void *ptr = heap->Alloc((int)siz, expand, zero);
		if(ptr)
			totalGCPages += siz;
		return ptr;
	}
	
	void GC::heapFree(void *ptr, size_t siz)
	{
		if(!siz)
			siz = heap->Size(ptr);
		totalGCPages -= siz;
		heap->Free(ptr);
	}	
	
	size_t GC::GetBytesInUse()
	{
		size_t bytes=0;
		for(int i=0; i < kNumSizeClasses; i++) {
			bytes += containsPointersRCAllocs[i]->GetBytesInUse();
			bytes += containsPointersAllocs[i]->GetBytesInUse();
			bytes += noPointersAllocs[i]->GetBytesInUse();
		}
		bytes += largeAlloc->GetBytesInUse();
		return bytes;
	}

	void GC::Poke()
	{
		ReapZCT();
		heap->Decommit();

		if(pendingStatusChange)
		{
			{
				USING_CALLBACK_LIST(this);
				for (GCCallback *cb = m_callbacks; cb; cb = cb->nextCB)
					cb->oom(statusTo);
			}
			pendingStatusChange = false;
		}
	}

 	void GC::allocaInit()
 	{
 		top_segment = NULL;
 		stacktop = NULL;
 #ifdef _DEBUG
 		stackdepth = 0;
 #endif
 		pushAllocaSegment(AVMPLUS_PARAM_ALLOCA_DEFSIZE);
 	}
 	
 	void GC::allocaShutdown()
 	{
 		while (top_segment != NULL)
 			popAllocaSegment();
 		top_segment = NULL;
 		stacktop = NULL;
 	}
 	
 	void GC::allocaPopToSlow(void* top)
 	{
 		GCAssert(top_segment != NULL);
 		GCAssert(!(top >= top_segment->start && top <= top_segment->limit));
 		while (!(top >= top_segment->start && top <= top_segment->limit))
 			popAllocaSegment();
 		GCAssert(top_segment != NULL);
 	}
 	
 	void* GC::allocaPushSlow(size_t nbytes)
 	{
 		size_t alloc_nbytes = nbytes;
 		if (alloc_nbytes < AVMPLUS_PARAM_ALLOCA_DEFSIZE)
 			alloc_nbytes = AVMPLUS_PARAM_ALLOCA_DEFSIZE;
 		pushAllocaSegment(alloc_nbytes);
 		void *p = stacktop;
 		stacktop = (char*)stacktop + nbytes;
 		return p;
 	}
 	
 	void GC::pushAllocaSegment(size_t nbytes)
 	{
 		GCAssert(nbytes % 8 == 0);
 #ifdef _DEBUG
 		stackdepth += nbytes;
 #endif
 		void* memory = AllocRCRoot(nbytes);
 		AllocaStackSegment* seg = new AllocaStackSegment;
 		seg->start = memory;
 		seg->limit = (void*)((char*)memory + nbytes);
 		seg->top = NULL;
 		seg->prev = top_segment;
 		if (top_segment != NULL)
 			top_segment->top = stacktop;
 		top_segment = seg;
 		stacktop = memory;
 	}
 	
 	void GC::popAllocaSegment()
 	{
 #ifdef _DEBUG
 		stackdepth -= (char*)top_segment->limit - (char*)top_segment->start;
 #endif
 		FreeRCRoot(top_segment->start);
 		AllocaStackSegment* seg = top_segment;
 		top_segment = top_segment->prev;
 		if (top_segment != NULL)
 			stacktop = top_segment->top;
 		delete seg;
 	} 

	GCAutoExit::GCAutoExit(GC *_gc) : gc(NULL)
	{
		// we only resest the stack if its not already set
		if(_gc->GetStackEnter() == 0) {
			gc = _gc;
			_gc->SetStackEnter(this);
		}
	}
	
	GCAutoExit::~GCAutoExit()
	{
		if(gc)
			gc->SetStackEnter(0);
	}

	void GC::SetStackEnter(void *enter) 
	{ 
		if(stackEnter == 0 || enter == 0)
			stackEnter = (uintptr_t)enter; 
	}
}
