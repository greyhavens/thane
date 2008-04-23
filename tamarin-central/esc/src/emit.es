/* -*- mode: java; tab-width: 4; insert-tabs-mode: nil; indent-tabs-mode: nil -*- */
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

/* Rough sketch:

   The emitter provides abstractions for common code generation
   patterns, and some arbitrary amount of utilities to help in code
   generation.  The client starts by creating an emitter, then
   creating scripts on that emitter and classes and other traits on
   those scripts, methods on the classes, and so on.  Boilerplate code
   is inserted for you, so code is generated for class creation when
   you create a class on a script.

   These sketches are particularly rough right now because the needs
   of the code generator (verify.es) are not known precisely yet.  But
   I expect that there will be quite a bit of code in here, and it
   will encapsulate a lot of useful knowledge about how things are
   done on the AVM.

   One thing I'm apparently unresolved on here is when to create structures
   in the ABCFile; right now there's a mix of late and early.  Since the
   abcfile is not laid out until it is finalized this matters little, except
   for the sake of clarity.

   Sometimes this OO setup does not seem natural, other times it simplifies...
*/

namespace Emit;
{
    use default namespace Emit;
    use namespace Util;
    use namespace Abc;
    use namespace Asm;

    use namespace Ast;

    class ABCEmitter
    {
        public var file, constants;
        /*private*/ var scripts = [];

        function ABCEmitter() {
            file = new ABCFile;
            constants = new ABCConstantPool;
            file.addConstants(constants);
            Object_name = nameFromIdent("Object");
            Array_name = nameFromIdent("Array");
            RegExp_name = nameFromIdent("RegExp");
        }

        public function newScript(): Script {
            var s = new Script(this);
            scripts.push(s);
            return s;
        }

        public function finalize() {
            function f(s) { s.finalize() }
            forEach(f, scripts);
            return file;
        }

        public var Object_name;
        public var Array_name;
        public var RegExp_name;
        public var meta_construct_name;

        public function namespace( ns:NAMESPACE ) {
            switch type ( ns ) {
                case (int_ns:IntrinsicNamespace) {
                    return constants.namespace(CONSTANT_Namespace, constants.stringUtf8("intrinsic"));  // FIXME
                }
                case (pn:PrivateNamespace) {
                    return constants.namespace(CONSTANT_PrivateNamespace, constants.stringUtf8(pn.name));
                }
                case (pn:ProtectedNamespace) {
                    return constants.namespace(CONSTANT_ProtectedNamespace, constants.stringUtf8(pn.name));
                }
                case (pn:PublicNamespace) {
                    return constants.namespace(CONSTANT_Namespace, constants.stringUtf8(pn.name));
                }
                case (int_ns:InternalNamespace) {
                    return constants.namespace(CONSTANT_PackageInternalNS, constants.stringUtf8(int_ns.name));
                }
                case (un:UserNamespace) {
                    /// return constants.namespace(CONSTANT_ExplicitNamespace, constants.stringUtf8(pn.name));
                    return constants.namespace(CONSTANT_Namespace, constants.stringUtf8(un.name));
                }
                case (an:AnonymousNamespace) {
                    /// return constants.namespace(CONSTANT_PackageInternalNS, constants.stringUtf8(an.name));
                    return constants.namespace(CONSTANT_Namespace, constants.stringUtf8(an.name));
                }
                case (imp_ns:ImportNamespace) {
                    internalError("", 0, "Unimplemented namespace ImportNamespace");  // FIXME: source pos
                }
                case (x:*) {
                    internalError("", 0, "Unimplemented namespace " + ns); // FIXME: source pos
                }
            }
        }
        function flattenNamespaceSet(nss /*:[[NAMESPACE]]*/) {
            var new_nss = [];
            for( let i = 0; i <nss.length; i++ ) {
                let temp = nss[i];
                for( let q = 0; q < temp.length; q++) {
                    new_nss.push(namespace(temp[q]));
                } 
            } 
            return new_nss;
        }
        public function multiname(mname, is_attr) {
            let {nss, ident} = mname;
            return constants.Multiname(constants.namespaceset(flattenNamespaceSet(nss)), constants.stringUtf8(ident), is_attr);
        }
        public function qname(qn, is_attr ) {
            let {ns, id} = qn;
            return constants.QName(namespace(ns), constants.stringUtf8(id), is_attr);
        }
        public function nameFromIdent(id) {
            return constants.QName(constants.namespace(CONSTANT_PackageNamespace, constants.stringUtf8("")),
                                   constants.stringUtf8(id),false);
        }

