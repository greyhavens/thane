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

/* Protocols:
   
   - the 'this' receiver object is passed as the first argument to any call, 
     the value may be null
   - local(0) contains the current 'this' object inside any function body,
     its value is always an object
   - inside class instance methods, the 'this' object is on the scope stack 
     outside the activation object
   - inside constructors, the 'this' object is on the scope stack in the 
     constructor body, also outside the activation object
   - inside other functions, the 'this' object is not on the scope stack
   - inside script top levels the 'this' object is on the scope stack; it
     should be the global object.
   - inside class initializers the 'this' object is on the scope stack; it
     is presumably the class object (but this remains to be verified).
*/

namespace Gen;
{
    use default namespace Gen;

    use namespace Util;
    use namespace Abc;
    use namespace Asm;
    use namespace Emit;
    use namespace Ast;

    /* A context is a structure with the following fields.
     *
     * In the object:
     *
     *    asm      -- the current function's assembler
     *    stk      -- the current function's binding stack (labels, ribs)
     *    target   -- the current trait target
     *
     * Invariant fields in CTX.prototype:
     *
     *    emitter  -- the unique emitter
     *    script   -- the only script we care about in that emitter
     *    cp       -- the emitter's constant pool
     *    filename -- the source file name (for error messages)
     *
     * Default fields in CTX.prototype that may be overridden in the object:
     *
     *    scope_reg -- register holding the activation/scope object (default 0 -- no register)
     *    push_this -- true iff reg0 should be pushed as first scope obj (default false)
     *    
     * FIXME, there are probably at least two targets: one for LET, another
     * for VAR/CONST/FUNCTION.
     *
     * Never construct a CTX by hand, always go through the push* functions
     * near the end of this file.
     */
    dynamic class CTX 
    {
        const asm, stk, target;

        function CTX (asm, stk, target) 
            : asm = asm
            , stk = stk
            , target = target
        { }
    }

    // Emit debug info or not
    var emit_debug = true;

    Gen function internalError(ctx, msg) {
        Util::internalError(ctx.filename, ctx.lineno, msg);
    }

    Gen function syntaxError(ctx, msg) {
        Util::syntaxError(ctx.filename, ctx.lineno, msg);
    }

    /* Returns an ABCFile structure */
    function cg(tree: PROGRAM) {
        let e = new ABCEmitter;
        let s = e.newScript();

        CTX.prototype.emitter = e;
        CTX.prototype.script = s;
        CTX.prototype.cp = e.constants;
        CTX.prototype.filename = tree.file;
        CTX.prototype.scope_reg = 0;

        cgProgram(pushProgram(s), tree);
        return e.finalize();
    }

    /* Support for "eval".  Compile 'tree' to a special script.
     *
     * The script is compiled as a top-level function with the name
     * given by "name", and the script stores it into ESC::eval_hook,
     * where the caller will find it (that's awful style, but it works
     * around a problem with the way name lookup is done in Tamarin,
     * namely, that a file loading a script can't seem to access names
     * bound by that script).
     *
     * Anyhow the new function will be passed an array of scope
     * objects, which it must push onto its scope stack (scopedesc
     * details how many elements, and how to push them).
     *
     * Upon entry, the function must bind the var and function
     * fixtures in 'tree' in the innermost scope object; these
     * properties are deletable.  Then it must run the program.  (No
     * other fixture types are allowed.)
     *
     * The function can't contain a "return" statement (because it was
     * parsed as a <Program>) but the statement result value is
     * captured and is return if the script terminates normally.
     */

