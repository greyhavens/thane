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

#include "avmshell.h"

#include "Platform.h"

#ifdef __SUNPRO_CC
#define PRIVATE __hidden
#else
#define PRIVATE
#endif

#ifdef WIN32
#ifndef UNDER_CE
bool P4Available();
#endif
#elif defined AVMPLUS_UNIX
bool P4Available();
#endif

// [tpr] I removed the override global new stuff, it was apparently added b/c we'd crash when 
// OS would use our new pre-main that was fixed by making the project not export any 
// symbols other than main

namespace avmplus {
	namespace NativeID {
		using namespace avmshell;
		#include "shell_toplevel.cpp"
	}

#ifdef AVMPLUS_JITMAX
	extern int jitmax;
    extern int jitmin;
#endif
}

namespace avmshell
{
	const int kScriptTimeout = 15;
	const int kScriptGracePeriod = 5;

	bool show_error = false;

	ShellToplevel::ShellToplevel(AbcEnv* abcEnv) : Toplevel(abcEnv)
	{
		shellClasses = (ClassClosure**) core()->GetGC()->Calloc(avmplus::NativeID::shell_toplevel_abc_class_count, sizeof(ClassClosure*), MMgc::GC::kZero | MMgc::GC::kContainsPointers);
	}

	void Shell::usage()
	{
		AvmLog("avmplus shell " AVMPLUS_VERSION_USER " build " AVMPLUS_BUILD_CODE "\n\n");
		AvmLog("usage: avmplus\n");
		#ifdef DEBUGGER
			AvmLog("          [-d]          enter debugger on start\n");
		#endif
		AvmLog("          [-memstats]   generate statistics on memory usage\n");
		AvmLog("          [-memlimit d] limit the heap size to d pages\n");

		AvmLog("          [-cache_bindings N]   size of bindings cache (0 = unlimited)\n");
		AvmLog("          [-cache_metadata N]   size of metadata cache (0 = unlimited)\n");
		AvmLog("          [-cache_methods  N]   size of method cache (0 = unlimited)\n");

		#ifdef _DEBUG
			AvmLog("          [-Dgreedy]    collect before every allocation\n");
		#endif /* _DEBUG */
		#ifdef DEBUGGER
			AvmLog("          [-Dnogc]      don't collect\n");
			AvmLog("          [-Dnoincgc]   don't use incremental collection\n");
			AvmLog("          [-Dastrace N] display AS execution information, where N is [1..4]\n");
			AvmLog("          [-Dlanguage l] localize runtime errors, languages are:\n");
			AvmLog("                        en,de,es,fr,it,ja,ko,zh-CN,zh-TW\n");
		#endif

		AvmLog("          [-Dinterp]    do not generate machine code, interpret instead\n");

		#ifdef AVMPLUS_VERBOSE
			AvmLog("          [-Dverbose]   trace every instruction (verbose!)\n");
			AvmLog("          [-Dverbose_init] trace the builtins too\n");
			AvmLog("          [-Dbbgraph]   output JIT basic block graphs for use with Graphviz\n");
		#endif

    #if defined FEATURE_NANOJIT
		AvmLog("          [-Ojit]       use jit always, never interp\n");
		AvmLog("          [-Dnocse]     disable CSE optimization \n");
        #ifdef AVMPLUS_IA32
        AvmLog("          [-Dnosse]     use FPU stack instead of SSE2 instructions\n");
        #endif /* AVMPLUS_IA32 */
    #endif
	#ifdef AVMPLUS_JITMAX
        AvmLog("          [-jitmax N-M] jit the Nth to Mth methods only; N- and -M are also valid.\n");
	#endif
		
		#ifdef AVMPLUS_VERIFYALL
	    AvmLog("          [-Dverifyall] verify greedily instead of lazily\n");
		#endif
		#ifdef AVMPLUS_SELFTEST
		AvmLog("          [-Dselftest[=component,category,test]]  run selftests\n");
		#endif
		AvmLog("          [-Dtimeout]   enforce maximum 15 seconds execution\n");
		AvmLog("          [-error]      crash opens debug dialog, instead of dumping\n");
		#ifdef VMCFG_EVAL
		AvmLog("          [-repl]       read-eval-print mode\n");
		#endif // VMCFG_EVAL
		AvmLog("          [-log]\n");
		AvmLog("          [-- args]     args passed to AS3 program\n");
		AvmLog("          [-jargs ... ;] args passed to Java runtime\n");
		AvmLog("          filename.abc ...\n");
		AvmLog("          [--] application args\n");
		Platform::GetInstance()->exit(1);
	}

