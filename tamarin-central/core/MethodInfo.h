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

#ifndef __avmplus_MethodInfo__
#define __avmplus_MethodInfo__


#ifdef verify
#undef verify
#endif

namespace avmplus
{
	typedef Atom (*AtomMethodProc)(MethodEnv*, int, uint32 *);
	typedef double (*DoubleMethodProc)(MethodEnv*, int, uint32 *);

#ifdef DEBUGGER
	class AbcFile;
	class DebuggerMethodInfo : public MMgc::GCObject
	{
	private:
		inline explicit DebuggerMethodInfo(int32_t _local_count, uint32_t _codeSize, int32_t _max_scopes) :
			firstSourceLine(0),
			lastSourceLine(0),
			offsetInAbc(0),
			local_count(_local_count), 
			codeSize(_codeSize), 
			max_scopes(_max_scopes) {}

	public:

		static DebuggerMethodInfo* create(AvmCore* core, int32_t _local_count, uint32_t _codeSize, int32_t _max_scopes);

		DWB(AbcFile*)			file;				// the abc file from which this method came
		int32_t					firstSourceLine;	// source line number where function starts
		int32_t					lastSourceLine;		// source line number where function ends
		int32_t					offsetInAbc;		// offset in abc file
		uint32_t				codeSize;			// abc size pre-jit, native size post jit
		const int32_t			local_count;		// FIXME: merge with localCount above; this one may be visible to a debugger?
		const int32_t			max_scopes;			// FIXME: merge with maxScopeDepth above; this one is not used by the VM but may be visible to a debugger?
		Stringp					localNames[1];		// array of names for args and locals in framep order, written with explicit WBRC (actually local_count)
	};
#endif

	class MethodSignature;