    function cgEval(tree: PROGRAM, name: String, scopedesc: String) {

        function evalInits() {
            let d = [];
            for ( let i=0 ; i < scopedesc.length ; i++ )
                d.push(new Ast::EvalScopeInitExpr(i, scopedesc.charAt(i)));
            return d;
        }

        let attr = new Ast::FuncAttr(null);
        attr.capture_result = true;
        return cg(new Ast::Program( [], 
                                    new Ast::Block(new Ast::Head([],[]), 
                                                   [new Ast::ExprStmt(new Ast::SetExpr(Ast::assignOp,
                                                                                       new Ast::LexicalRef(new Ast::QualifiedIdentifier(new Ast::LexicalRef(new Ast::Identifier("ESC",[[Ast::noNS]])),
                                                                                                                                        "eval_hook")),
                                                                                       new Ast::LexicalRef(new Ast::Identifier(name, [[Ast::noNS]]))))]),
                                    new Ast::Head([[new Ast::PropName(new Ast::Name(Ast::noNS, name)),
                                                    new Ast::MethodFixture(new Ast::Func(new Ast::FuncName(Ast::ordinaryFunction, name),
                                                                                         tree.block,
                                                                                         new Ast::Head([[new Ast::TempName(100000),
                                                                                                         new Ast::ValFixture(Ast::anyType,false)]],
                                                                                                       evalInits()),
                                                                                         1,
                                                                                         new Ast::Head([],[]),
                                                                                         [],
                                                                                         Ast::anyType,
                                                                                         attr),
                                                                           Ast::anyType,
                                                                           false,
                                                                           false,
                                                                           false)]],
                                                  []),
                                    tree.attr,
                                    tree.file));
    }

    function cgDebugFile(ctx) {
        let {asm, cp} = ctx;
        if( emit_debug && ctx.filename != null )
            asm.I_debugfile(cp.stringUtf8(ctx.filename));
    }

    function cgProgram(ctx, prog) {
        cgDebugFile(ctx);
        if (prog.head.fixtures != null)
            cgFixtures(ctx, prog.head.fixtures);
        cgBlock(ctx, prog.block);
    }

    function hasTrait(traits, name, kind) {
        for (let i = 0, l =traits.length; i < l; i++) {
            let t = traits[i];
            if(t.name==name && ((t.kind&15)==kind))
                return true;
        }
        return false;
    }

    function probeTrait(traits, name) {
        for (let i=0, limit=traits.length ; i < limit ; i++) {
            let t = traits[i];
            if(t.name == name)
                return [true, t.kind & 15];
        }
        return [false, 0];
    }
    
    function cgFixtures(ctx, fixtures) {
        let {target, asm, emitter} = ctx;
        for ( let i=0 ; i < fixtures.length ; i++ ) {
            let [fxname, fx] = fixtures[i];
            let name = emitter.fixtureNameToName(fxname);

            if (fx is ValFixture) {
                if (checkTrait(fxname, name, TRAIT_Slot))
                    target.addTrait(new ABCSlotTrait(name, 0, false, 0, emitter.typeFromTypeExpr(fx.type), 0, 0)); 
                // FIXME when we have more general support for type annos
            }
            else if (fx is MethodFixture) {
                let methidx = cgFunc(ctx, fx.func);
                if (target is Method || target is Script) {
                    // Normal function definition
                    if (checkTrait(fxname, name, TRAIT_Slot))
                        target.addTrait(new ABCSlotTrait(name, 0, false, 0, 0, 0, 0));
                    asm.I_findpropstrict(name);
                    asm.I_newfunction(methidx);
                    asm.I_setproperty(name);
                }
                else {
                    // target.addTrait(new ABCOtherTrait(name, 0, TRAIT_Method, 0, methidx));
                    let trait_kind;
                    if (fx.func.name.kind == getterFunction) {
                        trait_kind = TRAIT_Getter;
                        if (hasTrait(target.traits, name, trait_kind))
                            dupTrait(fxname, trait_kind, trait_kind);
                    }
                    else if (fx.func.name.kind == setterFunction) {
                        trait_kind = TRAIT_Setter;
                        if (hasTrait(target.traits, name, trait_kind))
                            dupTrait(fxname, trait_kind, trait_kind);
                    }
                    else {
                        trait_kind = TRAIT_Method;
                        checkTrait(fxname, name, trait_kind, true);
                    }
                    let methattrs = 0;
                    if (fx.isOverride)
                        methattrs |= ATTR_Override;
                    if (fx.isFinal)
                        methattrs |= ATTR_Final;
                    target.addTrait(new ABCOtherTrait(name, methattrs, trait_kind, 0, methidx));
                }
            }
            else if (fx is ClassFixture) {
                checkTrait(fxname, name, TRAIT_Slot, true);
                let clsidx = cgClass(ctx, fx.cls);
                target.addTrait(new ABCOtherTrait(name, 0, TRAIT_Class, 0, clsidx));
            }
            else if (fx is InterfaceFixture) {
                checkTrait(fxname, name, TRAIT_Slot, true);
                let ifaceidx = cgInterface(ctx, fx.iface);
                target.addTrait(new ABCOtherTrait(name, 0, TRAIT_Class, 0, ifaceidx));
            }
            else if (fx is NamespaceFixture) {
                checkTrait(fxname, name, TRAIT_Slot, true);
                target.addTrait(new ABCSlotTrait(name, 0, true, 0, 
                                                 emitter.qname(new Ast::Name(Ast::noNS, "Namespace"),false), 
                                                 emitter.namespace(fx.ns), CONSTANT_Namespace));
            }
            else if (fx is TypeFixture) {
                //print ("warning: ignoring type fixture");
            }
            else {
                Gen::internalError(ctx, "Unhandled fixture type " + fx);
            }
        }

        function checkTrait(fxname, name, kind, unique=false) {
            let [has_trait, trait_kind] = probeTrait(target.traits, name);
            if (has_trait && (unique || trait_kind != kind))
                dupTrait(fxname, kind, trait_kind);
            return !has_trait;
        }

        function dupTrait(fxname, newkind, oldkind) {
            if (newkind != oldkind)
                Gen::syntaxError(ctx, "Incompatible fixture : name=" + fxname +
                                 ", newkind=" + newkind + ", oldkind=" + oldkind );
            else
                Gen::syntaxError(ctx, "Duplicate fixture : name=" + fxname);
        }
    }

