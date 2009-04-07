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

#include "avmplus.h"

// Adobe patent application tracking #P721, entitled Application Profiling, inventors: T. Reilly

#ifdef DEBUGGER
namespace avmplus
{
	using namespace MMgc;

	// store this in a thread local to capture FixedAlloc alloc traffic
	GCThreadLocal<avmplus::Sampler*> tls_sampler;

	/* static */
	void recordAllocationSample(const void* item, size_t size)
	{
		avmplus::Sampler* sampler = tls_sampler;
		if (sampler && sampler->sampling())
			sampler->recordAllocationSample(item, size);
	}

	/* static */
	void recordDeallocationSample(const void* item, size_t size)
	{
		avmplus::Sampler* sampler = tls_sampler;
		if( sampler /*&& sampler->sampling*/ )
			sampler->recordDeallocationSample(item, size);
	}

	Sampler::Sampler(AvmCore* _core) : 
		GCRoot(_core->GetGC()),
	    sampleIteratorVTable(NULL),
	    slotIteratorVTable(NULL),
		core(_core),
		fakeMethodNames(_core->GetGC()),
		allocId(1), 
		samples(NULL),
		currentSample(NULL),
		lastAllocSample(NULL),
		callback(NULL),
		timerHandle(0),
		uids(1024, GCHashtable::OPTION_MALLOC),
		ptrSamples(NULL),
		takeSample(0),
		numSamples(0), 
		samples_size(0),
		samplingNow(false),
		samplingAllAllocs(false),
		runningCallback(false),
		autoStartSampling(false),
		_sampling(true)
	{
		_core->GetGC()->GetGCHeap()->EnableHooks();
 		tls_sampler = this;
	}

	Sampler::~Sampler()
	{
		stopSampling();
 		Sampler* tls = tls_sampler;
 		if (tls == this)
 			tls_sampler = NULL;
	}

	void Sampler::init(bool sampling, bool autoStart)
	{
		this->_sampling = sampling;
		this->autoStartSampling = autoStart;
	}

	byte *Sampler::getSamples(uint32 &num)
	{
		num = numSamples;
		byte *start = samples;
		return start;
	}

	void Sampler::sample()
	{		
		AvmAssertMsg(sampling(), "How did we get here if sampling is disabled?");
		if(!samplingNow || !core->callStack || !sampleSpaceCheck())
			return;	
		writeRawSample(RAW_SAMPLE);
		numSamples++;
	}

	int Sampler::sampleSpaceCheck(bool callback_ok)
	{
		if(!samples)
			return 0;

		uint32 sampleSize = sizeof(Sample);
		uint32 callStackDepth = core->callStack ? core->callStack->depth() : 0;
		sampleSize += callStackDepth * sizeof(StackTrace::Element);
		sampleSize += sizeof(uint64) * 2;
		if( callback && callback_ok && !runningCallback && currentSample+sampleSize+samples_size/3 > (samples + samples_size)
			&& !core->GetGC()->Collecting() 
			&& !core->GetGC()->Reaping()
			)		{
			runningCallback = true;
			pauseSampling();
			Atom args[1] = { nullObjectAtom };
			Atom ret = callback->call(0, args);
			if( ret == falseAtom)
				stopSampling();
			else
				startSampling();
			runningCallback = false;
		}
		if(currentSample + sampleSize > samples+samples_size) {
/*
#ifdef AVMPLUS_VERBOSE
			core->console << "****** Exhausted Sample Buffer *******\n";
#endif
*/
			// exhausted buffer
			stopSampling();
			return 0;
		}
		return 1;
	}

	void Sampler::writeRawSample(SampleType sampleType)
	{
		CallStackNode *csn = core->callStack;
		uint32 depth = csn ? csn->depth() : 0;
		byte *p = currentSample;
		write(p, GC::ticksToMicros(VMPI_getPerformanceCounter()));
		write(p, sampleType);
		if(sampleType != DELETED_OBJECT_SAMPLE)
		{
			//if(depth == 0)
			//	AvmDebugMsg(false, "Please add SAMPLE_FRAME's to give this allocation some context.");
			write(p, depth);
			while(csn)
			{
				write(p, csn->info());
				write(p, csn->fakename());
				// FIXME: can filename can be stored in the AbstractInfo?
				write(p, csn->filename());
				write(p, csn->linenum());
#ifdef AVMPLUS_64BIT
				AvmAssert(sizeof(StackTrace::Element) == sizeof(MethodInfo *) + sizeof(Stringp) + sizeof(Stringp) + sizeof(int32_t) + sizeof(int32_t));
				write(p, (int) 0); // structure padding
#endif
				csn = csn->next();
				depth--;
			}
			AvmAssert(depth == 0);
		}
		// padding to keep 8 byte alignment
		align(p);
		currentSample = p;
		takeSample = 0;
	}