	void Shell::stackOverflow(MethodEnv *env)
	{
		if (inStackOverflow)
		{
			// Already handling a stack overflow, so do not
			// re-enter handler.
			return;
		}
			
		// Temporarily disable stack overflow checks
		// so that we can construct an exception object.
		// There should be plenty of margin before the
		// actual stack bottom to do this.
		inStackOverflow = true;

		Toplevel* toplevel = env->toplevel();

		Stringp errorMessage = getErrorMessage(kStackOverflowError);
		Atom args[2] = { nullObjectAtom, errorMessage->atom() };
		Atom errorAtom = toplevel->errorClass()->construct(1, args);
		Exception *exception = new (GetGC()) Exception(this, errorAtom);

		// Restore stack overflow checks
		inStackOverflow = false;

		// Throw the stack overflow exception
		throwException(exception);
	}
		
	void Shell::interruptTimerCallback(void* data)
	{
		((AvmCore*)data)->interrupted = true;
	}
	
	void Shell::interrupt(MethodEnv *env)
	{
		interrupted = false;

		Toplevel* toplevel = env->toplevel();

		if (gracePeriod) {
			// This script has already had its chance; it violated
			// the grace period.
			// Throw an exception it cannot catch.
			Stringp errorMessage = getErrorMessage(kScriptTerminatedError);
			Atom args[2] = { nullObjectAtom, errorMessage->atom() };
			Atom errorAtom = toplevel->errorClass()->construct(1, args);
			Exception *exception = new (GetGC()) Exception(this, errorAtom);
			exception->flags |= Exception::EXIT_EXCEPTION;
			throwException(exception);
		}

		// Give the script an additional grace period to
		// clean up, and throw an exception.
		gracePeriod = true;

		Platform::GetInstance()->setTimer(kScriptGracePeriod, interruptTimerCallback, this);

		toplevel->throwError(kScriptTimeoutError);
	}
	
	void Shell::initShellPool()
	{
		shellPool = AVM_INIT_BUILTIN_ABC(shell_toplevel, this);
	}

	Toplevel* Shell::initShellBuiltins()
	{
		// Initialize a new Toplevel.  This will also create a new
		// DomainEnv based on the builtinDomain.
		Toplevel* toplevel = initTopLevel();

		// Initialize the shell builtins in the new Toplevel
		handleActionPool(shellPool,
						 toplevel->domainEnv(),
						 toplevel,
						 NULL);

		return toplevel;
	}
	
	Toplevel* Shell::createToplevel(AbcEnv* abcEnv)
	{
		return new (GetGC()) ShellToplevel(abcEnv);
	}

#ifdef VMCFG_EVAL

	// FIXME, this is currently hokey for several reasons:
	//
	//  - Does not try to determine whether input is Latin1, UTF8, or indeed, already UTF16,
	//    but assumes UTF8, which can be dangerous.  Falls back to latin1 if the utf8 conversion
	//    fails, this seems ill-defined in the string layer though so it's just one more hack.
	//
	//  - Does not create an UTF16 string.  The string layer is actually broken on this count,
	//    because requesting an empty UTF16 string returns a constant that is a Latin1 string,
	//    and appending to it won't force the representation to UTF16 unless the data require
	//    that to happen.  See <URL:https://bugzilla.mozilla.org/show_bug.cgi?id=473995>.
	//
	//  - May incur copying because the terminating NUL is not accounted for in the original
	//    creation

