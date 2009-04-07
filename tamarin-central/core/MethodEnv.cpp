/* -*- Mode: C++; c-basic-offset: 4; indent-tabs-mode: t; tab-width: 4; -*- */
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


#include "avmplus.h"

namespace avmplus
{
#undef DEBUG_EARLY_BINDING

	// note that some of these have (partial) guts of Toplevel::coerce replicated here, for efficiency.
	// if you find bugs here, you might need to update Toplevel::coerce as well (and vice versa).
	// (Note: toplevel is passed as the final parameter as it's only used in exception cases,
	// thus preserving our precious FASTCALL args (only 2 on x86-32) for more-frequently used args.)
	static Atom* FASTCALL unbox1(Atom atom, Traits* t, Atom* args, Toplevel* toplevel)
	{
		// using computed-gotos here doesn't move the needle appreciably in my testing
		switch (Traits::getBuiltinType(t))
		{
			case BUILTIN_any:
				// my, that was easy
				break;

			case BUILTIN_boolean:
				atom = AvmCore::boolean(atom);
				break;

			case BUILTIN_int:
				atom = AvmCore::integer(atom);
				break;

			case BUILTIN_uint:
				atom = AvmCore::toUInt32(atom);
				break;

			case BUILTIN_namespace:
				// coerce undefined -> Namespace should yield null
				if (AvmCore::isNullOrUndefined(atom))
				{
					atom = 0;
					break;
				}
				if (atomKind(atom) != kNamespaceType)
					goto failure;
				atom = (Atom)atomPtr(atom);
				break;

			case BUILTIN_number:
			{
				#ifdef AVMPLUS_64BIT
					AvmAssert(sizeof(Atom) == sizeof(double));
					union 
					{
						double d;
						Atom a;
					};
					d = AvmCore::number(atom);
					atom = a;
				#else
					AvmAssert(sizeof(Atom)*2 == sizeof(double));
					union 
					{
						double d;
						Atom a[2];
					};
					d = AvmCore::number(atom);
					args[0] = a[0];
					args += 1;
					atom = a[1];	// fall thru, will be handled at end
				#endif
				break;
			}
			case BUILTIN_object:
				if (atom == undefinedAtom)
					atom = nullObjectAtom;
				break;

			case BUILTIN_string:
				atom = AvmCore::isNullOrUndefined(atom) ? NULL : (Atom)toplevel->core()->string(atom);
				break;

			case BUILTIN_null:
			case BUILTIN_void:
				AvmAssert(!"illegal, should not happen");
				atom = 0;
				break;

			case BUILTIN_date:
			case BUILTIN_math:
			case BUILTIN_methodClosure:
			case BUILTIN_qName:
			case BUILTIN_vector:
			case BUILTIN_vectordouble:
			case BUILTIN_vectorint:
			case BUILTIN_vectoruint:
			case BUILTIN_xml:
			case BUILTIN_xmlList:
				// a few intrinsic final classes can skip containsInterface calls
				if (AvmCore::isNullOrUndefined(atom))
				{
					atom = 0;
					break;
				}
				else if (atomKind(atom) == kObjectType)
				{
					Traits* actual = AvmCore::atomToScriptObject(atom)->traits();
					AvmAssert(actual->final);
					if (actual == t)
					{
						atom = (Atom)atomPtr(atom);
						break;
					}
				}
				// didn't break? that's a failure
				goto failure;

			case BUILTIN_array:
			case BUILTIN_class:
			case BUILTIN_error:
			case BUILTIN_function:
			case BUILTIN_none:
			case BUILTIN_regexp:
			case BUILTIN_vectorobj:	// unlike other vector types, vectorobj is NOT final
				if (AvmCore::isNullOrUndefined(atom))
				{
					atom = 0;
					break;
				}
				else if (atomKind(atom) == kObjectType)
				{
					Traits* actual = AvmCore::atomToScriptObject(atom)->traits();
					if (actual->containsInterface(t))
					{
						atom = (Atom)atomPtr(atom);
						break;
					}
				}
				// didn't break? that's a failure
				goto failure;
		}
		// every case increments by at least 1
		args[0] = atom;
		return args+1;

	failure:
		#ifdef AVMPLUS_VERBOSE
		// core->console << "checktype failed " << t << " <- " << atom << "\n";
		#endif
		toplevel->throwTypeError(kCheckTypeFailedError, toplevel->core()->atomToErrorString(atom), toplevel->core()->toErrorString(t));
		return NULL;	// unreachable
	}

	// Note that this function is (currently) only used for interpreted functions.
	inline Atom coerceAtom(AvmCore* core, Atom atom, Traits* t, Toplevel* toplevel)
	{
		switch (Traits::getBuiltinType(t))
		{
		case BUILTIN_number:
			return (atomKind(atom) == kDoubleType) ? atom : core->numberAtom(atom);
		case BUILTIN_int:
			return (atomKind(atom) == kIntegerType) ? atom : core->intAtom(atom);
		case BUILTIN_uint:
			return (atomKind(atom) == kIntegerType && atom >= 0) ? atom : core->uintAtom(atom);
		case BUILTIN_boolean:
			return (atomKind(atom) == kBooleanType) ? atom : AvmCore::booleanAtom(atom);
		case BUILTIN_object:
			return (atom == undefinedAtom) ? nullObjectAtom : atom;
		case BUILTIN_any:
			return atom;
		default:
			return toplevel->coerce(atom, t);
		}
	}
	
	// helper
	inline int MethodEnv::startCoerce(int argc, MethodSignaturep ms)
	{
		Toplevel* toplevel = this->toplevel();

		if (!ms->argcOk(argc))
		{
			toplevel->argumentErrorClass()->throwError(kWrongArgumentCountError, 
													   core()->toErrorString(method), 
													   core()->toErrorString(ms->requiredParamCount()), 
													   core()->toErrorString(argc));
		}

		// Can happen with duplicate function definitions from corrupt ABC data.  F1 is defined
		// and F2 overrides the F1 slot which is okay as long as F1's MethodEnv is never called again.
		if (method->declaringTraits() != this->declTraits)
		{
			toplevel->throwVerifyError(kCorruptABCError);
		}

		// now unbox everything, including instance and rest args
		const int param_count = ms->param_count();
		const int extra = argc > param_count ? argc - param_count : 0;
		AvmAssert(ms->rest_offset() > 0 && extra >= 0);

		return extra;
	}

	// helper
	inline Atom MethodEnv::endCoerce(int argc, uint32 *ap, MethodSignaturep ms)
	{
		// we know we have verified the method, so we can go right into it.
		AvmCore* core = this->core();
		const int bt = ms->returnTraitsBT();
		if (bt == BUILTIN_number)
		{
			AvmAssert(method->implN() != NULL);
			return core->doubleToAtom(method->implN()(this, argc, ap));
		}
		
		AvmAssert(method->impl32() != NULL);
		const Atom i = method->impl32()(this, argc, ap);
		switch (bt)
		{
		case BUILTIN_int:
			return core->intToAtom((int)i);
		case BUILTIN_uint:
			return core->uintToAtom((uint32)i);
		case BUILTIN_boolean:
			return i ? trueAtom : falseAtom;
		case BUILTIN_any:
		case BUILTIN_object:
		case BUILTIN_void:
			return (Atom)i;
		case BUILTIN_string:
			return ((Stringp)i)->atom();
		case BUILTIN_namespace:
			return ((Namespace*)i)->atom();
		default:
			return ((ScriptObject*)i)->atom();
		}
	}

	// In interp-only builds this could still be delegateInvoke or verifyEnter.
	//
	// OPTIMIZEME: It would be nice to avoid the unbox / rebox paths through
	// those functions in interpreter-only builds!
	
	inline bool MethodEnv::isInterpreted()
	{
		return impl32() == interp32 || implN() == interpN;
	}
	
	inline MethodSignaturep MethodEnv::get_ms()
	{
		if (!method->isResolved())
			method->resolveSignature(this->toplevel());
		return method->getMethodSignature();
	}
	
	// Optimization opportunities: since we call interp() directly, it is
	// probably possible to allocate its stack frame here and pass it in.
	// If we do so then interp() should deallocate it.  This affords us
	// the optimization of getting rid of alloca() allocation here, 
	// which means improved tail calls for once.  For another, if the argv
	// pointer points into the stack segment s.t. argv+argc+1 equals the
	// current stack pointer then the stack may be extended in place 
	// provided there's space.  But that optimization may equally well
	// be performed inside interp(), and in fact if we alloc temp
	// space on the alloca stack here then interp() would always perform
	// that optimization.  So we'd just be moving the decision into interp().
	
