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

#ifndef __avmplus_PoolObject__
#define __avmplus_PoolObject__


namespace avmplus
{
#ifdef FEATURE_NANOJIT
    class PageMgr;
#endif

#ifdef AVMPLUS_WORD_CODE
	
	// This needs to be a root because there are GCObjects referenced from the multinames
	// that are not protected by write barriers (namely, the NamespaceSet objects).
	// Other objects in the multinames are RC; the PrecomputedMultinames constructor
	// explicitly increments their reference counts, and the destructor decrements
	// them.  There are no barriers here, because we want to be able to reach in
	// and reference a Multiname structure directly.
	
	class PrecomputedMultinames : public MMgc::GCRoot
	{
	public:
		void *operator new(size_t size, size_t extra=0)
		{
			// 	GCRoot requires this allocation to come from FixedMalloc 
			return MMgc::FixedMalloc::GetInstance()->Alloc(size+extra);
		}

		PrecomputedMultinames(MMgc::GC* gc, PoolObject* pool);		
		~PrecomputedMultinames();
		void Initialize(PoolObject* pool);		// Eagerly parses all multinames
		uint32 nNames;							// Number of elements
		Multiname multinames[1];				// Allocated size is MAX(1,nName)
	};
	
#endif  // AVMPLUS_WORD_CODE
	
	/**
	 * The PoolObject class is a container for the pool of resources
	 * decoded from an ABC file: the constant pool, the methods
	 * defined in the ABC, the classes defined in the ABC, and so on.
	 */
	class PoolObject : public MMgc::GCFinalizedObject
	{
		friend class AbcParser;
		
	public:
		AvmCore *core;

		/** constants */
		List<int32_t> cpool_int;
		List<uint32_t> cpool_uint;
		List<double*, LIST_GCObjects> cpool_double;	// explicitly specify LIST_GCObject b/c these are GC-allocated ptrs
		List<Stringp> cpool_string;
		List<Namespacep> cpool_ns;
		List<NamespaceSetp> cpool_ns_set;

#ifndef AVMPLUS_64BIT
		// lists to keep int/uint atoms "sticky".
		// @todo this can/should go away when we convert to 64-bit Box atoms.
		List<Atom, LIST_GCObjects> cpool_int_atoms;	
		List<Atom, LIST_GCObjects> cpool_uint_atoms;	
#endif

		// explicitly specify LIST_NonGCObjects b/c these aren't really atoms, they are offsets
		List<Atom,LIST_NonGCObjects> cpool_mn;

		/** metadata -- ptrs into ABC, not gc-allocated */
		List<const byte*> metadata_infos;

		/** domain */
		DWB(Domain*) domain;
		
		/** constructors for class objects, for op_newclass */
		List<MethodInfo*> scripts;

		/** # of elements in metadata array */
		uint32 metadataCount;

		/** # of elements in cpool array */
		uint32 constantCount;
		uint32 constantIntCount;
		uint32 constantUIntCount;
		uint32 constantDoubleCount;
		uint32 constantStringCount;
		uint32 constantNsCount;
		uint32 constantNsSetCount;
		uint32 constantMnCount;

		/** # of elements in scripts array */
		uint32 scriptCount;

		/** flags to control certain bugfix behavior */
		uint32 bugFlags;
		// Numbers here correspond to Bugzilla bug numbers (i.e. bugzilla bug 444630 is kbug444630
		enum {
			kbug444630 = 0x00000001
		};

#ifdef AVMPLUS_WORD_CODE
		struct 
		{
			PrecomputedMultinames* cpool_mn;	// a GCRoot
		} word_code;
		void initPrecomputedMultinames();
#endif
		
        #ifdef FEATURE_NANOJIT
        DWB(PageMgr*) codePages;
        #endif

		PoolObject(AvmCore* core, ScriptBuffer& sb, const byte* startpos);
		~PoolObject();

		MethodInfo* getNamedScript(const Multiname* multiname) const;

		inline const byte* getMetadataInfoPos(uint32 index) { return metadata_infos[index]; }
		Traits* getTraits(Stringp name, Namespacep ns, bool recursive=true) const;
		Traits* getTraits(const Multiname& n, const Toplevel* toplevel, bool recursive=true) const;
		Traits* getTraits(Stringp name, bool recursive=true) const;

