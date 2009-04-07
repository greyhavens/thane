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

#ifndef __avmplus_AvmCore__
#define __avmplus_AvmCore__

namespace avmplus
{

#define OBJECT_TYPE		(core->traits.object_itraits)
#define CLASS_TYPE		(core->traits.class_itraits)
#define FUNCTION_TYPE	(core->traits.function_itraits)
#define ARRAY_TYPE		(core->traits.array_itraits)
#define STRING_TYPE		(core->traits.string_itraits)
#define NUMBER_TYPE		(core->traits.number_itraits)
#define INT_TYPE		(core->traits.int_itraits)
#define UINT_TYPE		(core->traits.uint_itraits)
#define BOOLEAN_TYPE	(core->traits.boolean_itraits)
#define VOID_TYPE		(core->traits.void_itraits)
#define NULL_TYPE		(core->traits.null_itraits)
#define NAMESPACE_TYPE	(core->traits.namespace_itraits)
#define VECTORINT_TYPE	(core->traits.vectorint_itraits)
#define VECTORUINT_TYPE	(core->traits.vectoruint_itraits)
#define VECTORDOUBLE_TYPE		(core->traits.vectordouble_itraits)
#define VECTOROBJ_TYPE	(core->traits.vectorobj_itraits)

const int kBufferPadding = 16;

	enum Runmode { RM_mixed, RM_jit_all, RM_interp_all };

	struct Config
	{
		#ifdef AVMPLUS_VERBOSE
		/**
		 * The verbose flag may be set to display each bytecode
		 * instruction as it is executed, along with a snapshot of
		 * the state of the stack and scope chain.
		 * Caution!  This shoots out a ton of output!
		 */
		bool verbose;
		bool verbose_addrs;
		#endif /* AVMPLUS_VERBOSE */

		#if VMCFG_METHOD_NAMES
		// if true, record original names of methods at runtime.
		// if false, don't (Function.toString will return things like "Function-21")
		bool methodNames;
		
		// give "Vector.<*>" instead of "Vector$object", etc
		bool oldVectorMethodNames;	
		#endif

		enum Runmode runmode;

        #if defined FEATURE_NANOJIT
		/**
		 * To speed up initialization, we don't use jit on
		 * $init methods; we use interp instead.  For testing
		 * purposes, one may want to force the jit to be used
		 * for all code including $init methods.  The
		 * jit switch forces all code to run through the jit
		 * instead of interpreter.
		 */
		bool cseopt;

        #if defined (AVMPLUS_IA32) || defined(AVMPLUS_AMD64)
		bool sse2;
		bool use_cmov;
		#endif

		#ifdef AVMPLUS_VERBOSE
		/**
		 * Genearate a graph for the basic blocks.  Can be used by
		 * 'dot' utility to generate a jpg.
		 */
		bool bbgraph;
		#endif //AVMPLUS_VERBOSE

        #endif // FEATURE_NANOJIT

        /**
		 * If this switch is set, executing code will check the
		 * "interrupted" flag to see whether an interrupt needs
		 * to be handled.
		 */
		bool interrupts;

#ifdef AVMPLUS_VERIFYALL
		bool verifyall;
#endif

#ifdef FEATURE_NANOJIT
        bool show_stats;
        bool tree_opt;
        bool verbose_live;
        bool verbose_exits;
#endif
	};

	/**
	 * The main class of the AVM+ virtual machine.  This is the
	 * main entry point to the VM for running ActionScript code.
	 */
	class AvmCore : public MMgc::GCRoot
	{
	public:
		/**
		 * The console object.  Text to be displayed to the developer
		 * or end-user can be directed to console, much like the cout
		 * object in C++ iostreams.
		 *
		 * The console object is a wrapper around the console output
		 * stream specified by the setConsoleStream method.
		 * Programs embedding AVM+ will typically implement
		 * the OutputStream interface and pass it in via
		 * setConsoleStream.
		 */
		PrintWriter console;

#ifdef VTUNE
		iJIT_IsProfilingActiveFlags VTuneStatus;

		iJIT_IsProfilingActiveFlags CheckVTuneStatus() 
		{
			iJIT_IsProfilingActiveFlags profiler = iJIT_IsProfilingActive();	
			return profiler;
		}
#endif // VTUNE

		/**
		 * The GC used by this AVM instance
		 */
		MMgc::GC * const gc;

		#ifdef DEBUGGER
		friend class CodegenLIR;
		private:
			Debugger*		_debugger;
			Profiler*		_profiler;
			Sampler*		_sampler;
		public:
			inline Debugger* debugger() const { return _debugger; }
			inline Profiler* profiler() const { return _profiler; }
			inline Sampler* get_sampler() const { return _sampler; }
			inline void sampleCheck() { if (_sampler) _sampler->sampleCheck(); }
		protected:
			virtual Debugger* createDebugger() { return NULL; }
			virtual Profiler* createProfiler() { return NULL; }
		public:
			int					langID;
			bool				passAllExceptionsToDebugger;
		#endif
#ifdef AVMPLUS_VERIFYALL
        List<MethodInfo*, LIST_GCObjects> verifyQueue;
		void enqFunction(MethodInfo* f);
		void enqTraits(Traits* t);
		void verifyEarly(Toplevel* toplevel);
#endif

		void branchCheck(MethodEnv *env, bool interruptable, int go)
		{
			if(go < 0)
			{
#ifdef DEBUGGER
				sampleCheck();
#endif
				if (interruptable && interrupted)
					interrupt(env);
			}
		}

	private:
		QCache*			m_tbCache;
		QCache*			m_tmCache;
		QCache*			m_msCache;
	public:
		inline QCache* tbCache() { return m_tbCache; }
		inline QCache* tmCache() { return m_tmCache; }
		inline QCache* msCache() { return m_msCache; }
		struct CacheSizes
		{
			enum { DEFAULT_BINDINGS = 32, DEFAULT_METADATA = 1, DEFAULT_METHODS = 32 };
			