	// fast/optimized call to a function without parameters
	Atom MethodEnv::coerceEnter(Atom thisArg)
	{
		MethodSignaturep ms = get_ms();

		startCoerce(0, ms);
		// check receiver type first
		// caller will coerce instance if necessary,
		// so make sure it was done.
		AvmAssert(thisArg == toplevel()->coerce(thisArg, ms->paramTraits(0)));
		if (isInterpreted())
		{
			// Tail call inhibited by &thisArg, and also by &thisArg in "else" clause
			return interpA(this, 0, &thisArg, ms);
		}
		else
		{
			unbox1(thisArg, ms->paramTraits(0), &thisArg, toplevel());
			return endCoerce(0, (uint32*)&thisArg, ms);
		}
	}
	
	Atom MethodEnv::coerceEnter(Atom thisArg, ArrayObject *a)
	{
		int argc = a->getLength();
		if (argc == 0)
			return coerceEnter(thisArg);

		MethodSignaturep ms = get_ms();
		const int extra = startCoerce(argc, ms);

		// check receiver type first
		// caller will coerce instance if necessary,
		// so make sure it was done.
		AvmAssert(thisArg == toplevel()->coerce(thisArg, ms->paramTraits(0)));

		if (isInterpreted())
		{
			// Tail call inhibited by local allocation/deallocation
			MMgc::GC::AllocaAutoPtr _atomv;
			Atom* atomv = (Atom*)VMPI_alloca(core(), _atomv, sizeof(Atom)*(argc+1));
			atomv[0] = thisArg;
			for ( int i=0 ; i < argc ; i++ )
				atomv[i+1] = a->getUintProperty(i);
			return coerceEnter(argc, atomv);
		}
		else
		{
			const int rest_offset = ms->rest_offset();
			const size_t extra_sz = rest_offset + sizeof(Atom)*extra;
			MMgc::GC::AllocaAutoPtr _ap;
			uint32 *ap = (uint32 *)VMPI_alloca(core(), _ap, extra_sz);

			unboxCoerceArgs(thisArg, a, ap, ms);
			return endCoerce(argc, ap, ms);
		}
	}

	// It is enough that the exception handling mechanism knows how to unwind the alloca stack.
	// If TRY captures the current stack top and the CATCH and/or FINALLY just reset it, then
	// we're done.
	//
	// Then we can just call it AVMPI_alloca() and be done.
	//
	// Woot!
	
	Atom MethodEnv::coerceEnter(Atom thisArg, int argc, Atom *argv)
	{
		MethodSignaturep ms = get_ms();

		const int extra = startCoerce(argc, ms);

		// check receiver type first
		// caller will coerce instance if necessary,
		// so make sure it was done.
		AvmAssert(thisArg == toplevel()->coerce(thisArg, ms->paramTraits(0)));

		if (isInterpreted())
		{
			// Tail call inhibited by local allocation/deallocation
			MMgc::GC::AllocaAutoPtr _atomv;
			Atom* atomv = (Atom*)VMPI_alloca(core(), _atomv, sizeof(Atom)*(argc+1));
			atomv[0] = thisArg;
			VMPI_memcpy(atomv+1, argv, sizeof(Atom)*argc);
			return coerceEnter(argc, atomv);
		}
		else
		{
			const int rest_offset = ms->rest_offset();
			const size_t extra_sz = rest_offset + sizeof(Atom)*extra;
			MMgc::GC::AllocaAutoPtr _ap;
			uint32 *ap = (uint32 *)VMPI_alloca(core(), _ap, extra_sz);
				
			unboxCoerceArgs(thisArg, argc, argv, ap, ms);
			return endCoerce(argc, ap, ms);
		}
	}

	// note that GCC typically restricts tailcalls to functions with similar signatures
	// ("sibcalls") -- see http://www.ddj.com/architect/184401756 for a useful explanation.
	// anyway, since we really want coerceUnboxEnter to be a tailcall from
	// here, be sure to keep it using a compatible signature...
	Atom MethodEnv::coerceEnter(int argc, Atom* atomv)
	{
		// Trying hard here to allow this to be a tail call, so don't inline
		// coerceUnboxEnter into this function - the allocation it performs
		// may prevent the compiler from performing the tail call.
		//
		// The tail call is important in order to keep stack consumption down in an
		// interpreter-only configuration, but it's good always.

		if (isInterpreted())
		{
			AvmCore* core = this->core();
			Toplevel* toplevel = this->toplevel();
			
			MethodSignaturep ms = get_ms();
			AvmAssert(atomv[0] == toplevel->coerce(atomv[0], ms->paramTraits(0)));
			startCoerce(argc, ms);
			const int param_count = ms->param_count();
			const int end = argc >= param_count ? param_count : argc;
			for ( int i=1 ; i <= end ; i++ )
				atomv[i] = coerceAtom(core, atomv[i], ms->paramTraits(i), toplevel);
			return interpA(this, argc, atomv, ms);
		}
		else
			return coerceUnboxEnter(argc, atomv);
	}


	// In principle we want this to be inlined if the compiler is not tailcall-aware,
	// and not inlined if it is tailcall-aware (as doing so may inhibit tail calls).
	Atom MethodEnv::coerceUnboxEnter(int argc, Atom* atomv)
	{
		MethodSignaturep ms = get_ms();
		AvmAssert(atomv[0] == toplevel()->coerce(atomv[0], ms->paramTraits(0)));
		const int extra = startCoerce(argc, ms);
		const int rest_offset = ms->rest_offset();
		const size_t extra_sz = rest_offset + sizeof(Atom)*extra;
		MMgc::GC::AllocaAutoPtr _ap;
		uint32 *ap = (uint32 *)VMPI_alloca(core(), _ap, extra_sz);
			
		unboxCoerceArgs(argc, atomv, ap, ms);
		return endCoerce(argc, ap, ms);
	}

	/**
	 * convert atoms to native args.  argc is the number of
	 * args, not counting the instance which is arg[0].  the
	 * layout is [instance][arg1..argN]
	 */
	void MethodEnv::unboxCoerceArgs(int argc, Atom* in, uint32 *argv, MethodSignaturep ms)
	{
		Toplevel* toplevel = this->toplevel();
		
		Atom* args = (Atom*)argv;

		const int param_count = ms->param_count();
		int end = argc >= param_count ? param_count : argc;
		for (int i=0; i <= end; i++)
			args = unbox1(in[i], ms->paramTraits(i), args, toplevel);
		while (end < argc)
			*args++ = in[++end];
	}

	void MethodEnv::unboxCoerceArgs(Atom thisArg, ArrayObject *a, uint32 *argv, MethodSignaturep ms)
	{
		Toplevel* toplevel = this->toplevel();
		int argc = a->getLength();

		Atom *args = unbox1(thisArg, ms->paramTraits(0), (Atom *) argv, toplevel);

		const int param_count = ms->param_count();
		int end = argc >= param_count ? param_count : argc;
		for (int i=0; i < end; i++)
			args = unbox1(a->getUintProperty(i), ms->paramTraits(i+1), args, toplevel);
		while (end < argc)
			*args++ = a->getUintProperty(end++);
	}

	void MethodEnv::unboxCoerceArgs(Atom thisArg, int argc, Atom* in, uint32 *argv, MethodSignaturep ms)
	{
		Toplevel* toplevel = this->toplevel();

		Atom *args = unbox1(thisArg, ms->paramTraits(0), (Atom *) argv, toplevel);

		const int param_count = ms->param_count();
		int end = argc >= param_count ? param_count : argc;
		for (int i=0; i < end; i++)
			args = unbox1(in[i], ms->paramTraits(i+1), args, toplevel);
		while (end < argc)
			*args++ = in[end++];
	}

#if VMCFG_METHODENV_IMPL32
	Atom MethodEnv::delegateInvoke(MethodEnv* env, int argc, uint32 *ap)
	{
		env->_impl32 = env->method->impl32();
		return env->_impl32(env, argc, ap);
	}
#endif // VMCFG_METHODENV_IMPL32

#if defined(FEATURE_NANOJIT) && VMCFG_METHODENV_IMPL32
	MethodEnv::MethodEnv(TrampStub, void* tramp, VTable *vtable)
		: _vtable(vtable), method(NULL), declTraits(NULL), activationOrMCTable(0)
	{
		union { void* v; AtomMethodProc p; };
		v = tramp;
		_impl32 = p;
		AVMPLUS_TRAITS_MEMTRACK_ONLY( tmt_add_inst( TMT_methodenv, this); )
	}
#endif