		Traits* getBuiltinTraits(Stringp name) const;

		void addPrivateNamedScript(Stringp name, Namespacep ns, MethodInfo *a);
		void addNamedTraits(Stringp name, Namespacep ns, Traits* traits);
		
		//
		// deferred parsing
		//

		void parseMultiname(const byte *pos, Multiname& m) const;

		Traits* resolveTypeName(uint32 index, const Toplevel* toplevel, bool allowVoid=false) const;
		Traits* resolveTypeName(const byte*& pc, const Toplevel* toplevel, bool allowVoid=false) const
		{
			return resolveTypeName(AvmCore::readU30(pc), toplevel, allowVoid);
		}

		void resolveQName(uint32_t index, Multiname &m, const Toplevel* toplevel) const;
		void resolveQName(const byte* &p, Multiname &m, const Toplevel* toplevel) const
		{
			resolveQName(AvmCore::readU30(p), m, toplevel);
		}

		Traits* resolveParameterizedType(const Toplevel* toplevel, Traits* base, Traits* type_param) const;


		void parseMultiname(Multiname& m, int index) const
		{
			AvmAssert (index >= 0 && index < int(constantMnCount));
			Atom a = cpool_mn[index];
			parseMultiname(atomToPos(a), m);
		}

		void parseMultiname(Atom a, Multiname& m) const
		{
			parseMultiname(atomToPos(a), m);
		}

		Namespacep getNamespace(int index) const;
		NamespaceSetp getNamespaceSet(int index) const;
		Stringp getString(int index) const;

		Atom getLegalDefaultValue(const Toplevel* toplevel, uint32 index, CPoolKind kind, Traits* t);
		static bool isLegalDefaultValue(BuiltinType bt, Atom value);

		Atom posToAtom(const byte* pos) const
		{
			return (pos - _abcStart)<<3 | kObjectType;
		}

		const byte* atomToPos(Atom a) const
		{
			AvmAssert((a&7)==kObjectType);
			return _abcStart + urshift(a,3);
		}

		// Index of the metadata info that means skip the associated definition
		List<uint32_t> stripMetadataIndexes;

		int version;
		
		ScriptBuffer code() 
		{ 
			return (ScriptBufferImpl*)_code; 
		}

		bool isCodePointer(const byte* pos)
		{
			return pos > &code()[0] && pos < _code->getBuffer() + code().getSize();
		}

	public:
		inline uint32_t classCount() const { return _classes.size(); }
		inline Traits* getClassTraits(uint32_t i) const { return _classes[i]; }

		inline uint32_t methodCount() const { return _methods.size(); }
		inline MethodInfo* getMethodInfo(uint32_t i) const { return _methods[i]; }
#ifdef DEBUGGER
		inline DebuggerMethodInfo* getDebuggerMethodInfo(uint32_t i) const { return (i < _method_dmi.size()) ? _method_dmi[i] : NULL; }
#endif
#if VMCFG_METHOD_NAMES
		Stringp getMethodInfoName(uint32_t i);
#endif
		
	private:
		DWB(MultinameHashtable*)					_namedTraits;
		DWB(MultinameHashtable*)					_privateNamedScripts;
		DWB(ScriptBufferImpl*)						_code;
		const byte * const							_abcStart;
		List<Traits*, LIST_GCObjects>				_classes;
		List<MethodInfo*, LIST_GCObjects>			_methods;
#ifdef DEBUGGER
		List<DebuggerMethodInfo*, LIST_GCObjects>	_method_dmi;
#endif
#if VMCFG_METHOD_NAMES
		// only allocated & populated if core->config.methodName is true...
		// if positive, an index into cpool_string; if negative, an index into cpool_mn
		// (always safe because those indices are limited to 30 bits)
		List<int32_t, LIST_NonGCObjects>			_method_name_indices;	
#endif


	public:
		// @todo, privatize & make into bitfield (requires API churn)
		bool						isBuiltin;	// true if this pool is baked into the player.  used to control whether callees will set their context.
	#ifdef AVMPLUS_VERBOSE
		bool						verbose;
	#endif
	};
}

#endif /* __avmplus_PoolObject__ */