        public function multinameL({nss:nss}, is_attr) {
            return constants.MultinameL(constants.namespaceset(flattenNamespaceSet(nss)), is_attr);
        }

        public function nameFromIdentExpr(e) {
            use namespace Ast;
            switch type (e) {
            case (id:Identifier) { return multiname(id,false) }
            case (qi:QualifiedIdentifier) { 
                switch type(qi.qual) {
                    case( lr:LexicalRef ) {
                        // Hack to deal with namespaces for now...
                        // later we will have to implement a namespace lookup to resolve qualified typenames
                        return qname(new Ast::Name(new AnonymousNamespace(lr.ident.ident), qi.ident), false)
                    }
                    case( e:* ) {
                        internalError("", 0, "Unimplemented: nameFromIdentExpr " + e); // FIXME: source pos
                    }
                }
                return multiname(id,false) 
            }
            case (x:*) { 
                internalError("", 0, "Unimplemented: nameFromIdentExpr " + e);  // FIXME: source pos
            }
            }
        }
        
        public function rtqname({ident:ident}, is_attr)
            constants.RTQName(constants.stringUtf8(ident), is_attr);

        public function rtqnameL(is_attr)
            constants.RTQNameL(is_attr);

        public function typeFromTypeExpr(t) {
            use namespace Ast;
            // not dealing with types for now
            switch type (t) {
                case (tn:TypeName) {
                    switch type( tn.ident ){
                        case(i:Identifier) {
                            let name = i.ident;
                            if( name=="String" || name=="Number" ||
                                name=="Boolean" || name=="int" ||
                                name=="uint" || name=="Object" ||
                                name=="Array" || name=="Class" ||
                                name=="Function") {
                                return nameFromIdent(name);
                            }
                            else if( name=="string" ) {
                                return nameFromIdent("String");
                            }
                            else if( name=="boolean" ) {
                                return nameFromIdent("Boolean");
                            }
                    else {
                        //print ("warning: unknown type name " + t + ", using Object");
                return nameFromIdent("Object");
                }
                        }
                    }
                }
                case (x:*) { 
                    // print ("warning: Unimplemented: typeFromTypeExpr " + t + ", using *");
                }
            }
            return 0;
            
        }

        // Use this only for places that need a QName, only works with basic class names
        // as Tamarin doesn't support 
        public function realTypeName(t) {
            use namespace Ast;
            // not dealing with types for now
            switch type (t) {
                case (tn:TypeName) {
                    return nameFromIdentExpr(tn.ident);
                }
                case (st:SpecialType) {
                    return 0;
                }
                case (x:*) { 
                    internalError("", 0, "Unimplemented: realTypeName " + t + ", using *") // FIXME source pos
                }
            }
            return 0;
            
        }

        public function fixtureNameToName(fn) {
            switch type (fn) {
            case (pn:PropName) {
                return qname(pn.name, false);
            }
            case (tn:TempName) {
                return qname (new Ast::Name(Ast::noNS, "$t"+tn.index),false);  // FIXME allocate and access actual temps
            }
            case (x:*) { 
                internalError("", 0, "Not a valid fixture name " + x); // FIXME source pos
            }
            }
        }
        
        public function fixtureTypeToType(fix) {
            switch type (fix) {
            case (vf:ValFixture) {
                return vf.type != null ? typeFromTypeExpr(vf.type) : 0 ;
            }
            case (mf:MethodFixture) {
                return 0;
            }
            case(x:*) { 
                internalError("", 0, "Unimplemented: fixtureTypeToType " + x);  // FIXME source pos
            }
            }
        }
        