			uint16_t bindings;
			uint16_t metadata;
			uint16_t methods;
			
			inline CacheSizes() : bindings(DEFAULT_BINDINGS), metadata(DEFAULT_METADATA), methods(DEFAULT_METHODS) {}
		};
		// safe to call at any time, but calling tosses existing caches, thus has a perf hit --
		// don't call cavalierly
		void setCacheSizes(const CacheSizes& cs);

	public:
        #if defined FEATURE_NANOJIT
		void initMultinameLate(Multiname& name, Atom index);
        #endif

		/**
		 * Redirects the standard output of the VM to the specified
		 * output stream.  Output from print() statements and
		 * error messages will be sent here.
		 * @param stream output stream to use for console output
		 */
		void setConsoleStream(OutputStream *stream);

		/**
		 * GCCallback functions 
		 */
		virtual void presweep();
		virtual void postsweep();
		virtual void oom(MMgc::MemoryStatus status);
		
		Config config;
        
        #ifdef FEATURE_NANOJIT // accessors
            inline bool quiet_opt() const { return false; } 
            #if defined AVMPLUS_IA32 || defined AVMPLUS_AMD64
            inline bool use_sse2() const { return config.sse2; }
			#endif
			#ifdef AVMPLUS_VERBOSE
                inline bool verbose_exits() const { return config.verbose_exits; }
                inline bool verbose_live() const { return config.verbose_live; }
            #endif
        #endif
		#ifdef AVMPLUS_VERBOSE
		inline bool verbose() const { return config.verbose; }
		#endif

#if defined FEATURE_NANOJIT
	    inline void SetJITEnabled(bool isEnabled) {
			config.runmode = (isEnabled) ? RM_mixed : RM_interp_all;
		}
        inline bool IsJITEnabled() const {
			return (config.runmode == RM_mixed || config.runmode == RM_jit_all) ? true : false;
		}
#else
        inline void SetJITEnabled(bool) {}
        inline bool IsJITEnabled() { return false; }
#endif

		/**
		 * If this is set to a nonzero value, executing code
		 * will check the stack pointer to make sure it
		 * doesn't go below this value.
		 */
		uintptr minstack;

		/**
		 * This method will be invoked when the first exception
		 * frame is set up.  This will be a good point to set
		 * minstack.
		 */
		virtual void setStackBase() {}
		
		/** Internal table of strings for boolean type ("true", "false") */
		DRC(Stringp) booleanStrings[2];

		/** Container object for traits of built-in classes */
		BuiltinTraits traits;

		/** PoolObject for built-in classes */
		PoolObject* builtinPool;

		/** Domain for built-in classes */
		Domain* builtinDomain;
		
		/** The location of the currently active defaultNamespace */
		Namespace *const*dxnsAddr;

		enum InterruptReason {
			ScriptTimeout = 1,
			ExternalInterrupt = 2
		};

		/**
		 * If this flag is set, an interrupt is in progress.
		 * This must be type int, not bool, since it will
		 * be checked by generated code.
		 * 
		 * Set to 1 for a timeout interrupt, 2 for
		 * an external (i.e., signal handler) interrupt.
		 */
		int interrupted;
		
		/**
		 * The default namespace, "public", that all identifiers
		 * belong to
		 */
		DRC(Namespacep) publicNamespace;
		VTable* namespaceVTable;

		#ifdef AVMPLUS_WITH_JNI
		Java* java;     /* java vm control */
		#endif

		/**
		 * Execute an ABC file that has been parsed into a
		 * PoolObject.
		 * @param pool PoolObject containing the ABC file to
		 *             execute
		 * @param domainEnv The DomainEnv object to execute
		 *                  against, or NULL if a new DomainEnv
		 *                  should be created
		 * @param toplevel the Toplevel object to execute against,
		 *                 or NULL if a Toplevel should be
		 *                 created.
		 * @throws Exception If an error occurs, an Exception object will
		 *         be thrown using the AVM+ exceptions mechanism.
		 *         Calls to handleActionBlock should be bracketed
		 *         in TRY/CATCH.
		 */
		Atom handleActionPool(PoolObject* pool,
								   DomainEnv* domainEnv,
								   Toplevel* &toplevel,
								   CodeContext *codeContext);

		ScriptEnv* prepareActionPool(PoolObject* pool,
									 DomainEnv* domainEnv,
									 Toplevel*& toplevel,
									 CodeContext *codeContext);
		
		void exportDefs(Traits* traits, ScriptEnv* scriptEnv);

		/**
		 * Parse the ABC block starting at offset start in code.
		 * @param code buffer holding the ABC block to parse
		 * @param start zero-indexed offset, in bytes, into the
		 *              buffer where the code begins
		 * @param toplevel the Toplevel object to execute against,
		 *                 or NULL if a Toplevel should be
		 *                 created.
		 * @param nativeMethods the NATIVE_METHOD table
		 * @param nativeClasses the NATIVE_CLASS table
		 * @param nativeScriptss the NATIVE_SCRIPT table
		 * @throws Exception If an error occurs, an Exception object will
		 *         be thrown using the AVM+ exceptions mechanism.
		 *         Calls to handleActionBlock should be bracketed
		 *         in TRY/CATCH.
		 */
		PoolObject* parseActionBlock(ScriptBuffer code,
									 int start,
									 Toplevel* toplevel,
									 Domain* domain,
									 const NativeInitializer* ninit,
									 const List<Stringp>* include_versions = NULL);
		