	MethodEnv::MethodEnv(MethodInfo* method, VTable *vtable)
		: _vtable(vtable)
		, method(method)
		, declTraits(method->declaringTraits())
		, activationOrMCTable(0)
	{
		AvmAssert(method != NULL);
		
#if VMCFG_METHODENV_IMPL32
	#if !defined(AVMPLUS_TRAITS_MEMTRACK) && !defined(MEMORY_INFO)
		MMGC_STATIC_ASSERT(offsetof(MethodEnv, _impl32) == 0);
	#endif
		// make the first call go to the method impl
		_impl32 = delegateInvoke;
#endif

		AvmCore* core = this->core();
		if (method->declaringTraits() != this->_vtable->traits)
		{
		#ifdef AVMPLUS_VERBOSE
			core->console << "ERROR " << method->getMethodName() << " " << method->declaringTraits() << " " << vtable->traits << "\n";
		#endif
			AvmAssertMsg(0, "(method->declaringTraits != vtable->traits)");
			toplevel()->throwVerifyError(kCorruptABCError);
		}

		if (method->needActivation())
		{
			// This can happen when the ABC has MethodInfo data but not MethodBody data
			if (!method->activationTraits())
			{
				toplevel()->throwVerifyError(kCorruptABCError);
			}

			VTable *activation = core->newVTable(method->activationTraits(), NULL, this->scope(), this->abcEnv(), toplevel());
			activation->resolveSignatures();
			setActivationOrMCTable(activation, kActivation);
		}
		AVMPLUS_TRAITS_MEMTRACK_ONLY( tmt_add_inst( TMT_methodenv, this); )
	}
	
#ifdef AVMPLUS_TRAITS_MEMTRACK 
	MethodEnv::~MethodEnv()
	{
		AVMPLUS_TRAITS_MEMTRACK_ONLY( tmt_sub_inst(TMT_methodenv, this); )
	}
#endif

#ifdef DEBUGGER
	void MethodEnv::debugEnter(	Traits** frameTraits, 
								CallStackNode* callstack,
								Atom* framep, 
								volatile sintptr *eip)
	{
		AvmAssert(this != 0);
		AvmAssert(callstack != 0); 

		// dont reset the parameter traits since they are setup in the prologue
		if (frameTraits) {
            // @todo the traits should be cleared by the jit / interp ?!?
		    MethodSignaturep ms = get_ms();
            const int param_count = ms->param_count();
            const int firstLocalAt = param_count+1;
            const int localCount = ms->local_count();
            AvmAssert(localCount >= firstLocalAt);
			VMPI_memset(&frameTraits[firstLocalAt], 0, (localCount-firstLocalAt)*sizeof(Traits*));
        }

		callstack->init(this, framep, frameTraits, eip, /*boxed*/false);
		
		debugEnterInner();
	}
	
	void MethodEnv::debugEnterInner()
	{
		AvmAssert(this != 0);
		AvmCore* core = this->core();

		// update profiler
		Profiler* profiler = core->profiler();
		Sampler* s = core->get_sampler();
		if (profiler && profiler->profilingDataWanted && !(s && s->sampling()))
			profiler->sendFunctionEnter(method);

		// this shouldn't ever be called unless there's a debugger
		AvmAssert(core->debugger() != NULL);
		Debugger* debugger = core->debugger();
		if (debugger)
			debugger->_debugMethod(this);
		
		if (s)
			s->sampleCheck();

		// method_id can legitimately be -1 for activations, but we don't care about those here,
		// so just ignore them.
		const int method_id = this->method->method_id();
		if (method_id >= 0)
		{
			this->abcEnv()->invocationCount(method_id) += 1;	// returns a reference, so this works
		}
	}

	void MethodEnv::debugExit(CallStackNode* callstack)
	{
		AvmAssert(this != 0);
		AvmAssert(callstack != 0);
		AvmCore* core = this->core();

		// update profiler 
		Profiler* profiler = core->profiler();
		Sampler* s = core->get_sampler();
		if (profiler && profiler->profilingDataWanted && !(s && s->sampling()))
			profiler->sendFunctionExit();

		core->callStack = callstack->next();

		// trigger a faked "line number changed" since we exited the function and are now back on the old line (well, almost)
		if (core->callStack && core->callStack->linenum() > 0)
		{
			int line = core->callStack->linenum();
			core->callStack->set_linenum(-1);
			Debugger* debugger = core->debugger();
			if (debugger) 
				debugger->debugLine(line);
		}
	}

	uint64_t MethodEnv::invocationCount() const 
	{ 
		const int method_id = this->method->method_id();
		if (method_id < 0) return 0;
		return this->abcEnv()->invocationCount(method_id);
	}
#endif // DEBUGGER

    void FASTCALL MethodEnv::nullcheckfail(Atom atom)
    {
		AvmAssert(AvmCore::isNullOrUndefined(atom));

		// TypeError in ECMA
		ErrorClass *error = toplevel()->typeErrorClass();
		if( error ){
			error->throwError(
					(atom == undefinedAtom) ? kConvertUndefinedToObjectError :
										kConvertNullToObjectError);
		} else {
			toplevel()->throwVerifyError(kCorruptABCError);
		}
    }

	void MethodEnv::npe()
	{
		toplevel()->throwTypeError(kConvertNullToObjectError);
	}

	void MethodEnv::interrupt()
	{
		this->core()->interrupt(this);
	}

    void MethodEnv::stkover()
    {
        this->core()->stackOverflow(this);
    }

	ArrayObject* MethodEnv::createRest(Atom* argv, int argc)
	{
		// create arguments Array using argv[param_count..argc]
		MethodSignaturep ms = get_ms();
		const int param_count = ms->param_count();
		Atom* extra = argv + param_count + 1;
		const int extra_count = argc > param_count ? argc - param_count : 0;
		return toplevel()->arrayClass->newarray(extra, extra_count);
	}

#if defined FEATURE_NANOJIT

	Atom MethodEnv::getpropertyHelper(Atom obj, /* not const */ Multiname *multi, VTable *vtable, Atom index)
	{
		if ((index&7) == kIntegerType)
		{
			return getpropertylate_i(obj, (int)(index>>3));
		}

		if ((index&7) == kDoubleType)
		{
			int i = AvmCore::integer_i(index);
			if ((double)i == AvmCore::atomToDouble(index))
			{
				return getpropertylate_i(obj, i);
			}
		}

		if (AvmCore::isObject(index))
		{
			ScriptObject* i = AvmCore::atomToScriptObject(index);
			if (i->traits() == core()->traits.qName_itraits)
			{
				QNameObject* qname = (QNameObject*) i;
				qname->getMultiname(*multi);
			}
			else if(!multi->isRtns() && AvmCore::isDictionary(obj))
			{
				return AvmCore::atomToScriptObject(obj)->getAtomProperty(index);
			}
			else
			{
				multi->setName(core()->intern(index));
			}
		}
		else
		{
			multi->setName(core()->intern(index));
		}

		return toplevel()->getproperty(obj, multi, vtable);
	}

	void MethodEnv::initpropertyHelper(Atom obj, /* not const */ Multiname *multi, Atom value, VTable *vtable, Atom index)
	{
		if ((index&7) == kIntegerType)
		{
			setpropertylate_i(obj, (int)(index>>3), value);
			return;
		}

		if ((index&7) == kDoubleType)
		{
			int i = AvmCore::integer(index);
			uint32 u = (uint32)(i);
			if ((double)u == AvmCore::atomToDouble(index))
			{
				setpropertylate_u(obj, u, value);
				return;
			}
			else if ((double)i == AvmCore::atomToDouble(index))
			{
				setpropertylate_i(obj, i, value);
				return;
			}
		}

		if (AvmCore::isObject(index))
		{
			ScriptObject* i = AvmCore::atomToScriptObject(index);
			if (i->traits() == core()->traits.qName_itraits)
			{
				QNameObject* qname = (QNameObject*) i;
				qname->getMultiname(*multi);
			}
			else
			{
				multi->setName(core()->intern(index));
			}
		}
		else
		{
			multi->setName(core()->intern(index));
		}

		initproperty(obj, multi, value, vtable);
	}

	void MethodEnv::setpropertyHelper(Atom obj, /* not const */ Multiname *multi, Atom value, VTable *vtable, Atom index)
	{
		if ((index&7) == kIntegerType)
		{
			setpropertylate_i(obj, (int)(index>>3), value);
			return;
		}

		if ((index&7) == kDoubleType)
		{
			int i = AvmCore::integer(index);
			uint32 u = (uint32)(i);
			if ((double)u == AvmCore::atomToDouble(index))
			{
				setpropertylate_u(obj, u, value);
				return;
			}
			else if ((double)i == AvmCore::atomToDouble(index))
			{
				setpropertylate_i(obj, i, value);
				return;
			}
		}

		if (AvmCore::isObject(index))
		{
			ScriptObject* i = AvmCore::atomToScriptObject(index);
			if (i->traits() == core()->traits.qName_itraits)
			{
				QNameObject* qname = (QNameObject*) i;
				qname->getMultiname(*multi);
			}	
			else if(!multi->isRtns() && AvmCore::isDictionary(obj))
			{
				AvmCore::atomToScriptObject(obj)->setAtomProperty(index, value);
				return;
			}
			else
			{
				multi->setName(core()->intern(index));
			}
		}
		else
		{
			multi->setName(core()->intern(index));
		}

		toplevel()->setproperty(obj, multi, value, vtable);
	}
	
