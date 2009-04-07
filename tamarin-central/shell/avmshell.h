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

#ifndef __avmshell__
#define __avmshell__

#include "avmplus.h"
#include "Selftest.h"
#include "Platform.h"
#include "File.h"

using namespace avmplus;


namespace avmshell
{
	class ByteArrayObject;
	class ByteArray;
	// avmplus and NSPR both typedef some basic types: we must disambiguate
	//using avmplus::uintptr;
	//using avmplus::uint64;
	//using avmplus::uint32;
	//using avmplus::uint16;
	//using avmplus::uint8;
	//using avmplus::wchar;
}

namespace avmplus
{
	class Dictionary;
}

#include "FileInputStream.h"
#include "ConsoleOutputStream.h"
#include "SystemClass.h"
#include "FileClass.h"
#include "DomainClass.h"
#include "DebugCLI.h"
#include "Profiler.h"
#include "DataIO.h"
#include "ByteArrayGlue.h"
#include "DictionaryGlue.h"
#include "SamplerScript.h"
#include "JavaGlue.h"

#define INT32_T_MAX		0x7FFFFFFF	//max value for a 32-bit integer
#define UINT32_T_MAX	0xFFFFFFFF	//max value for a 32-bit unsigned integer

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

namespace avmplus
{
	namespace NativeID
	{
        #include "shell_toplevel.h"
    }
}

namespace avmshell
{ 
	// exit codes
	enum {
		OUT_OF_MEMORY = 128,
	};
	
	class ShellCodeContext : public CodeContext
	{
	  public:
		DWB(DomainEnv*) m_domainEnv;
		virtual ~ShellCodeContext() {}
		virtual DomainEnv *domainEnv() const { return m_domainEnv; }
	};
	
	/**
	 * A command-line shell around the avmplus core.  This can be
	 * used to execute and debug .abc files from the command line.
	 */
	class Shell : public AvmCore
	{
	public:
		static int run(int argc, char *argv[]);

		void interrupt(MethodEnv *env);
		void stackOverflow(MethodEnv *env);

		void setEnv(Toplevel *toplevel, int argc, char *argv[]);

		virtual Toplevel* createToplevel(AbcEnv* abcEnv);
		Toplevel* initShellBuiltins();

		SystemClass* systemClass;
		PoolObject* shellPool;

	protected:
		Shell(MMgc::GC *gc);
		
		void initShellPool();
		void usage();

		bool executeProjector(int argc, char *argv[], int& exitCode);

		static void interruptTimerCallback(void* data);
#ifdef VMCFG_EVAL
		void repl(Toplevel* toplevel, DomainEnv* domainEnv);
		String* decodeBytesAsUTF16String(uint8_t* bytes, uint32_t nbytes, bool terminate=false);
		virtual String* readFileForEval(String* referencing_filename, String* filename);
#endif // VMCFG_EVAL

	private:
		OutputStream *consoleOutputStream;
		bool gracePeriod;
		bool inStackOverflow;
		int allowDebugger;

		int execute(int argc, char *argv[]);
		
	#ifdef DEBUGGER
	protected:
		virtual avmplus::Debugger* createDebugger() { AvmAssert(allowDebugger >= 0); return allowDebugger ? new (GetGC()) DebugCLI(this) : NULL; }
		virtual avmplus::Profiler* createProfiler() { AvmAssert(allowDebugger >= 0); return allowDebugger ? new (GetGC()) Profiler(this) : NULL; }
	private:
		inline DebugCLI* debugCLI() { return (DebugCLI*)debugger(); }
	#endif
	};

	class ShellToplevel : public Toplevel
	{
	public:
		ShellToplevel(AbcEnv* abcEnv);

		Shell* core() const {
			return (Shell*)Toplevel::core();
		}

		virtual ClassClosure *getBuiltinExtensionClass(int class_id) 
		{ 
            return shellClasses[class_id] ? shellClasses[class_id] : resolveShellClass(class_id);
		}

	private:

		ClassClosure* resolveShellClass(int class_id)
		{
			ClassClosure* cc = findClassInPool(class_id, core()->shellPool);
			WBRC(core()->GetGC(), shellClasses, &shellClasses[class_id], cc);
			return cc;
		}

		DWB(ClassClosure**) shellClasses;
	};
}

#endif /* __avmshell__ */
