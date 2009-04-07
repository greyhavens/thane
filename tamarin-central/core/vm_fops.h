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
 * Portions created by the Initial Developer are Copyright (C) 2008
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

#define ARGMASK(a,n) (nanojit::ARGSIZE_##a << (ARGSIZE_SHIFT*(n)))
#define SIG0(r) ARGMASK(r,0)
#define SIG1(r,a1) (SIG0(r) | ARGMASK(a1,1))
#define SIG2(r,a2,a1) (SIG1(r,a1) | ARGMASK(a2,2))
#define SIG3(r,a3,a2,a1) (SIG2(r,a2,a1) | ARGMASK(a3,3))
#define SIG4(r,a4,a3,a2,a1) (SIG3(r,a3,a2,a1) | ARGMASK(a4,4))
#define SIG5(r,a5,a4,a3,a2,a1) (SIG4(r,a4,a3,a2,a1) | ARGMASK(a5,5))
#define SIG6(r,a6,a5,a4,a3,a2,a1) (SIG5(r,a5,a4,a3,a2,a1) | ARGMASK(a6,6))
#define SIG7(r,a7,a6,a5,a4,a3,a2,a1) (SIG6(r,a6,a5,a4,a3,a2,a1) | ARGMASK(a7,7))
#define SIG8(r,a8,a7,a6,a5,a4,a3,a2,a1) (SIG7(r,a7,a6,a5,a4,a3,a2,a1) | ARGMASK(a8,8))

#define ARGSIZE_A ARGSIZE_P

#define FUNCTIONID(n) &ci_##n

#ifdef NJ_VERBOSE
    #define DEFINE_CALLINFO(f,sig,cse,fold,abi,name) \
        static const CallInfo ci_##name = { f, sig, cse, fold, abi, #name };
#else
    #define DEFINE_CALLINFO(f,sig,cse,fold,abi,name) \
        static const CallInfo ci_##name = { f, sig, cse, fold, abi };
#endif

#if _MSC_VER
	#define ABI_FUNCTION ABI_CDECL
	#define ABI_METHOD   ABI_THISCALL
#else
	// gcc, probably
	#define ABI_FUNCTION ABI_CDECL
	#define ABI_METHOD   ABI_CDECL
#endif

#define FUNCTION(f, sig, name) \
    DEFINE_CALLINFO(f,sig,0,0,ABI_FUNCTION,name)
#define CSEFUNCTION(f, sig, name) \
    DEFINE_CALLINFO(f,sig,1,0,ABI_FUNCTION,name)

#define METHOD(f, sig, name) \
    DEFINE_CALLINFO(f,sig,0,0,ABI_METHOD,name)
