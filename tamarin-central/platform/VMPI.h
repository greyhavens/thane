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
 * Portions created by the Initial Developer are Copyright (C) 1993-2006
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

#ifndef __avmplus_VMPI__
#define __avmplus_VMPI__

//Placeholder Plaform-specific includes

#ifdef _MSC_VER
	#define snprintf	_snprintf
#endif

#define VMPI_memcpy			memcpy
#define VMPI_memset 		memset	
#define VMPI_memcmp 		memcmp	
#define VMPI_memmove 		memmove	
#define VMPI_memchr			memchr	
#define VMPI_strcmp 		strcmp
#define VMPI_strcat 		strcat
#define VMPI_strchr 		strchr
#define VMPI_strrchr 		strrchr
#define VMPI_strcpy 		strcpy
#define VMPI_strlen 		strlen
#define VMPI_strncat 		strncat
#define VMPI_strncmp 		strncmp
#define VMPI_strncpy 		strncpy
#define VMPI_strtol			strtol
#define VMPI_strstr			strstr

#define VMPI_getenv			getenv

#define VMPI_sprintf		sprintf
#define VMPI_snprintf		snprintf

#define VMPI_atoi	atoi
#define VMPI_tolower tolower	
#define VMPI_islower islower	
#define VMPI_toupper toupper	
#define VMPI_isupper isupper	
#define VMPI_isdigit isdigit	
#define VMPI_isalnum isalnum	
#define VMPI_isxdigit isxdigit	
#define VMPI_isspace isspace	
#define VMPI_isgraph isgraph	
#define VMPI_isprint isprint	
#define VMPI_ispunct ispunct	
#define VMPI_iscntrl iscntrl	
#define VMPI_isalpha isalpha
#define VMPI_abort   abort

#ifdef UNDER_CE
	#define vsnprintf _vsnprintf
#endif

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#include <ctype.h>
#include <limits.h>

#if defined (WIN32)

	#if _MSC_VER > 1000
		#pragma warning(push)
		#pragma warning(disable: 4201)
	#endif

	#include <windows.h>
	#include <malloc.h>
	
	#if _MSC_VER > 1000
		#pragma warning(pop)
	#endif

	#ifdef _MSC_VER
	// MSVC doesn't support inttypes.h or most C99 types directly, so declare them ourself.
	
	#ifdef _ARM_
		// Windows Mobile doesn't provide intptr_t or uintptr_t, so we'll do it ourself
		typedef __int32				intptr_t; 
		typedef unsigned __int32	uintptr_t; 
		#endif
		
		typedef __int8				int8_t;
		typedef __int16				int16_t;
		typedef __int32				int32_t;
		typedef __int64				int64_t;
		typedef unsigned __int8		uint8_t;
		typedef unsigned __int16	uint16_t;
		typedef unsigned __int32	uint32_t; 
		typedef unsigned __int64	uint64_t;
	#else
		#include <inttypes.h>
	#endif

        #ifdef UNDER_CE
		#undef VMPI_abort
                #define VMPI_abort() TerminateProcess(GetCurrentProcess(), 0)
        #endif

#endif /* WIN32 */

#if defined (AVMPLUS_SYMBIAN)

	#include <inttypes.h>

	#include <e32std.h>
	
#endif /* AVMPLUS_SYMBIAN */

#if defined (UNIX)
	#include <inttypes.h>
#endif

#if defined (_MAC)

	#include <inttypes.h>
	#include <alloca.h>

#endif /* AVMPLUS_SYMBIAN */

#if defined (SOLARIS)

	#include <inttypes.h>
	#include <alloca.h>

#endif /* SOLARIS */

// legacy types
namespace avmplus
{
typedef int64_t		int64;
typedef int64_t		sint64;
typedef uint64_t	uint64;
typedef uint32_t	uint32;
typedef int32_t		int32;
typedef int32_t		sint32;
typedef uint16_t	uint16;

typedef int16_t		int16;

typedef int16_t		sint16;
typedef uint8_t		uint8;
typedef int8_t		int8;
typedef int8_t		sint8;
typedef uintptr_t	uintptr;
typedef intptr_t	sintptr;

typedef uint8_t		byte;
};
/* wchar is our version of wchar_t, since wchar_t is different sizes
 on different platforms, but we want to use UTF-16 uniformly. */
typedef uint16_t wchar;

// not ported or needed elsewhere (yet!)
#ifdef WIN32
void VMPI_WriteOnNamedSignal(const char *name, uint32_t *addr);
void *VMPI_OpenAndConnectToNamedPipe(const char *pipe);
FILE *VMPI_HandleToStream(void *handle);
void VMPI_CloseNamedPipe(void *handle);
#endif