    function cgBlock(ctx, b) {
        // FIXME -- more here
        cgHead(ctx, b.head);
        let stmts = b.stmts;
        for ( let i=0 ; i < stmts.length ; i++ )
            cgStmt(ctx, stmts[i]);
    }

    function extractNamedFixtures(fixtures) {
        let named = [];
        let fix_length = fixtures ? fixtures.length : 0;
        for(let i = 0; i < fix_length; ++i)
        {
            let [name,fixture] = fixtures[i];
            switch type (name) {
                case (pn:PropName) {
                    named.push([name,fixture]);
                }
                case (tn:TempName) {
                    // do nothing
                }
            }
        }
        return named;
    }
    
    function extractUnNamedFixtures(fixtures) {
        let named = [];
        let fix_length = fixtures ? fixtures.length : 0;
        for(let i = 0; i < fix_length; ++i)
        {
            let [name,fixture] = fixtures[i];
            switch type (name) {
                case (pn:PropName) {
                    // do nothing
                }
                case (tn:TempName) {
                    named.push([name,fixture]);
                }
            }
       }
       return named;
   }

    function cgClass(ctx, c) {
        let {asm, emitter, script} = ctx;
        
        let classname = emitter.qname(c.name,false);
        let basename = emitter.qname(c.baseName,false);
        let interfacenames = Util::map(function (n) { return emitter.qname(n,false) }, c.interfaceNames);

        let flags = 0;
        if (!(c.isDynamic))
            flags |= CONSTANT_ClassSealed;
        if (c.isFinal)
            flags |= CONSTANT_ClassFinal;
        let cls = script.newClass(classname, basename, interfacenames, flags, c.protectedns);
        
        let c_ctx = pushClass(ctx, cls);

        // static fixtures
        cgFixtures(c_ctx, c.classHead.fixtures);

        // cinit - init static fixtures
        let cinit = cls.getCInit();
        let cinit_ctx = pushCInit(ctx, cinit);
        cgDebugFile(cinit_ctx);
        cgHead(cinit_ctx, {fixtures:[], exprs:c.classHead.exprs});
        cgBlock(cinit_ctx, c.classBody);

        let inst = cls.getInstance();
        
        // Context for the instance
        let i_ctx = pushInstance(ctx, inst);
        
        // do instance slots
        cgFixtures(i_ctx, c.instanceHead.fixtures);  // FIXME instanceHead and instanceInits should be unified
        
        inst.setIInit(cgCtor(i_ctx, c.constructor, {fixtures:[],exprs:c.instanceHead.exprs}));
        
        let clsidx = cls.finalize();

        asm.I_findpropstrict(basename);
        asm.I_getproperty(basename);
        asm.I_dup();
        asm.I_pushscope();
        asm.I_newclass(clsidx);
        asm.I_popscope();
        asm.I_getglobalscope();
        asm.I_swap();
        asm.I_initproperty(classname);

        return clsidx;
    }

