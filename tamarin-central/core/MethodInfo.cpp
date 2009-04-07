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

#ifdef FEATURE_NANOJIT
#include "CodegenLIR.h"
#endif

namespace avmplus
{
	using namespace MMgc;

#if defined FEATURE_NANOJIT && !VMCFG_METHODENV_IMPL32
	MethodInfo::MethodInfo(void* tramp, Traits* declTraits) :
		_pool(declTraits->pool),
		_declaringTraits(declTraits),
		_abc_info_pos(NULL),
		_flags(RESOLVED),
		_method_id(-1)
	{
		union {
			void* t;
			AtomMethodProc p;
		};
		t = tramp;
		this->_impl32 = p;
	}
#endif

	MethodInfo::MethodInfo(InitMethodStub, Traits* declTraits) :
		_impl32(verifyEnter),
		_msref(declTraits->pool->core->GetGC()->emptyWeakRef),
		_declaringTraits(declTraits),
		_activationTraits(NULL),
		_pool(declTraits->pool),
		_abc_info_pos(NULL),
		_flags(RESOLVED),
		_method_id(-1)
	{
	}

	MethodInfo::MethodInfo(int method_id, 
							PoolObject* pool, 
							const uint8_t* abc_info_pos, 
							uint8_t abcFlags,
							const NativeMethodInfo* native_info) : 
		_impl32(verifyEnter),
		_msref(pool->core->GetGC()->emptyWeakRef),
		_declaringTraits(NULL),
		_activationTraits(NULL),
		_pool(pool),
		_abc_info_pos(abc_info_pos),
		_flags(abcFlags),
		_method_id(method_id)
	{

#if !defined(AVMPLUS_TRAITS_MEMTRACK) && !defined(MEMORY_INFO)
		MMGC_STATIC_ASSERT(offsetof(MethodInfo, _impl32) == 0);
#endif

		if (native_info)
		{
			this->_native.thunker = native_info->thunker;
			this->_native.handler = native_info->handler;
			this->_flags |= NEEDS_CODECONTEXT | NEEDS_DXNS | ABSTRACT_METHOD;
		}
		AVMPLUS_TRAITS_MEMTRACK_ONLY( tmt_add_inst(TMT_methodinfo, this); )
	}

#ifdef AVMPLUS_TRAITS_MEMTRACK
	MethodInfo::~MethodInfo()
	{
		AVMPLUS_TRAITS_MEMTRACK_ONLY( tmt_sub_inst(TMT_methodinfo, this); )
	}
#endif