		/**
		 * Execute the ABC block starting at offset start in code.
		 * @param code buffer holding the ABC block to execute
		 * @param start zero-indexed offset, in bytes, into the
		 *              buffer where the code begins
		 * @param toplevel the Toplevel object to execute against,
		 *                 or NULL if a Toplevel should be
		 *                 created.
		 * @param nativeMethods the NATIVE_METHOD table
		 * @param nativeClasses the NATIVE_CLASS table
		 * @param nativeScripts the NATIVE_SCRIPT table
		 * @throws Exception If an error occurs, an Exception object will
		 *         be thrown using the AVM+ exceptions mechanism.
		 *         Calls to handleActionBlock should be bracketed
		 *         in TRY/CATCH.
		 */
		Atom handleActionBlock(ScriptBuffer code,
									int start,
									DomainEnv* domainEnv,
									Toplevel* &toplevel,
									const NativeInitializer* ninit,
									CodeContext *codeContext);

#ifdef VMCFG_EVAL
		/**
		 * Compile the source code held in 'code' and then execute it
		 * as for handleActionBlock() above.
		 *
		 * @param code The code to be compiled and executed.  The string must be
		 *             NUL-terminated and the NUL is not considered part of the
		 *             input.  If 'code' is not in UTF-16 format it will be converted
		 *             to UTF-16 format, so it is highly advisable that the caller
		 *             has created an UTF-16 string.
		 * @param filename The name of the file originating the code, or
		 *                 NULL if the source code does not originate from a file.
		 *                 If not NULL then ActionScript's 'include' directive will
		 *                 be allowed in the program and files will be loaded
		 *                 relative to 'filename'.
		 * @param domainEnv  FIXME
		 * @param toplevel the Toplevel object to execute against,
		 *                 or NULL if a Toplevel should be
		 *                 created.
		 * @param ninit  FIXME
		 * @param codeContext  FIXME
		 * @throws Exception If an error occurs, an Exception object will
		 *         be thrown using the AVM+ exceptions mechanism.
		 *         Calls to handleActionBlock should be bracketed
		 *         in TRY/CATCH.
		 */
		Atom handleActionSource(String* code,
								String* filename,
								DomainEnv* domainEnv,
								Toplevel* &toplevel,
								const NativeInitializer* ninit,
								CodeContext *codeContext);
		
		/**
		 * Obtain input from a file to handle ActionScript's 'include' directive.
		 *
		 * This method invoked by the run-time compiler if the script uses 'include'
		 * and the use of 'include' is allowed because the script originated from
		 * a file; see 'handleActionSource()' above.
		 *
		 * 'referencingFilename' should be taken into
		 * account by this method if 'filename' is not an absolute file name.
		 *
		 * @param referencingFilename The name of the file from which the script 
		 *                            containing the 'include' directive was loaded
		 * @param filename  The filename in the 'include' directive.
		 * @return  A string representing the contents of the file named by 'filename'.
		 *          The string must NUL-terminated and the NUL is not considered part
		 *          of the input.  If the returned string is not in UTF-16 format then
		 *          it will be converted to UTF-16 format, so it is highly advisable
		 *          that the method has created an UTF-16 string.  If the file cannot
		 *          be opened or read then the return value should be NULL, an
		 *          exception should not be thrown.
		 */
		virtual String* readFileForEval(String* referencingFilename, String* filename) = 0;
#endif // VMCFG_EVAL

		/** Implementation of OP_equals */
		Atom equals(Atom lhs, Atom rhs);
		
		/**
		 * this is the abstract relational comparison algorithm according to ECMA 262 11.8.5
		 * @param lhs
		 * @param rhs
		 * @return trueAtom, falseAtom, or undefinedAtom
		 */
		Atom compare(Atom lhs, Atom rhs);

		/** Implementation of OP_strictequals */
		Atom stricteq(Atom lhs, Atom rhs);

		/**
		 * Helper method; returns true if the atom is a tagged ScriptObject
		 * pointer.  The actual type of the object is indicated by
		 * ScriptObject->vtable->traits.
		 */
		static bool isObject(Atom atom)
		{
			return (atom&7) == kObjectType && !isNull(atom);
		}

		static bool isPointer(Atom atom)
		{
			return (atom&7) < kSpecialType || (atom&7) == kDoubleType;
		}

		static bool isTraits(Atom type)
		{
			return type != 0 && (type&7) == 0;
		}

		static bool isNamespace(Atom atom)
		{
			return (atom&7) == kNamespaceType && !isNull(atom);
		}

		static BindingKind bindingKind(Binding b)
		{
			return BindingKind(uintptr_t(b) & 7);
		}

		static bool isMethodBinding(Binding b)
		{
			return bindingKind(b) == BKIND_METHOD;
		}

		static bool isAccessorBinding(Binding b)
		{
			return bindingKind(b) >= BKIND_GET;
		}

		static bool hasSetterBinding(Binding b)
		{
			return (bindingKind(b) & 6) == BKIND_SET;
		}

		static bool hasGetterBinding(Binding b)
		{
			return (bindingKind(b) & 5) == BKIND_GET;
		}

		static int bindingToGetterId(Binding b)
		{
			AvmAssert(hasGetterBinding(b));
			return int(uintptr_t(b)) >> 3;
		}

		static int bindingToSetterId(Binding b)
		{
			AvmAssert(hasSetterBinding(b));
			return 1 + (int(uintptr_t(b)) >> 3);
		}

		static int bindingToMethodId(Binding b)
		{
			AvmAssert(isMethodBinding(b));
			return int(uintptr_t(b)) >> 3;
		}

		static int bindingToSlotId(Binding b)
		{
			AvmAssert(isSlotBinding(b));
			return int(uintptr_t(b)) >> 3;
		}

		/** true if b is a var or a const */
		static int isSlotBinding(Binding b)
		{
			AvmAssert((BKIND_CONST & 6)==BKIND_VAR);
			return (bindingKind(b) & 6) == BKIND_VAR;
		}

		static Binding makeSlotBinding(uintptr_t id, BindingKind kind)
		{
			AvmAssert(kind == BKIND_VAR || kind == BKIND_CONST);
			return Binding((id << 3) | kind);
		}

		static Binding makeMGSBinding(uintptr_t id, BindingKind kind)
		{
			AvmAssert(kind == BKIND_METHOD || kind == BKIND_GET || kind == BKIND_SET);
			return Binding((id << 3) | kind);
		}