    function cgInterface(ctx, c) {
        let {asm, emitter, script} = ctx;
        
        let ifacename = emitter.qname(c.name,false);
        let interfacenames = Util::map(function (n) { return emitter.qname(n,false) }, c.interfaceNames);

        let iface = script.newInterface(ifacename, ctx.cp.stringUtf8(c.name.id), interfacenames);
        
        let ifaceidx = iface.finalize();

        asm.I_getglobalscope();
        asm.I_pushnull();
        asm.I_newclass(ifaceidx);
        asm.I_initproperty(ifacename);

        return ifaceidx;
    }
    
    /*  
     *  Generate code for a ctor.
     */
    function cgCtor(ctx, c, instanceInits) {
        let f = c.func;
        let formals_type = extractFormalTypes(ctx, f);
        let method = new Method(ctx.script.e, formals_type, "$construct", false, f.attr);

        let defaults = extractDefaultValues(ctx, f);
        if( defaults.length > 0 )
            method.setDefaults(defaults);
        
        if (!f.attr.is_native) {
            let asm = method.asm;
            let t = asm.getTemp();
            // FIXME: record that scopes must be restored here!
            let ctor_ctx = pushIInit(ctx, t, method);
       
            cgDebugFile(ctor_ctx);
            asm.I_getlocal(0);
            asm.I_dup();
            // Should this be instanceInits.inits only?
            asm.I_pushscope();  // This isn't quite right...
            for( let i = 0; i < instanceInits.length; i++ ) {
                cgExpr(ctor_ctx, instanceInits[i]);
                asm.I_pop();
            }
            cgHead(ctor_ctx, instanceInits);
            asm.I_popscope();
            //cgHead(ctor_ctx, instanceInits.inits, true);

            // Push 'this' onto scope stack
            //asm.I_getlocal(0);
            //asm.I_pushscope();
            // Create the activation object, and initialize params
            asm.I_newactivation();
            asm.I_dup();
            asm.I_setlocal(t);
            asm.I_dup();
            asm.I_pushwith();

            setupArguments(ctor_ctx, f);

            cgHead(ctor_ctx, f.params);

            for ( let i=0 ; i < c.settings.length ; i++ ) {
                cgExpr(ctor_ctx, c.settings[i]);
                asm.I_pop();
            }

            // Eval super args, and call super ctor
            asm.I_getlocal(0);
            let nargs = c.superArgs.length;
            for ( let i=0 ; i < nargs ; i++ )
                cgExpr(ctor_ctx, c.superArgs[i]);
            asm.I_constructsuper(nargs);
        
            asm.I_popscope();
            asm.I_getlocal(0);
            asm.I_pushscope();  //'this'
            asm.I_pushscope();  //'activation'
        
            cgHead(ctor_ctx, f.vars);

            cgBlock(ctor_ctx, f.block);
        
            asm.I_kill(t);
        }

        return method.finalize();
    }

    function extractFormalTypes(ctx, f:Func) {
        let {emitter, script} = ctx;
        function extractType([name,fixture])
            emitter.fixtureTypeToType(fixture);
        
        let named_fixtures = extractUnNamedFixtures(f.params.fixtures);
        
        return map(extractType, named_fixtures);
    }
        
    function extractDefaultValues(ctx, f:Func) {
        let {emitter, script} = ctx;
        function extractDefaults(expr)
            emitter.defaultExpr(expr);

        return map(extractDefaults, f.defaults);
    }
    
