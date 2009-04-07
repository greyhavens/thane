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


#include <windows.h>

#include "MMgc.h"

#ifdef MMGC_MEMORY_PROFILER
	#include <malloc.h>
	#include <strsafe.h>
	#include <DbgHelp.h>
    #include <io.h>
#endif

size_t VMPI_getVMPageSize()
{
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);

	return sysinfo.dwPageSize;
}

bool VMPI_canMergeContiguousRegions()
{
	return false;
}

bool VMPI_useVirtualMemory()
{
	return true;
}

void* VMPI_reserveMemoryRegion(void* address, size_t size)
{
	return VirtualAlloc(address,
						size,
						MEM_RESERVE
#ifdef _WIN64
						| MEM_TOP_DOWN
#endif
						,
						PAGE_NOACCESS);
}

bool VMPI_releaseMemoryRegion(void* address, size_t /*size*/)
{
	return VirtualFree(address, 0, MEM_RELEASE) == TRUE;
}

static bool CommitMemory(void* address, size_t size)
{
	address = VirtualAlloc(address,
						   size,
						   MEM_COMMIT
#ifdef _WIN64
						   | MEM_TOP_DOWN
#endif //#ifdef _WIN64
						   ,
						   PAGE_READWRITE);
	
#ifdef _DEBUG
	if (address == NULL)
		{
			MEMORY_BASIC_INFORMATION mbi;
			VirtualQuery(address, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
			LPVOID lpMsgBuf;
			FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
						   NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
						   (LPTSTR) &lpMsgBuf, 0, NULL );
			GCAssertMsg(false, (const char*)lpMsgBuf); //todo rishit
		}
#endif //_DEBUG
	
	return address != NULL;
}

