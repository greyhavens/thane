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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef MMGC_MEMORY_PROFILER
	#include <execinfo.h>
	#include <dlfcn.h>
	#include <cxxabi.h>
#endif

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/sysctl.h>

#if defined(__MACH__)
	#include <mach/mach.h>
#endif

#ifdef DARWIN
	#include <mach/mach_time.h>
#endif

#ifdef _MAC
	#define MAP_ANONYMOUS MAP_ANON
#endif

static const int kOSX105 = 9;

size_t VMPI_getVMPageSize()
{
	long v = sysconf(_SC_PAGESIZE);
	if (v == -1) v = 4096; // Mac 10.1 needs this
	return v;
}

bool VMPI_canMergeContiguousRegions()
{
#ifdef MMGC_64BIT		
	return true;
#else
	// this would be nice to keep the region list short but
	// doing so would require additional work to perform de-reserve (region splitting), when
	// that happens turn this back on for 10 5 and up, turning it on effectively disables de-reserve
	// which is why its on for 64 bit (we're we never de-reserve)
	return false;
#endif
}

static int get_major_version()
{
	int mib[2];
	mib[0]=CTL_KERN;
	mib[1]=KERN_OSRELEASE;
	char buf[10];
	size_t siz=sizeof(buf);
	sysctl(mib, 2, &buf, &siz, NULL, 0);
	return strtol(buf, 0, 10);
}

bool VMPI_useVirtualMemory()
{
#ifdef MMGC_64BIT		
	return true;
#else
	return get_major_version() >= kOSX105;
#endif
}

static int get_mmap_fdes(int delta)
{
	// ensure runtime version
		if(get_major_version() >= kOSX105)
			return VM_MAKE_TAG(VM_MEMORY_APPLICATION_SPECIFIC_1+delta);
		else
			return -1;
}

void* VMPI_reserveMemoryRegion(void *address, size_t size)
{
	void *addr = (char*)mmap(address,
							 size,
							 PROT_NONE,
							 MAP_PRIVATE | MAP_ANONYMOUS,
							 get_mmap_fdes(0), 0);
	
	// the man page for mmap documents it returns -1 to signal failure. 
	if (addr == (void *)-1) return NULL;
	
	if(address && address != addr) {
		// fail if we didn't get the right address
		VMPI_releaseMemoryRegion(addr, size);
		return NULL;
	}
	return addr;
}

bool VMPI_releaseMemoryRegion(void* address, size_t size)
{
	int result = munmap(address, size);
	return (result == 0);
}

bool VMPI_commitMemory(void* address, size_t size)
{
	char *got = (char*)mmap(address,
							size,
							PROT_READ | PROT_WRITE,
							MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED,
							get_mmap_fdes(0), 0);
	return (got == address);
}

bool VMPI_decommitMemory(char *address, size_t size)
{
	kern_return_t result = vm_deallocate(mach_task_self(), (vm_address_t)address, size);
	if(result == KERN_SUCCESS) 
	{
		result = vm_map(mach_task_self(), (vm_address_t*)&address, size, 0, FALSE, MEMORY_OBJECT_NULL, 0, FALSE, VM_PROT_NONE, VM_PROT_READ|VM_PROT_WRITE|VM_PROT_EXECUTE, VM_INHERIT_NONE);
	}
	return (result == KERN_SUCCESS);
}

void* VMPI_allocateAlignedMemory(size_t size)
{
	return valloc(size);
}

void VMPI_releaseAlignedMemory(void* address)
{
	free(address);
}

size_t VMPI_getVMPageCount(size_t /*pageSize*/)
{
	size_t private_bytes = 0;
	kern_return_t ret;
	task_t task = mach_task_self();

	vm_size_t pagesize = 0;
	ret = host_page_size(mach_host_self(), &pagesize);

	vm_address_t addr = VM_MIN_ADDRESS;
	vm_size_t size = 0;
	while (true)
	{
		mach_msg_type_number_t count = VM_REGION_TOP_INFO_COUNT;
		vm_region_top_info_data_t info;
		mach_port_t object_name;

		addr += size;

#ifdef MMGC_64BIT
		ret = vm_region_64(task, &addr, &size, VM_REGION_TOP_INFO, (vm_region_info_t)&info, &count, &object_name);
#else
		ret = vm_region(task, &addr, &size, VM_REGION_TOP_INFO, (vm_region_info_t)&info, &count, &object_name);
#endif

		if (ret != KERN_SUCCESS)
			break;
		private_bytes += info.private_pages_resident;
	}
	return private_bytes;
}

uint64_t VMPI_getPerformanceFrequency()
{
	static mach_timebase_info_data_t info;
	static uint64_t frequency = 0;
	if ( frequency == 0 ) {
		(void) mach_timebase_info(&info);
		frequency = (uint64_t) ( 1.0 / ( 1e-9 * (double) info.numer / (double) info.denom ) );
	}
	return frequency;
}

uint64_t VMPI_getPerformanceCounter()
{
	return mach_absolute_time();
}

#ifdef MMGC_MEMORY_PROFILER

	bool VMPI_captureStackTrace(uintptr_t* buffer, size_t bufferSize, uint32_t framesToSkip) 
	{
		void *array[18];
		uint32_t got = backtrace(array, bufferSize);
		for(uint32_t i=0;i < got-framesToSkip; i++)
			buffer[i] = (uintptr_t)array[i+framesToSkip];
		buffer[got-framesToSkip] = 0;
		Dl_info dlip;
		if(dladdr((const void*)buffer[got-framesToSkip-1], &dlip) != 0)
			buffer[got-framesToSkip-1] = 0;

		return true;
	}

	bool VMPI_getFunctionNameFromPC(uintptr_t pc, char *buffer, size_t bufferSize)
	{
		Dl_info dlip;
		int ret = dladdr((void * const)pc, &dlip);
		const char *sym = dlip.dli_sname;
		if(ret != 0 && sym) {
			size_t sz=bufferSize;
			int status=0;
			char *out = (char*) malloc(buffSize);
			char *ret = abi::__cxa_demangle(sym, out, &sz, &status);
			if(ret) {
				out = ret; // apparently demangle may realloc, so free this instead of out
				VMPI_strncpy(buffer, ret, bufferSize);
			} else {
				VMPI_strncpy(buffer, sym, bufferSize);
			}
			free(out); 
			return true;
		} 
		return false;
	}

	bool VMPI_getFileAndLineInfoFromPC(uintptr_t pc, char *buffer, size_t bufferSize, uint32_t* lineNumber) 
	{
		(void)buffer;
		(void)pc;
		(void)bufferSize;
		(void)lineNumber;
		return false;
		/*		uintptr_t array[2] = {pc, 0};
		char **strs = backtrace_symbols((void*const*)array, 1);
		VMPI_snprintf(buff, bufferSize, "%s", strs[0]);
		free(strs);
		*/
		//		GetFunctionName(pc, buffer, bufferSize);
		//	VMPI_snprintf(buffer, bufferSize, "0x%u:%s", pc, sym);
	}

#endif //MMGC_MEMORY_PROFILER
