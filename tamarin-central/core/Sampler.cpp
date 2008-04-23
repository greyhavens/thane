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

	Sampler::Sampler(GC *gc) : allocId(1), sampling(true),
			autoStartSampling(false), samplingNow(false), takeSample(0),
			numSamples(0), samples(gc->GetGCHeap()), currentSample(NULL), timerHandle(0)
	{
		fakeMethodInfos = new (gc) Hashtable(gc);
	}

	void Sampler::setCore(AvmCore *core)
	{
		this->core = core;
		allocId = 1;
	}

	Sampler::~Sampler()
	{
		stopSampling();
	}

	void Sampler::init(bool sampling, bool autoStart)
	{
		this->sampling = sampling;
		this->autoStartSampling = autoStart;
	}

	byte *Sampler::getSamples(uint32 &num)
	{
		num = numSamples;
		byte *start = samples.start();
		return start;
	}

	void Sampler::sample()
	{		
		AvmAssertMsg(sampling, "How did we get here if sampling is disabled?");
		if(!samplingNow || !core->callStack || !sampleSpaceCheck())
			return;	
		writeRawSample(RAW_SAMPLE);
		numSamples++;
	}

	int Sampler::sampleSpaceCheck()
	{
		if(!samples.start())
			return 0;

		uint32 sampleSize = sizeof(Sample);
		uint32 callStackDepth = core->callStack ? core->callStack->depth : 0;
		sampleSize += callStackDepth * sizeof(StackTrace::Element);
		sampleSize += sizeof(uint64) * 2;
		while(currentSample + sampleSize > samples.uncommitted()) {
			samples.grow();
			if(currentSample + sampleSize > samples.uncommitted()) {
				// exhausted buffer
				stopSampling();
				return 0;
			}
		}
		return 1;
	}

	void Sampler::writeRawSample(SampleType sampleType)
	{
		CallStackNode *csn = core->callStack;
		uint32 depth = csn ? csn->depth : 0;
		byte *p = currentSample;
		write(p, GC::ticksToMicros(GC::GetPerformanceCounter()));
		write(p, sampleType);
		if(sampleType != DELETED_OBJECT_SAMPLE)
		{
			//if(depth == 0)
			//	AvmDebugMsg(false, "Please add SAMPLE_FRAME's to give this allocation some context.");
			write(p, depth);
			while(csn)
			{
				write(p, (uint32)csn->info);
				// FIXME: can filename can be stored in the AbstractInfo?
				write(p, csn->filename);
				write(p, csn->linenum);
				csn = csn->next;
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
		memset(&s, 0, sizeof(Sample));
		read(p, s.micros);
		read(p, s.sampleType);
		AvmAssertMsg(s.sampleType == RAW_SAMPLE || 
				s.sampleType == NEW_OBJECT_SAMPLE || 
				s.sampleType == DELETED_OBJECT_SAMPLE, "Sample stream corruption.\n");
		if(s.sampleType != DELETED_OBJECT_SAMPLE)
		{
			read(p, s.stack.depth);
			s.stack.trace = p;
			p += s.stack.depth * (sizeof(uint32) + sizeof(Stringp) + sizeof(int));
		}
		// padding to keep 8 byte alignment
		align(p);
		if(s.sampleType != Sampler::RAW_SAMPLE)
		{
			read(p, s.id);
			if(s.sampleType == Sampler::NEW_OBJECT_SAMPLE)
			{
				read(p, s.weakRef);
				read(p, s.typeOrVTable);
			}
			else 
			{
				read(p, s.size);
			}
		}
	}

	uint64 Sampler::recordAllocationSample(AvmPlusScriptableObject *obj, Atom  typeOrVTable)
	{
		AvmAssertMsg(sampling, "How did we get here if sampling is disabled?");
		if(!samplingNow)
			return 0;

		// TODO: investigate not pounding on weak refs
		GCWeakRef *weakRef = obj->GetWeakRef();
 
		// do space check after obtaining weak ref, it can trigger time and delete sample's
		if(!sampleSpaceCheck())
			return 0;

		if(typeOrVTable < 7 && core->codeContext() && core->codeContext()->domainEnv()) {
			// and in toplevel
			typeOrVTable |= (uint32)core->codeContext()->domainEnv()->toplevel();
		}

		writeRawSample(NEW_OBJECT_SAMPLE);
		write(currentSample, allocId++);
		write(currentSample, weakRef);
		write(currentSample, typeOrVTable);
		AvmAssertMsg((uintptr)currentSample % 4 == 0, "Alignment should have occurred at end of raw sample.\n");
		numSamples++;

		return allocId-1; // GetWeakRef might cause GC which will traverse samples, ++ early
	}

	void Sampler::recordDeallocationSample(uint64 id, uint64 size)
	{
		AvmAssertMsg(sampling, "How did we get here if sampling is disabled?");
		// recordDeallocationSample doesn't honor the samplingNow flag
		// this is to avoid dropping deleted object samples when sampling is paused.
		if(!sampleSpaceCheck()) {
			return;
		}
		AvmAssert(id != 0);
		writeRawSample(DELETED_OBJECT_SAMPLE);
		write(currentSample, id);
		write(currentSample, size);
		AvmAssertMsg((uintptr)currentSample % 4 == 0, "Alignment should have occurred at end of raw sample.\n");
		numSamples++;
	}

	void Sampler::clearSamples()
	{
		currentSample = samples.start();
		numSamples = 0;
	}

	void Sampler::startSampling()
	{
		if(!sampling || samplingNow)
			return;

		{
			init(sampling, autoStartSampling);
		}

		if (!currentSample)
		{
			int megs=256;
			while(!currentSample && megs >= 16) {
				currentSample = samples.reserve(megs * 1024 * 1024);
				megs >>= 1;
			}
			if(!currentSample) {
				sampling = autoStartSampling = false;
				samples.free();
				return;
			}
		}
		
		samplingNow = true;
		if(timerHandle == 0)
			timerHandle = OSDep::startIntWriteTimer(1, &takeSample);
	}

	void Sampler::pauseSampling()
	{
		if(!sampling || !samplingNow)
			return;
		samplingNow = false;
	}

	void Sampler::stopSampling()
	{
		if(!sampling)
			return;

		if(timerHandle != 0) {
			OSDep::stopTimer(timerHandle);
			timerHandle = 0;
		}

		samples.free();

		samplingNow = false;
		numSamples = 0;
		currentSample = NULL;
	}

	void Sampler::initSampling()
	{
		if(!sampling)
			return;
		// prime fake function table
		createFakeFunction("[mark]");
		createFakeFunction("[sweep]");
		createFakeFunction("[reap]");
		createFakeFunction("[generate]");
		createFakeFunction("[verify]");
		createFakeFunction("[newclass]");

		if(autoStartSampling) {
			startSampling();
		}
		return;
	}

	void Sampler::createFakeFunction(const char *name)
	{
		if(!sampling)
			return;

		AvmAssert(core->builtinPool != NULL);
		Atom nameAtom = core->constant(name);
		Atom a = fakeMethodInfos->get(nameAtom);
		AbstractFunction *af = (AbstractFunction*)AvmCore::atomToGCObject(a);
		if(!af)
		{
			af = new (core->GetGC()) FakeAbstractFunction(AvmCore::atomToString(nameAtom));
			fakeMethodInfos->add(nameAtom, AvmCore::gcObjectToAtom(af));
			af->pool = core->builtinPool;
		}
	}

	AbstractFunction *Sampler::getFakeFunction(const char *name)
	{
		// this can't make any allocations, its called from sensitive areas (like from 
		// the GC marking routines).  For one we'll recurse but also GC state can get messed
		// up if for instance the allocation triggers a collection
		Stringp name_str = core->findInternedString(name, (int)strlen(name));
		if(name_str == NULL)
			return NULL;
		Atom a = fakeMethodInfos->get(name_str->atom());
		AvmAssertMsg(a != undefinedAtom, "name was interned but need to call createFakeFunction with that name");
		AbstractFunction *af = (AbstractFunction*)AvmCore::atomToGCObject(a);
		sampleCheck();
		return af;
	}

	FakeCallStackNode::FakeCallStackNode(AvmCore *core, const char *name)
	{
		memset(this, 0, sizeof(FakeCallStackNode));
		this->core = core;
		if(core)
		{
			AbstractFunction *af = core->sampler()->getFakeFunction(name);
			if(af)
				initialize(NULL, af, NULL, NULL, 0, NULL, NULL);
			else {
				// this is how the dtor knows what to do
				this->core = NULL;
			}
		}
	}

	FakeCallStackNode::~FakeCallStackNode()
	{
		if(core)
		{
			exit();
			core->sampleCheck();
		}
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
				if (s.weakRef)
				{
					GC::SetMark(s.weakRef);
				}
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
		uint32 num;
		byte *p = getSamples(num);
		for(uint32 i=0; i < num; i++)
		{
			Sample s;
			readSample(p, s);
			if(s.sampleType == NEW_OBJECT_SAMPLE) {
				if (s.weakRef && !s.weakRef->get())
				{				
					rewind(p, sizeof(GCWeakRef*)*2);
					write(p, (void *)0);
					p += sizeof(GCWeakRef*);
				}
			}
		}
	}
}
#endif
