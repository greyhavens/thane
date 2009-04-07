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

#include "MMgc.h"

#if defined(MMGC_MEMORY_INFO) || defined(MMGC_MEMORY_PROFILER)

namespace MMgc
{

#ifdef MMGC_MEMORY_PROFILER
	// increase this to get more info
	const int kMaxStackTrace = 16; // RtlCaptureStackBackTrace stops working when this is 32
	const int kNumTypes = 10;
	const int kNumTracesPerType = 5;

	// include total and swept memory totals in memory profiling dumps as opposed to just "live"
	const bool showTotal = false;	
	const bool showSwept = false;

	class StackTrace : public GCAllocObject
	{
	public:
		StackTrace(uintptr_t *trace) 
		{ 
			VMPI_memset(this, 0, sizeof(StackTrace));
			VMPI_memcpy(ips, trace, kMaxStackTrace * sizeof(void*));
		}
		uintptr_t ips[kMaxStackTrace];
		size_t skip;
		size_t size;
		size_t totalSize;
		size_t sweepSize;
		const char *package;
		const char *category;
		const char *name;
		size_t count;
		size_t totalCount;
		size_t sweepCount;
		StackTrace *master;
	};

	GCThreadLocal<const char*> memtag;
	GCThreadLocal<const void*> memtype;
	StackTrace *GetStackTrace();
	// print stack trace of caller
	void DumpStackTrace(int skip=1);

	MemoryProfiler::MemoryProfiler() : 
		traceTable(128, GCHashtable::OPTION_MALLOC | GCHashtable::OPTION_MT),
		stackTraceMap(128, GCHashtable::OPTION_MALLOC | GCHashtable::OPTION_MT),
		nameTable(128, GCHashtable::OPTION_MALLOC | GCHashtable::OPTION_STRINGS)
	{
	}

	MemoryProfiler::~MemoryProfiler()
	{
		GCHashtableIterator traceIter(&stackTraceMap);
		const void *obj;
		while((obj = traceIter.nextKey()) != NULL)
		{
			StackTrace *trace = (StackTrace*)traceIter.value();
			delete trace;
		}
		GCHashtableIterator nameIter(&nameTable);
		while((obj = nameIter.nextKey()) != NULL)
		{
			VMPI_free((void*)nameIter.value());			
		}
	}

	const char *MemoryProfiler::GetAllocationNameFromTrace(StackTrace *trace)
	{
		if(trace->name)
			return trace->name;

		char *name=NULL;

		int i=0;
		uintptr_t ip;
		while((ip = trace->ips[i++]) != 0) {
			// everytime we lookup an ip cache the result
			name = (char*)nameTable.get(ip);
			if(!name) {
				name = (char*)VMPI_alloc(256);
				if(VMPI_getFunctionNameFromPC(ip, name, 256) == false)
				{
					VMPI_snprintf(name, 256, "0x%x", ip);
				}
				nameTable.put((const void*)ip, name);
			}
			// keep going until we hit mutator code
			if(VMPI_strstr(name, "::Alloc") != NULL ||
				VMPI_strstr(name, "::LargeAlloc") != NULL ||
				VMPI_strstr(name, "::Calloc") != NULL ||
				VMPI_strstr(name, "operator new") != NULL) 
			{
				trace->skip++;
				continue;
			}
			break;
		}
		trace->name = name;
		return name;
	}

	StackTrace *MemoryProfiler::GetAllocationTrace(const void *obj)
	{
		return (StackTrace*)traceTable.get(obj);
	}

	const char * MemoryProfiler::GetAllocationName(const void *obj)
	{
		StackTrace *trace = (StackTrace*)traceTable.get(obj);
		return GetAllocationNameFromTrace(trace);
	}

	const char *MemoryProfiler::GetAllocationCategory(StackTrace *trace)
	{
		if(trace->master)
			trace = trace->master;
		if(trace->category)
			return trace->category;
		const char *cat = GetAllocationNameFromTrace(trace);
		trace->category = cat;
		return cat;
	}

