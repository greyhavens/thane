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


#ifndef __avmplus_NativeFunction__
#define __avmplus_NativeFunction__

namespace avmplus
{
	typedef avmplus::AbcEnv* AvmInstance;
	typedef avmplus::ScriptObject* AvmObject;
	typedef avmplus::String* AvmString;
	typedef avmplus::Namespace* AvmNamespace;
	typedef avmplus::Atom AvmBox;
	typedef avmplus::MethodEnv* AvmMethodEnv;
	typedef int32_t AvmBool32;

	#define kAvmThunkNull		nullObjectAtom
	#define kAvmThunkUndefined	undefinedAtom

	#define kAvmThunkInfinity		(MathUtils::kInfinity)
	#define kAvmThunkNegInfinity	(MathUtils::kNegInfinity)
	#define kAvmThunkNaN			(MathUtils::kNaN)
	
	typedef AvmObject		AvmRetType_AvmObject;
	typedef bool			AvmRetType_AvmBool32;	// bools are passed in as int32, but returned as bool, for historic reasons
	typedef int32_t			AvmRetType_int32_t;
	typedef uint32_t		AvmRetType_uint32_t;
	typedef AvmNamespace	AvmRetType_AvmNamespace;
	typedef AvmBox			AvmRetType_AvmBox;
	typedef AvmString		AvmRetType_AvmString;
	typedef void			AvmRetType_void;
	typedef double			AvmRetType_double;

	typedef avmplus::ScriptObject AvmObjectT;
	typedef avmplus::String AvmStringT;
	typedef avmplus::Namespace AvmNamespaceT;