	void Sampler::readSample(byte *&p, Sample &s)
	{
		VMPI_memset(&s, 0, sizeof(Sample));
		read(p, s.micros);
		read(p, s.sampleType);
		AvmAssertMsg(s.sampleType == RAW_SAMPLE || 
				s.sampleType == NEW_OBJECT_SAMPLE || 
				s.sampleType == DELETED_OBJECT_SAMPLE ||
				s.sampleType == NEW_AUX_SAMPLE, "Sample stream corruption.\n");

		if(s.sampleType != DELETED_OBJECT_SAMPLE)
		{
			read(p, s.stack.depth);
			s.stack.trace = p;
#ifndef AVMPLUS_64BIT
			AvmAssert(sizeof(StackTrace::Element) == sizeof(MethodInfo *) + sizeof(Stringp) + sizeof(Stringp) + sizeof(int32_t));
#else
			// Extra int because of the structure padding
			AvmAssert(sizeof(StackTrace::Element) == sizeof(MethodInfo *) + sizeof(Stringp) + sizeof(Stringp) + sizeof(int32_t) + sizeof(int32_t));
#endif
			p += s.stack.depth * sizeof(StackTrace::Element);
		}
		// padding to keep 8 byte alignment
		align(p);
		if(s.sampleType != Sampler::RAW_SAMPLE)
		{
			read(p, s.id);
			if(s.sampleType == Sampler::NEW_OBJECT_SAMPLE || s.sampleType == Sampler::NEW_AUX_SAMPLE)
			{
				read(p, s.ptr);
				read(p, s.typeOrVTable);
				read(p, s.alloc_size);
			}
			else 
			{
				read(p, s.size);
			}
		}
	}

	uint64 Sampler::recordAllocationSample(const void* item, uint64 size, bool callback_ok)
	{
		AvmAssertMsg(sampling(), "How did we get here if sampling is disabled?");
		if(!samplingNow)
			return 0;

		if(!samplingAllAllocs)
			return 0;

		if(!sampleSpaceCheck(callback_ok))
			return 0;

		(void)item;

		lastAllocSample = currentSample;
		writeRawSample(NEW_AUX_SAMPLE);
		uint64 uid = allocId++;
		uids.add(item, (void*)uid);
		write(currentSample, uid);
		write(currentSample, item);
		write(currentSample, (uintptr)0);
		write(currentSample, size);

		AvmAssertMsg((uintptr)currentSample % 4 == 0, "Alignment should have occurred at end of raw sample.\n");
		numSamples++;

		return uid; 
	}

	uint64 Sampler::recordAllocationInfo(AvmPlusScriptableObject *obj, uintptr typeOrVTable)
	{
		AvmAssertMsg(sampling(), "How did we get here if sampling is disabled?");
		if(!samplingNow)
			return 0;

		if( !samplingAllAllocs )
		{
			// Turn on momentarily to record the alloc for this object.
			samplingAllAllocs = true;
			recordAllocationSample(obj, 0);
			samplingAllAllocs = false;
		}

		byte* old_sample = lastAllocSample;
		Sample s;
		readSample(old_sample, s);
		old_sample = lastAllocSample;

		if(typeOrVTable < 7 && core->codeContext() && core->codeContext()->domainEnv()) {
			// and in toplevel
			typeOrVTable |= (uintptr)core->codeContext()->domainEnv()->toplevel();
		}

		AvmAssertMsg(s.sampleType == NEW_AUX_SAMPLE, "Sample stream corrupt - can only add info to an AUX sample.\n");
		AvmAssertMsg(s.ptr == (void*)obj, "Sample stream corrupt - last sample is not for same object.\n");

		byte* pos = currentSample;
		currentSample = old_sample;
		// Rewrite the sample as a NEW_OBJECT_SAMPLE
		writeRawSample(NEW_OBJECT_SAMPLE);

		write(currentSample, s.id);

		AvmAssertMsg( ptrSamples->get(obj)==0, "Missing dealloc sample - same memory alloc'ed twice.\n");
		ptrSamples->add(obj, currentSample);

		write(currentSample, s.ptr);

		write(currentSample, typeOrVTable);
		write(currentSample, s.alloc_size);

		AvmAssertMsg((uintptr)currentSample % 4 == 0, "Alignment should have occurred at end of raw sample.\n");
		currentSample = pos;

		return s.id;
	}