	String* Shell::decodeBytesAsUTF16String(uint8_t* bytes, uint32_t nbytes, bool terminate)
	{
		String* s = newStringUTF8((const char*)bytes, nbytes);
		if (s == NULL)
			s = newStringLatin1((const char*)bytes, nbytes);
		if (terminate)
			s = s->appendLatin1("\0", 1);
		return s;
	}

	String* Shell::readFileForEval(String* referencingFile, String* filename)
	{
		// FIXME, filename sanitazion is more complicated than this
		if (referencingFile != NULL && filename->charAt(0) != '/' && filename->charAt(0) != '\\') {
			// find the last slash if any, truncate the string there, append the
			// new filename
			int32_t x = referencingFile->lastIndexOf(newStringLatin1("/"));
			if (x != -1)
				filename = referencingFile->substring(0,x+1)->append(filename);
		}
		filename = filename->appendLatin1("\0", 1);
		
		// FIXME, not obvious that UTF8 is correct for all operating systems (far from it!)
		StUTF8String fn(filename);
		FileInputStream f(fn.c_str());
		if (!f.valid() || (uint64_t) f.length() >= UINT32_T_MAX)
			return NULL;

		uint32_t nbytes = (uint32_t) f.available();
		uint8_t* bytes = new uint8_t[nbytes];
		f.read(bytes, nbytes);
		String* str = decodeBytesAsUTF16String(bytes, nbytes, true);
		delete [] bytes;
		return str;
	}
	
	void Shell::repl(Toplevel* toplevel, DomainEnv* domainEnv)
	{
		const int kMaxCommandLine = 1024;
		char commandLine[kMaxCommandLine];

		ShellCodeContext* codeContext = new (GetGC()) ShellCodeContext();
		codeContext->m_domainEnv = domainEnv;

		console << "avmplus interactive shell\n"
				<< "Type '?' for help\n\n";
		for (;;)
		{
			TRY(this, kCatchAction_ReportAsError) 
			{
				String* code_string = NULL;
				bool record_time = false;
				AvmLog("> ");

				if(Platform::GetInstance()->getUserInput(commandLine, kMaxCommandLine) == NULL)
					return;

				commandLine[kMaxCommandLine-1] = 0;
				if (VMPI_strncmp(commandLine, "?", 1) == 0) {
					console << "Text entered at the prompt is compiled and evaluated unless\n"
							<< "it is one of these commands:\n\n"
							<< "  ?             print help\n"
							<< "  .input        collect lines until a line that reads '.end',\n"
							<< "                then eval the collected lines\n"
							<< "  .load file    load the file (source or compiled)\n"
							<< "  .quit         leave the repl\n"
							<< "  .time expr    evaluate expr and report the time it took.\n\n";
				}
				else if (VMPI_strncmp(commandLine, ".load", 5) == 0) {
					const char* s = commandLine+5;
					while (*s == ' ' || *s == '\t')
						s++;
					// wrong, handles only source code
					//readFileForEval(NULL, newStringLatin1(s));
					// FIXME: implement .load
					// Small amount of generalization of the code currently in the main loop should
					// take care of it.
					console << "The .load command is not implemented\n";
				}
				else if (VMPI_strncmp(commandLine, ".input", 6) == 0) {
					code_string = newStringLatin1("");
					for (;;) {
						if(Platform::GetInstance()->getUserInput(commandLine, kMaxCommandLine) == NULL)
							return;
						commandLine[kMaxCommandLine-1] = 0;
						if (VMPI_strncmp(commandLine, ".end", 4) == 0)
							break;
						code_string = code_string->appendLatin1(commandLine);
					}
					goto evaluate;
				}
				else if (VMPI_strncmp(commandLine, ".quit", 5) == 0) {
					return;
				}
				else if (VMPI_strncmp(commandLine, ".time", 5) == 0) {
					record_time = true;
					code_string = newStringLatin1(commandLine+5);
					goto evaluate;
				}
				else {
					// Always Latin-1 here so don't use the normal decoder
					code_string = newStringLatin1(commandLine);
				evaluate:
					double then = 0, now = 0;
					code_string = code_string->appendLatin1("\0", 1);
					if (record_time) 
						then = VMPI_getDate();
					Atom result = handleActionSource(code_string, NULL, domainEnv, toplevel, NULL, codeContext);
					if (record_time) 
						now = VMPI_getDate();
					if (result != undefinedAtom)
						console << string(result) << "\n";
					if (record_time)
						console << "Elapsed time: " << (now - then)/1000 << "s\n";
				}
			}
			CATCH(Exception *exception)
			{
#ifdef DEBUGGER
				if (!(exception->flags & Exception::SEEN_BY_DEBUGGER))
				{
					console << string(exception->atom) << "\n";
				}
				if (exception->getStackTrace()) {
					console << exception->getStackTrace()->format(this) << '\n';
				}
#else
				console << string(exception->atom) << "\n";
#endif
			}
			END_CATCH
			END_TRY
		}
	}
#endif // VMCFG_EVAL
	