        public function defaultLiteralExpr(lit)
        {
            switch type (lit) {
            case(ln:LiteralNull) {
                return {val:CONSTANT_Null, kind:CONSTANT_Null}
            }
            case(lu:LiteralUndefined) {
                return {val:0, kind:0}
            }
            case(ld:LiteralDouble) {
                let val = constants.float64(ld.doubleValue);
                return {val:val, kind:CONSTANT_Double};
            }
            case(ld:LiteralDecimal) {
                // FIXME: when we support decimal...
                let val = constants.float64(ld.decimalValue);
                return {val:val, kind:CONSTANT_Double};
            }
            case(li:LiteralInt) {
                let val = constants.int32(li.intValue);
                return {val:val, kind:CONSTANT_Integer};
            }
            case(lu:LiteralUInt) {
                let val = constants.uint32(lu.uintValue);
                return {val:val, kind:CONSTANT_UInt};
            }
            case(lb:LiteralBoolean) {
                let val = (lb.booleanValue ? CONSTANT_True : CONSTANT_False);
                return {val:val, kind:val};
            }
            case(ls:LiteralString) {
                let val = constants.stringUtf8(ls.strValue);
                return {val:val, kind:CONSTANT_Utf8};
            }
            case(ln:LiteralNamespace) {
                let val = constants.namespace(ln.namespaceValue);
                return  {val:val, kind:CONSTANT_Namespace};
            }
            case(x:*) {
                syntaxError("", 0, "Default expression must be a constant value " + x); // FIXME: source pos
            }
            }
        }

        public function defaultExpr(expr) {
            // FIXME: This outlaws ~0, -1, and so on.
            switch type (expr) {
            case(le:ILiteralExpr) {
                return defaultLiteralExpr(le);
            }
            case(lr:LexicalRef) {
                switch type ( lr.ident ) {
                case (i:Identifier) {
                    if( i.ident == "undefined" ) {
                        // Handle defualt expr of (... arg = undefined ...)
                        return defaultLiteralExpr(new LiteralUndefined());
                    }
                }
                } 
            }
            }
            syntaxError("", 0, "Default expression must be a constant value " + expr); // FIXME: source pos
        }
    }

    class Script
    {
        public var e, init, traits=[];

        function Script(e:ABCEmitter) {
            this.e = e;
            this.init = new Method(e,[], "", true, new Ast::FuncAttr(null));
        }

        public function newClass(name, basename, interfaces, flags, protectedns=null) {
            return new Emit::Class(this, name, basename, interfaces, flags, protectedns);
        }

        public function newInterface(ifacename, methname, interfaces) {
            return new Emit::Interface(this, ifacename, methname, interfaces);
        }

        public function addException(e) {
            return init.addException(e);
        }
        // Here we probably want: newVar, newConst, ... instead?
        public function addTrait(t) {
            return traits.push(t);
        }

        public function finalize() {
            var id = init.finalize();
            var si = new ABCScriptInfo(id);
            for ( var i=0 ; i < traits.length ; i++ )
                si.addTrait(traits[i]);
            e.file.addScript(si);
        }
    }
    
    class Class
    {
        public var s, name, basename, traits=[], instance=null, cinit, interfaces, flags, protectedns;

        function Class(script, name, basename, interfaces, flags, protectedns=null) {
            this.s = script;
            this.name = name;
            this.basename = basename;
            this.interfaces = interfaces;
            this.flags = flags;
            this.protectedns = protectedns;

            var asm = script.init;
        }

        public function getCInit() {
            if(cinit == null )
                cinit = new Method(s.e, [], "$cinit", true, new Ast::FuncAttr(null));
            return cinit;
        }

        public function getInstance() {
            if( this.instance == null )
                this.instance = new Instance(s, name, basename, interfaces, flags, protectedns);
            
            return this.instance;
        }
        
        public function addTrait(t) {
            traits.push(t);
        }

        public function finalize() {
            var instidx = instance.finalize();
            
            var clsinfo = new ABCClassInfo();
            clsinfo.setCInit(getCInit().finalize());
            for(let i = 0; i < traits.length; ++i)
                clsinfo.addTrait(traits[i]);
            
            var clsidx = s.e.file.addClass(clsinfo);
            
            assert(clsidx == instidx);

            return clsidx;
        }
    }
    
    // The documentation has issues here.
    //
    // The way ASC generates code:
    //   - the flags are ClassInterface|ClassSealed 
    //   - the class init has a body that just executes "returnvoid"
    //   - there is a method_info entry for the instance initializer 
    //     but no corresponding method_body
    //   - logic in cogen is responsible for generating global
    //     code that performs newclass/initproperty