	Atom MethodEnv::delpropertyHelper(Atom obj, /* not const */ Multiname *multi, Atom index)
	{
		AvmCore* core = this->core();

		if (AvmCore::isObject(obj) && AvmCore::isObject(index))
		{
            if (AvmCore::isXMLList(index) )
            {
                // Error according to E4X spec, section 11.3.1
                toplevel()->throwTypeError(kDeleteTypeError, core->toErrorString(toplevel()->toTraits(index)));
            }
			ScriptObject* i = AvmCore::atomToScriptObject(index);
			if (i->traits() == core->traits.qName_itraits)
			{
				QNameObject* qname = (QNameObject*) i;
				qname->getMultiname(*multi);
			}
			else if(!multi->isRtns() && AvmCore::isDictionary(obj))
			{
				bool res = AvmCore::atomToScriptObject(obj)->deleteAtomProperty(index);
				return res ? trueAtom : falseAtom;
			}
			else
			{
				multi->setName(core->intern(index));
			}
		}
		else
		{
			multi->setName(core->intern(index));
		}

		return delproperty(obj, multi);
	}

	void MethodEnv::initMultinameLateForDelete(Multiname& name, Atom index)
	{
		AvmCore *core = this->core();
		
		if (AvmCore::isObject(index))
		{
            if (AvmCore::isXMLList(index))
            {
                // Error according to E4X spec, section 11.3.1
                toplevel()->throwTypeError(kDeleteTypeError, core->toErrorString(toplevel()->toTraits(index)));
            }
			
			ScriptObject* i = AvmCore::atomToScriptObject(index);
			if (i->traits() == core->traits.qName_itraits)
			{
				QNameObject* qname = (QNameObject*) i;
				bool attr = name.isAttr();
				qname->getMultiname(name);
				if (attr)
					name.setAttr(attr);
				return;
			}
		}

		name.setName(core->intern(index));
	}		
#endif

	ScriptObject* MethodEnv::newcatch(Traits* traits)
	{
		AvmCore* core = this->core();
		Toplevel* toplevel = this->toplevel();
		if (traits == core->traits.object_itraits)
		{
			// don't need temporary vtable.  this is a scope for a finally clause
			// todo: asc shouldn't even call OP_newcatch in a finally clause
			return toplevel->objectClass->construct();
		}
		else
		{
			VTable *vt = core->newVTable(traits, NULL, this->scope(), this->abcEnv(), toplevel);
			vt->resolveSignatures();
			return core->newObject(vt, NULL);
		}
	}

#if defined FEATURE_NANOJIT
	ArrayObject* MethodEnv::createArgumentsHelper(int argc, uint32 *ap)
	{
		// create arguments using argv[1..argc].
		// Even tho E3 says create an Object, E4 says create an Array so thats what we will do.
		AvmAssert(argc >= 0);
		MMgc::GC::AllocaAutoPtr _atomv;
		Atom* atomv = (Atom*) VMPI_alloca(core(), _atomv, (argc+1) * sizeof(Atom));
		method->getMethodSignature()->boxArgs(core(), argc, ap, atomv);
		return createArguments(atomv, argc);
	}

	ArrayObject* MethodEnv::createRestHelper(int argc, uint32 *ap)
	{
		// create rest Array using argv[param_count..argc]
		MethodSignaturep ms = get_ms();
		const int rest_offset = ms->rest_offset();
		Atom* extra = (Atom*) (rest_offset + (char*)ap);
		const int param_count = ms->param_count();
		const int extra_count = argc > param_count ? argc - param_count : 0;
		return toplevel()->arrayClass->newarray(extra, extra_count);
	}

#endif // FEATURE_NANOJIT

	Atom MethodEnv::getpropertylate_i(Atom obj, int index) const
	{
		// here we put the case for bind-none, since we know there are no bindings
		// with numeric names.
		if ((obj&7) == kObjectType)
		{
			if (index >= 0)
			{
				// try dynamic lookup on instance.  even if the traits are sealed,
				// we might need to search the prototype chain
				return AvmCore::atomToScriptObject(obj)->getUintProperty(index);
			}
			else
			{
				// negative - we must intern the integer
				return AvmCore::atomToScriptObject(obj)->getAtomProperty(this->core()->internInt(index)->atom());
			}
		}
		else
		{
			// primitive types are not dynamic, so we can go directly
			// to their __proto__ object
			AvmCore* core = this->core();
			Toplevel *toplevel = this->toplevel();
			ScriptObject *protoObject = toplevel->toPrototype(obj);
			return protoObject->ScriptObject::getStringPropertyFromProtoChain(core->internInt(index), protoObject, toplevel->toTraits(obj));			
		}
	}

	Atom MethodEnv::getpropertylate_u(Atom obj, uint32 index) const
	{
		// here we put the case for bind-none, since we know there are no bindings
		// with numeric names.
		if ((obj&7) == kObjectType)
		{
			// try dynamic lookup on instance.  even if the traits are sealed,
			// we might need to search the prototype chain
			return AvmCore::atomToScriptObject(obj)->getUintProperty(index);
		}
		else
		{
			// primitive types are not dynamic, so we can go directly
			// to their __proto__ object
			AvmCore* core = this->core();
			Toplevel *toplevel = this->toplevel();
			ScriptObject *protoObject = toplevel->toPrototype(obj);
			return protoObject->ScriptObject::getStringPropertyFromProtoChain(core->internUint32(index), protoObject, toplevel->toTraits(obj));			
		}
	}

	ScriptObject* MethodEnv::finddef(const Multiname* multiname) const
	{
		Toplevel* toplevel = this->toplevel();

		ScriptEnv* script = getScriptEnv(multiname);
		if (script == (ScriptEnv*)BIND_AMBIGUOUS)
            toplevel->throwReferenceError(kAmbiguousBindingError, multiname);

		if (script == (ScriptEnv*)BIND_NONE)
            toplevel->throwReferenceError(kUndefinedVarError, multiname);

		ScriptObject* global = script->global;
		if (!global)
		{
			global = script->initGlobal();
			Atom argv[1] = { global->atom() };
			script->coerceEnter(0, argv);
		}
		return global;
	}

	ScriptEnv* MethodEnv::getScriptEnv(const Multiname *multiname) const
	{
		ScriptEnv *se = (ScriptEnv*)abcEnv()->domainEnv()->getScriptInit(*multiname);
		if(!se)
		{	
			// check privates
			se = abcEnv()->getPrivateScriptEnv(*multiname);
		}
		return se;
	}

	ScriptObject* MethodEnv::finddefNsset(NamespaceSet* nsset, Stringp name) const
	{
		Multiname m(nsset);
		m.setName(name);
		return finddef(&m);
	}

	ScriptObject* MethodEnv::finddefNs(Namespace* ns, Stringp name) const
	{
		Multiname m(ns, name);
		return finddef(&m);
	}

	ScriptObject* ScriptEnv::initGlobal()
	{
		// object not defined yet.  define it by running the script that exports it
		this->_vtable->resolveSignatures();
		// resolving the vtable also resolves the traits, if necessary
		ScriptObject* delegate = this->toplevel()->objectClass->prototype;
		return global = this->core()->newObject(this->_vtable, delegate);
	}

    ScriptObject* MethodEnv::op_newobject(Atom* sp, int argc) const
    {
		// pre-size the hashtable since we know how many vars are coming
		VTable* object_ivtable = toplevel()->object_ivtable;
		AvmCore* core = this->core();

		ScriptObject* o = new (core->GetGC(), object_ivtable->getExtraSize()) 
			ScriptObject(object_ivtable, toplevel()->objectClass->prototype,
					2*argc+1);

		for (; argc-- > 0; sp -= 2)
		{
			Atom name = sp[-1];
			//verifier should ensure names are String
			//todo have the verifier take care of interning too
			AvmAssert(AvmCore::isString(name));

			o->setAtomProperty(core->internString(name)->atom(), sp[0]);
		}
		return o;
    }