		static Binding makeGetSetBinding(Binding b)
		{
			AvmAssert(bindingKind(b) == BKIND_GET || bindingKind(b) == BKIND_SET);
			return Binding((uintptr_t(b) & ~7) | BKIND_GETSET);
		}

#if defined FEATURE_NANOJIT
		static Binding makeITrampBinding(uintptr_t id)
		{
			AvmAssert((id&7)==0); // addr must be 8-aligned
			return Binding(id | BKIND_ITRAMP);
		}

		static void* getITrampAddr(Binding b)
		{
			AvmAssert(bindingKind(b) == BKIND_ITRAMP);
			return (void*)(uintptr_t(b) & ~7);
		}
#endif

		/** true only if b is a var */
		static int isVarBinding(Binding b)
		{
			return bindingKind(b) == BKIND_VAR;
		}
		/** true only if b is a const */
		static int isConstBinding(Binding b)
		{
			return bindingKind(b) == BKIND_CONST;
		}
		
		/** Helper method; returns true if atom is an Function */
		bool isFunction(Atom atom)
		{
			return istype(atom, traits.function_itraits);
		}

		/** Helper method; returns true if atom's type is double */
		static bool isDouble(Atom atom)
		{
			return (atom&7) == kDoubleType;
		}

		/** Helper method; returns true if atom's type is int */
		static bool isInteger(Atom atom)
		{
			return (atom&7) == kIntegerType;
		}

		/** Helper method; returns true if atom's type is Number */
		static bool isNumber(Atom atom)
		{
			// accept kIntegerType(6) or kDoubleType(7)
			return (atom&6) == kIntegerType;
		}

		/** Helper method; returns true if atom's type is boolean */
		static bool isBoolean(Atom atom)
		{
			return (atom&7) == kBooleanType;
		}

		/** Helper method; returns true if atom's type is null */
		static bool isNull(Atom atom)
		{
			return ISNULL(atom);
		}

		/** Helper method; returns true if atom's type is undefined */
		static bool isUndefined(Atom atom)
		{
			return (atom == undefinedAtom);
		}

		static bool isNullOrUndefined(Atom atom)
		{
			return ((uintptr)atom) <= (uintptr)kSpecialType;
		}

#ifdef AVMPLUS_VERBOSE
		/** Disassembles an opcode and places the text in str. */
		void formatOpcode(PrintWriter& out, const byte *pc, AbcOpcode opcode, ptrdiff_t off, PoolObject* pool);
# ifdef AVMPLUS_WORD_CODE
		void formatOpcode(PrintWriter& out, const uintptr_t *pc, WordOpcode opcode, ptrdiff_t off, PoolObject* pool);
		void formatBits(PrintWriter& buffer, uint32 bits);
# endif
		static void formatMultiname(PrintWriter& out, uint32 index, PoolObject* pool);
#endif

		/**
		 * The resources table tracks what ABC's have been
		 * decoded, and avoids decoding the same one multiple
		 * times.
		 */
		Hashtable *resources;

		/**
		 * @name interned constants
		 * Constants used frequently in the VM; these are typically
		 * identifiers that are part of the core language semantics
		 * like "prototype" and "constructor".  These are interned
		 * up front and held in AvmCore for easy reference.
		 */
		/*@{*/

		DRC(Stringp) kconstructor;
		DRC(Stringp) kEmptyString;
		DRC(Stringp) ktrue;
		DRC(Stringp) kfalse;
		DRC(Stringp) kundefined;
		DRC(Stringp) knull;
		DRC(Stringp) ktoString;
		DRC(Stringp) ktoLocaleString;
		DRC(Stringp) kvalueOf;
		DRC(Stringp) klength;
		DRC(Stringp) kobject;
		DRC(Stringp) kfunction;
		DRC(Stringp) kxml;
		DRC(Stringp) kboolean;
		DRC(Stringp) knumber;
		DRC(Stringp) kstring;
		DRC(Stringp) kuri;
		DRC(Stringp) kprefix;
		DRC(Stringp) kglobal;
		DRC(Stringp) kcallee;
		DRC(Stringp) kNeedsDxns;
		DRC(Stringp) kAsterisk;
		DRC(Stringp) kVersion;
#if VMCFG_METHOD_NAMES
		DRC(Stringp) kanonymousFunc;
#endif
		Atom kNaN;

		DRC(Stringp) cachedChars[128];
		/*@}*/

		/** Constructor */
		AvmCore(MMgc::GC *gc);

		/** Destructor */
		~AvmCore();

		/**
		 * Parses builtin.abc into a PoolObject, to be executed
		 * later for each new Toplevel
		 */
		void initBuiltinPool();
		
		/**
		 * Initializes the specified Toplevel object by running
		 * builtin.abc
		 */
		Toplevel* initTopLevel();		

		virtual Toplevel* createToplevel(AbcEnv* abcEnv);
		
	public:
		/**
		 * toUInt32 is the ToUInt32 algorithm from
		 * ECMA-262 section 9.6, used in many of the
		 * native core objects
		 */
		inline static uint32 toUInt32(Atom atom)
		{
			return (uint32)integer(atom);
		}

		/**
		 * toInteger is the ToInteger algorithm from
		 * ECMA-262 section 9.4, used in many of the
		 * native core objects
		 */
		inline static double toInteger(Atom atom)
		{
			if (atomKind(atom) == kIntegerType) 
			{
				return (double) atomInt(atom);
			} 
			else 
			{
				return MathUtils::toInt(AvmCore::number(atom));
			}
		}

		/**
		 * Converts the passed atom to a 32-bit signed integer.
		 * If the atom is already an integer, it is simply
		 * decoded.  Otherwise, it is coerced to the int type
		 * and returned.  This is ToInt32() from E3 section 9.5
		 */
#ifdef AVMPLUS_64BIT
		static	int64 integer64(Atom atom)		{ return (int64)integer(atom); }
		static	int64 integer64_i(Atom atom)	{ return (int64)integer_i(atom); }
	#ifdef AVMPLUS_AMD64
		static	int64 integer64_d(double d)		{ return (int64)integer_d_sse2(d); }
		static	int64 integer64_d_sse2(double d){ return (int64)integer_d_sse2(d); }
	#else
		static	int64 integer64_d(double d)		{ return (int64)integer_d(d); }
	#endif
#endif
		static int integer(Atom atom);