    /* Create a method trait in the ABCFile
     * Generate code for the function
     * Return the function index
     */
    function cgFunc(ctx0, f:FUNC) {
        let {emitter, script, cp} = ctx0;
        let fntype = ctx0.stk != null && (ctx0.stk.tag == "instance" || ctx0.stk.tag == "class")? "method" : "function";  // brittle as hell
        let formals_types = extractFormalTypes({emitter:emitter, script:script}, f);
        let name = f.name ? f.name.ident : "";
        let method = new Method(emitter, formals_types, cp.stringUtf8(name), fntype != "function", f.attr);

        let defaults = extractDefaultValues({emitter:emitter, script:script}, f);
        if( defaults.length > 0 )
            method.setDefaults(defaults);
        
        if (!f.attr.is_native) {
            let asm = method.asm;

            /* Create a new rib and populate it with the values of all the
             * formals.  Add slot traits for all the formals so that the
             * rib have all the necessary names.  Later code generation
             * will add properties for all local (hoisted) VAR, CONST, and
             * FUNCTION bindings, and they will be added to the rib too,
             * but not initialized here.  (That may have to change, for
             * FUNCTION bindings at least.)
             *
             * FIXME: if a local VAR shadows a formal, there's more
             * elaborate behavior here, and the compiler must perform some
             * analysis and avoid the shadowed formal here.
             */
            let scope_reg = asm.getTemp();
            let capture_reg = 0;

            if (f.attr.capture_result)
                capture_reg = asm.getTemp();

            let fnctx = pushFunction(ctx0, fntype, scope_reg, capture_reg, (fntype != "function"), method);
            cgDebugFile(fnctx);

            asm.I_newactivation();
            asm.I_dup();
            asm.I_setlocal(scope_reg);
            asm.I_pushscope();

            setupArguments(fnctx, f);
            
            cgHead(fnctx, f.params);
            cgHead(fnctx, f.vars);

            /* Generate code for the body.  If there is no return statement in the
             * code then the default behavior of the emitter is to add a returnvoid
             * at the end, so there's nothing to worry about here.
             */
            cgBlock(fnctx, f.block);
            asm.killTemp(scope_reg);
            if (capture_reg) {
                asm.I_getlocal(capture_reg);
                asm.I_returnvalue();
            }
        }

        return method.finalize();
    }

    function setupArguments(ctx, f) {
        if (f.attr.uses_arguments) {
            // Create a fixture for "arguments", cgFixture ignores it if it's defined already.
            //
            // FIXME: what if the existing fixture is typed and won't admit the arguments object?
            // The rule for ES4 should probably be that if there is a typed "arguments" defined,
            // then no assignment will be generated here.
            //
            // Then initialize it.  It must be done first according to E262-3.

            cgFixtures(ctx, [[new PropName(new Ast::Name(Ast::noNS, "arguments")), 
                              new ValFixture(Ast::anyType, false)]]);
            cgExpr(ctx, new Ast::SetExpr(Ast::assignOp, 
                                         new Ast::LexicalRef(new Ast::Identifier("arguments",[[Ast::noNS]])), 
                                         new Ast::GetParam(f.numparams)));
            ctx.asm.I_pop();
        }
    }
    
    function cgHead(ctx, head) {
        let {asm, emitter, target} = ctx;
        
        function extractName([name,fixture])
            ctx.emitter.fixtureNameToName(name); //FIXME: shouldn't need ctx.
        
        function extractType([name,fixture])
            ctx.emitter.fixtureTypeToType(fixture); //FIXME: shouldn't need ctx.
        
        let named_fixtures = extractNamedFixtures(head.fixtures);
/*
        let formals = map(extractName, named_fixtures);
        let formals_type = map(extractType, named_fixtures);
        for ( let i=0 ; i < formals.length ; i++ ) {
            if(!hasTrait(target.traits, formals[i], TRAIT_Slot) )
                target.addTrait(new ABCSlotTrait(formals[i], 0, false, 0, formals_type[i]));
        }
*/
        cgFixtures(ctx, named_fixtures);
        for ( let i=0 ; i < head.exprs.length ; i++ ) {
            cgExpr(ctx, head.exprs[i]);
            asm.I_pop();
        }
    }

    function cgInits(ctx, inits, baseOnStk){
        let {asm, emitter} = ctx;

        let t = -1;
        let inits_length = inits?inits.length:0;
        for( let i=0; i < inits_length; ++i ) {
            let [name, init] = inits[i];

            let name_index = emitter.fixtureNameToName(name);

            if( baseOnStk ) {
                if(i < inits_length-1)
                    asm.I_dup();
            }
            else
                asm.I_findproperty(name_index);
            
            cgExpr(ctx, init);
            asm.I_setproperty(name_index);
        }
        if( inits_length == 0 && baseOnStk )
        {
            asm.I_pop();
        }
    }
    