	Atom MethodEnv::nextname(Atom objAtom, int index) const
	{
		if (index <= 0)
			return nullStringAtom;

		switch (objAtom&7)
		{
		case kObjectType:
			return AvmCore::atomToScriptObject(objAtom)->nextName(index);
		case kNamespaceType:
			return AvmCore::atomToNamespace(objAtom)->nextName(this->core(), index);
		default:
			ScriptObject* proto = toplevel()->toPrototype(objAtom);  // cn: types like Number are sealed, but their prototype could have dynamic properties
			return proto ? proto->nextName(index) : undefinedAtom;
		}
	}

	Atom MethodEnv::nextvalue(Atom objAtom, int index) const
	{
		if (index <= 0)
			return undefinedAtom;

		switch (objAtom&7)
		{
		case kObjectType:
			return AvmCore::atomToScriptObject(objAtom)->nextValue(index);
		case kNamespaceType:
			return AvmCore::atomToNamespace(objAtom)->nextValue(index);
		default:
			ScriptObject*  proto = toplevel()->toPrototype(objAtom);
			return (proto ? proto->nextValue(index) : undefinedAtom);
		}
	}

	int MethodEnv::hasnext(Atom objAtom, int index) const
	{
		if (index < 0)
			return 0;

		if (!AvmCore::isNullOrUndefined(objAtom))
		{
			switch (objAtom&7)
			{
			case kObjectType:
				return AvmCore::atomToScriptObject(objAtom)->nextNameIndex(index);
			case kNamespaceType:
				return AvmCore::atomToNamespace(objAtom)->nextNameIndex(index);
			default:
				ScriptObject* proto = toplevel()->toPrototype(objAtom);
				int nextIndex = ( proto ? proto->nextNameIndex(index) : 0);
				return nextIndex;
			}
		}
		else
		{
			return 0;
		}
	}

	int MethodEnv::hasnextproto(Atom& objAtom, int& index) const
	{
		if (index < 0)
			return 0;

		ScriptObject *delegate = NULL;
		
		if (!AvmCore::isNullOrUndefined(objAtom))
		{
			switch (objAtom&7)
			{
			case kObjectType:
				{
					ScriptObject *object = AvmCore::atomToScriptObject(objAtom);
					delegate = object->getDelegate();
					index = object->nextNameIndex(index);
				}
				break;
			case kNamespaceType:
				{
					index = AvmCore::atomToNamespace(objAtom)->nextNameIndex(index);
					delegate = toplevel()->namespaceClass->prototype;
				}
				break;
			default:
				{
					ScriptObject* proto = toplevel()->toPrototype(objAtom);
					delegate = proto ? proto->getDelegate() : NULL;
					index = ( proto ? proto->nextNameIndex(index) : 0);
				}
			}
		}
		else
		{
			index = 0;
		}

		while (index == 0 && delegate != NULL)
		{
			// Advance to next object on prototype chain
			ScriptObject *object = delegate;
			objAtom = object->atom();
			delegate = object->getDelegate();
			index = object->nextNameIndex(index);
		}

		if (index == 0)
		{
			// If we're done, set object local to null
			objAtom = nullObjectAtom;
		}

		return index != 0;
	}
	
	
	void MethodEnv::mopRangeCheckFailed() const
	{
		toplevel()->throwRangeError(kInvalidRangeError);
	}

	int MethodEnv::li8(int addr) const
	{
		const Domain *dom = domainEnv()->domain();

		if(addr < 0 || (uint32)(addr + 1) > dom->globalMemorySize)
			mopRangeCheckFailed();
		uint8 result = *(uint8 *)(dom->globalMemoryBase + addr);
		return result;
	}

	int MethodEnv::li16(int addr) const
	{
		const Domain *dom = domainEnv()->domain();

		if(addr < 0 || (uint32)(addr + 2) > dom->globalMemorySize)
			mopRangeCheckFailed();

		uint16_t result = *(uint16_t *)(dom->globalMemoryBase + addr);

		MOPS_SWAP_BYTES(&result);

		return result;
	}

	int MethodEnv::li32(int addr) const
	{
		const Domain *dom = domainEnv()->domain();

		if(addr < 0 || (uint32)(addr + 4) > dom->globalMemorySize)
			mopRangeCheckFailed();

		// TODO is using raw "int" type correct?
		int32_t result = *(int32_t *)(dom->globalMemoryBase + addr);

		MOPS_SWAP_BYTES(&result);

		return result;
	}

	double MethodEnv::lf32(int addr) const
	{
		const Domain *dom = domainEnv()->domain();

		if(addr < 0 || (uint32)(addr + 4) > dom->globalMemorySize)
			mopRangeCheckFailed();

		float result = *(float *)(dom->globalMemoryBase + addr);

		MOPS_SWAP_BYTES(&result);

		return result;
	}

	double MethodEnv::lf64(int addr) const
	{
		const Domain *dom = domainEnv()->domain();

		if(addr < 0 || (uint32)(addr + 8) > dom->globalMemorySize)
			mopRangeCheckFailed();

		double result = *(double *)(dom->globalMemoryBase + addr);

		MOPS_SWAP_BYTES(&result);

		return result;
	}

	void MethodEnv::si8(int value, int addr) const
	{
		const Domain *dom = domainEnv()->domain();

		if(addr < 0 || (uint32)(addr + 1) > dom->globalMemorySize)
			mopRangeCheckFailed();

		*(uint8 *)(dom->globalMemoryBase + addr) = (uint8)value;
	}

	void MethodEnv::si16(int value, int addr) const
	{
		const Domain *dom = domainEnv()->domain();

		if(addr < 0 || (uint32)(addr + 2) > dom->globalMemorySize)
			mopRangeCheckFailed();

		uint16 svalue = (uint16)value;

		MOPS_SWAP_BYTES(&svalue);

		*(uint16 *)(dom->globalMemoryBase + addr) = svalue;
	}

	void MethodEnv::si32(int value, int addr) const
	{
		const Domain *dom = domainEnv()->domain();

		if(addr < 0 || (uint32)(addr + 4) > dom->globalMemorySize)
			mopRangeCheckFailed();

		MOPS_SWAP_BYTES(&value);

		*(int *)(dom->globalMemoryBase + addr) = value;
	}

	void MethodEnv::sf32(double value, int addr) const
	{
		const Domain *dom = domainEnv()->domain();

		if(addr < 0 || (uint32)(addr + 4) > dom->globalMemorySize)
			mopRangeCheckFailed();

		float fvalue = (float)value;

		MOPS_SWAP_BYTES(&fvalue);

		*(float *)(dom->globalMemoryBase + addr) = fvalue;
	}

	void MethodEnv::sf64(double value, int addr) const
	{
		const Domain *dom = domainEnv()->domain();

		if(addr < 0 || (uint32)(addr + 8) > dom->globalMemorySize)
			mopRangeCheckFailed();

		MOPS_SWAP_BYTES(&value);

		*(double *)(dom->globalMemoryBase + addr) = value;
	}

	// see 13.2 creating function objects
    ClassClosure* MethodEnv::newfunction(MethodInfo *function,
									 ScopeChain* outer,
									 Atom* scopes) const
    {
		AvmCore* core = this->core();
		MMgc::GC* gc = core->GetGC();
		AbcEnv* abcEnv = this->abcEnv();

		// TODO: if we have already created a function and the scope chain
		// is the same as last time, re-use the old closure?

		// declaringTraits must have been filled in by verifier.
		Traits* ftraits = function->declaringTraits();
		AvmAssert(ftraits != NULL);
		AvmAssert(ftraits->scope != NULL);

		ScopeChain* fscope = ScopeChain::create(core->GetGC(), ftraits->scope, outer, *core->dxnsAddr);
		for (int i=outer->getSize(), n=fscope->getSize(); i < n; i++)
		{
			fscope->setScope(gc, i, *scopes++);
		}
		//core->console<<"New fscope: "<<fscope->format(core)<<"\n";

		FunctionClass* functionClass = toplevel()->functionClass;

		// the vtable for the new function object
		VTable* fvtable = core->newVTable(ftraits, functionClass->ivtable(), fscope, abcEnv, toplevel());
		fvtable->resolveSignatures();
		FunctionEnv *fenv = new (core->GetGC()) FunctionEnv(function, fvtable);
		fvtable->ivtable = toplevel()->object_ivtable;

		FunctionObject* c = new (core->GetGC(), fvtable->getExtraSize()) FunctionObject(fvtable, fenv);
		c->setDelegate( functionClass->prototype );

		c->createVanillaPrototype();
		c->prototype->setStringProperty(core->kconstructor, c->atom());
		c->prototype->setStringPropertyIsEnumerable(core->kconstructor, false);

		fenv->closure = c;
		
        return c;
    }