/**
* This method should return the difference in milliseconds between local time and UTC
* @return offset in milliseconds
*/
extern double		VMPI_getLocalTimeOffset();

/**
* This method should return the Daylight Savings time adjustment in milliseconds
* @return number of milliseconds corresponding to daylight savings adjustment when active, 0 otherwise
*/
extern double		VMPI_getDaylightSavingsTA(double time);

/**
* This method should return the current UTC date and time in milliseconds
* @return UTC date and time in milliseconds
*/
extern double		VMPI_getDate();

/**
* This method should return the system time in milliseconds
* The implementation of this method could either return the time elapsed since the system started or since epoc
* @return number of milliseconds elapsed
*/
extern uint64_t		VMPI_getTime();

/**
* This method is called to output log messages
* The implementation of this method is platform-defined
*  @param message NULL-terminated UTF8-encoded string
* @return none
*/
extern void VMPI_Log(const char* message);

/**
* This method is called to output debugging messages
* This method is specifically for debugging purposes and is invoked to output useful debug informaton
* The implementation of this method is platform-defined.  For example, it could be used to output the message to an IDE
* @param message NULL-terminated UTF8-encoded string
* @return none
*/
extern void VMPI_DebugLog(const char* message);

/**
* This method is called to interrupt the program execution during debugging
* Upon a call to this method, the platform execution should interrupt the program execution
* while maintaining the current call stack and associated program information such as the registers
* and return the control back to the user/developer
* This method is specifically for debugging purposes and is invoked to enter a debug interrupt to examine information like the variable values or call stack
* The implementation of this method is platform-defined.
* On platforms that cannot support debug interrupts the program execution should be aborted
* @return none
*/
extern void VMPI_DebugBreak();

/**
* This method is used to request a block of memory from the system
* @param size size, in bytes, of memory block requested
* @return pointer to the start of memory block if allocation was successful, NULL otherwise
*/
extern void*		VMPI_alloc(size_t size);

/**
* This method is used to free a previously allocated block
* @param ptr pointer to the memory that needs to be released
* @return none
*/
extern void			VMPI_free(void* ptr);


/**
* This method is used to get the size of the memory page of the system
* @return return the size, if bytes, of memory page
*/
extern size_t		VMPI_getVMPageSize();

/**
* Method to retrieve number of pages in virtual address space of a process
* @param pageSize size, in bytes, of a page.  This value is the same as the one returned by VMPI_getVMPageSize
* @return number of pages
* @see VMPI_getVMPageSize()
*/
extern size_t		VMPI_getVMPageCount(size_t pageSize);

/**
* Method to find whether the platform supports merging of contiguous memory regions from heap
* @return true if heap merging is supported, false otherwise	
*/
extern bool			VMPI_canMergeContiguousHeapRegions();

/**
* This method is used to reserve region(s) of memory, i.e. one or more memory pages, in the system's virtual address space
* This method is recommended to be supported on platforms with virtual memory
* @param address optional argument, indicating the base address of the region to be reserved.
* The system should treat this address as a hint and try to reserve a region as close to this address as possible
* If NULL, the system can determine the address of the region.
* @param size size of region, in bytes, to reserve
* @return point to base address of the reserved region, NULL otherwise
* @see VMPI_ReleaseMemoryRegion(), VMPI_CommitMemory()
*/
extern void*		VMPI_reserveMemoryRegion(void* address, size_t size);

/**
* This method is called to release region(s) of memory previously reserved.
* @param address base address of the region to be released
* @param size size, in bytes, of the region
* @return true if the function succeeds, false otherwise
*/
extern bool			VMPI_releaseMemoryRegion(void* address, size_t size);

/**
* This method is used to commit a pages(s) of memory in a previously reserved region
* @param address base address of the memory region to commit
* @param size size, in bytes, of the memory to commit 
* @return true if the function succeeds, false otherwise
* @see VMPI_ReserveMemoryRegion()
*/
extern bool			VMPI_commitMemory(void* address, size_t size);

/**
* This method is used to de-commit a pages(s) of memory that were previously commited
* @param address base address of the memory region to decommit
* @param size, in bytes, of the memory region to decommit
* @return true if the function succeeds, false otherwise
* @see VMPI_CommitMemory()
*/
extern bool			VMPI_decommitMemory(char *address, size_t size);

/**
* This method is used to allocate a block of memory with the base address aligned to the system page size
* This method should be implemented in systems that do not have virtual memory in lieu of APIs to reserve and commit memory regions
* @param size size, in bytes, of the block of memory to allocate
* @return pointer to start of the memory block if allocation was successful, NULL otherwise
* @see VMPI_ReleaseAlignedMemory()
*/
extern void*		VMPI_allocateAlignedMemory(size_t size);