#define CSEMETHOD(f, sig, name) \
    DEFINE_CALLINFO(f,sig,1,0,ABI_METHOD,name)

    FUNCTION(CALL_INDIRECT, SIG4(U,P,P,I,P), icalli)
    FUNCTION(CALL_INDIRECT, SIG4(A,P,P,I,P), acalli)
    FUNCTION(CALL_INDIRECT, SIG4(F,P,P,I,P), fcalli)
    FUNCTION(CALL_IMT, SIG5(I,P,P,P,I,P), icallimt)
    FUNCTION(CALL_IMT, SIG5(A,P,P,P,I,P), acallimt)
    FUNCTION(CALL_IMT, SIG5(F,P,P,P,I,P), fcallimt)

    METHOD(ENVADDR(MethodEnv::newActivation), SIG1(P,P), newActivation)
    METHOD(ENVADDR(MethodEnv::newcatch), SIG2(P,P,P), newcatch)
    METHOD(ENVADDR(MethodEnv::newfunction), SIG4(P,P,P,P,P), newfunction)
    METHOD(TOPLEVELADDR(Toplevel::op_call), SIG4(A,P,A,I,P), op_call)
    METHOD(TOPLEVELADDR(Toplevel::callproperty), SIG6(A,P,A,P,I,P,P), callproperty)
    CSEMETHOD(TOPLEVELADDR(Toplevel::coerce), SIG3(A,P,A,P), coerce)
    METHOD(ENVADDR(MethodEnv::npe), SIG1(V,P), npe)
    METHOD(ENVADDR(MethodEnv::interrupt), SIG1(V,P), interrupt)
    METHOD(ENVADDR(MethodEnv::nextname), SIG3(A,P,A,I), nextname)
    METHOD(ENVADDR(MethodEnv::nextvalue), SIG3(A,P,A,I), nextvalue)
    METHOD(ENVADDR(MethodEnv::hasnext), SIG3(I,P,A,I), hasnext)
    METHOD(COREADDR(AvmCore::coerce_s), SIG2(P,P,A), coerce_s)
    METHOD(COREADDR(AvmCore::string), SIG2(P,P,A), string)
    METHOD(TOPLEVELADDR(Toplevel::coerceobj), SIG3(V,P,P,P), coerceobj)
    METHOD(ENVADDR(MethodEnv::coerceAtom2SO), SIG3(P,P,A,P), coerceAtom2SO)
    CSEMETHOD(COREADDR(AvmCore::doubleToString), SIG2(P,P,F), doubleToString)
    CSEMETHOD(COREADDR(AvmCore::uintToString), SIG2(P,P,U), uintToString) 
    CSEMETHOD(COREADDR(AvmCore::intToString), SIG2(P,P,I), intToString) 
    CSEMETHOD(COREADDR(AvmCore::doubleToAtom), SIG2(A,P,F), doubleToAtom)
    CSEFUNCTION(FUNCADDR(AvmCore::boolean), SIG1(I,A), boolean)
    CSEFUNCTION(FUNCADDR(AvmCore::toUInt32), SIG1(U,A), toUInt32)
    CSEFUNCTION(FUNCADDR(AVMCORE_integer_d), SIG1(I,F), integer_d)
    CSEFUNCTION(FUNCADDR(AvmCore::integer_i), SIG1(I,A), integer_i)
    CSEFUNCTION(FUNCADDR(AvmCore::number_d), SIG1(F,A), number_d)
    CSEFUNCTION(FUNCADDR(AvmCore::integer_u), SIG1(U,A), integer_u)
    CSEFUNCTION(FUNCADDR(AVMCORE_integer), SIG1(I,A), integer)
    CSEFUNCTION(FUNCADDR(AvmCore::number), SIG1(F,A), number)
    METHOD(ENVADDR(MethodEnv::hasnextproto), SIG3(I,P,P,P), hasnextproto)
    METHOD(ENVADDR(MethodEnv::nullcheck), SIG2(V,P,A), nullcheck)
    CSEMETHOD(TOPLEVELADDR(Toplevel::toVTable), SIG2(P,P,A), toVTable)
    METHOD(TOPLEVELADDR(Toplevel::setproperty), SIG5(V,P,A,P,A,P), setproperty)
    METHOD(ENVADDR(MethodEnv::initproperty), SIG5(V,P,A,P,A,P), initproperty)
    METHOD(COREADDR(AvmCore::newPublicNamespace), SIG2(P,P,P), newPublicNamespace)
    METHOD(COREADDR(AvmCore::intern), SIG2(P,P,A), intern)
    FUNCTION(FUNCADDR(AvmCore::istypeAtom), SIG2(A,A,P), istypeAtom)
    CSEMETHOD(TOPLEVELADDR(Toplevel::toClassITraits), SIG2(P,P,A), toClassITraits)
    METHOD(TOPLEVELADDR(Toplevel::in_operator), SIG3(A,P,A,A), in)
    METHOD(TOPLEVELADDR(Toplevel::instanceof), SIG3(A,P,A,A), instanceof)
    CSEMETHOD(COREADDR(AvmCore::stricteq), SIG3(A,P,A,A), stricteq)
    METHOD(COREADDR(AvmCore::equals), SIG3(A,P,A,A), equals)
    CSEMETHOD(COREADDR(AvmCore::concatStrings), SIG3(P,P,P,P), concatStrings)
    METHOD(TOPLEVELADDR(Toplevel::add2), SIG3(A,P,A,A), add2)
    CSEFUNCTION(FUNCADDR(AvmCore::astype), SIG2(A,A,P), astype)
    CSEMETHOD(COREADDR(AvmCore::EscapeAttributeValue), SIG2(P,P,A), EscapeAttributeValue)
    CSEMETHOD(COREADDR(AvmCore::ToXMLString), SIG2(P,P,A), ToXMLString)
    METHOD(ENVADDR(MethodEnv::delpropertyHelper), SIG4(A,P,A,P,A), delpropertyHelper)
    METHOD(ENVADDR(MethodEnv::internRtns), SIG2(P,P,A), internRtns)
    METHOD(ENVADDR(MethodEnv::delproperty), SIG3(A,P,A,P), delproperty)
    METHOD(ENVADDR(MethodEnv::setpropertyHelper),  SIG6(V,P,A,P,A,P,A), setpropertyHelper)
    METHOD(ENVADDR(MethodEnv::initpropertyHelper), SIG6(V,P,A,P,A,P,A), initpropertyHelper)
    METHOD(ENVADDR(MethodEnv::setpropertylate_u), SIG4(V,P,A,U,A), setpropertylate_u)
    METHOD(VECTORDOUBLEADDR(DoubleVectorObject::_setUintProperty), SIG3(V,P,U,A), DoubleVectorObject_setUintProperty)
    METHOD(VECTORDOUBLEADDR(DoubleVectorObject::_setNativeUintProperty), SIG3(V,P,U,F), DoubleVectorObject_setNativeUintProperty)
    METHOD(VECTORINTADDR(IntVectorObject::_setUintProperty), SIG3(V,P,U,A), IntVectorObject_setUintProperty)
    METHOD(VECTORUINTADDR(UIntVectorObject::_setUintProperty), SIG3(V,P,U,A), UIntVectorObject_setUintProperty)
    METHOD(VECTORINTADDR(IntVectorObject::_setNativeUintProperty), SIG3(V,P,U,I), IntVectorObject_setNativeUintProperty)
    METHOD(VECTORUINTADDR(UIntVectorObject::_setNativeUintProperty), SIG3(V,P,U,U), UIntVectorObject_setNativeUintProperty)
    METHOD(ARRAYADDR(ArrayObject::_setUintProperty), SIG3(V,P,U,A), ArrayObject_setUintProperty)
    METHOD(VECTOROBJADDR(ObjectVectorObject::_setUintProperty), SIG3(V,P,U,A), ObjectVectorObject_setUintProperty)
    METHOD(ENVADDR(MethodEnv::setpropertylate_i), SIG4(V,P,A,I,A), setpropertylate_i)
    METHOD(VECTORDOUBLEADDR(DoubleVectorObject::_setIntProperty), SIG3(V,P,I,A), DoubleVectorObject_setIntProperty)
    METHOD(VECTORDOUBLEADDR(DoubleVectorObject::_setNativeIntProperty), SIG3(V,P,I,F), DoubleVectorObject_setNativeIntProperty)
    METHOD(VECTORINTADDR(IntVectorObject::_setIntProperty), SIG3(V,P,I,A), IntVectorObject_setIntProperty)
    METHOD(VECTORUINTADDR(UIntVectorObject::_setIntProperty), SIG3(V,P,I,A), UIntVectorObject_setIntProperty)
    METHOD(VECTORINTADDR(IntVectorObject::_setNativeIntProperty), SIG3(V,P,I,I), IntVectorObject_setNativeIntProperty)
    METHOD(VECTORUINTADDR(UIntVectorObject::_setNativeIntProperty), SIG3(V,P,I,U), UIntVectorObject_setNativeIntProperty)
    METHOD(ARRAYADDR(ArrayObject::_setIntProperty), SIG3(V,P,I,A), ArrayObject_setIntProperty)
    METHOD(VECTOROBJADDR(ObjectVectorObject::_setIntProperty), SIG3(V,P,I,A), ObjectVectorObject_setIntProperty)
    METHOD(TOPLEVELADDR(Toplevel::getproperty), SIG4(A,P,A,P,P), getproperty)
    METHOD(ENVADDR(MethodEnv::getpropertyHelper), SIG5(A,P,A,P,P,A), getpropertyHelper)
    METHOD(ENVADDR(MethodEnv::getpropertylate_u), SIG3(A,P,A,U), getpropertylate_u)
    METHOD(VECTORDOUBLEADDR(DoubleVectorObject::_getUintProperty), SIG2(A,P,U), DoubleVectorObject_getUintProperty)
    METHOD(VECTORDOUBLEADDR(DoubleVectorObject::_getNativeUintProperty), SIG2(F,P,U), DoubleVectorObject_getNativeUintProperty)
    METHOD(VECTORINTADDR(IntVectorObject::_getUintProperty), SIG2(A,P,U), IntVectorObject_getUintProperty)
    METHOD(VECTORUINTADDR(UIntVectorObject::_getUintProperty), SIG2(A,P,U), UIntVectorObject_getUintProperty)
    METHOD(VECTORINTADDR(IntVectorObject::_getNativeUintProperty), SIG2(I,P,U), IntVectorObject_getNativeUintProperty)
    METHOD(VECTORUINTADDR(UIntVectorObject::_getNativeUintProperty), SIG2(U,P,U), UIntVectorObject_getNativeUintProperty)
    METHOD(ARRAYADDR(ArrayObject::_getUintProperty), SIG2(A,P,U), ArrayObject_getUintProperty)
    METHOD(VECTOROBJADDR(ObjectVectorObject::_getUintProperty), SIG2(A,P,U), ObjectVectorObject_getUintProperty)
    METHOD(ENVADDR(MethodEnv::getpropertylate_i), SIG3(A,P,A,I), getpropertylate_i)
    METHOD(VECTORDOUBLEADDR(DoubleVectorObject::_getIntProperty), SIG2(A,P,I), DoubleVectorObject_getIntProperty)
    METHOD(VECTORDOUBLEADDR(DoubleVectorObject::_getNativeIntProperty), SIG2(F,P,I), DoubleVectorObject_getNativeIntProperty)
    METHOD(VECTORINTADDR(IntVectorObject::_getIntProperty), SIG2(A,P,I), IntVectorObject_getIntProperty)
    METHOD(VECTORUINTADDR(UIntVectorObject::_getIntProperty), SIG2(A,P,I), UIntVectorObject_getIntProperty)
    METHOD(VECTORINTADDR(IntVectorObject::_getNativeIntProperty), SIG2(I,P,I), IntVectorObject_getNativeIntProperty)
    METHOD(VECTORUINTADDR(UIntVectorObject::_getNativeIntProperty), SIG2(U,P,I), UIntVectorObject_getNativeIntProperty)
    METHOD(ARRAYADDR(ArrayObject::_getIntProperty), SIG2(A,P,I), ArrayObject_getIntProperty)
    METHOD(VECTOROBJADDR(ObjectVectorObject::_getIntProperty), SIG2(A,P,I), ObjectVectorObject_getIntProperty)
    CSEMETHOD(ENVADDR(MethodEnv::finddefNs), SIG3(P,P,P,P), finddefNs)
    CSEMETHOD(ENVADDR(MethodEnv::finddefNsset), SIG3(P,P,P,P), finddefNsset)
    METHOD(ENVADDR(MethodEnv::findproperty), SIG7(A,P,P,P,I,P,B,P), findproperty)
    METHOD(ENVADDR(MethodEnv::checkfilter), SIG2(V,P,A), checkfilter)
    METHOD(ENVADDR(MethodEnv::getdescendants), SIG3(A,P,A,P), getdescendants)
    METHOD(ENVADDR(MethodEnv::newclass), SIG5(P,P,P,P,P,P), newclass)
    METHOD(SCRIPTADDR(ArrayClass::newarray), SIG3(P,P,P,I), newarray)
    METHOD(ENVADDR(MethodEnv::op_newobject), SIG3(P,P,P,I), op_newobject)
    METHOD(TOPLEVELADDR(Toplevel::op_applytype), SIG4(A,P,A,I,P), op_applytype)
    METHOD(TOPLEVELADDR(Toplevel::op_construct), SIG4(A,P,A,I,P), op_construct)
    METHOD(ENVADDR(MethodEnv::callsuper), SIG4(A,P,P,I,P), callsuper)
    METHOD(TOPLEVELADDR(Toplevel::constructprop), SIG5(A,P,P,I,P,P), constructprop)
    METHOD(ENVADDR(MethodEnv::setsuper), SIG4(V,P,A,P,A), setsuper)
    METHOD(ENVADDR(MethodEnv::getsuper), SIG3(A,P,A,P), getsuper)
    METHOD(COREADDR(AvmCore::throwAtom), SIG2(V,P,A), throwAtom)
    CSEFUNCTION(FUNCADDR(MathUtils::mod), SIG2(F,F,F), mod)
    CSEMETHOD(COREADDR(AvmCore::_typeof), SIG2(P,P,A), typeof)
    FUNCTION(FUNCADDR(AvmCore::atomWriteBarrier), SIG4(V,P,P,P,A), atomWriteBarrier)
    METHOD(GCADDR(GC::writeBarrierRC), SIG4(V,P,P,P,P), writeBarrierRC)
    CSEMETHOD(COREADDR(AvmCore::uintToAtom), SIG2(A,P,U), uintToAtom)
    CSEMETHOD(COREADDR(AvmCore::intToAtom), SIG2(A,P,I), intToAtom)
    METHOD(COREADDR(AvmCore::compare), SIG3(A,P,A,A), compare)
    CSEMETHOD(ENVADDR(MethodEnv::createRestHelper), SIG3(P,P,I,P), createRestHelper)
    CSEMETHOD(ENVADDR(MethodEnv::createArgumentsHelper), SIG3(P,P,I,P), createArgumentsHelper)
    METHOD(COREADDR(AvmCore::initMultinameLate), SIG3(V,P,P,A), initMultinameLate)
    METHOD(ENVADDR(MethodEnv::initMultinameLateForDelete), SIG3(V,P,P,A), initMultinameLateForDelete)
    CSEFUNCTION(FUNCADDR(MathUtils::doubleToBool), SIG1(I,F), doubleToBool)
    METHOD(CLASSCLOSUREADDR(ClassClosure::newInstance), SIG1(P,P), newInstance)
    METHOD(ENVADDR(MethodEnv::stkover), SIG1(V,P), stkover)

    METHOD(EFADDR(ExceptionFrame::endTry), SIG1(V,P), endTry)
    METHOD(EFADDR(ExceptionFrame::beginTry), SIG2(V,P,P), beginTry)
    FUNCTION(SETJMP, SIG2(I,P,I), fsetjmp)
    METHOD(COREADDR(AvmCore::beginCatch), SIG5(P,P,P,P,P,P), beginCatch)