    /**
     * given a classInfo, create a new ClassClosure object and return it on the stack.
     */

	ClassClosure* MethodEnv::newclass(Traits* ctraits,
							ClassClosure *base,
							ScopeChain* outer,
							Atom* scopes) const
    {
		AvmCore* core = this->core();
		MMgc::GC* gc = core->GetGC();
		// adds clarity to what is usually just global$init()
		SAMPLE_FRAME("[newclass]", core);
		Toplevel* toplevel = this->toplevel();

		Traits* itraits = ctraits->itraits;

		// finish resolving the base class
		if (!base && itraits->base)
		{
			// class has a base but no base object was provided
			ErrorClass *error = toplevel->typeErrorClass();
			if( error )
				error->throwError(kConvertNullToObjectError);
			else
				toplevel->throwTypeError(kCorruptABCError);
		}

		// have to use local variables for CodeWarrior
		Traits* baseIvtableTraits = base ? base->ivtable()->traits : 0;
		Traits* itraitsBase = itraits->base;
		// make sure the traits of the base vtable matches the base traits
		if (!(base == NULL && itraits->base == NULL || base != NULL && itraitsBase == baseIvtableTraits))
		{
			ErrorClass *error = toplevel->verifyErrorClass();
			if( error )
				error->throwError(kInvalidBaseClassError);
			else
				toplevel->throwTypeError(kCorruptABCError);
		}

		ctraits->resolveSignatures(toplevel);
		itraits->resolveSignatures(toplevel);

		// class scopechain = [..., class]
		ScopeChain* cscope = ScopeChain::create(core->GetGC(), ctraits->scope, outer, *core->dxnsAddr);
		int i = outer->getSize();
		for (int n=cscope->getSize(); i < n; i++)
		{
			cscope->setScope(gc, i, *scopes++);
		}
		//core->console<<"NewClass: "<<ctraits<<"\n";
		//core->console<<"New cscope: "<<cscope->format(core)<<"\n";

		ScopeChain* iscope = ScopeChain::create(core->GetGC(), itraits->scope, cscope, *core->dxnsAddr);

		AbcEnv* abcEnv = this->abcEnv();
		VTable* cvtable = NULL;
		VTable* ivtable = NULL;

		// We're defining class
		// This is a little weird, as the cvtable should have the ivtable as its base
		// i.e. Class$ derives from Class
		if (itraits == core->traits.class_itraits)
		{
			// note that the ivtable gets cscope, not iscope... this allows us to avoid having to patch
			// scope in the ClassClass ctor like we used to.
			ivtable = core->newVTable(itraits, base ? base->ivtable() : NULL, cscope, abcEnv, toplevel);
			ivtable->resolveSignatures();
			cvtable = core->newVTable(ctraits, ivtable, cscope, abcEnv, toplevel);
			cvtable->resolveSignatures();
		}
		else
		{
			cvtable = core->newVTable(ctraits, toplevel->class_ivtable, cscope, abcEnv, toplevel);
			// Don't resolve signatures for Object$ until after Class has been set up
			// which should happen very soon after Object is setup.
			if (itraits != core->traits.object_itraits)
				cvtable->resolveSignatures();
			ivtable = core->newVTable(itraits, base ? base->ivtable() : NULL, iscope, abcEnv, toplevel);
			ivtable->resolveSignatures();
		}
		cvtable->ivtable = ivtable;

		if (itraits == core->traits.object_itraits) 
		{
			// we just defined Object
			toplevel->object_ivtable = ivtable;

			// We can finish setting up the toplevel object now that
			// we have the real Object vtable
			VTable* toplevel_vtable = toplevel->global()->vtable;
			toplevel_vtable->base = ivtable;
			toplevel_vtable->linked = false;
			toplevel_vtable->resolveSignatures();
		}
		else if (itraits == core->traits.class_itraits) 
		{
			// we just defined Class
			toplevel->class_ivtable = ivtable;
			
			// Can't run the Object$ initializer until after Class is done since
			// Object$ needs the real Class vtable as its base
			VTable* objectclass_ivtable = toplevel->objectClass->vtable;
			objectclass_ivtable->base = ivtable;
			objectclass_ivtable->resolveSignatures();
			objectclass_ivtable->init->coerceEnter(toplevel->objectClass->atom());
		}

		CreateClassClosureProc createClassClosure = cvtable->traits->getCreateClassClosureProc();
		ClassClosure *cc;
		if (createClassClosure != NULL)
		{
			cc = (*createClassClosure)(cvtable);
		}
		else
		{
			cc = new (core->GetGC(), cvtable->getExtraSize()) ClassClosure(cvtable);
			AvmAssert(cc->prototype == NULL);
			cc->createVanillaPrototype();
		}

		if (cc->prototype)
		{
			// C.prototype.__proto__ = Base.prototype
			if (base != NULL) 
				cc->prototype->setDelegate( base->prototype );

			// C.prototype.constructor = C {DontEnum}
			cc->prototype->setStringProperty(core->kconstructor, cc->atom());
			cc->prototype->setStringPropertyIsEnumerable(core->kconstructor, false);
		}

		AvmAssert(i == iscope->getSize()-1);
		iscope->setScope(gc, i, cc->atom());
		//core->console<<"New iscope: "<<iscope->format(core)<<"\n";
		if (toplevel->classClass)
		{
			cc->setDelegate( toplevel->classClass->prototype );
		}

		// Invoke the class init function.
		// Don't run it for Object - that has to wait until after Class$
		// is set up
		if (cvtable != toplevel->objectClass->vtable)
			cvtable->init->coerceEnter(cc->atom());
		return cc;
    }

    void MethodEnv::initproperty(Atom obj, const Multiname* multiname, Atom value, VTable* vtable) const
    {
		Toplevel* toplevel = this->toplevel();
		Binding b = toplevel->getBinding(vtable->traits, multiname);
		if (AvmCore::bindingKind(b) == BKIND_CONST)
		{
			if (vtable->init != this)
				toplevel->throwReferenceError(kConstWriteError, multiname, vtable->traits);
			b = AvmCore::makeSlotBinding(AvmCore::bindingToSlotId(b), BKIND_VAR);
		}
		toplevel->setproperty_b(obj, multiname, value, vtable, b);
    }

	void MethodEnv::setpropertylate_i(Atom obj, int index, Atom value) const
	{
		if (AvmCore::isObject(obj))
		{
			ScriptObject* o = AvmCore::atomToScriptObject(obj);
			if (index >= 0)
			{
				o->setUintProperty(index, value);
			}
			else
			{
				// negative index - we must intern the integer
				o->setAtomProperty(this->core()->internInt(index)->atom(), value);
			}
		}
		else
		{
			// obj represents a primitive Number, Boolean, int, or String, and primitives
			// are sealed and final.  Cannot add dynamic vars to them.

			// throw a ReferenceError exception  Property not found and could not be created.
			Multiname tempname(core()->publicNamespace, core()->internInt(index));
			toplevel()->throwReferenceError(kWriteSealedError, &tempname, toplevel()->toTraits(obj));
		}
	}

	void MethodEnv::setpropertylate_u(Atom obj, uint32 index, Atom value) const
	{
		if (AvmCore::isObject(obj))
		{
			AvmCore::atomToScriptObject(obj)->setUintProperty(index, value);
		}
		else
		{
			// obj represents a primitive Number, Boolean, int, or String, and primitives
			// are sealed and final.  Cannot add dynamic vars to them.

			// throw a ReferenceError exception  Property not found and could not be created.
			Multiname tempname(core()->publicNamespace, core()->internUint32(index));
			toplevel()->throwReferenceError(kWriteSealedError, &tempname, toplevel()->toTraits(obj));
		}
	}