	void ChangeSize(StackTrace *trace, int delta)
	{ 
		trace->size += delta; 
		trace->count += (delta > 0) ? 1 : -1;
		GCAssert(trace->count != 0 || trace->size == 0);
		if(delta > 0) {
			trace->totalSize += delta; 
			trace->totalCount++; 
		}
	}

	void MemoryProfiler::Alloc(const void *item, size_t size)
	{
		StackTrace *trace = GetStackTrace();
		traceTable.put(item, trace);

		ChangeSize(trace, (int)size);

		if(memtype)
			trace->master = (StackTrace*)traceTable.get(memtype);
		memtype = NULL;
		
		if(memtag)
			trace->category = memtag;
		memtag = NULL;
	}

	void MemoryProfiler::Free(const void *item, size_t size)
	{
		StackTrace *trace = (StackTrace*)traceTable.get(item);

		ChangeSize(trace, -1 * int(size));
		// FIXME: how to know this is a sweep?
#if 0
		if(poison == 0xba) {
			trace->sweepSize += size;	
			trace->sweepCount++;
		}
#endif
	}
	
	StackTrace *MemoryProfiler::GetStackTrace()
	{
		uintptr_t trace[kMaxStackTrace];
		VMPI_memset(trace, 0, sizeof(trace));

		VMPI_captureStackTrace(trace, kMaxStackTrace, 2);
		StackTrace *st = (StackTrace*)stackTraceMap.get(trace); 
		if(!st) {
			st = new StackTrace(trace);
			stackTraceMap.put(st, st);
		}
		return st;
	}
	
	class PackageGroup : public GCAllocObject
	{
	public:
		PackageGroup(const char *name) : categories(16, GCHashtable::OPTION_MALLOC), name(name), size(0), count(0) {}
		const char *name;
		size_t size;
		size_t count;
		GCHashtable categories; // key == category name, value == CategoryGroup*
	};

	// data structure to gather allocations by type with the top 5 traces
	class CategoryGroup : public GCAllocObject
	{
	public:
		CategoryGroup(const char *name) : name(name), size(0), count(0) 
		{
			VMPI_memset(traces, 0, sizeof(traces));
		}
		const char *name;
		size_t size;
		size_t count;
		// biggest kNumTracesPerType traces
		StackTrace *traces[kNumTracesPerType ? kNumTracesPerType : 1];
	};

	const char *MemoryProfiler::Intern(const char *name, size_t len)
	{
		// input doesn't have to be zero terminated
		char *buff = (char*)alloca(len+1);
		VMPI_strncpy(buff, name, len);
		buff[len]='\0';
		char *iname = (char*)nameTable.get(buff);
		if(iname)
			return iname;
		iname = (char*)VMPI_alloc(len+1);
		VMPI_strncpy(iname, name, len);
		iname[len]='\0';
		nameTable.put(iname, iname);
		return iname;
	}

	// TODO duplicate code in here and in GetAllocationName
	const char *MemoryProfiler::GetPackage(StackTrace *trace)
	{
		if(trace->package)
			return trace->package;
		const char *name = GetAllocationNameFromTrace(trace);
		const char *colons = name ? VMPI_strstr(name, "::") : NULL;
		const char *package="global";
		if(colons) {
			colons += 2;
			// two sets of colons indicates a namespace
			const char *colons2 = VMPI_strstr(colons, "::");
			if(colons2)
				package = Intern(name, colons-name);
		}
		trace->package = package;
		return package;
	}

#define PERCENT(all, some) (((double)some*100.0)/(double)all)