	Shell::Shell(MMgc::GC* gc) : AvmCore(gc)
	{
		systemClass = NULL;
		
		gracePeriod = false;
		inStackOverflow = false;

		allowDebugger = -1;	// aka "not yet set" 

			consoleOutputStream = new (gc) ConsoleOutputStream();
	
		setConsoleStream(consoleOutputStream);
	}

#ifndef UNDER_CE
	bool Shell::executeProjector(int argc, char *argv[], int& exitCode)
	{
		TRY(this, kCatchAction_ReportAsError)
		{
			uint8 header[8];

			char* executablePath = new char[VMPI_strlen(argv[0])+1];
			VMPI_strcpy(executablePath, argv[0]);

			FileInputStream file(executablePath);
			delete [] executablePath;

			if (!file.valid())
			{
				return false;
			}
			
			file.seek(file.length() - 8);
			file.read(header, 8);

			// Check the magic number
			if (header[0] != 0x56 ||
				header[1] != 0x34 ||
				header[2] != 0x12 ||
				header[3] != 0xFA)
			{
				return false;
			}

			int abcLength = (header[4]     |
							 header[5]<<8  |
							 header[6]<<16 |
							 header[7]<<24);

			ScriptBuffer code = newScriptBuffer(abcLength);
			file.seek(file.length() - 8 - abcLength);
			file.read(code.getBuffer(), abcLength);
			
			initBuiltinPool();
			initShellPool();

			SystemClass::user_argc = argc-1;
			SystemClass::user_argv = &argv[1];
		
			// init toplevel internally
			Toplevel* toplevel = initShellBuiltins();

			// Create a new Domain for the user code
			Domain* domain = new (GetGC()) Domain(this,
											 builtinDomain);

			// Return a new DomainEnv for the user code
			DomainEnv* domainEnv = new (GetGC()) DomainEnv(this,
													  domain,
													  toplevel->domainEnv());

			ShellCodeContext* codeContext = new (GetGC()) ShellCodeContext();
			codeContext->m_domainEnv = domainEnv;
				
			// parse new bytecode
			handleActionBlock(code, 0, domainEnv, toplevel, NULL, codeContext);
		}
		CATCH(Exception *exception)
		{
			#ifdef DEBUGGER
			if (!(exception->flags & Exception::SEEN_BY_DEBUGGER))
			{
				console << string(exception->atom) << "\n";
			}
			if (exception->getStackTrace()) {
				console << exception->getStackTrace()->format(this) << '\n';
			}
			#else
			// [ed] always show error, even in release mode,
			// see bug #121382
			console << string(exception->atom) << "\n";
			#endif
			exit(1);
		}
		END_CATCH
		END_TRY
				
		exitCode = 0;
		return true;
	}
#endif