	Atom MethodEnv::callsuper(const Multiname* multiname, int argc, Atom* atomv) const
	{
		VTable* base = this->_vtable->base;
		Toplevel* toplevel = this->toplevel();
		Binding b = toplevel->getBinding(base->traits, multiname);
		switch (AvmCore::bindingKind(b))
		{
		default:
			toplevel->throwReferenceError(kCallNotFoundError, multiname, base->traits);

		case BKIND_METHOD:
		{
			#ifdef DEBUG_EARLY_BINDING
			core()->console << "callsuper method " << base->traits << " " << multiname->name << "\n";
			#endif
			MethodEnv* superenv = base->methods[AvmCore::bindingToMethodId(b)];
			return superenv->coerceEnter(argc, atomv);
		}
		case BKIND_VAR:
		case BKIND_CONST:
		{
			#ifdef DEBUG_EARLY_BINDING
			core()->console << "callsuper slot " << base->traits << " " << multiname->name << "\n";
			#endif
			uint32 slot = AvmCore::bindingToSlotId(b);
			Atom method = AvmCore::atomToScriptObject(atomv[0])->getSlotAtom(slot);
			return toplevel->op_call(method, argc, atomv);
		}
		case BKIND_SET:
		{
			#ifdef DEBUG_EARLY_BINDING
			core()->console << "callsuper setter " << base->traits << " " << multiname->name << "\n";
			#endif
			// read on write-only property
			toplevel->throwReferenceError(kWriteOnlyError, multiname, base->traits);
		}
		case BKIND_GET:
		case BKIND_GETSET:
		{
			#ifdef DEBUG_EARLY_BINDING
			core()->console << "callsuper getter " << base->traits << " " << multiname->name << "\n";
			#endif
			// Invoke the getter
			int m = AvmCore::bindingToGetterId(b);
			MethodEnv *f = base->methods[m];
			Atom atomv_out[1] = { atomv[0] };
			Atom method = f->coerceEnter(0, atomv_out);
			return toplevel->op_call(method, argc, atomv);
		}
		}
	}

	Atom MethodEnv::delproperty(Atom obj, const Multiname* multiname) const
	{
		Toplevel* toplevel = this->toplevel();
		Traits* traits = toplevel->toTraits(obj); // includes null check
		if (AvmCore::isObject(obj))
		{
			Binding b = toplevel->getBinding(traits, multiname);
			if (b == BIND_NONE) 
			{
				bool b = AvmCore::atomToScriptObject(obj)->deleteMultinameProperty(multiname);
#ifdef AVMPLUS_WORD_CODE
				// Deleting a deletable bound property means deleting a dynamic global property, so
				// invalidate the lookup cache (because subsequent lookups should fail).
				if (b)
					core()->invalidateLookupCache();
#endif
				return b ? trueAtom : falseAtom;
			}
			else if (AvmCore::isMethodBinding(b))
			{
				if (multiname->contains(core()->publicNamespace) && toplevel->isXmlBase(obj))
				{
					// dynamic props should hide declared methods on delete
					ScriptObject* so = AvmCore::atomToScriptObject(obj);
					bool b = so->deleteMultinameProperty(multiname);
					return b ? trueAtom : falseAtom;
				}
			}
		}
		else
		{
			toplevel->throwReferenceError(kDeleteSealedError, multiname, traits);
		}

		return falseAtom;
	}
	
    Atom MethodEnv::getsuper(Atom obj, const Multiname* multiname) const
    {
		VTable* vtable = this->_vtable->base;
		Toplevel* toplevel = this->toplevel();
		Binding b = toplevel->getBinding(vtable->traits, multiname);
        switch (AvmCore::bindingKind(b))
        {
		default:
			toplevel->throwReferenceError(kReadSealedError, multiname, vtable->traits);

		case BKIND_METHOD: 
		{
			#ifdef DEBUG_EARLY_BINDING
			core->console << "getsuper method " << vtable->traits << " " << multiname->name << "\n";
			#endif
			// extracting a virtual method
			MethodEnv *m = vtable->methods[AvmCore::bindingToMethodId(b)];
			return toplevel->methodClosureClass->create(m, obj)->atom();
		}

        case BKIND_VAR:
        case BKIND_CONST:
			#ifdef DEBUG_EARLY_BINDING
			core->console << "getsuper slot " << vtable->traits << " " << multiname->name << "\n";
			#endif
			return AvmCore::atomToScriptObject(obj)->getSlotAtom(AvmCore::bindingToSlotId(b));

		case BKIND_SET:
		{
			#ifdef DEBUG_EARLY_BINDING
			core->console << "getsuper setter " << vtable->traits << " " << multiname->name << "\n";
			#endif
			// read on write-only property
			toplevel->throwReferenceError(kWriteOnlyError, multiname, vtable->traits);
		}
		case BKIND_GET:
		case BKIND_GETSET:
		{
			#ifdef DEBUG_EARLY_BINDING
			core->console << "getsuper getter " << vtable->traits << " " << multiname->name << "\n";
			#endif
			// Invoke the getter
			int m = AvmCore::bindingToGetterId(b);
			MethodEnv *f = vtable->methods[m];
			Atom atomv_out[1] = { obj };
			return f->coerceEnter(0, atomv_out);
		}
		}
    }

	
    void MethodEnv::setsuper(Atom obj, const Multiname* multiname, Atom value) const
    {
		VTable* vtable = this->_vtable->base;
		Toplevel* toplevel = this->toplevel();
		Binding b = toplevel->getBinding(vtable->traits, multiname);
        switch (AvmCore::bindingKind(b))
        {
		default:
			toplevel->throwReferenceError(kWriteSealedError, multiname, vtable->traits);

		case BKIND_CONST:
			toplevel->throwReferenceError(kConstWriteError, multiname, vtable->traits);

		case BKIND_METHOD: 
			toplevel->throwReferenceError(kCannotAssignToMethodError, multiname, vtable->traits);

		case BKIND_GET: 
			#ifdef DEBUG_EARLY_BINDING
			core()->console << "setsuper getter " << vtable->traits << " " << multiname->name << "\n";
			#endif
			toplevel->throwReferenceError(kConstWriteError, multiname, vtable->traits);

		case BKIND_VAR:
			#ifdef DEBUG_EARLY_BINDING
			core()->console << "setsuper slot " << vtable->traits << " " << multiname->name << "\n";
			#endif
			AvmCore::atomToScriptObject(obj)->setSlotAtom(AvmCore::bindingToSlotId(b), value);
            return;

		case BKIND_SET: 
		case BKIND_GETSET: 
		{
			#ifdef DEBUG_EARLY_BINDING
			core()->console << "setsuper setter " << vtable->traits << " " << multiname->name << "\n";
			#endif
			// Invoke the setter
			uint32 m = AvmCore::bindingToSetterId(b);
			AvmAssert(m < vtable->traits->getTraitsBindings()->methodCount);
			MethodEnv* method = vtable->methods[m];
			Atom atomv_out[2] = { obj, value };
			// coerce value to proper type, then call enter
			method->coerceEnter(1, atomv_out);
			return;
		}
        }
    }

	Atom MethodEnv::findWithProperty(Atom atom, const Multiname* multiname)
	{
		Toplevel* toplevel = this->toplevel();
		if ((atom&7)==kObjectType)
		{
			// usually scope objects are ScriptObject's

			ScriptObject* o = AvmCore::atomToScriptObject(atom);

			// search the delegate chain for a value.  we must look at
			// traits at each step along the way in case we have class
			// instances on the scope chain
			do
			{
				// ISSUE it is incorrect to return a reference to an object on the prototype
				// chain, we should only return the scopechain object; the next operation
				// could be a setproperty, and we don't want to mutate prototype objects this way.

				// look at the traits first, stop if found.
				Binding b = toplevel->getBinding(o->traits(), multiname);
				if (b != BIND_NONE)
					return atom;
				if (o->hasMultinameProperty(multiname))
					return atom;
			}
			while ((o = o->getDelegate()) != NULL);
		}
		else
		{
			// primitive value on scope chain!

			// first iteration looks at traits only since primitive values don't have
			// dynamic variables

			Binding b = toplevel->getBinding(toplevel->toTraits(atom), multiname);
			if (b != BIND_NONE)
				return atom;

			// then we continue starting at the prototype for this primitive type
			ScriptObject* o = toplevel->toPrototype(atom);
			do
			{
				Binding b = toplevel->getBinding(o->traits(), multiname);
				if (b != BIND_NONE)
					return atom;
				if (o->hasMultinameProperty(multiname))
					return atom;
			}
			while ((o = o->getDelegate()) != NULL);
		}

		return nullObjectAtom;
	}