	void Sampler::recordDeallocationSample(const void* item, uint64 size)
	{
		AvmAssertMsg(sampling(), "How did we get here if sampling is disabled?");
		AvmAssert(item != 0);
		// recordDeallocationSample doesn't honor the samplingNow flag
		// this is to avoid dropping deleted object samples when sampling is paused.
		uint64 uid = (uint64)uids.get(item);
		// If we didn't find a UID then this wasn't memory that the sampler knew was allocated
		if(uid && sampleSpaceCheck(false)) {

			
//			if( !uid )
//				uid = (uint64)-1;

			writeRawSample(DELETED_OBJECT_SAMPLE);
			write(currentSample, uid);
			write(currentSample, size);

			numSamples++;

			AvmAssertMsg((uintptr)currentSample % 4 == 0, "Alignment should have occurred at end of raw sample.\n");
		}

		// Nuke the ptr in the sample stream for the newobject sample
		if( samples )
		{

		byte* oldptr = 0;
		if( (oldptr = (byte*)ptrSamples->get(item)) != 0 )
		{
#ifdef _DEBUG
				void* oldval = 0;
				read(oldptr, oldval);
				AvmAssertMsg(oldval==item, "Sample stream corrupt, dealloc doesn't point to correct address");
				rewind(oldptr, sizeof(void*));
#endif
			write(oldptr, (void*)0);
			ptrSamples->remove(item);
		}
		}
		if(uid)
			uids.remove(item);
	}

	void Sampler::clearSamples()
	{
		//samples->free();
		currentSample = samples;
		GCHashtable* t = ptrSamples;
		ptrSamples = new MMgc::GCHashtable(4096, GCHashtable::OPTION_MALLOC);
		delete t;
		numSamples = 0;
	}

	void Sampler::startSampling()
	{
		if (!_sampling || samplingNow)
			return;

		if (!currentSample)
		{
			int megs=16;
			while(!currentSample && megs > 0) {
				samples_size = megs*1024*1024;
				currentSample = samples = new byte[samples_size];
				megs >>= 1;
			}
			if(!currentSample) {
				_sampling = autoStartSampling = false;
				return;
			}
		}

		init(_sampling, autoStartSampling);
		
		if( !ptrSamples ) 
		{
			ptrSamples = new MMgc::GCHashtable(1024, GCHashtable::OPTION_MALLOC);
		}

		samplingNow = true;
		if(timerHandle == 0)
			timerHandle = OSDep::startIntWriteTimer(1, &takeSample);
	}

	void Sampler::pauseSampling()
	{
		if (!_sampling || !samplingNow)
			return;
		samplingNow = false;
	}

	void Sampler::sampleInternalAllocs(bool b)
	{
		samplingAllAllocs = b;
	}

	void Sampler::setCallback(ScriptObject* callback)
	{
		this->callback = callback;
	}

	void Sampler::stopSampling()
	{
		if (!_sampling)
			return;

		if( samples )
			delete [] samples;
		samples = 0;
		samples_size = 0;

		if(timerHandle != 0) {
			OSDep::stopTimer(timerHandle);
			timerHandle = 0;
		}

		if( ptrSamples ) {
			delete ptrSamples;
			ptrSamples = 0;
		}

		samplingNow = false;
		numSamples = 0;
		currentSample = NULL;
	}

	void Sampler::initSampling()
	{
		if (!_sampling)
			return;

		// prime fake function table

		createFakeFunction("XML");
		createFakeFunction("Array");
		createFakeFunction("Date");

		createFakeFunction("[mark]");
		createFakeFunction("[sweep]");
		createFakeFunction("[reap]");
		createFakeFunction("[generate]");
		createFakeFunction("[verify]");
		createFakeFunction("[newclass]");

		createFakeFunction("[activation-object]");

		if(autoStartSampling) {
			startSampling();
		}
		return;
	}

	void Sampler::createFakeFunction(const char *name)
	{
		if (!_sampling)
			return;

		Stringp s = core->internConstantStringLatin1(name);
		// save it in fakeMethodNames just to be sure it isn't cleared from the intern-name list.
		if (fakeMethodNames.indexOf(s) < 0)
			fakeMethodNames.add(s);
	}

	Stringp Sampler::getFakeFunctionName(const char* name)
	{
		// this can't make any allocations, it's called from sensitive areas (like from 
		// the GC marking routines).  For one we'll recurse but also GC state can get messed
		// up if for instance the allocation triggers a collection
		Stringp name_str = core->findInternedString(name, (int)VMPI_strlen(name));
		if (name_str != NULL)
		{
			sampleCheck();
		}
		return name_str;
	}

	/* sample data has pointers need to scan */
	void Sampler::presweep()
	{
		uint32 num;
		byte *p = getSamples(num);
		for(uint32 i=0; i < num ; i++)
		{
			Sample s;
			readSample(p, s);			
			if(s.sampleType == NEW_OBJECT_SAMPLE) {
				// keep all weak refs and type's live, in postsweep we'll erase our weak refs
				// to objects that were finalized.  we can't nuke them here b/c pushing the
				// types could cause currently unmarked things to become live
				if (s.typeOrVTable > 7 && !GC::GetMark((void*)s.typeOrVTable))
				{
					GCWorkItem item((void*)s.typeOrVTable, (uint32)GC::Size((void*)s.typeOrVTable), true);
					core->gc->PushWorkItem(item);
				}
			}
		}
	}

	void Sampler::postsweep()
	{
	}

}
#endif // DEBUGGER