    void MethodInfo::setInterpImpl() 
	{
		MethodSignaturep ms = getMethodSignature();
		if (ms->returnTraitsBT() == BUILTIN_number)
			_implN = avmplus::interpN;
		else
			_impl32 = avmplus::interp32;
    }

#ifdef DEBUGGER
	/*static*/ AvmBox MethodInfo::debugEnterExitWrapper32(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
	{
		CallStackNode csn(CallStackNode::kEmpty); 
		env->debugEnter(/*frametraits*/0, &csn, /*framep*/0, /*eip*/0); 
		const AvmBox result = env->method->thunker()(env, argc, argv);
		env->debugExit(&csn);
		return result;
	}

	/*static*/ double MethodInfo::debugEnterExitWrapperN(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
	{
		CallStackNode csn(CallStackNode::kEmpty); 
		env->debugEnter(/*frametraits*/0, &csn, /*framep*/0, /*eip*/0); 
		const double result = (reinterpret_cast<AvmThunkNativeThunkerN>(env->method->thunker()))(env, argc, argv);
		env->debugExit(&csn);
		return result;
	}
#endif

	/*static*/ Atom MethodInfo::verifyEnter(MethodEnv* env, int argc, uint32* ap)
	{
		MethodInfo* f = env->method;

		#ifdef AVMPLUS_VERIFYALL
		// never verify late in verifyall mode
		AvmAssert(!f->pool()->core->config.verifyall);
		#endif

		f->verify(env->toplevel());

        AvmAssert(f->impl32() != MethodInfo::verifyEnter);
#if VMCFG_METHODENV_IMPL32
		env->_impl32 = f->impl32();
#endif
		return f->impl32()(env, argc, ap);
	}

	void MethodInfo::verify(Toplevel *toplevel)
	{
		AvmAssert(declaringTraits()->isResolved());
		resolveSignature(toplevel);
		AvmCore* core = this->pool()->core;
		if (isNative())
		{
			union {
				Atom (*impl32)(MethodEnv*, int, uint32 *);
				AvmThunkNativeThunker thunker;
				AvmThunkNativeThunkerN thunkerN;
			} u;
	#ifdef DEBUGGER
			if (core->debugger())
			{
				MethodSignaturep ms = getMethodSignature();
				if (ms->returnTraitsBT() == BUILTIN_number)
					u.thunkerN = MethodInfo::debugEnterExitWrapperN;
				else
					u.thunker = MethodInfo::debugEnterExitWrapper32;
			}
			else
	#endif
			{
				u.thunker = this->thunker();
			}
			this->_impl32 = u.impl32;
		}
		else
		{
			#ifdef DEBUGGER
			// just a fake CallStackNode here , so that if we throw a verify error, 
			// we get a stack trace with the method being verified as its top entry.
			// init with an empty setup when debugger() isn't present, so we can
			// skip the call to getMethodName(), which is nonzero
			CallStackNode callStackNode(CallStackNode::kNoOp);
			if (core->debugger())
				callStackNode.init(this->pool()->core, this->getMethodName());
			#endif /* DEBUGGER */

			if (!_abc.body_pos)
			{
				// no body was supplied in abc
				toplevel->throwVerifyError(kNotImplementedError, this->pool()->core->toErrorString(this));
			}


		    #if defined FEATURE_NANOJIT
			Verifier verifier(this, toplevel);

			if ((core->IsJITEnabled()) && !suggestInterp())
			{
				PERFM_NTPROF("verify & IR gen");

				CodegenLIR jit(this);
                #if defined AVMPLUS_WORD_CODE
                #ifdef AVMPLUS_DIRECT_THREADED
				WordcodeEmitter translator(this, interpGetOpcodeLabels());
                #else
				WordcodeEmitter translator(this);
                #endif
				TeeWriter teeWriter(&translator, &jit);
				CodeWriter *coder = &teeWriter;
                #else
				CodeWriter *coder = &jit;
                #endif

				TRY(core, kCatchAction_Rethrow)
				{
				    verifier.verify(coder);	// pass 2 - data flow

					PERFM_TPROF_END();
			
					if (!jit.overflow)
						jit.emitMD(); // pass 3 - generate code

					// the MD buffer can overflow so we need to re-iterate
					// over the whole thing, since we aren't yet robust enough
					// to just rebuild the MD code.

					// mark it as interpreted and try to limp along
					if (jit.overflow) {
						setInterpImpl();
					}
	                #ifdef AVMPLUS_WORD_CODE
					else {
						if (_abc.word_code.translated_code) 
						{
							set_word_code(core->GetGC(), NULL);
						}
						if (_abc.word_code.exceptions) 
						{
							_abc.word_code.exceptions = NULL;
						}
					}
                    #endif
				}
				CATCH (Exception *exception) 
				{
					// re-throw exception
					core->throwException(exception);
				}
				END_CATCH
				END_TRY
			}
			else
			{
			    // NOTE copied below
                #if defined AVMPLUS_WORD_CODE
                #ifdef AVMPLUS_DIRECT_THREADED
				WordcodeEmitter translator(this, interpGetOpcodeLabels());
                #else
				WordcodeEmitter translator(this);
                #endif
				CodeWriter *coder = &translator;
                #else
				CodeWriter stubWriter;
				CodeWriter *coder = &stubWriter;
                #endif
			    verifier.verify(coder); // pass2 dataflow
				setInterpImpl();
				// NOTE end copy
			}
            #else // FEATURE_NANOJIT

			Verifier verifier(this, toplevel);

			// NOTE copied from above
            #if defined AVMPLUS_WORD_CODE
            #ifdef AVMPLUS_DIRECT_THREADED
			WordcodeEmitter translator(this, interpGetOpcodeLabels());
            #else
			WordcodeEmitter translator(this);
            #endif
			CodeWriter *coder = &translator;
            #else
			CodeWriter stubWriter;
			CodeWriter *coder = &stubWriter;
            #endif
			verifier.verify(coder); // pass2 dataflow
			setInterpImpl();
			// NOTE end copy

            #endif // FEATURE_NANOJIT
			
			#ifdef DEBUGGER
			// no explicit exit call needed for fake CallStackNodes, they auto-cleanup in dtor
			// (note that this is true even if we didn't call CallStackNode::init; the dtor is a no-op in that case)
			//callStackNode.exit();
			#endif /* DEBUGGER */
		}
	}

#ifdef DEBUGGER

	/*static*/ DebuggerMethodInfo* DebuggerMethodInfo::create(AvmCore* core, int32_t local_count, uint32_t codeSize, int32_t max_scopes)
	{
		MMgc::GC* gc = core->GetGC();
		const uint32_t extra = (local_count <= 1) ? 0 : (sizeof(Stringp)*(local_count-1));

		DebuggerMethodInfo* dmi = new (gc, extra) DebuggerMethodInfo(local_count, codeSize, max_scopes);
		const Stringp undef = core->kundefined;
		for (int32_t i=0; i<local_count; i++)
		{
			WBRC(gc, dmi, &dmi->localNames[i], undef);
		}
		return dmi;
	}

	AbcFile* MethodInfo::file() const
	{
		DebuggerMethodInfo* dmi = this->dmi();
		return dmi ? (AbcFile*)(dmi->file) : NULL;
	}
	
	int32_t MethodInfo::firstSourceLine() const
	{
		DebuggerMethodInfo* dmi = this->dmi();
		return dmi ? dmi->firstSourceLine : 0;
	}

	int32_t MethodInfo::lastSourceLine() const
	{
		DebuggerMethodInfo* dmi = this->dmi();
		return dmi ? dmi->lastSourceLine : 0;
	}

	int32_t MethodInfo::offsetInAbc() const
	{
		DebuggerMethodInfo* dmi = this->dmi();
		return dmi ? dmi->offsetInAbc : 0;
	}

	uint32_t MethodInfo::codeSize() const
	{
		DebuggerMethodInfo* dmi = this->dmi();
		return dmi ? dmi->codeSize : 0;
	}

	int32_t MethodInfo::local_count() const
	{
		DebuggerMethodInfo* dmi = this->dmi();
		return dmi ? dmi->local_count : 0;
	}

	int32_t MethodInfo::max_scopes() const
	{
		DebuggerMethodInfo* dmi = this->dmi();
		return dmi ? dmi->max_scopes : 0;
	}

	void MethodInfo::setFile(AbcFile* file) 
	{
		DebuggerMethodInfo* dmi = this->dmi();
		if (dmi)
			dmi->file = file;
	}

	Stringp MethodInfo::getArgName(int index) 
	{ 
		return getRegName(index); 
	}

	Stringp MethodInfo::getLocalName(int index) 
	{ 
		return getRegName(index+getMethodSignature()->param_count()); 
	}

	void MethodInfo::updateSourceLines(int32_t linenum, int32_t offset)
	{
		DebuggerMethodInfo* dmi = this->dmi();
		if (dmi)
		{
			if (dmi->firstSourceLine == 0 || linenum < dmi->firstSourceLine)
				dmi->firstSourceLine = linenum;

			if (dmi->offsetInAbc == 0 || offset < dmi->offsetInAbc)
				dmi->offsetInAbc = offset;

			if (dmi->lastSourceLine == 0 || linenum > dmi->lastSourceLine)
				dmi->lastSourceLine = linenum;
		}
	}

	// Note: dmi() can legitimately return NULL (for MethodInfo's that were synthesized by Traits::genInitBody).
	// However: 
	// -- we assert that the result is not null because we should never be called for such MethodInfo instances.
	// -- the caller always checks for null (despite the claim above) because crashing the Debugger would be a faux pas,
	// and historically the Debugger code is riddled with lots of extra such checks-for-null, so why stop now?
	DebuggerMethodInfo* MethodInfo::dmi() const
	{
		// rely on the fact that not-in-pool MethodInfo returns -1,
		// which will always be > methodCount as uint32
		const uint32_t method_id = uint32_t(this->method_id());
		AvmAssert(_pool->core->debugger() != NULL);
		// getDebuggerMethodInfo quietly returns NULL for out-of-range.
		DebuggerMethodInfo* d = _pool->getDebuggerMethodInfo(method_id);
		AvmAssert(d != NULL);
		return d;
	}

	Stringp MethodInfo::getRegName(int slot) const 
	{
		DebuggerMethodInfo* dmi = this->dmi();

		if (dmi && slot >= 0 && slot < dmi->local_count)
			return dmi->localNames[slot];

		return this->pool()->core->kundefined;
	}

	void MethodInfo::setRegName(int slot, Stringp name)
	{
		DebuggerMethodInfo* dmi = this->dmi();

		if (!dmi || slot < 0 || slot >= dmi->local_count)
			return;

		AvmCore* core = this->pool()->core;

		// [mmorearty 5/3/05] temporary workaround for bug 123237: if the register
		// already has a name, don't assign a new one
		if (dmi->localNames[slot] != core->kundefined)
			return;

		WBRC(core->GetGC(), dmi, &dmi->localNames[slot], core->internString(name));
	}

	/**
	 * convert ap[start]...ap[start+count-1] entries from their native types into
	 * Atoms.  The result is placed into out[to]...out[to+count-1].
	 * 
	 * The traitArr is used to determine the type of conversion that should take place.
	 * traitArr[start]...traitArr[start+count-1] are used.
	 *
	 * If the method is interpreted then we just copy the Atom, no conversion is needed.
	 */
	void MethodInfo::boxLocals(void* src, int srcPos, Traits** traitArr, Atom* dest, int destPos, int length)
	{
		int size = srcPos+length;
		int at = destPos;

		// if we are running jit then the types are native and we
		// need to box em.
		if (_flags & JIT_IMPL)
		{
			// each entry is a pointer into the function's stack frame
			int64_t* in = (int64_t*)src;			// WARNING this must match with JIT generator (endianness issue???)

			// now probe each type and do the atom conversion.
			AvmCore* core = this->pool()->core;
			for (int i=srcPos; i<size; i++)
			{
				Traits* t = traitArr[i];
				void *p = &in[i];   // jit uses 8B per entry
				if (t == NUMBER_TYPE) 
				{
					dest[at] = core->doubleToAtom( *((double*)p) );
				}
				else if (t == INT_TYPE)
				{
					dest[at] = core->intToAtom( *((int*)p) );
				}
				else if (t == UINT_TYPE)
				{
					dest[at] = core->uintToAtom( *((uint32*)p) );
				}
				else if (t == BOOLEAN_TYPE)
				{
					dest[at] = *((int*)p) ? trueAtom : falseAtom;
				}
				else if (!t || t == OBJECT_TYPE || t == VOID_TYPE)
				{
					dest[at] = *((Atom*)p);
				}
				else
				{
					// it's a pointer type, either null or some specific atom tag.
					void* ptr = *((void**)p); // unknown pointer
					if (t == STRING_TYPE)
					{
						dest[at] = ((Stringp)ptr)->atom();
					}
					else if (t == NAMESPACE_TYPE)
					{
						dest[at] = ((Namespace*)ptr)->atom();
					}
					else 
					{
						dest[at] = ((ScriptObject*)ptr)->atom();
					}
				}
				at++;
			}
		}
		else
		{
			// no JIT then we know they are Atoms and we just copy them
			Atom* in = (Atom*)src;
			for(int i=srcPos; i<size; i++)
				dest[at++] = in[i];
		}
	}

	/**
	 * convert in[0]...in[count-1] entries from Atoms to native types placing them 
	 * in ap[start]...out[start+count-1].
	 * 
	 * The traitArr is used to determine the type of conversion that should take place.
	 * traitArr[start]...traitArr[start+count-1] are used.
	 *
	 * If the method is interpreted then we just copy the Atom, no conversion is needed.
	 */
	void MethodInfo::unboxLocals(Atom* src, int srcPos, Traits** traitArr, void* dest, int destPos, int length)
	{
		#ifdef AVMPLUS_64BIT
		AvmAssertMsg(false, "are these ops right for 64-bit?  alignment of int/uint/bool?\n");
		#endif
		int size = destPos+length;
		int at = srcPos;

		// If the method has been jit'd then we need to box em, otherwise just
		// copy them 
		if (_flags & JIT_IMPL)
		{
			// we allocated double sized entry for each local src CodegenJIT
			int64_t* out = (int64_t*)dest;		// WARNING this must match with JIT generator

			// now probe each type and conversion.
			AvmCore* core = this->pool()->core;
			for (int i=destPos; i<size; i++)
			{
				Traits* t = traitArr[i];
				void *p = &out[i];      // JIT uses 8B per entry
				if (t == NUMBER_TYPE) 
				{
					*((double*)p) = AvmCore::number_d(src[at++]);
				}
				else if (t == INT_TYPE)
				{
					*((int*)p) = AvmCore::integer_i(src[at++]);
				}
				else if (t == UINT_TYPE)
				{
					*((uint32*)p) = AvmCore::integer_u(src[at++]);
				}
				else if (t == BOOLEAN_TYPE)
				{
					*((int*)p) = (int)(src[at++]>>3);
				}
				else if (!t || t == OBJECT_TYPE || t == VOID_TYPE)
				{
					*((Atom*)p) = src[at++];
				}
				else
				{
					// ScriptObject, String, Namespace, or Null
					*((sintptr*)p) = (src[at++] & ~7);
				}
			}
		}
		else
		{
			// no JIT then we know they are Atoms and we just copy them
			Atom* out = (Atom*)dest;
			for(int i=destPos; i<size; i++)
				out[i] = src[at++];
		}
	}

#endif //DEBUGGER


#ifdef AVMPLUS_VERBOSE
	Stringp MethodInfo::format(AvmCore* core) const
	{
		Stringp n = getMethodName();
		return n ?
				n->appendLatin1("()") :
				core->newConstantStringLatin1("?()");
	}
#endif // AVMPLUS_VERBOSE

	bool MethodInfo::makeMethodOf(Traits* traits)
	{
		AvmAssert(!isResolved());
		
		if (!_declaringTraits)
		{
			_declaringTraits = traits;
			_flags |= NEED_CLOSURE;
			return true;
		}
		else
		{
			#ifdef AVMPLUS_VERBOSE
			if (pool()->verbose)
				pool()->core->console << "WARNING: method " << this << " was already bound to " << declaringTraits() << "\n";
			#endif

			return false;
		}
	}

	void MethodInfo::makeIntoPrototypeFunction(const Toplevel* toplevel)
	{
		// Duplicate function definitions can happen with well formed ABC data.  We need
		// to clear out data on AbstractionFunction so it can correctly be re-initialized.
		// If our old function is ever used incorrectly, we throw an verify error in 
		// MethodEnv::coerceEnter.
		if (this->_declaringTraits)
		{
			_flags &= ~RESOLVED;
			this->_declaringTraits = NULL;
		}

		// make sure param & return types are fully resolved.
		// this does not set the verified flag, so real verification will
		// still happen before the function runs the first time.
		resolveSignature(toplevel);

		// type of F is synthetic subclass of Function, with a unique
		// [[call]] property and a unique scope

		const int method_id = this->method_id();
		Traits* ftraits = Traits::newFunctionTraits(toplevel, pool(), method_id);
		this->_declaringTraits = ftraits;
	}
	
	/**
	 * convert native args to atoms.  argc is the number of
	 * args, not counting the instance which is arg[0].  the
	 * layout is [instance][arg1..argN]
	 */
	void MethodSignature::boxArgs(AvmCore* core, int argc, const uint32_t* ap, Atom* out) const
	{
		MMGC_STATIC_ASSERT(sizeof(Atom) == sizeof(void*));	// if this ever changes, this function needs smartening
		typedef const Atom* ConstAtomPtr;
		// box the typed args, up to param_count
		const int param_count = this->param_count();
		for (int i=0; i <= argc; i++)
		{
			Atom atom;
			const BuiltinType bt = (i <= param_count) ? this->paramTraitsBT(i) : BUILTIN_any;
			switch (bt)
			{
				case BUILTIN_number:
					atom =  core->doubleToAtom(*(const double *)ap);
				#ifdef AVMPLUS_64BIT
					// nothing
				#else
					ap += 1;
				#endif
					break;

				case BUILTIN_int:
					atom = core->intToAtom((int32_t)*ConstAtomPtr(ap));
					break;

				case BUILTIN_uint:
					atom = core->uintToAtom((uint32_t)*ConstAtomPtr(ap));
					break;

				case BUILTIN_boolean:
					atom = (*ConstAtomPtr(ap)) ? trueAtom : falseAtom;
					break;

				case BUILTIN_object:
				case BUILTIN_any:
					atom = *ConstAtomPtr(ap);
					break;

				case BUILTIN_string:
					atom = ((Stringp)*ConstAtomPtr(ap))->atom();
					break;

				case BUILTIN_namespace:
					atom = ((Namespacep)*ConstAtomPtr(ap))->atom();
					break;

				default:
					atom = ((ScriptObject*)*ConstAtomPtr(ap))->atom();
					break;
			}
			out[i] = atom;
			ap += sizeof(Atom) / sizeof(uint32_t);
		}
	}

	static uint32_t argSize(Traits* t) { return Traits::getBuiltinType(t) == BUILTIN_number ? sizeof(double) : sizeof(Atom); }

	MethodSignature* FASTCALL MethodInfo::_buildMethodSignature(const Toplevel* toplevel) 
	{ 
		PoolObject* pool = this->pool();
		AvmCore* core = pool->core;
		GC* gc = core->GetGC();

		const byte* pos = this->_abc_info_pos;
		const uint32_t param_count = pos ? AvmCore::readU30(pos) : 0;
		uint32_t optional_count = 0;
		uint32_t rest_offset = 0;
		Traits* returnType;
		Traits* receiverType;
		
		// we only need param_count+optional_count extra, but we don't know
		// optional_count yet, so over-allocate for the worst case. (we know optional_count<=param_count).
		// this wastes space, but since we only cache a few dozen of these, it's preferable to walking the data twice
		// to get an exact count.
		const uint32_t extra = sizeof(MethodSignature::AtomOrType) * (param_count + (hasOptional() ? param_count : 0));
		MethodSignature* ms = new (gc, extra) MethodSignature();
		if (pos)
		{
			returnType = pool->resolveTypeName(pos, toplevel, /*allowVoid=*/true);
			receiverType = (_flags & NEED_CLOSURE) ? declaringTraits() : core->traits.object_itraits;
			rest_offset = argSize(receiverType);
#ifdef AVMPLUS_UNCHECKED_HACK
			uint32_t untyped_args = 0;
#endif
			for (uint32_t i=1; i <= param_count; i++)
			{
				Traits* argType = pool->resolveTypeName(pos, toplevel);
				WB(gc, ms, &ms->_args[i].paramType, argType);
				rest_offset += argSize(argType);
#ifdef AVMPLUS_UNCHECKED_HACK
				untyped_args += (argType == NULL);
#endif
			}
			AvmCore::skipU30(pos); // name_index;
			pos++; // abcFlags;
#ifdef AVMPLUS_UNCHECKED_HACK
			// toplevel!=NULL check is so we only check when resolveSignature calls us (not subsequently)
			if (toplevel != NULL)
			{
				// HACK - compiler should do this, and only to toplevel functions
				// that meet the E4 criteria for being an "unchecked function"
				// the tests below are too loose

				// if all params and return types are Object then make all params optional=undefined
				if (param_count == 0)
					_flags |= IGNORE_REST;
				if (!hasOptional() && returnType == NULL && param_count > 0 && untyped_args == param_count)
				{
					_flags |= HAS_OPTIONAL | IGNORE_REST | UNCHECKED;
					// oops -- the ms we allocated is too small, has no space for optional values
					// but it's easy to re-create, especially since we know the types are all null
					const uint32_t extra = sizeof(MethodSignature::AtomOrType) * (param_count + param_count);
					ms = new (gc, extra) MethodSignature();
					// don't need to re-set paramTypes: they are inited to NULL which is the right value
				}
			}
			if (_flags & UNCHECKED)
			{
				optional_count = param_count;
				for (uint32_t j=0; j < optional_count; j++)
				{
					//WBATOM(gc, ms, &ms->_args[param_count+1+j].defaultValue, undefinedAtom);
					// since we know we're going from NULL->undefinedAtom we can skip the WBATOM call
					ms->_args[param_count+1+j].defaultValue = undefinedAtom;
				}
			}
			else 
#endif // AVMPLUS_UNCHECKED_HACK
			if (hasOptional())
			{
				optional_count = AvmCore::readU30(pos);
				for (uint32_t j=0; j < optional_count; j++)
				{
					const int param = param_count-optional_count+1+j;
					const int index = AvmCore::readU30(pos);
					CPoolKind kind = (CPoolKind)*pos++;

					// check that the default value is legal for the param type
					Traits* argType = ms->_args[param].paramType;
					const Atom value = pool->getLegalDefaultValue(toplevel, index, kind, argType);
					WBATOM(gc, ms, &ms->_args[param_count+1+j].defaultValue, value);
				}
			}
			
			if (!isNative())
			{
				const byte* body_pos = this->abc_body_pos();
				if (body_pos)
				{
					ms->_max_stack = AvmCore::readU30(body_pos);
					ms->_local_count = AvmCore::readU30(body_pos);
					const int init_scope_depth = AvmCore::readU30(body_pos);
					ms->_max_scope = AvmCore::readU30(body_pos) - init_scope_depth;
				#ifdef AVMPLUS_WORD_CODE
				#else
					ms->_abc_code_start = body_pos;
					AvmCore::skipU30(ms->_abc_code_start); // code_length
				#endif
				}
			}
		}
		else
		{
			// this is a synthesized MethodInfo from genInitBody
			AvmAssert(param_count == 0);
			rest_offset = sizeof(Atom);
			returnType = pool->core->traits.void_itraits;
			receiverType = declaringTraits();
			// values derived from Traits::genInitBody()
			const int max_stack = 2;
			const int local_count = 1;
			const int init_scope_depth = 1;
			const int max_scope_depth = 1;
			ms->_max_stack = max_stack;
			ms->_local_count = local_count;
			ms->_max_scope = max_scope_depth - init_scope_depth;
		#ifdef AVMPLUS_WORD_CODE
		#else
			ms->_abc_code_start = this->abc_body_pos();
			AvmCore::skipU30(ms->_abc_code_start, 5);
		#endif
		}
		ms->_frame_size = ms->_local_count + ms->_max_scope + ms->_max_stack;
		#ifndef AVMPLUS_64BIT
		// The interpreter wants this to be padded to a doubleword boundary because
		// it allocates two objects in a single alloca() request - the frame and
		// auxiliary storage, in that order - and wants the second object to be
		// doubleword aligned.
		ms->_frame_size = (ms->_frame_size + 1) & ~1;
		#endif
		ms->_param_count = param_count;
		ms->_optional_count = optional_count;
		ms->_rest_offset = rest_offset; 
		ms->_flags = this->_flags; 
		WB(gc, ms, &ms->_returnTraits, returnType);
		WB(gc, ms, &ms->_args[0].paramType, receiverType);

		AvmAssert(_msref->get() == NULL);
		_msref = ms->GetWeakRef();
		core->msCache()->add(ms);
		return ms;
	}

	MethodSignature* FASTCALL MethodInfo::_getMethodSignature() 
	{ 
		AvmAssert(this->isResolved());
		// note: MethodSignaturep are always built the first time in resolveSignature; this is only 
		// executed for subsequent re-buildings. Thus we pass NULL for toplevel (it's only used
		// for verification errors, but those will have been caught prior to this) and for
		// abcGen and imtBuilder (since those only need to be done once).
		MethodSignature* ms = _buildMethodSignature(NULL);
		return ms;
	}

	void MethodInfo::update_max_stack(int max_stack)
	{
		MethodSignature* ms = (MethodSignature*)_msref->get();
		if (ms)
		{
			ms->_max_stack = max_stack;
		}
	}

	void MethodInfo::resolveSignature(const Toplevel* toplevel)
	{
		if (!(_flags & RESOLVED))
		{
			MethodSignature* ms = _buildMethodSignature(toplevel);
			
			if (!isNative())
			{
				if (ms->frame_size() < 0 || ms->local_count() < 0 || ms->max_scope() < 0)
					toplevel->throwVerifyError(kCorruptABCError);
			}

			if (ms->paramTraits(0)->isInterface)
				_flags |= ABSTRACT_METHOD;

			_flags |= RESOLVED;
		}
	}

#ifdef DEBUGGER
	uint32 MethodInfo::size()  
	{
		uint32 size = sizeof(MethodInfo);
		size += getMethodSignature()->param_count() * 2 * sizeof(Atom);
		size += codeSize();
		return size;
	}
#endif

	bool MethodInfo::usesCallerContext() const
	{
		return pool()->isBuiltin && (!isNative() || (_flags & NEEDS_CODECONTEXT));
	}

	// Builtin + non-native functions always need the dxns code emitted 
	// Builtin + native functions have flags to specify if they need the dxns code
	bool MethodInfo::usesDefaultXmlNamespace() const
	{
		return pool()->isBuiltin && (!isNative() || (_flags & NEEDS_DXNS));
	}

#if VMCFG_METHOD_NAMES
	Stringp MethodInfo::getMethodName() const 
	{
		Stringp name = NULL;
		const int method_id = this->method_id();
		
		PoolObject* pool = this->pool();
		AvmCore* core = pool->core;
		if (core->config.methodNames)
		{
			name = pool->getMethodInfoName(method_id);
			if (name && name->length() == 0) 
			{
				name = core->kanonymousFunc;	
			}
			
			Traitsp t = this->declaringTraits();
			if (t)
			{
				Stringp tname = t->format(core);
				if (core->config.oldVectorMethodNames)
				{
					// Tamarin used to incorrectly return the internal name of these
					// Vector types rather than the "official" name due to initialization
					// order. Names on the left are "more correct" but old builds might
					// require the "classic" name for compatibility purposes, so check.
					struct NameMapRec { const char* n; const char* o; };
					static const NameMapRec kNameMap[4] = 
					{
						{ "Vector.<Number>", "Vector$double" }, 
						{ "Vector.<int>", "Vector$int" }, 
						{ "Vector.<uint>", "Vector$uint" }, 
						{ "Vector.<*>", "Vector$object" },
					};
					for (int i = 0; i < 4; ++i)
					{
						if (tname->equalsLatin1(kNameMap[i].n))
							tname = core->newConstantStringLatin1(kNameMap[i].o);
					}
				};
				
				if (this == t->init)
				{
					// careful, name could be null, that's ok for init methods
					if (t->posType() == TRAITSTYPE_SCRIPT_FROM_ABC)
					{
						name = tname->appendLatin1("$init");
					}
					else if (t->posType() == TRAITSTYPE_CLASS_FROM_ABC)
					{
						name = tname->appendLatin1("cinit");
					}
					else
					{
						AvmAssert(t->posType() == TRAITSTYPE_INSTANCE_FROM_ABC || t->posType() == TRAITSTYPE_ACTIVATION);
						name = tname;
					}
				}
				else if (name)
				{
					const char* sep;
					if (_flags & IS_GETTER)
						sep = "/get ";
					else if (_flags & IS_SETTER)
						sep = "/set ";
					else 
						sep = "/";
					name = tname->appendLatin1(sep)->append(name);
				}
			}
		}
		
		// if config.methodNames isn't set, might as well still return a non-null result
		if (name == NULL)
			name = core->concatStrings(core->newConstantStringLatin1("MethodInfo-"), core->intToString(method_id));
		
		return name;
	}
#endif		

#ifdef AVMPLUS_TRAITS_MEMTRACK
	MethodSignature::MethodSignature()
	{
		AVMPLUS_TRAITS_MEMTRACK_ONLY( tmt_add_inst( TMT_methodsig, this); )
	}

	MethodSignature::~MethodSignature()
	{
		AVMPLUS_TRAITS_MEMTRACK_ONLY( tmt_sub_inst( TMT_methodsig, this); )
	}
#endif

}