/**
* This method is used to release a block of memory via VMPI_AllocateAlignedMemory
* @param address pointer to the start of the memory block to be released
* @return none
*/
extern void			VMPI_releaseAlignedMemory(void* address);

/**
* Method to get the frequency of a high performance counter/timer on the system
* On platforms where no API to retrieve this information should return a number that closely
* matches its timer frequency
* @return 64-bit value indicating the frequency of the system's performance counter or clock with highest resolution
* @see VMPI_getPerformanceCounter()
*/
extern uint64_t 	VMPI_getPerformanceFrequency();

/**
* Method to get the current value of the system's performance counter/timer
* Platforms that support a high performance counter should return its current value
* Platforms that do not have a high performance counter should return the current timer/clock
* value that was used as a basis to calculate the frequency returned from VMPI_getPerformanceFrequency()
* @return 64-bit value indicating the current value of the counter
* @see VMPI_getPerformanceFrequency()
*/extern uint64_t 	VMPI_getPerformanceCounter();

/**
* Method to obtain the stack backtrace
* Used by the MMgc memory profiler to get call stack information
* @param buffer buffer to fill the call stack data with
* @param bufferSize size, in bytes, of the buffer passed
* @param framesToSkip number of function frames to skip from the start of trace 
* @return true if back trace was captured successfully, false otherwise
*/
extern bool			VMPI_captureStackTrace(uintptr_t* buffer, size_t bufferSize, uint32_t framesToSkip);

/**
* Method to retrieve the name of the method/function given a specific address in code space
* Used by the MMgc memory profiler to get and display function names
* @param pc address whose corresponding function name should be returned
* @param buffer buffer to write the function name to
* @param bufferSize size, in bytes, of the buffer passed
* @return true if the function name was retrieved, false otherwise
*/
extern bool			VMPI_getFunctionNameFromPC(uintptr_t pc, char *buffer, size_t bufferSize);

/**
* Method to retrieve the source filename and line number given a specific address in a code space
* Used by the MMgc memory profiler to display location info of source code
* @param pc address of code whose corresponding location should be returned
* @param buffer buffer to write the filename to
* @param bufferSize size, in bytes, of the buffer for filename
* @param out param to write the line number to
* @return true if the file and line number info was retrieved successfully, false otherwise
*/
extern bool			VMPI_getFileAndLineInfoFromPC(uintptr_t pc, char *buffer, size_t bufferSize, uint32_t* lineNumber);

/**
* Type defintion for an opaque data type representing platform-defined spin lock used by MMgc
* @see VMPI_lockCreate(), VMPI_lockAcquire()
*/
typedef void* vmpi_spin_lock_t;

/**
* Method to create a new instance of vmpi_spin_lock_t
* This instance will subsequently be passed to acquire/release lock methods
* @return newly created vmpi_spin_lock_t instance
*/
extern vmpi_spin_lock_t		VMPI_lockCreate();

/**
* Method to destroy the vmpi_spin_lock_t instance
* This method is called when MMgc no longer intends to use the vmpi_spin_lock_t
* instance created and return via VMPI_lockCreate.
* The implementation can safely destroy the lock instance.
* @param lock instance of vmpi_spin_lock_t to be destroyed
* @return none
* @see VMPI_lockCreate
*/
extern void			VMPI_lockDestroy(vmpi_spin_lock_t lock);

/**
* Method to acquire a lock on a vmpi_spin_lock_t instance
* If this method returns true, MMgc assumes that the lock was acquired successfully
* During a call to this method, if the lock was held by some other thread then the
* implementation should wait until the lock becomes available
* Return value of false is considered to be an error condition and should only happen
* in exception situations
* @param lock instance to acquire the lock on
* @return true if lock was successfully acquired, false in event of failure
*/
extern bool			VMPI_lockAcquire(vmpi_spin_lock_t lock);

/**
* Method to release a lock on a vmpi_spin_lock_t instance
* @param lock instance to release the lock on
* @return true if lock was successfully release, false in event of failure
*/
extern bool			VMPI_lockRelease(vmpi_spin_lock_t lock);

/**
 * can two consecutive calls to VMPI_reserveMemoryRegion be freed with one VMP_releaseMemoryRegion call?
 * @return true if it can
 */
extern bool VMPI_canMergeContiguousRegions();

/**
 * are the virtual memory API's implemented and usable for this platform/OS?
 * @return true if they are
 */
extern bool VMPI_useVirtualMemory();

/**
 * spin up a thread on the named signal name and write a 1 to addr when it fires
 */
extern void VMPI_WriteOnNamedSignal(const char *name, uint32_t *addr);

/**
 * connect to a named pipe
 */
extern void *VMPI_OpenAndConnectToNamedPipe(const char *pipeName);

#endif /* __avmplus_VMPI__ */