		// convert atom to integer when we know it is already a legal signed-32 bit int value
		static int32_t integer_i(Atom a)
		{
			if (atomKind(a) == kIntegerType)
			{
				return (int32_t)atomInt(a);
			}
			else
			{
				// TODO since we know value is legal int, use faster d->i
				return MathUtils::real2int(atomToDouble(a));
			}
		}

		// convert atom to integer when we know it is already a legal unsigned-32 bit int value
		static uint32_t integer_u(Atom a)
		{
			if (atomKind(a) == kIntegerType)
			{
				return (uint32_t)atomInt(a);
			}
			else
			{
				// TODO figure out real2int for unsigned
				return (uint32_t) atomToDouble(a);
			}
		}

#ifdef AVMPLUS_SSE2_ALWAYS
        inline static int integer_d(double d) {
            return integer_d_sse2(d);
        }
        inline Atom doubleToAtom(double n) {
            return doubleToAtom_sse2(n);
        }
#else
		static int integer_d(double d);
		Atom doubleToAtom(double n);
#endif

#if defined (AVMPLUS_IA32) || defined(AVMPLUS_AMD64)
		static int integer_d_sse2(double d);
		Atom doubleToAtom_sse2(double n);
#endif

	private:
		static int doubleToInt32(double d);

	public:
		static double number_d(Atom a)
		{
			AvmAssert(isNumber(a));

			if (atomKind(a) == kIntegerType)
				return (int32_t)atomInt(a);
			else
				return atomToDouble(a);
		}

		/**
		 * intAtom is similar to the integer method, but returns
		 * an atom instead of a C++ int.
		 */
		Atom intAtom(Atom atom)
		{
			return intToAtom(AvmCore::integer(atom));
		}

		Atom uintAtom(Atom atom)
		{
			return uintToAtom(AvmCore::toUInt32(atom));
		}

		/**
		 * Converts the passed atom to a C++ bool.
		 * If the atom is already an E4 boolean, it is simply
		 * decoded.  Otherwise, it is coerced to the boolean type
		 * and returned.
		 * [ed] 12/28/04 use int because bool is sometimes byte-wide.
		 */
		static int boolean(Atom atom);

		/**
		 * Returns the passed atom's string representation.
		 * If the passed atom is not a string, it is coerced
		 * to type string using the ECMAScript coercion rules.
		 */
		Stringp string(Atom atom);

		Stringp coerce_s(Atom atom);

		/**
		 * Returns true if the passed atom is of string type.
		 */
		static bool isString(Atom atom)
		{
			return atomKind(atom) == kStringType && !isNull(atom);
		}

		static bool isName(Atom atom)
		{
			return isString(atom) && atomToString(atom)->isInterned();
		}

		/**
		 * an interned atom is canonicalized in this way:
		 * boolean -> "true" or "false"
		 * number -> intern'ed string value
		 * string -> intern'ed string value
		 * object -> intern'ed result of toString()
		 *
		 * this way, interned atoms are suitable to be used as map keys and can
		 * be compared using ==.
		 * @param atom
		 * @return
		 */
		Stringp intern(Atom atom);

		Namespacep internNamespace(Namespacep ns);

		/** Helper function; reads a signed 24-bit integer from pc */
		static int readS24(const byte *pc)
		{
			#ifdef AVMPLUS_UNALIGNED_ACCESS
				// unaligned short access still faster than 2 byte accesses
				return ((uint16_t*)pc)[0] | ((int8_t*)pc)[2]<<16;
			#else
				return pc[0] | pc[1]<<8 | ((int8_t*)pc)[2]<<16;
			#endif
		}

        /**
         * Returns the size of the instruction + all it's operands.  For OP_lookupswitch the size will not include
         * the size for the case targets.
         */
		static int calculateInstructionWidth(const byte* p)
		{
            int a, b;
            unsigned int c, d;
			const byte* p2 = p;
            readOperands(p2, c, a, d, b);
			return int(p2-p);
		}

        /**
         * Read in some operands for the instruction located at *pc.  
         * Returns the size of the instruction, but will not read in all the case targets for 
         * an OP_lookupswitch, since there will be a variable number of them. 
         */
        static void readOperands(const byte* &pc, unsigned int& imm32, int& imm24, unsigned int& imm32b, int& imm8 )
        {
            AbcOpcode opcode = (AbcOpcode)*pc++;
            int op_count = opcodeInfo[opcode].operandCount;

            imm8 = pc[0];
			if( opcode == OP_pushbyte || opcode == OP_debug )
			{
				// these two operands have a byte as their first operand, which is not encoded
				// with the variable length encoding scheme for bigger numbers, because it will
				// never be larger than a byte.
				--op_count;
				pc++;
			}

			if( op_count > 0 )
			{
                if( opcode >= OP_ifnlt && opcode <= OP_lookupswitch )
                {
                    imm24 = AvmCore::readS24(pc);
                    pc += 3;
                }
                else
                {
    				imm32 = AvmCore::readU30(pc);
                }

				if( opcode == OP_debug )
                {
                    --op_count; //OP_debug has a third operand of a byte
                    pc++;
                }
                if( op_count > 1 )
				{
					imm32b = AvmCore::readU30(pc);
				}
			}
        }
		/** 
         * Helper function; reads an unsigned 32-bit integer from pc 
         * See AbcParser::readS32 for more explanation of the variable length
         * encoding scheme.  
         * 
         * 2nd argument is set to the actual size, in bytes, of the number read in,
         * and third argument is the version of the ABC 
         */
		static uint32 readU30(const byte *&p)
		{
			unsigned int result = p[0];
			if (!(result & 0x00000080))
			{
				p++;
				return result;
			}
			result = (result & 0x0000007f) | p[1]<<7;
			if (!(result & 0x00004000))
			{
				p += 2;
				return result;
			}
			result = (result & 0x00003fff) | p[2]<<14;
			if (!(result & 0x00200000))
			{
				p += 3;
				return result;
			}
			result = (result & 0x001fffff) | p[3]<<21;
			if (!(result & 0x10000000))
			{
				p += 4;
				return result;
			}
			result = (result & 0x0fffffff) | p[4]<<28;
			p += 5;
			return result;
		}