	typedef AvmBox (*AvmThunkNativeThunker)(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	typedef double (*AvmThunkNativeThunkerN)(AvmMethodEnv env, uint32_t argc, AvmBox* argv);

	typedef void (AvmObjectT::*AvmThunkNativeMethodHandler)();
	typedef void (*AvmThunkNativeFunctionHandler)(AvmObject obj);

	const uintptr_t kUnboxMask = ~uintptr_t(7);
	#define AvmThunkUnbox_AvmReceiver(t,r)	((t)(uintptr_t(r) & kUnboxMask))
	#define AvmThunkUnbox_AvmObject(r)		((ScriptObject*)(r))
	#define AvmThunkUnbox_AvmBool32(r)		((r) != 0)
	#define AvmThunkUnbox_int32_t(r)		int32_t(r)
	#define AvmThunkUnbox_uint32_t(r)		uint32_t(r)
	#define AvmThunkUnbox_AvmNamespace(r)	((Namespace*)(r))
	#define AvmThunkUnbox_AvmBox(r)			(r)
	#define AvmThunkUnbox_AvmString(r)		((String*)(r))
	#define AvmThunkUnbox_void(r)			(error ??? illegal)
	#define AvmThunkUnbox_double(r)			AvmThunkUnbox_double_impl(&(r))

	#define AvmThunkArgSize_AvmObject		1
	#define AvmThunkArgSize_AvmBool32		1
	#define AvmThunkArgSize_int32_t			1
	#define AvmThunkArgSize_uint32_t		1
	#define AvmThunkArgSize_AvmNamespace	1
	#define AvmThunkArgSize_AvmBox			1
	#define AvmThunkArgSize_AvmString		1
	#define AvmThunkArgSize_void			(error ??? illegal)
#ifdef AVMPLUS_64BIT
	#define AvmThunkArgSize_double			1
#else
	#define AvmThunkArgSize_double			2
#endif

	inline double AvmThunkUnbox_double_impl(const AvmBox* b)
	{
	#if defined(AVMPLUS_64BIT)
		AvmAssert(sizeof(AvmBox) == sizeof(double));
		return *(const double*)b;
	#elif defined(AVMPLUS_UNALIGNED_ACCESS)
		AvmAssert(sizeof(AvmBox)*2 == sizeof(double));
		return *(const double*)b;
	#else
		AvmAssert(sizeof(AvmBox)*2 == sizeof(double));
		union {
			double d;
			AvmBox b[2];
		} u;
		// @todo, does this need endian attention?
		u.b[0] = b[0];
		u.b[1] = b[1];
		return u.d;
	#endif
	}

	// trick, since values are compile-time known we usually don't need to call intToAtom, can statically transform them
	// good for ints and ints currently
	#define AvmThunkCanBeSmallIntAtom(v)	(!((v) & 0xF0000000))
	#define AvmThunkSmallIntAtom(v)			((((Atom)(v))<<3) | kIntegerType)
		
	// note, this isn't complete -- only the ones currently needed are defined.
	// expand as necessary. macros to take advantage of the fact that most
	// args are compile-time constants.
	#define AvmThunkCoerce_int32_t_double(v)	double(v)
	#define AvmThunkCoerce_int32_t_uint32_t(v)	uint32_t(v)
	#define AvmThunkCoerce_int32_t_AvmBox(v)	(AvmThunkCanBeSmallIntAtom(v) ? AvmThunkSmallIntAtom(v) : env->core()->intAtom(v))

	#define AvmThunkCoerce_uint32_t_double(v)	double(v)
	#define AvmThunkCoerce_uint32_t_int32_t(v)	int32_t(v)
	#define AvmThunkCoerce_uint32_t_AvmBox(v)	(AvmThunkCanBeSmallIntAtom(v) ? AvmThunkSmallIntAtom(v) : env->core()->intAtom(v))

	#define AvmThunkCoerce_AvmBool32_AvmBox(v)	((v) ? trueAtom : falseAtom)

#ifdef _DEBUG
	inline double AvmThunkCoerce_AvmBox_double(AvmBox v) { AvmAssert((v) == kAvmThunkUndefined); (void)v; return kAvmThunkNaN; }
	inline AvmString AvmThunkCoerce_AvmBox_AvmString(AvmBox v) { AvmAssert((v) == kAvmThunkUndefined || (v) == kAvmThunkNull); (void)v; return NULL; }
	inline AvmObject AvmThunkCoerce_AvmBox_AvmObject(AvmBox v) { AvmAssert((v) == kAvmThunkUndefined || (v) == kAvmThunkNull); (void)v; return NULL; }
#else
	#define AvmThunkCoerce_AvmBox_double(v)		(kAvmThunkNaN)
	#define AvmThunkCoerce_AvmBox_AvmString(v)	(NULL)
	#define AvmThunkCoerce_AvmBox_AvmObject(v)	(NULL)
#endif

	#define AvmThunkCoerce_AvmString_AvmBox(v)	((v) ? (v)->atom() : nullStringAtom)

	#define AvmThunkConstant_AvmString(v)		(env->method->pool()->cpool_string[v])
	
	#define AVMTHUNK_GET_METHOD_HANDLER(env)	((env)->method->handler_method())
	#define AVMTHUNK_GET_FUNCTION_HANDLER(env)	((env)->method->handler_function())

	union AvmThunkNativeHandler
	{
		AvmThunkNativeMethodHandler method;
		AvmThunkNativeFunctionHandler function;
	};

	struct NativeMethodInfo
	{
	public:
		AvmThunkNativeHandler handler;
		AvmThunkNativeThunker thunker;
#ifndef AVMPLUS_NO_STATIC_POINTERS
		int32_t method_id;
#endif
	};

	struct NativeClassInfo
	{
	public:
		CreateClassClosureProc createClassClosure;
#ifndef AVMPLUS_NO_STATIC_POINTERS
		int32_t class_id;
#endif
		uint32_t sizeofClass;
		uint32_t sizeofInstance;
	};


	// ---------------

	class NativeInitializer
	{
	public:
		NativeInitializer(AvmCore* core, 
			const uint8_t* abcData,
			uint32_t abcDataLen,
			uint32_t methodCount,
			uint32_t classCount);

		~NativeInitializer();

		PoolObject* parseBuiltinABC(Domain* domain, const List<Stringp, LIST_RCObjects>* includes = NULL);

		const NativeMethodInfo* getNativeInfo(uint32_t i) const { return get_method(i); }
	
		#ifdef AVMPLUS_NO_STATIC_POINTERS
			typedef void (*FillInProc)(NativeMethodInfo* m, NativeClassInfo* c);
			void fillIn(FillInProc p);
			inline const NativeClassInfo* get_class(uint32_t i) const { AvmAssert(i < classCount); return &classes[i]; }
		#else
			void fillInMethods(const NativeMethodInfo* methodEntry);
			void fillInClasses(const NativeClassInfo* classEntry);
			inline const NativeClassInfo* get_class(uint32_t i) const { AvmAssert(i < classCount); return classes[i]; }
		#endif
		
	private:
		#ifdef AVMPLUS_NO_STATIC_POINTERS
			typedef NativeMethodInfo MethodType;
			typedef NativeClassInfo ClassType;
			inline const NativeMethodInfo* get_method(uint32_t i) const { AvmAssert(i < methodCount); return &methods[i]; }
		#else
			typedef const NativeMethodInfo* MethodType;
			typedef const NativeClassInfo* ClassType;
			inline const NativeMethodInfo* get_method(uint32_t i) const { AvmAssert(i < methodCount); return methods[i]; }
		#endif

	private:
		AvmCore* const			core;
		const uint8_t* const	abcData;
		uint32_t const			abcDataLen;
		MethodType* const		methods;
		ClassType* const		classes;
		const uint32_t			methodCount;
		const uint32_t			classCount;
	};

	#define _NATIVE_METHOD_CAST_PTR(CLS, PTR) \
		reinterpret_cast<AvmThunkNativeMethodHandler>((void(CLS::*)())(PTR))

	#define AVMTHUNK_NATIVE_CLASS_GLUE(CLS) \
		static ClassClosure* CLS##_createClassClosure(VTable* cvtable) \
		{ return new (cvtable->gc(), cvtable->getExtraSize()) CLS(cvtable); } 

	#define AVMTHUNK_DECLARE_NATIVE_INITIALIZER(NAME) \
		extern PoolObject* initBuiltinABC_##NAME(AvmCore* core, Domain* domain, const List<Stringp, LIST_RCObjects>* includes);

#ifdef AVMPLUS_NO_STATIC_POINTERS

	#define AVMTHUNK_BEGIN_NATIVE_TABLES(NAME) \
		static void fillIn_##NAME(NativeMethodInfo* m, NativeClassInfo* c) { 

	#define AVMTHUNK_END_NATIVE_TABLES() \
		}
	