	void MemoryProfiler::DumpFatties()
	{
		GCHashtable packageTable(128, GCHashtable::OPTION_MALLOC);

		size_t residentSize=0;
		size_t residentCount=0;
		size_t packageCount=0;

		FILE *out = GCHeap::GetGCHeap()->GetSpyFile();

		// rip through all allocation sites and sort into package and categories
		GCHashtableIterator iter(&stackTraceMap);
		const void *obj;
		while((obj = iter.nextKey()) != NULL)
		{
			StackTrace *trace = (StackTrace*)iter.value();
			size_t size;

			if(showSwept) {
				size = trace->sweepSize;
			} else if(showTotal) {
				size = trace->totalSize;
			} else {
				size = trace->size;
			}

			if(size == 0)
				continue;

			residentSize += size;

			size_t count = trace->master != NULL ? 0 : trace->count;
			residentCount += trace->count;

			const char *pack = GetPackage(trace);
			PackageGroup *pg = (PackageGroup*) packageTable.get((void*)pack);
			if(!pg) {
				pg = new PackageGroup(pack);
				packageTable.add(pack, pg);
				packageCount++;
			}
			pg->size += size;
			pg->count += count;

			const char *cat = GetAllocationCategory(trace);			
			CategoryGroup *tg = (CategoryGroup *) pg->categories.get((void*)cat);
			if(!tg)  {
				tg = new CategoryGroup(cat);
				pg->categories.add((void*)cat, tg);
			}			
			tg->size += size;
			tg->count += count;

			// insertion sort StackTrace
			for(int j=0; j < kNumTracesPerType; j++) {
				if(tg->traces[j] == trace)
					break;
			  if(!tg->traces[j] || tg->traces[j]->size < size) {
					if(j != kNumTracesPerType-1) {
						VMPI_memmove(&tg->traces[j+1], &tg->traces[j], (kNumTracesPerType-j-1)*sizeof(void*));
					}
					tg->traces[j] = trace;
					break;
				}
			}
		}

		// reporting time....
		PackageGroup **packages = (PackageGroup**)alloca(packageCount*sizeof(PackageGroup*));
		VMPI_memset(packages, 0, packageCount*sizeof(PackageGroup*));

		GCHashtableIterator pack_iter(&packageTable);
		const char *package;
		while((package = (const char*)pack_iter.nextKey()) != NULL)
		{
			PackageGroup* pg = (PackageGroup*)pack_iter.value();
			for(unsigned j=0; j<packageCount; j++) {
				if(packages[j] == NULL || packages[j]->size < pg->size) {
					if(j != packageCount-1) {
						VMPI_memmove(&packages[j+1], &packages[j], (packageCount-j-1)*sizeof(PackageGroup*));
					}
					packages[j]=pg;
					break;
				}
			}				
		}

		fprintf(out, "\n\nMemory allocation report for %u allocations, totaling %u kb (%u ave) across %u packages\n", residentCount, residentSize>>10, residentSize / residentCount, packageCount);
		for(unsigned i=0; i<packageCount; i++)
		{
			PackageGroup* pg = packages[i];

			int numTypes = pg->categories.count();

			if(numTypes == 0)
				continue;

			// sort CategoryGroup's into this array
			CategoryGroup **residentFatties = (CategoryGroup**) alloca(numTypes * sizeof(CategoryGroup *));
			VMPI_memset(residentFatties, 0, numTypes * sizeof(CategoryGroup *));
			GCHashtableIterator iter(&pg->categories);
			const char *name;
			while((name = (const char*)iter.nextKey()) != NULL)
			{
				CategoryGroup *tg = (CategoryGroup*)iter.value();
				// TODO refactor insertion sort into sub routine
				for(int j=0; j<numTypes; j++) {
					if(!residentFatties[j]) {
						residentFatties[j] = tg;
						break;
					}
					if(residentFatties[j]->size < tg->size) {
						if(j != numTypes-1) {
							VMPI_memmove(&residentFatties[j+1], &residentFatties[j], (numTypes-j-1) * sizeof(CategoryGroup *));
						}
						residentFatties[j] = tg;
						break;
					}
				}			
			}
			
			fprintf(out, "%s - %3.1f%% - %u kb %u items, avg %u b\n", pg->name, PERCENT(residentSize, pg->size),  (unsigned int)pg->size>>10, pg->count, (unsigned int)(pg->count ? pg->size/pg->count : 0));
				
			// result capping
			if(numTypes > kNumTypes)
				numTypes = kNumTypes;

			for(int i=0; i < numTypes; i++)
			{
				CategoryGroup *tg = residentFatties[i];
				if(!tg) 
					break;
				fprintf(out, "\t%s - %3.1f%% - %u kb %u items, avg %u b\n", tg->name, PERCENT(residentSize, tg->size), (unsigned int)tg->size>>10, tg->count, (unsigned int)(tg->count ? tg->size/tg->count : 0));
				for(int j=0; j < kNumTracesPerType; j++) {
					StackTrace *trace = tg->traces[j];
					if(trace) {
						size_t size = trace->size;
						size_t count = trace->count;
						if(showSwept) {
							size = trace->sweepSize;
							count = trace->sweepCount;
						} else if(showTotal) {
							size = trace->totalSize;
							count = trace->totalCount;
						}
						fprintf(out,"\t\t %3.1f%% - %u kb - %u items - ", PERCENT(tg->size, size), size>>10, count);
						PrintStackTraceByTrace(trace);
					}
				}
			}
		}

		GCHashtableIterator pi(&packageTable);
		while(pi.nextKey() != NULL)
		{
			PackageGroup* pg = (PackageGroup*)pi.value();
			GCHashtableIterator iter(&pg->categories);
			while(iter.nextKey() != NULL)
				delete (CategoryGroup*)iter.value();
			delete pg;
		}
	}
	