		// when you need to skip over a u30 and don't care about the result,
		// this is slightly faster.
		static void skipU30(const uint8_t*& p, int count = 1)
		{
			while (count-- > 0)
			{
				if (!(p[0] & 0x80)) { p += 1; continue; }
				if (!(p[1] & 0x80)) { p += 2; continue; }
				if (!(p[2] & 0x80)) { p += 3; continue; }
				if (!(p[3] & 0x80)) { p += 4; continue; }
				//if (!(*p[4] & 0x80)) { p += 5; continue; }	// test should be unnecessary
				AvmAssert(!(p[4] & 0x80));
				p += 5;
			}
		}

		/** Helper function; reads an unsigned 16-bit integer from pc */
		static int32_t readU16(const byte *pc)
		{
			#ifdef AVMPLUS_UNALIGNED_ACCESS
				// unaligned short access still faster than 2 byte accesses
				return *((uint16_t*)pc);
			#else
				return pc[0] | pc[1]<<8;
			#endif
		}

		/**
		 * this is the implementation of the actionscript "is" operator.  similar to java's
		 * instanceof.  returns true/false according to AS rules.  in particular, it will return
		 * false if value==null.
		 */
		static bool istype(Atom atom, Traits* itraits);

		/**
		 * this is the implementation of the actionscript "is" operator.  similar to java's
		 * instanceof.  returns true/false according to AS rules.  in particular, it will return
		 * false if value==null.
		 */
		static Atom istypeAtom(Atom atom, Traits* itraits) 
		{ 
			return istype(atom, itraits) ? trueAtom : falseAtom; 
		}

		/**
		 * implements ECMA as operator.  Returns the same value, or null.
		 */
		static Atom astype(Atom atom, Traits* expected)
		{
			return istype(atom, expected) ? atom : nullObjectAtom;
		}

		/**
		 * implementation of OP_increg, decreg, increment, decrement which correspond to
		 * ++ and -- operators in AS.
		 */
		void increment_d(Atom *atom, int delta);

		/**
		 * implementation of OP_increg, decreg, increment, decrement which correspond to
		 * ++ and -- operators in AS.
		 */
		void increment_i(Atom *atom, int delta);
		
		/**
		 * ES3's internal ToPrimitive() function
		 */
		static Atom primitive(Atom atom);

		/** OP_toboolean; ES3 ToBoolean() */
		static Atom booleanAtom(Atom atom);

		/** OP_tonumber; ES3 ToNumber */
		Atom numberAtom(Atom atom);
		
		/**
		 * ES3's internal ToNumber() function for internal use
		 */
		static double number(Atom atom);

		/**
		 * The interrupt method is called from executing code
		 * when the interrupted flag is set.
		 */
		virtual void interrupt(MethodEnv *env) = 0;

		/**
		 * This is called when the stack overflows
		 * (when the machine stack pointer is about to go below
		 *  minstack)
		 */
		virtual void stackOverflow(MethodEnv *env) = 0;

		void _stackOverflow(MethodEnv *env) { stackOverflow(env); }
		
		/**
		 * Throws an exception.  Constructs an Exception object
		 * and calls throwException.
		 */
		void throwAtom(Atom atom);

		/**
		 * The AVM+ equivalent of the C++ "throw" statement.
		 * Throws an exception, transferring control to the
		 * nearest CATCH block.
		 */
		void throwException(Exception *exception);

		/**
		 * throwErrorV is a convenience function for throwing
		 * an exception with a formatted error message,
		 */
		void throwErrorV(ClassClosure *type, int errorID, Stringp arg1=0, Stringp arg2=0, Stringp arg3=0);

		/**
		 * formatErrorMessageV is a convenience function for
		 * assembling an error message with varags.
		 */
		String* formatErrorMessageV( int errorID, Stringp arg1=0, Stringp arg2=0, Stringp arg3=0);

		/**
		 * Convenience methods for converting various objects into value 
		 * strings used for error message output.
		 */
		String* toErrorString(int d);
		String* toErrorString(MethodInfo* m);
		String* toErrorString(const Multiname& n);
		String* toErrorString(const Multiname* n);
		String* toErrorString(Namespacep ns);
		String* toErrorString(const Traits* t);
		String* toErrorString(const char* s);
		String* toErrorString(const wchar* s);
		String* atomToErrorString(Atom a);

		/**
		 * getErrorMessage returns the format string for an
		 * errorID.  Override to provide format strings for
		 * additional implementation-dependent error strings.
		 */
		virtual String* getErrorMessage(int errorID);

		#ifdef DEBUGGER
		/**
		 * willExceptionBeCaught walks all the way up the
		 * ActionScript stack to see if there is any "catch"
		 * statement which is going to catch the specified
		 * exception.
		 */
		bool willExceptionBeCaught(Exception* exception);

		/**
		 * findErrorMessage searches an error messages table.
		 * Only available in debugger builds.
		 */
		String* findErrorMessage(int errorID,
								 int* mapTable,
								 const char** errorTable,
								 int numErrors);

		/**
		 * Determines the language id of the given platform
		 */
		virtual int determineLanguage();
		

		/**
		 * Creates a StackTrace from the current executing call stack
		 */
		StackTrace* newStackTrace();

		#ifdef _DEBUG
		void dumpStackTrace();
		#endif
#endif /* DEBUGGER */

		/** The call stack of currently executing code. */
		CallStackNode *callStack;

		CodeContextAtom codeContextAtom;

		CodeContext* codeContext() const;

