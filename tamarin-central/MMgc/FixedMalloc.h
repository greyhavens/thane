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

#ifndef __Malloc__
#define __Malloc__

namespace MMgc
{
	/**
	 * A general purpose memory allocator using size classes 
	 */
	class MMGC_API FixedMalloc : public GCAllocObject
	{
		friend class GCHeap;
	public:
		// FixedMalloc is controlled by GCHeap now, these are just API compat stubs
		static void Init() {}
		static void Destroy() {}

		static FixedMalloc *GetInstance();

		inline void* Alloc(size_t size)
		{
			void *item;
			GCAssert(size + 3 > size);
			// overflow detection
			if(size+3 < size)
				return NULL;
			size = (size+3)&~3;
			if (size <= (size_t)kLargestAlloc) {
				item = FindSizeClass(size)->Alloc(size);
			} else {
				item = LargeAlloc(size);
			}
			return item;
		}

		inline void Free(void *item)
		{
			if(item == 0)
				return;

			// small things are never allocated on the 4K boundary b/c the block
			// header structure is stored there, large things always are		
			if(IsLargeAlloc(item)) {
				LargeFree(item);
			} else {		
				FixedAllocSafe::GetFixedAllocSafe(item)->Free(item);
			}
		}

		size_t Size(const void *item)
		{
			size_t size;
			if(IsLargeAlloc(item)) {
				size = LargeSize(item);
			} else {		
				size = FixedAlloc::Size(item);
			}
#ifdef MMGC_MEMORY_INFO
			size -= DebugSize();
#endif
			return size;
		}

		void *Calloc(size_t num, size_t elsize)
		{
			uint64_t size = (uint64_t)num * (uint64_t)elsize;
			if(size > 0xfffffff0) 
			{
				GCAssertMsg(false, "Attempted allocation overflows size_t\n");
				return NULL;
			}
			return Alloc(num * elsize);
		}

		size_t GetTotalSize();
		size_t GetBytesInUse();
		
	private:
		void _Init(GCHeap *heap);
		void _Destroy();
#ifdef MMGC_64BIT
		const static int kLargestAlloc = 2016;	
#else
		const static int kLargestAlloc = 2032;	
#endif
		const static int kNumSizeClasses = 41;
		const static int16_t kSizeClasses[kNumSizeClasses];
		const static uint8_t kSizeClassIndex[32];

		GCHeap *m_heap;
		FixedAllocSafe *m_allocs[kNumSizeClasses];	
		size_t numLargeChunks;

		FixedAllocSafe *FindSizeClass(size_t size) const;

		static bool IsLargeAlloc(const void *item)
		{
			// space made in ctor
			item = GetRealPointer(item);
			return ((uintptr_t) item & 0xFFF) == 0;
		}

		void *LargeAlloc(size_t size);	
		void LargeFree(void *item);
		size_t LargeSize(const void *item);
	};
}
#endif /* __Malloc__ */