	void SetMemTag(const char *s)
	{
		if(memtag == NULL)
			memtag = s;
	}
	void SetMemType(const void *s)
	{
		GCAssert(GC::GetGC(s)->IsGCMemory(s));
		if(memtype == NULL)
			memtype = s;
	}
	
	void DumpStackTraceHelper(uintptr_t *trace)
	{
		char out[2048];
		char *tp = out;
		*tp++ = '\n';
		for(int i=0; trace[i] != 0; i++) {
			char buff[256];
			*tp++ = '\t';		*tp++ = '\t';		*tp++ = '\t';		
			if(VMPI_getFunctionNameFromPC(trace[i], buff, sizeof(buff)) == false)
			{
				VMPI_snprintf(buff, sizeof(buff), "0x%x", trace[i]);
			}
			VMPI_strncpy(tp, buff, sizeof(buff));
			tp += VMPI_strlen(buff);

			uint32_t lineNum;
			if(VMPI_getFileAndLineInfoFromPC(trace[i], buff, sizeof(buff), &lineNum) == false)
			{
				VMPI_snprintf(buff, sizeof(buff), "0x%x", trace[i]);
			}
			else
			{
				VMPI_snprintf(buff, sizeof(buff), "%s:%d", buff, lineNum);
			}
			*tp++ = '(';
			VMPI_strncpy(tp, buff, sizeof(buff));
			tp += VMPI_strlen(buff);
			*tp++ = ')';
			tp += VMPI_sprintf(tp, " - 0x%x", (unsigned int) trace[i]);
			*tp++ = '\n';
			if(tp - out > 1500) {
				GCLog(out);
				GCLog("\n");
				tp = out;
			}
		}
		*tp++ = '\n';
		*tp = '\0';

		fputs(out, GCHeap::GetGCHeap()->GetSpyFile());
	}

	void PrintStackTraceByTrace(StackTrace *trace)
	{
		DumpStackTraceHelper(&trace->ips[trace->skip]);
	}

	void PrintStackTrace(const void *item)
	{
		if(GCHeap::GetGCHeap()->GetProfiler()) {
			StackTrace *trace = GCHeap::GetGCHeap()->GetProfiler()->GetAllocationTrace(item);
			GCAssertMsg(trace != NULL, "Trace was null");
			PrintStackTraceByTrace(trace);
		}
	}