		/** env of the highest catch handler on the call stack, or NULL */
		ExceptionFrame *exceptionFrame;
		
		Exception *exceptionAddr;

		/**
		 * Searches the exception handler table of info for
		 * a try/catch block that contains the instruction at pc
		 * and matches the type of exception.
		 * @param info      the method to search the exception handler
		 *                  table of
		 * @param pc        the program counter at the point where
		 *                  the exception occurred; either a pointer into
		 *                  bytecode or into native compiled code
		 * @param exception the exception object that was thrown;
		 *                  used to match the type.
		 * @return ExceptionHandler object for the exception
		 *         handler that matches, or re-throws the passed-in
		 *         exception if no handler is found.
		 */
		ExceptionHandler* findExceptionHandler(MethodInfo *info,
											   sintptr pc,
											   Exception *exception);
		
		ExceptionHandler* beginCatch(ExceptionFrame *ef,
				MethodInfo *info, sintptr pc, Exception *exception);

		/**
		 * Just like findExceptionHandler(), except that this function
		 * returns NULL if it can't find an exception handler, whereas
		 * findExceptionHandler() re-throws the passed-in exception if
		 * it can't find a handler.
		 */
		ExceptionHandler* findExceptionHandlerNoRethrow(MethodInfo *info,
														sintptr pc,
														Exception *exception);

		/**
		 * Returns true if the passed atom is an XML object,
		 * as defined in the E4X Specification.
		 */				
		inline static bool isXML(Atom atm)
		{
			return isBuiltinType(atm, BUILTIN_xml);
		}

		/**
		 * Returns true if the passed atom is a XMLList object,
		 * as defined in the E4X Specification.
		 */		
		static bool isXMLList(Atom atm)
		{
			return isBuiltinType(atm, BUILTIN_xmlList);
		}

		inline static bool isXMLorXMLList(Atom atm)
		{
			return isBuiltinTypeMask(atm, (1<<BUILTIN_xml)|(1<<BUILTIN_xmlList));
		}

		/* Returns tru if the atom is a Date object */
		inline static bool isDate(Atom atm)
		{
			return isBuiltinType(atm, BUILTIN_date);
		}

		// From http://www.w3.org/TR/2004/REC-xml-20040204/#NT-Name
		static bool isLetter(wchar c);
		static bool isDigit(wchar c);
		static bool isCombiningChar(wchar c);
		static bool isExtender(wchar c);

		Stringp ToXMLString (Atom a);
		Stringp EscapeElementValue (const Stringp s, bool removeLeadingTrailingWhitespace);
		Stringp EscapeAttributeValue (Atom v);

		/**
		 * Converts an Atom to a E4XNode if its traits match.
		 * Otherwise, null is returned. (An exception is NOT thrown)
		 */
		static E4XNode* atomToXML(Atom atm);

		/**
		 * Converts an Atom to a XMLObject if its traits match.
		 * Otherwise, null is returned. (An exception is NOT thrown)
		 */
		static XMLObject* atomToXMLObject(Atom atm);

		/**
		 * Converts an Atom to a XMLListObject if its traits match.
		 * Otherwise, null is returned. (An exception is NOT thrown)
		 */
		static XMLListObject* atomToXMLList(Atom atm);

		/**
		 * Returns true if the passed atom is a QName object,
		 * as defined in the E4X Specification.
		 */		
		static bool isQName(Atom atm)
		{
			return isBuiltinType(atm, BUILTIN_qName);
		}

		/**
		 * Returns true if the passed atom is a Dictionary object,
		 * as defined in the E4X Specification.
		 */		
		static bool isDictionary(Atom atm);

		static bool isDictionaryLookup(Atom key, Atom obj)
		{
			return isObject(key) && isDictionary(obj);
		}

		/**
		 * Returns true if the passed atom is a valid XML name,
		 * as defined in the E4X Specification.
		 */		
		bool isXMLName(Atom arg);

		/**
		 * Converts an Atom to a QNameObject if its traits match.
		 * Otherwise, null is returned. (An exception is NOT thrown)
		 */
		static QNameObject* atomToQName(Atom atm);

		/** Implementation of OP_typeof */		
		Stringp _typeof (Atom arg);

		/** The XML entities table, used by E4X */
		Hashtable *xmlEntities;
		
	private:
		static bool isBuiltinType(Atom atm, BuiltinType bt);
		static bool isBuiltinTypeMask(Atom atm, int btmask);

	private:
		//
		// this used to be Heap
		//

		/** size of interned String table */
		int stringCount;

		/** number of deleted entries in our String table */
		int deletedCount;
		#define AVMPLUS_STRING_DELETED ((Stringp)(1))

		/** size of interned Namespace table */
		int nsCount;

		int numStrings;
		int numNamespaces;
				
	public:

		static Namespacep atomToNamespace(Atom atom)
		{
			AvmAssert((atom&7)==kNamespaceType);
			return (Namespacep)(atom&~7);
		}
		
		static double atomToDouble(Atom atom)
		{
			AvmAssert((atom&7)==kDoubleType);
			return *(const double*)(atom&~7);
		}

		/**
		 * Convert an Atom of kStringType to a Stringp
		 * @param atom atom to convert.  Note that the Atom
		 *             must be of kStringType
		 */
		static Stringp atomToString(Atom atom)
		{
			AvmAssert((atom&7)==kStringType);
			return (Stringp)(atom&~7);
		}

		// Avoid adding validation checks here and returning NULL.  If this
		// is returning a bad value, the higher level function should be fixed
		// or AbcParser/Verifier should be enhanced to catch this case.
		static ScriptObject* atomToScriptObject(const Atom atom)
		{
			AvmAssert((atom&7)==kObjectType);
			return (ScriptObject*)(atom&~7);
		}
	
		// helper function, allows generic objects to work with Hashtable
		// and AtomArray, uses double type which is the only non-RC
		// GCObject type
		static Atom gcObjectToAtom(const void* obj);
		static const void* atomToGCObject(Atom a) { return (const void*)(a&~7); }
		static bool isGCObject(Atom a) { return (a&7)==kDoubleType; }