SSE2_ONLY(
    CSEMETHOD(COREADDR(AvmCore::doubleToAtom_sse2), SIG2(A,P,F), doubleToAtom_sse2)
    CSEFUNCTION(FUNCADDR(AvmCore::integer_d_sse2), SIG1(I,F), integer_d_sse2)
)

#ifdef DEBUGGER
    METHOD(ENVADDR(MethodEnv::debugEnter), SIG5(V,P,P,P,P,P), debugEnter)
    METHOD(ENVADDR(MethodEnv::debugExit), SIG2(V,P,P), debugExit)
    METHOD(DEBUGGERADDR(Debugger::debugFile), SIG2(V,P,P), debugFile)
    METHOD(DEBUGGERADDR(Debugger::debugLine), SIG2(V,P,I), debugLine)
    METHOD(COREADDR(AvmCore::sampleCheck), SIG1(V,P), sampleCheck)
#endif

	// MOPS helper functions
    METHOD(ENVADDR(MethodEnv::li8),  SIG2(I,P,I), li8)
    METHOD(ENVADDR(MethodEnv::li16), SIG2(I,P,I), li16)
    METHOD(ENVADDR(MethodEnv::li32), SIG2(I,P,I), li32)
    METHOD(ENVADDR(MethodEnv::lf32), SIG2(F,P,I), lf32)
    METHOD(ENVADDR(MethodEnv::lf64), SIG2(F,P,I), lf64)
    METHOD(ENVADDR(MethodEnv::si8),  SIG3(V,P,I,I), si8)
    METHOD(ENVADDR(MethodEnv::si16), SIG3(V,P,I,I), si16)
    METHOD(ENVADDR(MethodEnv::si32), SIG3(V,P,I,I), si32)
    METHOD(ENVADDR(MethodEnv::sf32), SIG3(V,P,F,I), sf32)
    METHOD(ENVADDR(MethodEnv::sf64), SIG3(V,P,F,I), sf64)