	// ---------------

	#define AVMTHUNK_BEGIN_NATIVE_METHODS(NAME) 

	#define _AVMTHUNK_NATIVE_METHOD(CLS, METHID, IMPL) \
		m[METHID].handler.method = _NATIVE_METHOD_CAST_PTR(CLS, &IMPL); \
		m[METHID].thunker = (AvmThunkNativeThunker)avmplus::NativeID::METHID##_thunk; 

	#define AVMTHUNK_NATIVE_METHOD(METHID, IMPL) \
		_AVMTHUNK_NATIVE_METHOD(ScriptObject, METHID, IMPL)

	#define AVMTHUNK_NATIVE_METHOD_STRING(METHID, IMPL) \
		_AVMTHUNK_NATIVE_METHOD(avmplus::String, METHID, IMPL)

	#define AVMTHUNK_NATIVE_METHOD_NAMESPACE(METHID, IMPL) \
		_AVMTHUNK_NATIVE_METHOD(avmplus::Namespace, METHID, IMPL)

	#define AVMTHUNK_NATIVE_FUNCTION(METHID, IMPL) \
		m[METHID].handler.function = reinterpret_cast<AvmThunkNativeFunctionHandler>(IMPL); \
		m[METHID].thunker = (AvmThunkNativeThunker)avmplus::NativeID::METHID##_thunk; 

	#define AVMTHUNK_END_NATIVE_METHODS() 

	// ---------------

	#define AVMTHUNK_BEGIN_NATIVE_CLASSES(NAME) 

	#define AVMTHUNK_NATIVE_CLASS(CLSID, CLS, INST) \
		c[CLSID].createClassClosure = (CreateClassClosureProc)CLS##_createClassClosure; \
		c[CLSID].sizeofClass = sizeof(CLS); \
		c[CLSID].sizeofInstance = sizeof(INST); 

	#define AVMTHUNK_END_NATIVE_CLASSES() 