    // Handles scopes and finally handlers and returns a label, if appropriate, to
    // branch to.  "tag" is one of "function", "break", "continue"

    function unstructuredControlFlow(ctx, hit, jump, msg) {
        let {stk, asm} = ctx;
        while (stk != null) {
            if (hit(stk)) {
                if (jump)
                    asm.I_jump(stk.branchTarget);
                return;
            }
            else {
                if(stk.scope_reg) {
                    asm.I_popscope();
                }
                if (stk.tag == "finally") {
                    /* The verifier can't deal with all these combinations, it appears to
                       be a limitation of how it does control flow analysis.  So throw
                       a SyntaxError here until the verifier can be fixed.
                    let myreturn = stk.nextReturn++;
                    asm.I_pushint(ctx.cp.int32(myreturn));
                    asm.I_setlocal(stk.returnreg);
                    asm.I_jump(stk.label);
                    stk.returnAddresses[myreturn] = asm.I_label(undefined);
                    */
                    Gen::internalError(ctx, "Limitation: Can't generate code for break/continue/return past 'finally' block.");
                }
            }
            stk = stk.link;
        }
        Gen::syntaxError(ctx, msg);
    }

    function restoreScopes(ctx) {
        let {stk, asm} = ctx;
        loop(stk);

        function loop(stk) {
            if (stk == null)
                return;
            if (stk.tag != "function")
                loop(stk.link);
            if (stk.push_this) {
                // function, script
                asm.I_getlocal(0);
                asm.I_pushscope();
            }
            if (stk.scope_reg) {
                asm.I_getlocal(stk.scope_reg);
                if (stk.tag == "with")
                    asm.I_pushwith();
                else
                    asm.I_pushscope();
            }
        }
    }

    // The following return extended contexts

    function pushProgram(script)
        new CTX( script.init.asm, 
                 { tag: "script", push_this: true, link: null }, 
                 script );

    function pushClass({asm, stk}, cls)
        new CTX( asm,
                 { tag:"class", link: stk }, 
                 cls );

    function pushCInit({asm, stk}, cinit)
        new CTX( asm,
                 { tag: "function", "type": "cinit", link: stk }, 
                 cinit );

    function pushInstance({asm, stk}, inst)
        new CTX( asm, 
                 { tag:"instance", target: inst, link: stk },
                 inst );

    function pushIInit({stk}, scope_reg, iinit)
        new CTX( iinit.asm,
                 { tag:"function", "type": "iinit", scope_reg: scope_reg, link: stk },
                 iinit );

    function pushFunction({stk}, function_type, scope_reg, capture_reg, push_this, func)
        new CTX( func.asm, 
                 { tag: "function", 
                   "type": function_type, 
                   scope_reg: scope_reg, 
                   capture_reg: capture_reg,
                   push_this: push_this,
                   link: stk },
                 func );

    function pushBreak({asm, stk, target}, branchTarget)
        new CTX( asm, 
                 { tag:"break", label: null, branchTarget: branchTarget, link: stk }, 
                 target );

    function pushLabel({asm, stk, target}, label, branchTarget)
        new CTX( asm, 
                 { tag:"break", label: label, branchTarget: branchTarget, link: stk }, 
                 target );

    function pushContinue({asm, stk, target}, labels, branchTarget)
        new CTX( asm, 
                 { tag:"continue", labels: labels, branchTarget: branchTarget, link: stk },
                 target );

    function pushWith({asm, stk, target}, scope_reg)
        new CTX( asm,
                 { tag:"with", scope_reg: scope_reg, link: stk },
                 target );

    function pushLet(ctx /*more*/) {
    }

    function pushCatch({asm, stk, target}, scope_reg )
        new CTX( asm, 
                 { tag: "catch", scope_reg: scope_reg, link: stk },
                 target );

    function pushFinally({asm, stk, target}, label, returnreg)
        new CTX( asm,
                 { tag: "finally", label: label, returnreg: returnreg, returnAddresses: new Array(), nextReturn: 0, link: stk },
                 target );
}