	/**
	 * return the object on the scope chain that owns the given property.
	 * if no object has that property, return scope[0].  we search each
	 * delegate chain but only return objects that are on the scope chain.
	 * this way, find+get and find+set are both correct.  find+set should
	 * not mutate a prototype object.
	 */
	Atom MethodEnv::findproperty(ScopeChain* outer,
								 Atom* scopes,
								 int extraScopes,
								 const Multiname* multiname,
								 bool strict,
								 Atom* withBase)
    {
		Toplevel* toplevel = this->toplevel();

		// look in each with frame in the current stack frame
		Atom* scopep = &scopes[extraScopes-1];
		if (withBase)
		{
			for (; scopep >= withBase; scopep--)
			{
				Atom result = findWithProperty(*scopep, multiname);
				if (!AvmCore::isNull(result))
				{
					return result;
				}
			}
		}

		// if we're in global$init (outer_depth==0), don't check "this" scope just yet.
		int outer_depth = outer->getSize();
		for (; scopep > scopes; scopep--)
		{
			Atom a = *scopep;
			Traits* t = toplevel->toTraits(a);
			Binding b = toplevel->getBinding(t, multiname);
			if (b != BIND_NONE)
				return a;
		}

		const ScopeTypeChain* outerTraits = outer->scopeTraits();

		if (outer_depth > 0 && scopep >= scopes)
		{
			// consider "this" scope now, but constrain it to the declaringTraits of
			// the current method (verifier ensures this is safe)
			Atom a = *scopep;
			Traits *t;
			if (outerTraits->fullsize > outerTraits->size)
			{
				// scope traits has extra constraint for "this" scope, see OP_newclass in verifier
				t = outerTraits->getScopeTraitsAt(outerTraits->size);
			}
			else
			{
				// "this" scope type is the runtime type
				t = toplevel->toTraits(a);
			}

			Binding b = toplevel->getBinding(t, multiname);
			if (b != BIND_NONE)
				return a;
		}

		// now search outer scopes
		for (int i=outer_depth-1; i > 0; i--)
		{
			if (outerTraits->getScopeIsWithAt(i))
			{
				Atom result = findWithProperty(outer->getScope(i), multiname);
				if (!AvmCore::isNull(result))
					return result;
			}
			else
			{
				// only look at the properties on the captured (verify time) type, not the actual type,
				// of the outer scope object.
				Atom a = outer->getScope(i);
				Traits* t = outerTraits->getScopeTraitsAt(i);
				Binding b = toplevel->getBinding(t, multiname);
				if (b != BIND_NONE)
					return a;
			}
		}

		// No imported definitions or global scope can have attributes.  (Using filter
		// operator with non existent attribute will get here. 
		if (multiname->isAttr())
		{
			if (strict)
				toplevel->throwReferenceError(kUndefinedVarError, multiname);
			return undefinedAtom;
		}

		// now we have searched all the scopes, except global
		{
			ScriptObject* global = AvmCore::atomToScriptObject(outer->getSize() > 0 ? outer->getScope(0) : *scopes);
			ScriptObject* obj = findglobalproperty(global, multiname);
			if (obj == NULL) {
				if (strict)
					toplevel->throwReferenceError(kUndefinedVarError, multiname);
				obj = global;
			}
			return obj->atom();
		}
	}
	
	ScriptObject* MethodEnv::findglobalproperty(ScriptObject* target_global, const Multiname* multiname)
	{
		Toplevel* toplevel = this->toplevel();
		
		// look for imported definition (similar logic to OP_finddef).  This will
		// find definitions in this script and in other scripts.
		ScriptEnv* script = getScriptEnv(multiname);
		if (script != (ScriptEnv*)BIND_NONE)
		{
			if (script == (ScriptEnv*)BIND_AMBIGUOUS)
				toplevel->throwReferenceError(kAmbiguousBindingError, multiname);

			ScriptObject* global = script->global;
			if (global == NULL)
			{
				global = script->initGlobal();
				Atom argv[1] = { script->global->atom() };
				script->coerceEnter(0, argv);
			}
			return global;
		}

		// no imported definition found.  look for dynamic props
		// on the global object

		ScriptObject* global = target_global;

		// search the delegate chain for a value.  The delegate
		// chain for the global object will only contain vanilla
		// objects (Object.prototype) so we can skip traits

		ScriptObject* o = global;
		do
		{
			if (o->hasMultinameProperty(multiname))
				return global;
		}
		while ((o = o->getDelegate()) != NULL);

		return NULL;
    }

	Namespace* MethodEnv::internRtns(Atom nsAtom)
	{
		if (((nsAtom&7) != kNamespaceType) || AvmCore::isNull(nsAtom))
			toplevel()->throwTypeError(kIllegalNamespaceError);
		return core()->internNamespace(AvmCore::atomToNamespace(nsAtom));
	}

	ArrayObject* MethodEnv::createArguments(Atom *atomv, int argc)
	{
		Toplevel* toplevel = this->toplevel();
		ArrayObject *arguments = toplevel->arrayClass->newarray(atomv+1,argc);
		ScriptObject *closure;
		if (method->needClosure())
		{
			closure = toplevel->methodClosureClass->create(this, atomv[0]);
		}
		else
		{
			closure = ((FunctionEnv*)this)->closure;
		}
		arguments->setStringProperty(core()->kcallee, closure->atom());
		arguments->setStringPropertyIsEnumerable(core()->kcallee, false);
		return arguments;
	}

	Atom MethodEnv::getdescendants(Atom obj, const Multiname* multiname)
	{
		if (AvmCore::isObject (obj))
		{
			return core()->atomToScriptObject(obj)->getDescendants (multiname);
		}
		else
		{
			// Rhino simply returns undefined for other Atom types
			// SpiderMonkey throws TypeError.  We're doing TypeError.
			toplevel()->throwTypeError(kDescendentsError, core()->toErrorString(toplevel()->toTraits(obj)));
			return undefinedAtom; // not reached
		}
	}

	Atom MethodEnv::getdescendantslate(Atom obj, Atom index, bool attr)
	{
		if (AvmCore::isObject(index))
		{
			ScriptObject* i = AvmCore::atomToScriptObject(index);
			if (i->traits() == core()->traits.qName_itraits)
			{
				QNameObject* qname = (QNameObject*) i;
				Multiname n2;
				qname->getMultiname(n2);
				if (attr)
					n2.setAttr(attr);
				return getdescendants(obj, &n2);
			}
		}

		// convert index to string

		AvmCore* core = this->core();
		Multiname name(core->publicNamespace, core->intern(index));
		if (attr)
			name.setAttr();
		return getdescendants(obj, &name);
	}

	void MethodEnv::checkfilter(Atom obj)
	{
		if ( !toplevel()->isXmlBase(obj) )
		{
			toplevel()->throwTypeError(kFilterError, core()->toErrorString(toplevel()->toTraits(obj)));
		}
	}

		
	/**
	 * implements ECMA implicit coersion.  returns the coerced value,
	 * or throws a TypeError if coersion is not possible.
	 */
    ScriptObject* MethodEnv::coerceAtom2SO(Atom atom, Traits* expected) const
    {
		AvmAssert(expected != NULL);
		AvmAssert(!expected->isMachineType());
		AvmAssert(expected != core()->traits.string_itraits);
		AvmAssert(expected != core()->traits.namespace_itraits);

		if (AvmCore::isNullOrUndefined(atom))
			return NULL;

		if ((atom&7) == kObjectType)
		{
			ScriptObject* so = AvmCore::atomToScriptObject(atom);
			if (so->traits()->containsInterface(expected))
			{
				return so;
			}
		}

		// failed
#ifdef AVMPLUS_VERBOSE
		//core->console << "checktype failed " << expected << " <- " << atom << "\n";
#endif
		toplevel()->throwTypeError(kCheckTypeFailedError, core()->atomToErrorString(atom), core()->toErrorString(expected));
		return NULL;
    }

	VTable *MethodEnv::getActivation()
	{
		int type = getType();
		switch(type)
		{
		case kActivation:
			return (VTable *)(activationOrMCTable&~7);
		case kActivationMethodTablePair:
			return getPair()->activation;
		default:
			return NULL;
		}
	}

    ScriptObject *MethodEnv::newActivation()
    {
        VTable *vtable = getActivation();
		AvmCore *core = this->core();
        MMgc::GC *gc = core->GetGC();
		SAMPLE_FRAME("[activation-object]", core);
		ScriptObject* obj = new (gc, vtable->getExtraSize()) ScriptObject(vtable, 0/*delegate*/);
        MethodEnv *init = vtable->init;
        if (init)
			init->coerceEnter(obj->atom());
		return obj;
    }

	WeakKeyHashtable *MethodEnv::getMethodClosureTable()
	{
		int type = getType();
		if(!activationOrMCTable)
		{
			WeakKeyHashtable *wkh = new (core()->GetGC()) WeakKeyHashtable(core()->GetGC());			
			setActivationOrMCTable(wkh, kMethodTable);
			return wkh;
		}
		else if(type == kActivation)
		{
			WeakKeyHashtable *wkh = new (core()->GetGC()) WeakKeyHashtable(core()->GetGC());
			ActivationMethodTablePair *amtp = new (core()->GetGC()) ActivationMethodTablePair(getActivation(), wkh);					
			setActivationOrMCTable(amtp, kActivationMethodTablePair);
			return wkh;
		}
		else if(type == kActivationMethodTablePair)
		{
			return getPair()->methodTable;
		}
		return (WeakKeyHashtable*)(activationOrMCTable&~7);
	}
}