	/**
	 * MethodInfo is the base class for all functions that
	 * can be executed by the VM: Actionscript functions,
	 * native functions, etc.
	 */
#ifdef AVMPLUS_TRAITS_MEMTRACK
	class MethodInfo : public MMgc::GCFinalizedObject
#else
	class MethodInfo : public MMgc::GCObject
#endif
	{
		friend class CodegenLIR;
		friend class CodegenIMT;
	public:
		/** @name flags from .abc - limited to a BYTE */
		/*@{*/
		/** need arguments[0..argc] */
		static const int NEED_ARGUMENTS			= 0x00000001;

		/** need activation object */
		static const int NEED_ACTIVATION		= 0x00000002;

		/** need arguments[param_count+1..argc] */		
		static const int NEED_REST				= 0x00000004;

		/** has optional parameters */
		static const int HAS_OPTIONAL			= 0x00000008;

		/** allow extra args, but dont capture them */
		static const int IGNORE_REST			= 0x00000010;

		/** method is native */
		static const int NATIVE					= 0x00000020;

		/** method sets default namespace */
		static const int SETS_DXNS				= 0x00000040;

		/** method has table for parameter names */
		static const int HAS_PARAM_NAMES		= 0x00000080;

		/*@}*/
	private:
		/** @name internal flags - upper 3 BYTES available */
		/*@{*/
		// set iff this is a getter
		static const int IS_GETTER				= 0x00000100;

		// set iff this is a setter
		static const int IS_SETTER				= 0x00000200;

		static const int OVERRIDE				= 0x00000400;

		static const int NON_INTERRUPTIBLE		= 0x00000800;

		static const int UNBOX_THIS				= 0x00001000;

		static const int NEEDS_CODECONTEXT		= 0x00002000;

		static const int HAS_EXCEPTIONS			= 0x00004000;

		static const int NEEDS_DXNS				= 0x00008000;

		static const int VERIFIED				= 0x00010000;

#ifdef AVMPLUS_VERIFYALL
		static const int VERIFY_PENDING			= 0x00020000;
#endif

		/** indicates method is final, no overrides allowed */
		static const int FINAL					= 0x00040000;

		/** indicates the function is a method, that pushes the
		    receiver object onto the scope chain at method entry */
		static const int NEED_CLOSURE			= 0x00080000;

		/** set to indicate that a function has no bytecode body. */
		static const int ABSTRACT_METHOD		= 0x00100000;

		/**
		 * set once the signature types have been resolved and
		 * override signatures have been checked.
		 */
		static const int RESOLVED				= 0x00200000;

		/**
		 * set to indictate that a function has been 
		 * recommended to be interpreted. 
		 */
		static const int SUGGEST_INTERP			= 0x00400000;

		/**
		 * set to indicate that a function has been compiled
		 * to native code by the jit compiler.
		 */
		static const int JIT_IMPL				= 0x00800000;
		
#ifdef AVMPLUS_UNCHECKED_HACK
		static const int UNCHECKED				= 0x01000000;
#endif

		// unused:								= 0x02000000;
		// unused:								= 0x04000000;
		// unused:								= 0x08000000;
		// unused:								= 0x10000000;
		// unused:								= 0x20000000;
		// unused:								= 0x40000000;
		// unused:								= 0x80000000;

		/*@}*/

    public:
		// ctor for all normal methods.
		MethodInfo(int _method_id, 
					PoolObject* pool, 
					const uint8_t* abc_info_pos, 
					uint8_t abcFlags,
					const NativeMethodInfo* native_info);
		
		// special ctor for the methodinfo generated for slot-initialization when no init method is present
		enum InitMethodStub { kInitMethodStub };
		MethodInfo(InitMethodStub, Traits* declTraits);

#if defined FEATURE_NANOJIT && !VMCFG_METHODENV_IMPL32
		MethodInfo(void* tramp, Traits* declTraits);
#endif

		static Atom verifyEnter(MethodEnv* env, int argc, uint32* ap);

		inline uintptr_t iid() const { return ((uintptr_t)this)>>3; }

		bool usesCallerContext() const;

		// Builtin + non-native functions always need the dxns code emitted 
		// Builtin + native functions have flags to specify if they need the dxns code
		bool usesDefaultXmlNamespace() const;

		void resolveSignature(const Toplevel* toplevel);

#ifdef DEBUGGER
	public:
		static AvmBox debugEnterExitWrapper32(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		static double debugEnterExitWrapperN(AvmMethodEnv env, uint32_t argc, AvmBox* argv);

		void boxLocals(void* src, int srcPos, Traits** traitArr, Atom* dest, int destPos, int length);
		void unboxLocals(Atom* src, int srcPos, Traits** traitArr, void* dest, int destPos, int length);

		void setFile(AbcFile* file);
		void setRegName(int index, Stringp name);

		Stringp getArgName(int index);
		Stringp getLocalName(int index);

		AbcFile* file() const;
		int32_t firstSourceLine() const;
		int32_t lastSourceLine() const;
		int32_t offsetInAbc() const;
		uint32_t codeSize() const;
		int32_t local_count() const;
		int32_t max_scopes() const;

		void updateSourceLines(int32_t linenum, int32_t offset);

	private:
		Stringp getRegName(int index) const;
		DebuggerMethodInfo* dmi() const;
#endif

	public:

		void verify(Toplevel* toplevel);
        void setInterpImpl();

	public:
		
		void makeIntoPrototypeFunction(const Toplevel* toplevel);
		bool makeMethodOf(Traits* type);


		inline int allowExtraArgs() const { return _flags & (NEED_REST|NEED_ARGUMENTS|IGNORE_REST); }
		inline int hasExceptions() const { return _flags & HAS_EXCEPTIONS; }
		inline int hasMethodBody() const { return !(_flags & ABSTRACT_METHOD); }
		inline int hasOptional() const { return _flags & HAS_OPTIONAL; }
		inline int isNative() const { return _flags & NATIVE; }
		inline int isNonInterruptible() { return _flags & NON_INTERRUPTIBLE; }
		inline int isResolved() { return _flags & RESOLVED; }
		inline int needActivation() const { return _flags & NEED_ACTIVATION; }
		inline int needArguments() const { return _flags & NEED_ARGUMENTS; }
		inline int needClosure() const { return _flags & NEED_CLOSURE; }
		inline int needRest() const { return _flags & NEED_REST; }
		inline int needRestOrArguments() const { return _flags & (NEED_REST|NEED_ARGUMENTS); }
		inline int setsDxns() const { return _flags & SETS_DXNS; }
		inline int suggestInterp() const { return _flags & SUGGEST_INTERP; }
		inline int unboxThis() const { return _flags & UNBOX_THIS; }
		inline int isJitImpl() const { return _flags & JIT_IMPL; } // @ todo, Flash needs this for now, remove soon

		inline void setUnboxThis() { _flags |= UNBOX_THIS; }
		inline void setSuggestInterp() { _flags |= SUGGEST_INTERP; }
		inline void setHasExceptions() { _flags |= HAS_EXCEPTIONS; }
		inline void setNeedsDxns() { _flags |= NEEDS_DXNS; }
		inline void setFinal() { _flags |= FINAL; }
		inline void setOverride() { _flags |= OVERRIDE; }
		inline void makeNonInterruptible() { _flags |= NON_INTERRUPTIBLE; }

		inline void setKind(TraitKind kind) 
		{ 
			if (kind == TRAIT_Getter)
				_flags |= MethodInfo::IS_GETTER;
			else if (kind == TRAIT_Setter)
				_flags |= MethodInfo::IS_SETTER;
		}
		
#ifdef AVMPLUS_VERIFYALL
		inline int isVerified() const { return _flags & VERIFIED; }
		inline int isVerifyPending() { return _flags & VERIFY_PENDING; }
		inline void setVerified() { _flags = _flags | VERIFIED & ~VERIFY_PENDING; }
		inline void setVerifyPending() { _flags |= VERIFY_PENDING; }
#endif


#ifdef AVMPLUS_TRAITS_MEMTRACK
	protected:
		virtual ~MethodInfo();
#endif

	public:

#if VMCFG_METHOD_NAMES
		Stringp FASTCALL getMethodName() const;
#endif		

#ifdef AVMPLUS_VERBOSE
		Stringp format(AvmCore* core) const;
#endif
#ifdef DEBUGGER
		uint32 size();
#endif

	public:

		inline PoolObject* pool() const { return _pool; }

		inline AvmThunkNativeThunker thunker() const { AvmAssert(isNative()); return _native.thunker; }
		inline AvmThunkNativeMethodHandler handler_method() const { AvmAssert(isNative()); return _native.handler.method; }
		inline AvmThunkNativeFunctionHandler handler_function() const { AvmAssert(isNative()); return _native.handler.function; }

		inline const uint8_t* abc_body_pos() const { AvmAssert(!isNative()); return _abc.body_pos; }
		inline void set_abc_body_pos(const uint8_t* p) { AvmAssert(!isNative()); _abc.body_pos = p; }
		inline void set_abc_body_pos_wb(MMgc::GC* gc, const uint8_t* p) { AvmAssert(!isNative()); WB(gc, this, &_abc.body_pos, p); }

		inline ExceptionHandlerTable* abc_exceptions() const { AvmAssert(!isNative()); return _abc.exceptions; }
		inline void set_abc_exceptions(MMgc::GC* gc, ExceptionHandlerTable* e) { AvmAssert(!isNative()); WB(gc, this, &_abc.exceptions, e); }

	#ifdef AVMPLUS_WORD_CODE

		inline ExceptionHandlerTable* word_code_exceptions() const { AvmAssert(!isNative()); return _abc.word_code.exceptions; }
		inline void set_word_code_exceptions(MMgc::GC* gc, ExceptionHandlerTable* e) { AvmAssert(!isNative()); WB(gc, this, &_abc.word_code.exceptions, e); }

		inline const uintptr_t* word_code_start() const { AvmAssert(!isNative()); return _abc.word_code.translated_code->data; }
		inline void set_word_code(MMgc::GC* gc, TranslatedCode* translated_code) 
		{ 
			AvmAssert(!isNative()); 
			WB(gc, this, &_abc.word_code.translated_code, translated_code);
		}

		inline int word_code_cache_size() const { AvmAssert(!isNative()); return _abc.word_code.cache_size; }
		inline void set_word_code_cache_size(int s) { AvmAssert(!isNative()); _abc.word_code.cache_size = s; }
	
	#endif

		inline int method_id() const { return _method_id; }

		inline AtomMethodProc impl32() const { return _impl32; }
		inline DoubleMethodProc implN() const { return _implN; }
		inline Traits* declaringTraits() const { return _declaringTraits; }
		inline Traits* activationTraits() const { return _activationTraits; }

		inline void set_activationTraits(Traits* t) { _activationTraits = t; }

		inline MethodSignaturep getMethodSignature()
		{
			AvmAssert(isResolved());
			AvmAssert(_msref != NULL);
			MethodSignaturep ms;
			if ((ms = (MethodSignaturep)_msref->get()) == NULL)
				ms = _getMethodSignature();
			return ms;
		}

		void update_max_stack(int max_stack);

	private:
		MethodSignature* FASTCALL _getMethodSignature();
		MethodSignature* FASTCALL _buildMethodSignature(const Toplevel* toplevel);

	private:
		struct NativeInfo
		{
			AvmThunkNativeThunker thunker;
			AvmThunkNativeHandler handler;
		};

		struct AbcInfo
		{
			const uint8_t*			body_pos;
			ExceptionHandlerTable*	exceptions;		// we write this once, in Verifier, with an explicit WB.  so no DWB.
	#ifdef AVMPLUS_WORD_CODE
			struct 
			{
				TranslatedCode*			translated_code;	// The object that contains the code pointed to by body_pos, written with explicit WB
				// We write this once, in WordcodeTranslator, with an explicit WB.  so no DWB.
				// The contents are the same as the 'exceptions' structure above, except the 'from', 'to', and 'target' fields.
				ExceptionHandlerTable*	exceptions;
				int						cache_size;     // Number of items in lookup cache
			} word_code;
	#endif
		};

	// ------------------------ DATA SECTION BEGIN
	private:
		// these are (probably) most-frequently accessed so put at offset zero (allow LIR to generate trivially smaller code)
		union 
		{
			AtomMethodProc		_impl32;
			DoubleMethodProc	_implN;
		};
		DWB(MMgc::GCWeakRef*)	_msref;				// our MethodSignature 
		DWB(Traits*)			_declaringTraits;
		DWB(Traits*)			_activationTraits;
		PoolObject* const		_pool;
		const uint8_t* const	_abc_info_pos;		// pointer to abc MethodInfo record 
		int						_flags;				// see bitmask defs above 
		const int				_method_id;
		union 
		{
			NativeInfo			_native;			// stuff used only for Native methods (formerly in NativeMethod)
			AbcInfo				_abc;				// stuff used only for bytecode methods (formerly in MethodInfo)
		};
	// ------------------------ DATA SECTION END
	};

	class MethodSignature : public QCachedItem
	{
		friend class MethodInfo;
	public:
#ifdef AVMPLUS_TRAITS_MEMTRACK 
		MethodSignature();
		virtual ~MethodSignature();
#endif
	private:
		union AtomOrType
		{
			Traits* paramType;
			Atom defaultValue;
		};
		
	public:
		inline Traits* returnTraits() const { return _returnTraits; }
		inline BuiltinType returnTraitsBT() const { return Traits::getBuiltinType(_returnTraits); }

		inline int param_count() const { return _param_count; }
		inline int optional_count() const { return _optional_count; }
		inline int rest_offset() const { return _rest_offset; }

		inline int max_stack() const { AvmAssert(!(_flags & MethodInfo::NATIVE)); return _max_stack; }
		inline int local_count() const { AvmAssert(!(_flags & MethodInfo::NATIVE)); return _local_count; }
		inline int max_scope() const { AvmAssert(!(_flags & MethodInfo::NATIVE)); return _max_scope; }
		inline int frame_size() const { AvmAssert(!(_flags & MethodInfo::NATIVE)); return _frame_size; }

	#ifdef AVMPLUS_WORD_CODE
	#else
		inline const uint8_t* abc_code_start() const { return _abc_code_start; }
	#endif
	
		inline int requiredParamCount() const { return _param_count - _optional_count; }

		inline Traits* paramTraits(int i) const { AvmAssert(i >= 0 && i <= _param_count); return _args[i].paramType; }
		inline BuiltinType paramTraitsBT(int i) const { AvmAssert(i >= 0 && i <= _param_count); return Traits::getBuiltinType(_args[i].paramType); }
		inline Atom getDefaultValue(int i) const { AvmAssert(i >= 0 && i < _optional_count); return _args[i+_param_count+1].defaultValue; }

		inline bool argcOk(int argc) const
		{
			const int ALLOW_EXTRA_ARGS = MethodInfo::NEED_REST | MethodInfo::NEED_ARGUMENTS | MethodInfo::IGNORE_REST;
			return argc >= (_param_count - _optional_count) && 
					(argc <= _param_count || (_flags & ALLOW_EXTRA_ARGS));
		}

		void boxArgs(AvmCore* core, int argc, const uint32_t* ap, Atom* out) const;
		
	// ------------------------ DATA SECTION BEGIN
	private:
		Traits*		_returnTraits;		// written with explicit WB
	#ifdef AVMPLUS_WORD_CODE
	#else
		const uint8_t*	_abc_code_start; // start of ABC body
	#endif
		int			_param_count;		// number of declared parameters including optionals 
		int			_optional_count;	// last optional_count params are optional 
		int			_rest_offset;		// offset to first rest arg, including the instance parameter. this is sum(sizeof(paramType(0..N)))
		int			_flags;				// dupe of owner's flags, for convenience
		int			_max_stack;			// abc-only: max stack
		int			_local_count;		// abc-only: maximum number of local registers
		int			_max_scope;			// abc-only: maximum depth of local scope stack
		int			_frame_size;		// abc-only: total size of frame in number of Atoms, derived from other values above
		AtomOrType	_args[1];			// lying, actually 1+param_count+optional_count
	// ------------------------ DATA SECTION END
	};
	
}

#endif /* __avmplus_MethodInfo__ */