    class Interface
    {
        public var script, ifacename, methname, interfaces, traits=[];

        function Interface(script, ifacename, methname, interfaces) 
            : script=script
            , ifacename=ifacename
            , methname=methname
            , interfaces=interfaces 
        {}

        public function finalize() {
            var clsinfo = new ABCClassInfo();

            var iinit = new Instance(script, ifacename, 0, interfaces, CONSTANT_ClassInterface|CONSTANT_ClassSealed);
            var cinit = (new Method(script.e, [], methname, false, new Ast::FuncAttr(null))).finalize();
            clsinfo.setCInit(cinit);
            for(let i = 0; i < traits.length; ++i)
                clsinfo.addTrait(traits[i]);

            var clsidx = script.e.file.addClass(clsinfo);
            
            var iinitm = new Method(script.e, [], methname, false, new Ast::FuncAttr(null), true);
            iinit.setIInit(iinitm.finalize());
            iinit.finalize();

            return clsidx;

        }
    }

    class Instance 
    {
        public var s, name, basename, flags, interfaces, traits = [], iinit, protectedns;
        
        function Instance(s:Script, name, basename, interfaces, flags, protectedns=null) 
            : s=s
            , name=name
            , basename=basename 
            , interfaces=interfaces
            , flags=flags
            , protectedns=protectedns
        {
        }
        
        public function setIInit(method) {
            iinit = method
        }
        public function addTrait(t) {
            traits.push(t);
        }
        
        public function finalize() {
            if (protectedns != null) {
                flags |= CONSTANT_ClassProtectedNs;
                pnsid = s.e.namespace(protectedns);
            }
            else
                pnsid = 0;
            var instinfo = new ABCInstanceInfo(name, basename, flags, pnsid, interfaces);
            
            instinfo.setIInit(iinit);
            
            for(let i = 0; i < traits.length; i++)
                instinfo.addTrait(traits[i]);
            
            return s.e.file.addInstance(instinfo);
        }
    }

    class Method // extends AVM2Assembler
    {
        public var e, formals, name, asm, traits = [], finalized=false, defaults = null, exceptions=[], attr=null, bodyless;

        function Method(e:ABCEmitter, formals:Array, name, standardPrologue, attr, bodyless=false) {
            //super(e.constants, formals.length);
            this.formals = formals;
            this.e = e;
            this.name = name;
            this.attr = attr;
            this.bodyless = bodyless;

            if (!bodyless && !attr.is_native) {
                asm = new AVM2Assembler(e.constants, formals.length - (attr.uses_rest ? 1 : 0), attr);
                // Standard prologue -- but is this always right?
                // ctors don't need this - have a more complicated prologue
                if(standardPrologue) {
                    asm.I_getlocal_0();
                    asm.I_pushscope();
                }
            }
        }

        public function addTrait(t) {
            return traits.push(t);
        }

        public function setDefaults(d) {
            defaults = d;
        }

        public function addException(e) {
            return exceptions.push(e)-1;
        }

        public function finalize() {
            if (finalized)
                return;
            finalized = true;

            var flags = 0;

            if (!bodyless && !attr.is_native) {
                // Standard epilogue for lazy clients.
                asm.I_returnvoid();
                flags = asm.flags;
            } 
            else if (attr.is_native)
                flags = METHOD_Native;

            var meth = e.file.addMethod(new ABCMethodInfo(name, formals, 0, flags, defaults, null));
            if (!bodyless && !attr.is_native) {
                var body = new ABCMethodBodyInfo(meth);
                body.setMaxStack(asm.maxStack);
                body.setLocalCount(asm.maxLocal);
                body.setInitScopeDepth(0);
                body.setMaxScopeDepth(asm.maxScope);
                body.setCode(asm.finalize());
                for ( var i=0 ; i < traits.length ; i++ )
                    body.addTrait(traits[i]);
            
                for ( var i=0 ; i < exceptions.length; i++ )
                    body.addException(exceptions[i]);
            
                e.file.addMethodBody(body);
            }

            return meth;
        }
    }
}