	int Shell::execute(int argc, char *argv[])
	{
		AvmCore::CacheSizes cacheSizes;	// defaults to unlimited

		minstack = Platform::GetInstance()->getStackBase();

		TRY(this, kCatchAction_ReportAsError)
		{
#if defined FEATURE_NANOJIT
			#if defined (AVMPLUS_IA32) || defined(AVMPLUS_AMD64)
			#ifdef AVMPLUS_SSE2_ALWAYS
			config.sse2 = true;
			#else
			if (!P4Available()) {
				config.sse2 = false;
			}
			#endif
			#endif
#endif

            #if !defined(UNDER_CE) && !defined(AVM_SHELL_NO_PROJECTOR)
			    int exitCode = 0;

				if (executeProjector(argc, argv, exitCode))
				{
					return exitCode;
				}
            #endif
						
			if (argc < 2) {
				usage();
			}

			int filenamesPos = -1;
			int endFilenamePos = -1;
			char *filename = NULL;

			bool do_log = false;
#ifdef DEBUGGER
			bool enter_debugger_on_launch = false;
#endif
			bool do_repl = false;
#ifdef AVMPLUS_VERBOSE
			bool do_verbose = false;
#endif
#ifdef AVMPLUS_SELFTEST
			bool do_selftest = false;
			const char* st_component = NULL;
			const char* st_category = NULL;
			const char* st_name = NULL;
			char *st_mem = NULL;
#endif
			
			bool nodebugger = false;
			for (int i=1; i<argc && endFilenamePos == -1; i++) {

				char *arg = argv[i];

				// options available to development builds.
				if (arg[0] == '-') 
				{
					if (arg[1] == 'D') {
						if (!VMPI_strcmp(arg+2, "timeout")) {
							config.interrupts = true;
						}
						else if (!VMPI_strcmp(arg+2, "nodebugger")) {
							// allow this option even in non-DEBUGGER builds to make test scripts simpler
							nodebugger = true;
						}
						#ifdef AVMPLUS_IA32
						else if (!VMPI_strcmp(arg+2, "nosse")) {
                            #if defined FEATURE_NANOJIT
							config.sse2 = false;
							#endif
						}
						#endif

	                    #ifdef AVMPLUS_VERIFYALL
						else if (!VMPI_strcmp(arg+2, "verifyall")) {
							config.verifyall = true;
						}
		                #endif /* AVMPLUS_VERIFYALL */

	                    #ifdef _DEBUG
						else if (!VMPI_strcmp(arg+2, "greedy")) {
							GetGC()->greedy = true;
						}
		                #endif /* _DEBUG */

	                    #ifdef DEBUGGER
						else if (!VMPI_strcmp(arg+2, "nogc")) {
							GetGC()->nogc = true;
						} else if (!VMPI_strcmp(arg+2, "noincgc")) {
							GetGC()->incremental = false;
						} else if (!VMPI_strcmp(arg+2, "astrace")) {
							avmplus::Debugger::astrace_console = (avmplus::Debugger::TraceLevel) VMPI_strtol(argv[++i], 0, 10);
						} else if (!VMPI_strcmp(arg+2, "language")) {
							langID=-1;
							for (int j=0;j<kLanguages;j++) {
								if (!VMPI_strcmp(argv[i+1],languageNames[j].str)) {
									langID=j;
									break;
								}
							}
							if (langID==-1) {
								langID = VMPI_atoi(argv[i+1]);
							}
							i++;
						}
                    	#endif /* DEBUGGER */
							
						#ifdef AVMPLUS_SELFTEST
						else if (!VMPI_strncmp(arg+2, "selftest", 8)) {
							do_selftest = true;
							if (arg[10] == '=') {
								size_t k = VMPI_strlen(arg+11);
								st_mem = new char[k+1];
								VMPI_strcpy(st_mem, arg+11);
								char *p = st_mem;
								st_component = p;
								while (*p && *p != ',')
									p++;
								if (*p == ',')
									*p++ = 0;
								st_category = p;
								while (*p && *p != ',')
									p++;
								if (*p == ',')
									*p++ = 0;
								st_name = p;
								if (*st_component == 0)
									st_component = NULL;
								if (*st_category == 0)
									st_category = NULL;
								if (*st_name == 0)
									st_name = NULL;
							}
						}
						#endif
						#ifdef AVMPLUS_VERBOSE
						else if (!VMPI_strcmp(arg+2, "verbose")) {
							do_verbose = true;
						} else if (!VMPI_strcmp(arg+2, "verbose_init")) {
                            do_verbose = this->config.verbose = true;
                        }
						#endif

                    #if defined FEATURE_NANOJIT
                        #ifdef AVMPLUS_VERBOSE
						else if (!VMPI_strcmp(arg+2, "bbgraph")) {
							config.bbgraph = true;  // generate basic block graph (only valid with JIT)
                        }
						#endif
                    #endif

                    #if defined FEATURE_NANOJIT
						else if (!VMPI_strcmp(arg+2, "nocse")) {
							config.cseopt = false;
						}
					#endif

						else if (!VMPI_strcmp(arg+2, "interp")) {
							config.runmode = RM_interp_all;
						} else {
							usage();
						}
					} 
					else if (!VMPI_strcmp(arg, "-cache_bindings")) {
						cacheSizes.bindings = (uint16_t)VMPI_strtol(argv[++i], 0, 10);
					} else if (!VMPI_strcmp(arg, "-cache_metadata")) {
						cacheSizes.metadata = (uint16_t)VMPI_strtol(argv[++i], 0, 10);
					} else if (!VMPI_strcmp(arg, "-cache_methods")) {
						cacheSizes.methods = (uint16_t)VMPI_strtol(argv[++i], 0, 10);
					}
                #if defined FEATURE_NANOJIT
					else if (!VMPI_strcmp(arg, "-Ojit")) {
                        config.runmode = RM_jit_all;
					} 
				#endif
				#ifdef AVMPLUS_JITMAX
					else if (!VMPI_strcmp(arg, "-jitmax")) {
                        char* val = argv[++i];
                        char* dash = VMPI_strchr(val,'-');
                        if (dash) {
                            if (dash==&val[0]) 
                                jitmax = VMPI_atoi(&val[1]); // -n form
                            else {
                                int32_t hl = VMPI_strlen(dash);
                                dash[0] = '\0'; // hammer argv ;)  - go boom?
                                jitmin = VMPI_atoi(val); 
                                if (hl>1)
                                    jitmax = VMPI_atoi(&dash[1]); 
                            }
                        } else {                                            
                            jitmax = VMPI_atoi(val);
                        }
                    }
				#endif
					else if (!VMPI_strcmp(arg, "-memstats")) {
						GetGC()->GetGCHeap()->Config().gcstats = true;
						GetGC()->GetGCHeap()->Config().autoGCStats = true;
					} else if (!VMPI_strcmp(arg, "-memlimit")) {
						GetGC()->GetGCHeap()->SetHeapLimit(VMPI_strtol(argv[++i], 0, 10));
					} else if (!VMPI_strcmp(arg, "-log")) {
						do_log = true;
					} 
					#ifdef VMCFG_EVAL
					else if (!VMPI_strcmp(arg, "-repl")) {
						do_repl = true;
					}
					#endif // VMCFG_EVAL
					else if (!VMPI_strcmp(arg, "-error")) {
						show_error = true;
						#ifdef WIN32
						#ifdef UNDER_CE
						AvmAssert(0);
						#else
						SetErrorMode(0);  // set to default
						#endif
						#endif // WIN32
					}
#ifdef AVMPLUS_WITH_JNI
					else if (!VMPI_strcmp(arg, "-jargs")) {
						// all the following args until the semi colon is for java.
						//@todo fix up this hard limit
						bool first = true;
						Java::startup_options = new char[256];
						VMPI_memset(Java::startup_options, 0, 256);

						for(i++; i<argc; i++)
						{
							if (*argv[i] == ';')
								break;
							if (!first) VMPI_strcat(Java::startup_options, " ");
							VMPI_strcat(Java::startup_options, argv[i]);
							first = false;
						}
						AvmAssert(VMPI_strlen(Java::startup_options) < 256);
					}
#endif /* AVMPLUS_WITH_JNI */
                                    
	                #ifdef DEBUGGER
					else if (!VMPI_strcmp(arg, "-d")) {
						enter_debugger_on_launch = true;
					}
		            #endif /* DEBUGGER */

					else if(arg[1] == '-' && arg[2] == 0) {
						if (endFilenamePos == -1)
							endFilenamePos = i;
					}
					else {
						usage();
					}
				} 
				else
				{
					if (filenamesPos == -1)
						filenamesPos = i;
					filename = arg;
				}
			}
			
			this->allowDebugger = nodebugger ? 0 : 1;
		
			if (!filename && !do_repl
#ifdef AVMPLUS_SELFTEST
				&& !do_selftest
#endif
				) 
			{
				usage();
			}

			if( do_log )
			{
				// open logfile based on last filename
				char* lastDot = VMPI_strrchr(filename, '.');
				if(lastDot)
				{
					//filename could contain '/' or '\' as their separator, look for both
					char* lastPathSep1 = VMPI_strrchr(filename, '/');
					char* lastPathSep2 = VMPI_strrchr(filename, '\\');
					if(lastPathSep1 < lastPathSep2) //determine the right-most 
						lastPathSep1 = lastPathSep2;

					//if dot is before the separator, the filename does not have an extension
					if(lastDot < lastPathSep1)
						lastDot = NULL;
				}

				//if no extension then take the entire filename or 
				size_t logFilenameLen = (lastDot == NULL) ? VMPI_strlen(filename) : (lastDot - filename); 

				char* logFilename = new char[logFilenameLen + 5];  // 5 bytes for ".log" + null char
				VMPI_strncpy(logFilename,filename,logFilenameLen);
				VMPI_strcpy(logFilename+logFilenameLen,".log");

				Platform::GetInstance()->initializeLogging(logFilename);

				delete [] logFilename;
			}

			setCacheSizes(cacheSizes);

#ifdef AVMPLUS_VERBOSE
#if VMCFG_METHOD_NAMES
			// verbose requires methodnames (in avmshell, anyway), before calling initBuiltinPool.
			if (do_verbose)
				this->config.methodNames = true;
#endif
#endif

#ifdef DEBUGGER
#if VMCFG_METHOD_NAMES
			// debugger in avmshell always enables methodnames. 
			if (this->allowDebugger)
				this->config.methodNames = true;
#endif
#endif
			
			initBuiltinPool();
			initShellPool();

#ifdef AVMPLUS_VERBOSE
			if (do_verbose)
				config.verbose = true;
#endif

#ifdef AVMPLUS_SELFTEST
			if (do_selftest) {
				selftests(this, st_component, st_category, st_name);
				if (st_mem != NULL)
					delete [] st_mem;
				return 0;  // FIXME: not ideal
			}
#endif
			
			#ifdef DEBUGGER
			if (enter_debugger_on_launch)
			{
				// Activate the debug CLI and stop at
				// start of program
				debugCLI()->activate();
				debugCLI()->stepInto();
			}
			#endif

			// start the 15 second timeout if applicable
			if (config.interrupts) {
				Platform::GetInstance()->setTimer(kScriptTimeout, interruptTimerCallback, this);
			}

			if(endFilenamePos == -1)
				endFilenamePos = argc;
			SystemClass::user_argc = argc-endFilenamePos-1; 
			SystemClass::user_argv = &argv[endFilenamePos+1];
		
			// init toplevel internally
			Toplevel* toplevel = initShellBuiltins();

			// Create a new Domain for the user code
			Domain* domain = new (GetGC()) Domain(this,
											 builtinDomain);

			// Return a new DomainEnv for the user code
			DomainEnv* domainEnv = new (GetGC()) DomainEnv(this,
													  domain,
													  toplevel->domainEnv());

			ShellCodeContext* lastCodeContext = 0;

			// execute each abc file
			for (int i=filenamesPos; filename && i < endFilenamePos; i++)
			{
				filename = argv[i];

				#ifdef AVMPLUS_VERBOSE
				if (config.verbose) {
					console << "run " << filename << "\n";
				}
				#endif

				FileInputStream f(filename);
				bool isValid = f.valid() && ((uint64_t)f.length() < UINT32_T_MAX); //currently we cannot read files > 4GB
				if (!isValid) {
                    console << "cannot open file: " << filename << "\n";
					return(1);
				}

				ShellCodeContext* codeContext = new (GetGC()) ShellCodeContext();
				codeContext->m_domainEnv = domainEnv;
				
				// parse new bytecode
				if (isValid)
				{
					ScriptBuffer code = newScriptBuffer((size_t)f.available());
					f.read(code.getBuffer(), (size_t)f.available());
#ifdef VMCFG_EVAL
					if (AbcParser::canParse(code) == 0)
						handleActionBlock(code, 0, domainEnv, toplevel, NULL, codeContext);
					else {
						// FIXME: I'm assuming code is UTF8 - OK for now, but easy to go wrong; it could be 8-bit ASCII
						String* code_string = decodeBytesAsUTF16String(code.getBuffer(), (uint32_t)code.getSize(), true);
						String* filename_string = decodeBytesAsUTF16String((uint8_t*)filename, (uint32_t)VMPI_strlen(filename));
						ScriptBuffer empty;		// With luck: allow the
						code = empty;			//    buffer to be garbage collected
						handleActionSource(code_string, filename_string, domainEnv, toplevel, NULL, codeContext);
					}
#else
					handleActionBlock(code, 0, domainEnv, toplevel, NULL, codeContext);
#endif // VMCFG_EVAL
				}

				lastCodeContext = codeContext;
			}

			#ifdef MMGC_COUNTERS
			console << "\nGC stats\n";
			console << "mark item         " << MMgc::GC::MarkItemCount << "\n";
			console << "mark null ptr     " << MMgc::GC::marknullCount << "\n";
			console << "mark alloc        " << MMgc::GC::markallocCount << "\n";
			console << "mark large        " << MMgc::GC::marklargeCount << "\n";
			console << "mark skip         " << MMgc::GC::markskipCount << "\n";
			console << "TrapWriteCount    " << MMgc::GC::TrapWriteCount << "\n";

			console << "\nGCAlloc stats\n";
			console << "SweepBlockCount   " << MMgc::GCAlloc::SweepBlockCount << "\n";
			console << "AllocCount        " << MMgc::GCAlloc::AllocCount << "\n";
			console << "FreeItemCount     " << MMgc::GCAlloc::FreeItemCount << "\n";
			#endif

			#ifdef VMCFG_EVAL
			if (do_repl)
				repl(toplevel, domainEnv);
			#endif // VMCFG_EVAL
		}
		CATCH(Exception *exception)
		{
			#ifdef DEBUGGER
			if (!(exception->flags & Exception::SEEN_BY_DEBUGGER))
			{
				console << string(exception->atom) << "\n";
			}
			if (exception->getStackTrace()) {
				console << exception->getStackTrace()->format(this) << '\n';
			}
			#else
			// [ed] always show error, even in release mode,
			// see bug #121382
			console << string(exception->atom) << "\n";
			#endif

			Platform::GetInstance()->exit(1);
		}
		END_CATCH
		END_TRY

#ifdef AVMPLUS_WITH_JNI
		if (Java::startup_options) delete Java::startup_options;
#endif /* AVMPLUS_WITH_JNI */
		return 0;
	}

	/* static */
	int Shell::run(int argc, char *argv[]) 
	{
		MMgc::GCHeapConfig conf;
		//conf.verbose = true;
		MMgc::GCHeap::Init(conf);
		
		int exitCode = 0;
		{
			
			MMGC_ENTER;
			if(MMGC_ENTER_STATUS == 0)
			{
				MMgc::GC *gc = new MMgc::GC(MMgc::GCHeap::GetGCHeap());
				MMGC_GCENTER(gc);
				Shell* shell = new Shell(gc);
				exitCode = shell->execute(argc, argv);
				delete shell;
				delete gc;
			}
			else
			{
				// allowing control to flow below to Destroy results in tons of leak asserts
				return OUT_OF_MEMORY;
			}	
		}

		MMgc::GCHeap::Destroy();
	 	return exitCode;
	}
}