	private:
		/** search the string intern table */
		int findString(const wchar *s, int len);
		int findString(Stringp s);

		/** search the namespace intern table */
		int findNamespace(Namespacep ns);

	public:

		// String creation. If len is omitted, zero-termination is assumed.
		Stringp newStringLatin1(const char* str, int len = -1);
		Stringp newStringUTF8(const char* str, int len = -1, bool strict = false);
		Stringp newStringUTF16(const wchar* str, int len = -1);

		// decodes UTF16LE or UTF16BE.
		Stringp newStringEndianUTF16(bool littleEndian, const wchar* str, int len = -1);
		
		// like newStringLatin1, but the string constant is assumed to remain valid
		// for the life of the AvmCore. Generally, should only be used for literal
		// strings, eg newConstantStringLatin1("foo")
		Stringp newConstantStringLatin1(const char* str);


		// variants on the newStringXXX() calls that also intern the string.
		Stringp internStringLatin1(const char* s, int len = -1);
		Stringp internStringUTF8(const char* s, int len = -1, bool constant = false);
		Stringp internStringUTF16(const wchar* s, int len = -1);

		// like internStringLatin1, but the string constant is assumed to remain valid
		// for the life of the AvmCore. Generally, should only be used for literal
		// strings, eg internStringLatin1("foo")
		Stringp internConstantStringLatin1(const char* s);

		/**
		 * intern the given string atom which has already been allocated
		 * @param atom
		 * @return
		 */
		Stringp internString(Stringp s);
		Stringp internString(Atom atom);
		Stringp internInt(int n);
		Stringp internDouble(double d);
		Stringp internUint32(uint32 ui);


#ifdef DEBUGGER
		/**
		 * intern without allocating memory, returns NULL if its not already interned
		 */
		Stringp findInternedString(const char *s, int len);
#endif

		static bool getIndexFromAtom(Atom a, uint32 *result)
		{
			if (AvmCore::isInteger(a))
			{
				*result = uint32(a >> 3);
				return true;
			}
			else
			{
				AvmAssert(AvmCore::isString(a));
				return AvmCore::getIndexFromString(atomToString(a), result); 
			}
		}

		static bool getIndexFromString(Stringp s, uint32 *result);
			
		ScriptBufferImpl* newScriptBuffer(size_t size);
		VTable* newVTable(Traits* traits, VTable* base, ScopeChain* scope, AbcEnv* abcEnv, Toplevel* toplevel);

		RegExpObject* newRegExp(RegExpClass* regExpClass,
								Stringp pattern,
								Stringp options);

		ScriptObject* newObject(VTable* ivtable, ScriptObject *delegate);

		FrameState* newFrameState(int frameSize, int scopeBase, int stackBase);
        Namespacep newNamespace(Atom prefix, Atom uri, Namespace::NamespaceType type = Namespace::NS_Public);
		Namespacep newNamespace(Atom uri, Namespace::NamespaceType type = Namespace::NS_Public);
		Namespacep newNamespace(Stringp uri, Namespace::NamespaceType type = Namespace::NS_Public);
		Namespacep newPublicNamespace(Stringp uri) { return newNamespace(uri); }
		NamespaceSet* newNamespaceSet(int nsCount);

		Stringp uintToString(uint32 i);
		Stringp intToString(int i);
		Stringp doubleToString(double d);
		Stringp concatStrings(Stringp s1, Stringp s2);
		
		Atom uintToAtom(uint32 n);
		Atom intToAtom(int n);

		Atom allocDouble(double n)
		{
			union { 
				double *d;
				void *v;
			};
			v = GetGC()->Alloc(sizeof(double), 0);
			*d = n;
			return kDoubleType | (uintptr)v;
		}
		
		void rehashStrings(int newlen);
		void rehashNamespaces(int newlen);

		// static version for smart pointers
		static void atomWriteBarrier(MMgc::GC *gc, const void *container, Atom *address, Atom atomNew);

		static void decrementAtomRegion(Atom *ar, int length);

	public:
#ifdef AVMPLUS_VERBOSE
		Stringp format(Atom atom);
#endif
#if VMCFG_METHOD_NAMES
		Stringp formatAtomPtr(Atom atom);
#endif

	private:
		// hash set containing intern'ed strings
		DRC(Stringp) * strings;
		// hash set containing namespaces
		DRC(Namespacep) * namespaces;

#ifdef AVMPLUS_INTERNINT_CACHE
		// See code in AvmCore::internInt
		// cache of interned names of nonnegative integers (numeric value % 256)
		class IndexString : public MMgc::GCObject {
		public:
			int value;
			DRCWB(Stringp) string;
		};
		
		IndexString* index_strings[256];
#endif
		
#ifdef AVMPLUS_WORD_CODE
	private:
		// Saturating counter.  
		uint32 lookup_cache_timestamp;
	public:
		uint32 lookupCacheTimestamp() { return lookup_cache_timestamp == ~0U ? 0 : lookup_cache_timestamp; }
		bool   lookupCacheIsValid(uint32 t) { return t == lookup_cache_timestamp; }
		void   invalidateLookupCache() { if (lookup_cache_timestamp != ~0U) ++lookup_cache_timestamp; }
#endif
		
		// avoid multiple inheritance issues
		class GCInterface : MMgc::GCCallback
		{
		public:
			GCInterface(MMgc::GC * _gc) : MMgc::GCCallback(_gc), core(NULL) {}
			void SetCore(AvmCore* _core) { this->core = _core; }
			void presweep() { if(core) core->presweep(); }
			void postsweep() { if(core) core->postsweep(); }
			void log(const char *str) { if(core) core->console << str; }
			void oom(MMgc::MemoryStatus status) { if(core) core->oom(status); }
		private:
			AvmCore *core;
		};
		GCInterface gcInterface;
	};
}

#endif /* __avmplus_AvmCore__ */