	const char* GetAllocationName(const void *obj)
	{
		if(GCHeap::GetGCHeap()->GetProfiler())
			return GCHeap::GetGCHeap()->GetProfiler()->GetAllocationName(obj);
		return NULL;
	}

	void ChangeSizeForObject(const void *object, int size)
	{
		if(GCHeap::GetGCHeap()->GetProfiler()) {
			StackTrace *st = GCHeap::GetGCHeap()->GetProfiler()->GetAllocationTrace(object);
			if(st)
				st->size += size;
			GCAssert(int(st->size) >= 0);
		}
	}

	unsigned GCStackTraceHashtable::equals(const void *k, const void *k2)
	{
		if(k == NULL || k2 == NULL)
			return false;
		return VMPI_memcmp(k, k2, kMaxStackTrace * sizeof(void*)) == 0;
	}

	unsigned GCStackTraceHashtable::hash(const void *k)
	{
		const int *array = (const int*)k;
		int hash = 0;
		for(int i=0;i<kMaxStackTrace; i++)
			hash += array[i];
		return hash;
	}

#else

	void PrintStackTrace(const void *) {}
	const char* GetAllocationName(const void *) { return NULL; }
	void ChangeSizeForObject(const void *, int ) {}

#endif


#ifdef MMGC_MEMORY_INFO

// end user servicable parts
 
	size_t DebugSize()
	{ 
	#ifdef MMGC_64BIT
		// Our writeback pointer is 8 bytes so we need to round up to the next 8 byte
		// size.  (only 5 DWORDS are used)
		return 6 * sizeof(int); 
	#else
		return 4 * sizeof(int); 
	#endif
	}

	/* 
	 * allocate the memory such that we can detect underwrites, overwrites and remember
	 * the allocation stack in case of a leak.   Memory is laid out like so:
	 *
	 * first four bytes == size / 4 
	 * second four bytes == stack trace index
	 * size data bytes
	 * 4 bytes == 0xdeadbeef
	 * last 4/8 bytes - writeback pointer
	 *
	 * Its important that the stack trace index is not stored in the first 4 bytes,
	 * it enables the leak detection to work see ~FixedAlloc.  Underwrite detection isn't
	 * perfect, an assert will be fired if the stack table index is invalid (greater than
	 * the table size or to an unused table entry) or if the size gets mangled and the
	 * end tag isn't at mem+size.  
	*/
	void DebugDecorate(const void *item, size_t size)
	{
		item = GetRealPointer(item);

		int *mem = (int*)item;
		// set up the memory
		*mem++ = (int)size;
		*mem++ = 0;
		mem += (size>>2);
		*mem++ = 0xdeadbeef;
		*mem = 0;
	#ifdef MMGC_64BIT
		*(mem+1) = 0;
		*(mem+2) = 0;
	#endif
	}

	void DebugFreeHelper(const void *item, int poison, size_t wholeSize)
	{
		int *ip = (int*) item;
		int size = *ip;
 		int *endMarker = ip + 2 + (size>>2);

		// clean up
		*ip = 0;
		ip += 2;

		// this can be called twice on some memory in inc gc 
		if(size == 0)
			return;

		if (*endMarker != (int32_t)0xdeadbeef)
		{
			// if you get here, you have a buffer overrun.  The stack trace about to
			// be printed tells you where the block was allocated from.  To find the
			// overrun, put a memory breakpoint on the location endMarker is pointing to.
			GCDebugMsg("Memory overwrite detected\n", false);
			PrintStackTrace(item);
			GCAssert(false);
		}

		// size is the non-Debug size, so add 4 to get last 4 bytes, don't
		// touch write back pointer space
		VMPI_memset(ip, poison, wholeSize+4);
	}

	void *DebugFree(const void *item, int poison, size_t size)
	{
		item = (int*) item - 2;
		DebugFreeHelper(item, poison, size);
		return (void*)item;
	}
#endif // defined MMGC_MEMORY_INFO

} // namespace MMgc

#endif // defined(MMGC_MEMORY_INFO) || defined(MMGC_MEMORY_PROFILER)