	#define AVMTHUNK_DEFINE_NATIVE_INITIALIZER(NAME) \
		PoolObject* initBuiltinABC_##NAME(AvmCore* core, Domain* domain, const List<Stringp, LIST_RCObjects>* includes) { \
			NativeInitializer ninit(core, \
				avmplus::NativeID::NAME##_abc_data, \
				avmplus::NativeID::NAME##_abc_length, \
				avmplus::NativeID::NAME##_abc_method_count, \
				avmplus::NativeID::NAME##_abc_class_count); \
			ninit.fillIn(fillIn_##NAME); \
			return ninit.parseBuiltinABC(domain, includes); \
		}

#else

	#define AVMTHUNK_BEGIN_NATIVE_TABLES(NAME)
	#define AVMTHUNK_END_NATIVE_TABLES()
	
	// ---------------

	#define AVMTHUNK_BEGIN_NATIVE_METHODS(NAME) \
		static const NativeMethodInfo NAME##_methodEntries[] = {
			
	#define _AVMTHUNK_NATIVE_METHOD(CLS, METHID, IMPL) \
		{ { _NATIVE_METHOD_CAST_PTR(CLS, &IMPL) }, (AvmThunkNativeThunker)avmplus::NativeID::METHID##_thunk, avmplus::NativeID::METHID },

	#define AVMTHUNK_NATIVE_METHOD(METHID, IMPL) \
		_AVMTHUNK_NATIVE_METHOD(ScriptObject, METHID, IMPL)

	#define AVMTHUNK_NATIVE_METHOD_STRING(METHID, IMPL) \
		_AVMTHUNK_NATIVE_METHOD(avmplus::String, METHID, IMPL)

	#define AVMTHUNK_NATIVE_METHOD_NAMESPACE(METHID, IMPL) \
		_AVMTHUNK_NATIVE_METHOD(avmplus::Namespace, METHID, IMPL)

	// C++ won't let us auto-init a union to a field other than the first one, nor will it
	// allow us to reliably cast between a pointer-to-function and pointer-to-member-function,
	// thus this inline function to massage the few places that need it.
	inline AvmThunkNativeMethodHandler _to_method_handler(AvmThunkNativeFunctionHandler function)
	{
		AvmThunkNativeHandler handler;
		handler.function = function;
		return handler.method;
	}

	#define AVMTHUNK_NATIVE_FUNCTION(METHID, IMPL) \
		{ { _to_method_handler(reinterpret_cast<AvmThunkNativeFunctionHandler>(IMPL)) }, (AvmThunkNativeThunker)avmplus::NativeID::METHID##_thunk, avmplus::NativeID::METHID },

	#define AVMTHUNK_END_NATIVE_METHODS() \
		{ { NULL }, NULL, -1 } };

	// ---------------

	#define AVMTHUNK_BEGIN_NATIVE_CLASSES(NAME) \
		const NativeClassInfo NAME##_classEntries[] = {

	#define AVMTHUNK_NATIVE_CLASS(CLSID, CLS, INST) \
		{ (CreateClassClosureProc)CLS##_createClassClosure, avmplus::NativeID::CLSID, sizeof(CLS), sizeof(INST) },

	#define AVMTHUNK_END_NATIVE_CLASSES() \
		{ NULL, -1, 0, 0 } };

	#define AVMTHUNK_DEFINE_NATIVE_INITIALIZER(NAME) \
		PoolObject* initBuiltinABC_##NAME(AvmCore* core, Domain* domain, const List<Stringp, LIST_RCObjects>* includes) { \
			NativeInitializer ninit(core, \
				avmplus::NativeID::NAME##_abc_data, \
				avmplus::NativeID::NAME##_abc_length, \
				avmplus::NativeID::NAME##_abc_method_count, \
				avmplus::NativeID::NAME##_abc_class_count); \
			ninit.fillInClasses(NAME##_classEntries); \
			ninit.fillInMethods(NAME##_methodEntries); \
			return ninit.parseBuiltinABC(domain, includes); \
		}

#endif // AVMPLUS_NO_STATIC_POINTERS

	#define AVM_INIT_BUILTIN_ABC_IN_DOMAIN(MAPNAME, CORE, DOMAIN) \
		avmplus::NativeID::initBuiltinABC_##MAPNAME((CORE), (DOMAIN), NULL)

	#define AVM_INIT_BUILTIN_ABC(MAPNAME, CORE) \
		avmplus::NativeID::initBuiltinABC_##MAPNAME((CORE), (CORE)->builtinDomain, NULL)

}	

#endif /* __avmplus_NativeFunction__ */