bool VMPI_commitMemory(void* address, size_t size)
{
	bool success = false;
	MEMORY_BASIC_INFORMATION mbi;	
	do {
		VirtualQuery(address, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
		size_t commitSize = size > mbi.RegionSize ? mbi.RegionSize : size;
		success = CommitMemory(address, commitSize);
		address = (char*)address + commitSize;
		size -= commitSize;
	} while(size > 0 && success);
	
	return success;
}

bool VMPI_decommitMemory(char *address, size_t size)
{
	bool success = false;
	MEMORY_BASIC_INFORMATION mbi;	
	do {
		VirtualQuery(address, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
		size_t commitSize = size > mbi.RegionSize ? mbi.RegionSize : size;
		success = VirtualFree(address, commitSize, MEM_DECOMMIT) == TRUE;
		address += commitSize;
		size -= commitSize;
	} while(size > 0 && success);
	
	return success;
}

void* VMPI_allocateAlignedMemory(size_t size)
{
	return VirtualAlloc(NULL, size, MEM_COMMIT
#ifdef _WIN64
						| MEM_TOP_DOWN
#endif
						, PAGE_READWRITE);
}

void VMPI_releaseAlignedMemory(void* address)
{
	VMPI_releaseMemoryRegion(address, 0);
}

size_t VMPI_getVMPageCount(size_t pageSize)
{
	void *addr = 0;
	size_t ret;
	size_t bytes=0;
	MEMORY_BASIC_INFORMATION mib;
	while(true)
	{
		ret = VirtualQuery(addr, &mib, sizeof(MEMORY_BASIC_INFORMATION));
		if(ret == 0)
			break;

		if((mib.State & MEM_COMMIT) && (mib.Type & MEM_PRIVATE))
			bytes += mib.RegionSize;

			addr = (void*) ((intptr_t)mib.BaseAddress + mib.RegionSize);
	}

	return bytes / pageSize;
}

uint64_t VMPI_getPerformanceFrequency()
{
	static uint64_t gPerformanceFrequency = 0;		
	if (gPerformanceFrequency == 0) {
		QueryPerformanceFrequency((LARGE_INTEGER*)&gPerformanceFrequency);
	}
	return gPerformanceFrequency;
}

uint64_t VMPI_getPerformanceCounter()
{
	LARGE_INTEGER value;
	QueryPerformanceCounter(&value);
	return value.QuadPart;
}

#ifdef MMGC_MEMORY_PROFILER

	// --------------------------------------------------------------------------
	// --------------------------------------------------------------------------
	// --------------------------------------------------------------------------

	// helper snarfed and simplified from main flash player code.
	// since we only need it here and only for debug, I didn't bother
	// migrating the whole thing.
	class DynamicLoadLibraryHelper
	{
	protected:
		DynamicLoadLibraryHelper(const char* p_dllName, bool p_required = true);
		virtual ~DynamicLoadLibraryHelper();

		FARPROC GetProc(const char* p_funcName, bool p_required = true);

	public:	
		// note that this is only if any of the *required* ones failed;
		// some "optional" ones may be missing and still have this return true.
		bool AllRequiredItemsPresent() const { return m_allRequiredItemsPresent; }

	private:
		HMODULE	m_lib;
		bool m_allRequiredItemsPresent;
	};

	// --------------------------------------------------------------------------
	// --------------------------------------------------------------------------
	// --------------------------------------------------------------------------

#define GETPROC(n) do { m_##n = (n##ProcPtr)GetProc(#n); } while (0)
#define GETPROC_OPTIONAL(n) do { m_##n = (n##ProcPtr)GetProc(#n, false); } while (0)

	// --------------------------------------------------------------------------
	DynamicLoadLibraryHelper::DynamicLoadLibraryHelper(const char* p_dllName, bool p_required) : 
	m_lib(NULL),
		m_allRequiredItemsPresent(true)	// assume the best
	{
		m_lib = ::LoadLibraryA(p_dllName);
		if (p_required && (m_lib == NULL || m_lib == INVALID_HANDLE_VALUE))
		{
			// don't assert here... it will trigger a DebugBreak(), which will crash
			// systems not running a debugger... and QE insists that they be able
			// to run Debug builds on debuggerless Win98 systems... (sigh)
			//GCAssertMsg(0, p_dllName);
			m_allRequiredItemsPresent = false;
		}
	}

	// --------------------------------------------------------------------------
	FARPROC DynamicLoadLibraryHelper::GetProc(const char* p_funcName, bool p_required)
	{
		FARPROC a_proc = NULL;
		if (m_lib != NULL && m_lib != INVALID_HANDLE_VALUE)
		{
			a_proc = ::GetProcAddress(m_lib, p_funcName); 
		}
		if (p_required && a_proc == NULL)
		{
			// don't assert here... it will trigger a DebugBreak(), which will crash
			// systems not running a debugger... and QE insists that they be able
			// to run Debug builds on debuggerless Win98 systems... (sigh)
			//GCAssertMsg(0, p_funcName);
			m_allRequiredItemsPresent = false;
		}
		return a_proc;
	}

	// --------------------------------------------------------------------------
	DynamicLoadLibraryHelper::~DynamicLoadLibraryHelper()
	{
		if (m_lib != NULL && m_lib != INVALID_HANDLE_VALUE)
		{
			::FreeLibrary(m_lib);
		}
	}

	// --------------------------------------------------------------------------
	// --------------------------------------------------------------------------
	// --------------------------------------------------------------------------

	// --------------------------------------------------------------------------
	class DbgHelpDllHelper : public DynamicLoadLibraryHelper
	{
	public:
		DbgHelpDllHelper();

	public:

		typedef BOOL (__stdcall *StackWalk64ProcPtr)(
			DWORD                             MachineType,
			HANDLE                            hProcess,
			HANDLE                            hThread,
			LPSTACKFRAME64                    StackFrame,
			PVOID                             ContextRecord,
			PREAD_PROCESS_MEMORY_ROUTINE64    ReadMemoryRoutine,
			PFUNCTION_TABLE_ACCESS_ROUTINE64  FunctionTableAccessRoutine,
			PGET_MODULE_BASE_ROUTINE64        GetModuleBaseRoutine,
			PTRANSLATE_ADDRESS_ROUTINE64      TranslateAddress
			);

		typedef PVOID (__stdcall *SymFunctionTableAccess64ProcPtr)(
			HANDLE  hProcess,
			DWORD64 AddrBase
			);

		typedef DWORD64 (__stdcall *SymGetModuleBase64ProcPtr)(
			HANDLE  hProcess,
			DWORD64 qwAddr
			);

		typedef BOOL (__stdcall *SymGetLineFromAddr64ProcPtr)(
			HANDLE                  hProcess,
			DWORD64                 qwAddr,
			PDWORD                  pdwDisplacement,
			PIMAGEHLP_LINE64        Line64
			);

		typedef BOOL (__stdcall *SymGetSymFromAddr64ProcPtr)(
			HANDLE                  hProcess,
			DWORD64                 qwAddr,
			PDWORD64                pdwDisplacement,
			PIMAGEHLP_SYMBOL64      Symbol
			);

		typedef BOOL (__stdcall *SymInitializeProcPtr)(
			HANDLE   hProcess,
			PSTR     UserSearchPath,
			BOOL     fInvadeProcess
			);

	public:
		StackWalk64ProcPtr					m_StackWalk64;
		SymFunctionTableAccess64ProcPtr		m_SymFunctionTableAccess64;
		SymGetModuleBase64ProcPtr			m_SymGetModuleBase64;
		SymGetLineFromAddr64ProcPtr			m_SymGetLineFromAddr64;
		SymGetSymFromAddr64ProcPtr			m_SymGetSymFromAddr64;
		SymInitializeProcPtr				m_SymInitialize;
	};

	// --------------------------------------------------------------------------
	DbgHelpDllHelper::DbgHelpDllHelper() : 
	DynamicLoadLibraryHelper("dbghelp.dll"),
		m_StackWalk64(NULL),
		m_SymFunctionTableAccess64(NULL),
		m_SymGetModuleBase64(NULL),
		m_SymGetLineFromAddr64(NULL),
		m_SymGetSymFromAddr64(NULL),
		m_SymInitialize(NULL)
	{
		GETPROC(StackWalk64);
		GETPROC(SymFunctionTableAccess64);
		GETPROC(SymGetModuleBase64);
		GETPROC(SymGetLineFromAddr64);
		GETPROC(SymGetSymFromAddr64);
		GETPROC(SymInitialize);
	}

	// declaring this statically will dynamically load the dll and procs
	// at startup, and never ever release them... if this ever becomes NON-debug
	// code, you might want to have a way to toss all this... but for _DEBUG
	// only, it should be fine
	static DbgHelpDllHelper g_DbgHelpDll;
	static const int MaxNameLength = 256;

#ifdef _WIN64 
#define MACHINETYPE IMAGE_FILE_MACHINE_AMD64
#else
#define MACHINETYPE IMAGE_FILE_MACHINE_I386
#endif

	bool InitDbgHelp()
	{
		static bool inited = false;
		if(!inited) {
			if(!g_DbgHelpDll.m_SymInitialize ||
				!(*g_DbgHelpDll.m_SymInitialize)(GetCurrentProcess(), NULL, true)) {
					LPVOID lpMsgBuf;
					if(FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
						(LPTSTR) &lpMsgBuf, 0, NULL )) 
					{
						MMgc::GCDebugMsg("See lpMsgBuf", true);
						LocalFree(lpMsgBuf);
					}			
					return false;
			}
			inited = true;
		}
		return true;
	}

typedef USHORT NTAPI RtlCaptureStackBackTrace_Function(
	IN ULONG frames_to_skip,
	IN ULONG frames_to_capture,
	OUT PVOID *backtrace,
	OUT PULONG backtrace_hash);

static RtlCaptureStackBackTrace_Function* const RtlCaptureStackBackTrace_fn = 
	(RtlCaptureStackBackTrace_Function*)
	GetProcAddress(GetModuleHandleA("ntdll.dll"), "RtlCaptureStackBackTrace");

	bool VMPI_captureStackTrace(uintptr_t *buffer, size_t bufferSize, uint32_t framesToSkip)
	{
		int num = RtlCaptureStackBackTrace_fn(framesToSkip, bufferSize - 1, (PVOID*)bufferSize, NULL);
		buffer[num] = 0;
		return true;
	}

	bool VMPI_getFileAndLineInfoFromPC(uintptr_t pc, char *filenameBuffer, size_t bufferSize, uint32_t* lineNumber)
	{
		if(!InitDbgHelp())
			return false;

		// gleaned from IMAGEHLP_SYMBOL64 docs
		IMAGEHLP_SYMBOL64 *pSym = (IMAGEHLP_SYMBOL64 *) alloca(sizeof(IMAGEHLP_SYMBOL64) + MaxNameLength);
		pSym->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
		pSym->MaxNameLength = MaxNameLength;

		DWORD64 offsetFromSymbol;
		if(!g_DbgHelpDll.m_SymGetSymFromAddr64 ||
			!(*g_DbgHelpDll.m_SymGetSymFromAddr64)(GetCurrentProcess(), pc, &offsetFromSymbol, pSym)) {
				return false;
		}

		// get line
		IMAGEHLP_LINE64 line;
		VMPI_memset(&line, 0, sizeof(IMAGEHLP_LINE64));
		line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
		DWORD offsetFromLine;
		if(!g_DbgHelpDll.m_SymGetLineFromAddr64 ||
			!(*g_DbgHelpDll.m_SymGetLineFromAddr64)(GetCurrentProcess(), pc, &offsetFromLine, &line)) {
				return false;
		}


		// success!
		char *fileName = line.FileName + VMPI_strlen(line.FileName);

		// skip everything up to last slash
		while(fileName > line.FileName && *fileName != '\\')
			fileName--;
		fileName++;

		StringCchPrintfA(filenameBuffer, bufferSize, "%s", fileName);
		*lineNumber = line.LineNumber;

		return true;
	}

	bool VMPI_getFunctionNameFromPC(uintptr_t pc, char *buffer, size_t bufferSize)
	{
		if(!InitDbgHelp())
			return false;

		// gleaned from IMAGEHLP_SYMBOL64 docs
		IMAGEHLP_SYMBOL64 *pSym = (IMAGEHLP_SYMBOL64 *) alloca(sizeof(IMAGEHLP_SYMBOL64) + MaxNameLength);
		pSym->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
		pSym->MaxNameLength = MaxNameLength;

		DWORD64 offsetFromSymbol;
		if(!g_DbgHelpDll.m_SymGetSymFromAddr64 ||
			!(*g_DbgHelpDll.m_SymGetSymFromAddr64)(GetCurrentProcess(), pc, &offsetFromSymbol, pSym)) {
				return false;
		}

		StringCchPrintfA(buffer, bufferSize, "%s", pSym->Name);
		//printf("%s\n", pSym->Name);
		return true;
	}

/**
 * compiled with the /W4 warning level
 * which is quite picky.  Disable warnings we don't care about.
 */
#ifdef _MSC_VER
	#pragma warning(disable:4127) //conditional expression is constant
#endif

class SignalData : public MMgc::GCAllocObject
{
public:
	SignalData(uint32_t *addr, HANDLE handle) 
		: profilerAddr(addr), eventHandle(handle) {}
	uint32_t *profilerAddr;
	HANDLE eventHandle;
};

DWORD WINAPI WaitForMemorySignal(LPVOID lpParam)
{
	SignalData *sig_data = (SignalData*)lpParam;
	while(true) {
		WaitForSingleObject(sig_data->eventHandle, INFINITE);
		*(sig_data->profilerAddr) = true;
	}
	delete sig_data;
	return 0;
}

void VMPI_WriteOnNamedSignal(const char *name, uint32_t *addr)
{
	HANDLE m_namedSharedObject;

	SECURITY_DESCRIPTOR sd;
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, TRUE, 0, FALSE);

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = FALSE;
	sa.lpSecurityDescriptor = &sd;

	m_namedSharedObject = CreateEventA(&sa, FALSE, FALSE, name);
	if(m_namedSharedObject == NULL){
		LPVOID lpMsgBuf;
		FormatMessageA( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
								NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
								(LPTSTR) &lpMsgBuf, 0, NULL );
		fputs((const char*)lpMsgBuf, stderr);
		return;
	}
	SignalData *sig_data = new SignalData(addr, m_namedSharedObject);
	CreateThread(NULL, 0, WaitForMemorySignal, sig_data, 0, NULL);
}

void *VMPI_OpenAndConnectToNamedPipe(const char *pipeName)
{
  char name[256];
  _snprintf(name, sizeof(name), "\\\\.\\pipe\\%s", pipeName);
  HANDLE pipe = CreateNamedPipeA((LPCSTR)name, PIPE_ACCESS_OUTBOUND, PIPE_TYPE_BYTE, 1, 4096, 4096, 100, NULL);
  ConnectNamedPipe(pipe, NULL);
  return (void*)pipe;
}

FILE *VMPI_HandleToStream(void *handle)
{
	return _fdopen(_open_osfhandle((intptr_t)handle, 0), "w");
}

void VMPI_CloseNamedPipe(void *handle)
{
	FlushFileBuffers(handle);
	CloseHandle((HANDLE)handle);
}

#endif // MMGC_MEMORY_PROFILER
