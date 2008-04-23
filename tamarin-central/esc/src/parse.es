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

{
    use default namespace Parse;
    use namespace Lex;

    type PATTERNS = [PATTERN];
    type PATTERN =
          ( ObjectPattern
          | ArrayPattern
          | SimplePattern
          | IdentifierPattern );

    type FIELD_PATTERNS = [FIELD_PATTERN];
    type FIELD_PATTERN = FieldPattern;

    class FieldPattern {
        use default namespace public;
        const ident: Ast::IDENT_EXPR;
        const ptrn: PATTERN;
        function FieldPattern (ident,ptrn)
            : ident = ident
            , ptrn = ptrn { }
    }

    class ObjectPattern {
        const ptrns //: FIELD_PATTERNS;
        function ObjectPattern (ptrns)
            : ptrns = ptrns { }
    }

    class ArrayPattern { 
        const ptrns //: PATTERNS;
        function ArrayPattern (ptrns)
            : ptrns = ptrns { }
    }

    class SimplePattern {
        const expr : Ast::EXPR;
        function SimplePattern (expr)
            : expr = expr { }
    }

    class IdentifierPattern {
        const ident : Ast::IDENT;
        function IdentifierPattern (ident)
            : ident = ident { }
    }

    public class ENUM { 
        var s;
        public function ENUM(s) : s=s {}
        public function toString() 
            s;
    }

    class ALPHA extends ENUM { function ALPHA(s) : super(s) {} }
    const noColon = new ALPHA("noColon");
    const allowColon = new ALPHA("allowColon");

    class BETA extends ENUM { function BETA(s) : super(s) {} }
    const noIn = new BETA("noIn");
    const allowIn = new BETA("allowIn");

    class GAMMA extends ENUM { function GAMMA(s) : super(s) {} }
    const noExpr = new GAMMA("noExpr");
    const allowExpr = new GAMMA("allowExpr");

    class TAU extends ENUM { function TAU(s) : super(s) {} }
    const globalBlk = new TAU("globalBlk");
    const classBlk = new TAU("classBlk");
    const interfaceBlk = new TAU("interfaceBlk");
    const localBlk = new TAU("localBlk");

    class OMEGA extends ENUM { function OMEGA(s) : super(s) {} }
    const fullStmt = new OMEGA("fullStmt");
    const abbrevStmt = new OMEGA("abbrevStmt");

    type ENV = [Ast::FIXTURES];

    type PRAGMAS = Pragmas;
    class Pragmas 
    {
        use default namespace public;
        var openNamespaces //: [[Ast::NAMESPACE]];
        var defaultNamespace: Ast::NAMESPACE;
        function Pragmas (pragmas) 
        {
            if (pragmas==null)
            {
                this.openNamespaces = [[]];
                this.defaultNamespace = Ast::noNS;  // same as new Ast::PublicNamespace ("");
            }
            else
            {
                this.openNamespaces = Util::copyArray (pragmas.openNamespaces);
                this.defaultNamespace = pragmas.defaultNamespace;
            }

            if (this.openNamespaces[this.openNamespaces.length-1].length !== 0) { 
                this.openNamespaces.push([]);  // otherwise reuse the last one pushed
            }
        }
    }

    type PRAGMA_ENV = [Ast::PRAGMAS];

    Parse function syntaxError(ctx, msg) {
        let filename = "";
        let position = 0;
        if (ctx is Context)
            ctx = ctx.parser;
        if (ctx is Parser) {
            filename = ctx.scan.filename;
            position = ctx.position();
        }
        Util::syntaxError(filename, position, msg);
    }

    Parse function internalError(ctx, msg) {
        let filename = "";
        let position = 0;
        if (ctx is Context)
            ctx = ctx.parser;
        if (ctx is Parser) {
            filename = ctx.scan.filename;
            position = ctx.position();
        }
        Util::internalError(filename, position, msg);
    }

    class Context
    {
        use default namespace public;
        var env: ENV;
        var varHeads: [Ast::HEAD];
        var letHeads: [Ast::HEAD];
        var ctor: Ast::CTOR;
        var pragmas: PRAGMAS
        var pragmaEnv: PRAGMA_ENV; // push one PRAGMAS for each scope
        var top_function: Ast::FuncAttr;
        var parser;

        function Context (topFixtures, parser)
            : env = [topFixtures]
            , varHeads = []
            , letHeads = [] 
            , ctor = null
            , pragmas = null
            , pragmaEnv = []
            , parser = parser
        {
            //print ("topFixtures.length=",topFixtures.length);
            //            print ("env[0].length=",env[0].length);
            top_function = new Ast::FuncAttr(null);  // global context
        }

        function enterVarBlock () 
        {
            use namespace Ast;
            let varHead = new Ast::Head ([],[]);
            this.varHeads.push(varHead);
            this.env.push (varHead.fixtures);
            this.pragmas = new Pragmas (this.pragmas);
            this.pragmaEnv.push (this.pragmas);
        }

        function exitVarBlock () 
        {
            let varHead = this.varHeads.pop ();
            this.env.pop ();
            this.pragmaEnv.pop ();
            if (this.pragmaEnv.length === 0) {
                this.pragmas = null;
            }
            else {
                this.pragmas = this.pragmaEnv[this.pragmaEnv.length-1];
            }
            return varHead;
        }

        function enterFunction(attrs: ATTRS) {
            use namespace Ast; // language bug, needs fixing
            let node = new FuncAttr(top_function);
            node.is_native = attrs.native;
            top_function.children.push(node);
            top_function = node;
            return node;
        }

        function topFunction() {
            return top_function;
        }

        function exitFunction() {
            use namespace Ast; // language bug, needs fixing
            let top = top_function;
            top_function = top_function.parent;
            return top;
        }

        function hasFixture (fxtrs,fb) {
            use namespace Ast;
            let [fn,f1] = fb;
            switch type (fn) {
            case (fn: Ast::PropName) {
                if (hasName (fxtrs,fn.name.id,fn.name.ns)) {
                    //print("hasName ",ns,"::",id);
                    let f2 = getFixture (fxtrs,id,ns);
                    if (f1 is Ast::ValFixture && f2 is Ast::ValFixture) {
                        if (Ast::isAnyType(f1.Ast::type)) 
                            return true;
                        if (Ast::isAnyType(f2.Ast::type))
                            return true;
                        // other positive cases here
                    }
                    Parse::syntaxError(this, "Incompatible fixture redefinition "+fn.id);
                }
            }
            case (fn: Ast::TempName) {
                return false;  // for now
            }
            }
        }

        function addVarFixtures (fxtrs, isStatic=false) 
        {
            use namespace Ast;
            let varHead = this.varHeads[this.varHeads.length-(isStatic?2:1)];
            for (let n = 0, len = fxtrs.length; n < len; ++n)  // until array conact works
            {
                let fb = fxtrs[n];
                /// if (!hasFixture (varHead.Ast::fixtures,fb)) {
                    varHead.fixtures.push (fxtrs[n]);
                /// }
            }
        }

        function addVarInits (inits, isStatic=false) 
        {
            use namespace Ast;
            let varHead = this.varHeads[this.varHeads.length-(isStatic?2:1)];
            for (let n = 0, len = inits.length; n < len; ++n)  // until array conact works
                varHead.exprs.push (inits[n]);
        }

        function enterLetBlock () 
        {
            use namespace Ast;
            let letHead = new Ast::Head ([],[]);
            this.letHeads.push(letHead);
            this.env.push (letHead.fixtures);
            this.pragmas = new Pragmas (this.pragmas);
            this.pragmaEnv.push (this.pragmas);
        }

        function exitLetBlock () 
        {
            let letHead = this.letHeads.pop ();
            this.env.pop ();
            this.pragmaEnv.pop ();
            this.pragmas = this.pragmaEnv[this.pragmaEnv.length-1];
            return letHead;
        }

        function addLetFixtures (fxtrs) 
        {
            use namespace Ast;
            let letHead = this.letHeads[this.letHeads.length-1];
            for (let n = 0, len = fxtrs.length; n < len; ++n)  // until array conact works
                letHead.fixtures.push (fxtrs[n]);
        }

        function addLetInits (inits) 
        {
            use namespace Ast;
            let letHead = this.letHeads[this.letHeads.length-1];
            for (let n = 0, len = inits.length; n < len; ++n)  // until array conact works
                letHead.exprs.push (inits[n]);
        }

        function openNamespace (ident: Ast::IDENT_EXPR) {
            pushNamespace( evalIdentExprToNamespace (ident) );
        }

        function pushNamespace(ns) {
            let opennss = this.pragmas.openNamespaces;
            opennss[opennss.length-1].push (ns);
        }

        function defaultNamespace (ident: Ast::IDENT_EXPR) {
            let ns = evalIdentExprToNamespace (ident);
            this.pragmas.defaultNamespace = ns;
        }

        function hasName (fxtrs,id,ns) 
        {
            use namespace Ast;

            if (fxtrs.length==0)
                return false;

            let pn = fxtrs[0][0];
            if (pn.name.id==id && pn.name.ns.hash()==ns.hash())  // FIXME: need ns compare
                return true;
            return hasName (fxtrs.slice(1,fxtrs.length), id, ns);
        }

        function getFixture (fxtrs,id,ns) 
        {
            use namespace Ast;

            if (fxtrs.length===0) 
                Parse::syntaxError(this, "Name not found " + ns + "::" + id);

            let pn = fxtrs[0][0];
            if (pn.name.id==id && pn.name.ns.toString()==ns.toString()) 
                return fxtrs[0];
            return getFixture (fxtrs.slice (1,fxtrs.length),id,ns);
        }

        /*

        two dimensional search

        repeat for each shadowed name
            each name in each head
                dup is error

        for each namespace set
            find all names in the inner most head

        */

        function findFixtureWithNames (id, nss, it: Ast::INIT_TARGET?) {
            let env = this.env;
            let start, end;

            switch (it) {
            case Ast::instanceInit:
                start = env.length-2;
                stop = start;
                break;
            case null:
                start = env.length-1;
                stop = 0;
                break;
            default:
                Parse::internalError(this, "findFixtureWithName: unimplemented target " + it);
            }

            for (let i=start; i>=stop; --i) {   // for each head
                let ns = null;
                let fxtrs = env[i];
                for (let j=nss.length-1; j>=0; --j) {
                    if (hasName (fxtrs,id,nss[j])) {
                        if (ns !== null)
                            Parse::syntaxError(this, "Ambiguous reference to " + id);
                        ns = nss[j];
                    }
                }
                if (ns!==null) {
                    return getFixture (fxtrs,id,ns);
                }
            }

            return null;
        }

        function findFixtureWithIdentifier (id: Ast::IDENT, it: Ast::INIT_TARGET?) {
            //print ("this.pragmas=",this.pragmas);
            let nsss = this.pragmas.openNamespaces;
            //print ("nsss.length=",nsss.length);
            for (let i=nsss.length-1; i>=0; --i) {
                //print ("nsss[",i,"]=",nsss[i]);
                let fx = findFixtureWithNames (id, nsss[i], it);
                if (fx !== null) 
                    return fx;
            }
            /*
            for ( let i=0 ; i < this.env.length ; i++ )
                print("E [" + this.env[i] + "]");
            for ( let i=0 ; i < this.pragmas.openNamespaces.length ; i++ )
                print("N [" + this.pragmas.openNamespaces[i] + "]");
            print(it);
            */
            Parse::syntaxError(this, "Fixture not found: " + id);
        }

        function evalIdentExprToNamespace (ident: Ast::IDENT_EXPR) : Ast::NAMESPACE {
            use namespace Ast;

            let fxtr = null;
            let val = null;

            switch type (ident) {
            case (id: Identifier) {
                fxtr = findFixtureWithIdentifier (id.ident,null);
                if (fxtr[1] is NamespaceFixture)
                    return fxtr[1].ns;
                Parse::internalError(this, "Fixture with unknown value " + fxtr[1]);
            }
            case (reserved_ns: ReservedNamespace) {
                // this goes away soon, probably?
                return reserved_ns.ns;
            }
            case (x: *) {
                Parse::internalError(this, "evalIdentExprToNamespace: case not implemented " + x);
            }
            }
        }

        function resolveIdentExpr (ident: Ast::IDENT_EXPR, it: Ast::INIT_TARGET) : Ast::FIXTURE_NAME {
            if (ident is Ast::Identifier)
                return findFixtureWithIdentifier (ident.Ast::ident, it)[0];

            Parse::internalError(this, "resolveIdentExpr: case not implemented " + ident);
        }
    }

    class Parser
    {
        var scan    : Scanner;
        var cx      : Context;
        var filename: String;

        function Parser(src,topFixtures,filename="") {
            this.cx = new Context (topFixtures, this);
            this.scan = new Scanner (src,filename);
            this.filename = filename;
        }

        var defaultNamespace: Ast::NAMESPACE;
        var currentPackageName: string;
        var currentClassName: string;

        // Token queue abstractions
        // BEGIN

        // Token queue
        //
        // T0 is the current token, L0 its line number
        // T1 is the next token, L1 its line number
        // LP is the line number of the previous token
        //
        // The line number of a token is the 1-based line number of
        // the last character of the token.
        //
        // Invariants:
        //   T0 is NONE only before start() and when the stream is exhausted.
        //   T1 is NONE whenever it's invalid
        //   L1 is invalid iff T1 is invalid
        //   LP, L0, and L1 are 0 if there is "no information"
        //   The scanner never returns Eol
        //
        // Every token passes through this interface so we try to save time
        // when possible.
        //
        // Current optimizations:
        //   * avoid function calls
        //     - scan.start() is shallow
        //     - Token::tokenKind and Token::tokenText have been in-lined
        //   * avoid allocations
        //     - Communicate with lexer via multiple variables rather than 
        //       using multiple return values
        //
        // Future optimizations (maybe):
        //   - With static typing to "uint" it may be possible to avoid write 
        //     barriers on assignments to T0, T1, LP, L0, and L1
        //   - With tokenStore typed as Vector.<uint> it may be possible to
        //     avoid range checks and checks for holes
        //
        // What we need the compiler to do:
        //   - Early binding to instance vars and methods is important
        //   - We would probably benefit from the compiler inlining hd
        //     everywhere; we can declare it "final" or just make Parser
        //     itself "final"

        var T0=Token::NONE, T1=Token::NONE, LP=0, L0=0, L1=0;
        const tokenStore = Token::tokenStore;

        function start() {
            T0 = scan.start();
            L0 = scan.lnCoord;
        }

        function div() {
            Util::assert( T0 === Token::BREAK && T1 === Token::NONE );
            T0 = scan.div();
            L0 = scan.lnCoord;
            return hd();
        }

        function regexp() {
            Util::assert( T0 === Token::BREAK && T1 === Token::NONE );
            T0 = scan.regexp();
            L0 = scan.lnCoord;
            return hd();
        }

        function hd ()
            tokenStore[T0].Token::kind;

        function lexeme ()
            tokenStore[T0].Token::text;

        function hd2 () {
            if (T1 === Token::NONE) {
                T1 = scan.start();
                L1 = scan.lnCoord;
            }
            return tokenStore[T1].Token::kind;
        }

        function lexeme2 () {
            hd2 ();
            return tokenStore[T1].Token::text;
        }

        function next () {
            LP = L0;
            T0 = T1;
            L0 = L1;
            T1 = Token::NONE;
            if (T0 === Token::NONE) {
                T0 = scan.start();
                L0 = scan.lnCoord;
            }
        }

        function replace (t) {
            T0 = t;
        }

        function newline ()
            LP < L0;

        function position ()
            L0;

        function tokenText(t)
            tokenStore[t].Token::text;

        // Token queue abstractions
        // END

        // If the current token is not tc then throw an error.
        // Otherwise consume the token.

        function eat (tc) {
            let tk = hd ();
            if (tk !== tc)
                Parse::syntaxError(this, "Expecting " + tokenText(tc) + " found " + tokenText(tk));
            next ();
        }

        function match (tc) {
            let tk = hd ();
            if (tk !== tc)
                return false;
            next ();
            return true;
        }

        function makeIdentifier(id, nss) : Ast::Identifier {
            let ident = new Ast::Identifier(id, nss);
            if (id === "arguments") 
                cx.topFunction().uses_arguments = true;
            else if (id === "eval")
                cx.topFunction().uses_eval = true;
            return ident;
        }

        /*

        Notation

        []             list
        (fl,el)        head
        fl             fixture list
        el             expr list
        il             init list
        sl             stmt list
        it             init target = VAR, LET (default=LET)
        ie             init expr
        se             set expr

        initexpr       init it (fl,el) il
        letexpr        let (fl,el) el
        block          (fl,el) sl

      

        Bindings

        var x = y      [x], init VAR () [x=y]
        var [x] = y    [x], init VAR ([t0],[init t0=y]) [x=t0[0]]

        let (x=y) ...  let ([x], init x=y) ...
        let x=y             [x], init x=y]

        Assignments

        x = y          [],  set x=y
        [x] = y        [],  let ([t0],[init t0=y]) [set x=t0[0]]

        Blocks

        { }            () {}
        {stmt}         () {stmt}
        {let x}        ([x],[x=undef]) {}       is undef the right val?
        let (x) {}     ([x],[x=undef]) {}       what about reping uninit?

        Mixture

        { var x = y; let z = y }  =>
            ([x],[]) { blkstmt ([z],[]) { init VAR () x=y; init LET () z=y } }


        assignment, create a let for each aggregate, a temp for
        each level of nesting

        x = y              set x=y
        [x] = y            let (t0=y) set x=t0[0]
        [[x]] = y          let (t0=y) let (t1=t0[0]) set x=t1[0]
        [[x],[x]] = y      let (t0=y) let (t1=t0[0]) set x=t1[0]
                                    , let (t1=t0[1]) set x=t1[0]
        
        initialization, create an init rather than a set for the
        leaf nodes

        var x = v            let (t0=v) init () [x=t0]
        var [x] = v          let (t0=v) init () [x=t0[0]]
        var [x,[y,z]] = v    let (t0=v) init () [x=t0[0]]
                                      , let (t1=t0[1]) init () [y=t1[0], z=t1[1]]
        var [x,[y,[z]]] = v  let (t0=v) init () [x=t0[0]]
                                      , let (t1=t0[1]) init () [y=t1[0]
                                                     , let (t2=t1[0]) init () [z=t2[0]]

        for initialization, we need to know the namespace and the target 
        so we make INITS to go into the InitExpr inside the LetExpr

        let x = y          init x=y

        flattening.

        var [x,[y,z]] = v  let (t0=v) init () [x=t0[0]]
                                    , let (t1=t0[1]) init () [y=t1[0], z=t1[0]]

                           t0=v
                           x=t0[0]
                           t1=t0[1]
                           y=t1[0]
                           z=t1[1]
        head = {[t0,x,t1,y,z],

        flattening doesn't work because it mixes named and temporary
        fixtures

        lets and params have the same problem. both allow destructuring
        patterns that can expand into a nested expression.

        let ([x,[y,z]]=v) ...

        top heads only have named fixtures. sub heads only have temporaries.
        temporaries are always immediately initialized. a head is a list of
        fixtures and a list of expressions. the expressions get evaluated
        in the scope outside the head.

        settings is a sub head. it has temporary fixtures and init exprs that
        target instance variables

        */

        function desugarAssignmentPattern (p: PATTERN, t: Ast::TYPE_EXPR, e: Ast::EXPR, op: Ast::ASSIGNOP) : [Ast::FIXTURES, Ast::EXPR]
            desugarPattern (p,t,e,null,null,null,op);

        function desugarBindingPattern (p: PATTERN, t: Ast::TYPE_EXPR, e: Ast::EXPR?,
                                        ns: Ast::NAMESPACE?, it: Ast::INIT_TARGET?, ro: boolean?)
            : [Ast::FIXTURES, Ast::EXPR]
            desugarPattern (p,t,e,ns,it,ro,null);

        function desugarPattern (p: PATTERN, 
                                 t: Ast::TYPE_EXPR, 
                                 e: Ast::EXPR?,
                                 ns: Ast::NAMESPACE?, 
                                 it: Ast::INIT_TARGET?, 
                                 ro: boolean?, 
                                 op: Ast::ASSIGNOP?) : [Ast::FIXTURES, Ast::EXPR] 
        {
            return desugarSubPattern (p,t,e,0);

            function identExprFromExpr (e: Ast::EXPR) : Ast::IDENT_EXPR {
                if (!(e is Ast::LexicalRef))
                    Parse::syntaxError(this, "Invalid initializer left-hand-side " + e);
                return ie = e.Ast::ident;
            }

            function desugarSubPattern (p: PATTERN, t: Ast::TYPE_EXPR, e: Ast::EXPR?, n: int) : [Ast::FIXTURES, Ast::EXPR] {
                let fxtrs, exprs, expr;
                switch type (p) {
                case (p:IdentifierPattern) {
                    let nm = new Ast::PropName (new Ast::Name(ns,p.ident));
                    let fx = new Ast::ValFixture (t,ro);
                    fxtrs = [[nm,fx]];
                    let inits = (e !== null) ? ([[nm,e]]) : []; // FIXME
                    expr = new Ast::InitExpr (it, new Ast::Head ([],[]), inits);
                }
                case (p:SimplePattern) {
                    if (e === null) 
                        Parse::syntaxError(this, "Simple pattern without initializer");
                    fxtrs = [];
                    if (it != null) { // we have an init target so must be an init
                        let ie = identExprFromExpr (p.expr);
                        let nm = cx.resolveIdentExpr (ie,it);
                        expr = new Ast::InitExpr (it, new Ast::Head ([],[]), [[nm,e]]);
                    }
                    else {
                        expr = new Ast::SetExpr (op,p.expr,e);
                    }
                }
                //case (p: (ArrayPattern|ObjectPattern)) {
                case (p: *) {
                    let tn = new Ast::TempName (n);
                    fxtrs = [];
                    exprs = [];
                    let ptrns = p.ptrns;
                    for (let i=0; i<ptrns.length; ++i) {
                        let sub = ptrns[i];
                        let typ, exp, ptn;
                        /// switch type (sub) {
                        /// case (sub: FieldPattern) {
                        if (sub is FieldPattern) {
                            typ = new Ast::FieldTypeRef (t,sub.ident);
                            exp = new Ast::ObjectRef (new Ast::GetTemp (n), sub.ident);
                            ptn = sub.ptrn;
                        }
                        /// case (pat: *) {
                        else {
                            typ = new Ast::ElementTypeRef (t,i);
                            exp = new Ast::ObjectRef (new Ast::GetTemp (n), makeIdentifier (i,[[Ast::noNS]]));
                                      // FIXME what is the ns of a temp and how do we refer it
                            ptn = sub;
                        }
                        /// }

                        let [fx,ex] = desugarSubPattern (ptn,typ,exp,n+1);
                        for (let j=0; j<fx.length; ++j) fxtrs.push(fx[j]);
                        exprs.push(ex);
                    }
                    let head = new Ast::Head ([[tn,new Ast::ValFixture (Ast::anyType,false)]],
                                              [new Ast::InitExpr (Ast::letInit,new Ast::Head([],[]),[[tn,e]])]);
                    expr = new Ast::LetExpr (head, exprListToCommaExpr(exprs));
                }
                }
                return [fxtrs,expr];
            }
        }

        // Parse rountines

        public function identifier () : Ast::IDENT {
            let t = hd();
            if (t == Token::Identifier || Token::isCtxReserved(t))
                return let (str = lexeme()) next(), str;
            Parse::syntaxError(this, "Expecting identifier, found " + lexeme () );
        }

        function qualifier() : (Ast::IDENT | Ast::NAMESPACE) {
            switch (hd()) {
            case Token::Internal:
            case Token::Intrinsic:
            case Token::Private:
            case Token::Protected:
            case Token::Public:
                return reservedNamespace();
            case Token::Mult:
                return let (qual = lexeme ()) next(), qual;
            default:
                return identifier ();
            }
        }

        function reservedNamespace () : Ast::NAMESPACE {
            switch (hd ()) {
            case Token::Internal:  return let (ns = new Ast::InternalNamespace (currentPackageName)) next(), ns;
            case Token::Public:    return let (ns = new Ast::PublicNamespace (currentPackageName)) next(), ns;
            case Token::Intrinsic: return let (ns = new Ast::IntrinsicNamespace) next(), ns;
            case Token::Private:   return let (ns = new Ast::PrivateNamespace (currentClassName)) next(), ns;
            case Token::Protected: return let (ns = new Ast::ProtectedNamespace (currentClassName)) next(), ns;
            default:               return null;
            }
        }

        function qualifiedNameIdentifier (qual: Ast::EXPR) : Ast::IDENT_EXPR {
            switch (hd()) {
            case Token::LeftBracket: return new Ast::QualifiedExpression(qual, brackets());
            case Token::Mult:        next(); return new Ast::QualifiedIdentifier(qual, "*"); // Iffy, E4X
            default:                 return new Ast::QualifiedIdentifier(qual, identifier());
            }
        }

        function brackets() {
            eat(Token::LeftBracket);
            let expr = commaExpression (allowColon, allowIn);
            eat (Token::RightBracket);
            return expr;
        }

        function simpleQualifiedName () : Ast::IDENT_EXPR {
            let qual = qualifier ();

            if (match(Token::DoubleColon)) {
                if (qual is String)
                    qual = new Ast::LexicalRef (makeIdentifier (qual,cx.pragmas.openNamespaces));
                else
                    qual = new Ast::LiteralNamespace (qual, position());
                return qualifiedNameIdentifier (qual);
            }

            if (qual is String)
                return makeIdentifier (qual, cx.pragmas.openNamespaces);

            return new Ast::ReservedNamespace (qual);
        }

        function nonAttributeQualifiedName () : Ast::IDENT_EXPR 
            hd() == Token::LeftParen ? expressionQualifiedIdentifier() : simpleQualifiedName();

        function propertyName () : Ast::IDENT_EXPR {
            let name = nonAttributeQualifiedName ();
            if (match(Token::LeftDotAngle)) {
                Parse::internalError(this, "Broken parser rules");
                name = typeExpressionList ();
                switch (hd ()) {
                case Token::UnsignedRightShift:
                    // downgrade >>> to >> to eat one >
                    replace (Token::RightShift);
                    break;
                case Token::RightShift:
                    // downgrade >> to > to eat one >
                    replace (Token::GreaterThan);
                    break;
                default:
                    eat (Token::GreaterThan);
                    break;
                }
            }

            return name;
        }

        function primaryName () : Ast::IDENT_EXPR {
            if (hd() === Token::Identifier && hd2() === Token::Dot) {
                let path_element = lexeme ();
                next();
                next();
                let path_elements = path ([path_element]);
                let name = propertyName ();
                return new Ast::UnresolvedPath (path_elements, name);
            }
            return propertyName ();
        }

        function path (path_elements: [Ast::IDENT]) : [Ast::IDENT] {
            while (hd() === Token::Identifier && hd2() === Token::Dot) {
                path_elements.push( lexeme() );
                next();
                next();
            }
            return path_elements;
        }

        function parenExpression () : Ast::EXPR {
            eat (Token::LeftParen);
            let expr = commaExpression (allowColon, allowIn);
            eat (Token::RightParen);

            return expr;
        }

        function exprListToCommaExpr(es) {
            let expr = es[0];
            for ( let i=1 ; i < es.length ; i++ )
                expr = new Ast::BinaryExpr(Ast::commaOp, expr, es[i]);
            return expr;
        }

        function objectLiteral (alpha: ALPHA) : Ast::TYPE_EXPR {
            let pos = position();             // Record source location of initial left brace
            eat (Token::LeftBrace);
            let fields = fieldList (alpha);
            eat (Token::RightBrace);

            let texpr;
            switch (alpha) {
            case allowColon:
                texpr = match(Token::Colon) ? typeExpression () : new Ast::ObjectType ([]); // FIXME I mean {*}
                break;
            default:
                texpr = new Ast::ObjectType ([]); // FIXME I mean {*}
                break;
            }

            return new Ast::LiteralObject (fields, texpr, pos);
        }

        function fieldList (alpha: ALPHA) : LITERAL_FIELDS {
            let fields = [] : [LITERAL_FIELD];
            if (hd () != Token::RightBrace) {
                do 
                    fields.push (literalField(alpha));
                while (match(Token::Comma));
            }
            return fields;
        }

        function literalField (alpha: ALPHA) : Ast::FIELD_TYPE {
            let tag = match(Token::Const) ? Ast::constTag : Ast::varTag;
            let fn = fieldName ();
            let expr = null;

            // The colon could be absent if this is a destructuring
            // shorthand, but we can't expand it here -- that must be
            // done later, and only in legal contexts.

            if (match(Token::Colon)) {
                switch (hd ()) {
                case Token::LeftBrace:   // short cut to avoid recursion
                    expr = objectLiteral (alpha);
                    break;
                case Token::LeftBracket:
                    expr = arrayLiteral (alpha);
                    break;
                default:
                    expr = assignmentExpression (allowColon, allowIn);
                    break;
                }
            }

            return new Ast::LiteralField (tag,fn,expr);
        }

        function fieldName () : Ast::IDENT_EXPR {
            switch (hd ()) {
            case Token::StringLiteral:
                return let (name = new Ast::Identifier(lexeme(), [[Ast::noNS]])) next(), name;

            case Token::IntLiteral:
            case Token::UIntLiteral:
                return let (name = new Ast::Identifier(String(parseInt(lexeme())), [[Ast::noNS]])) next(), name;

            case Token::DoubleLiteral:
                return let (name = new Ast::Identifier(String(parseFloat(lexeme())), [[Ast::noNS]])) next(), name;

            case Token::DecimalLiteral:
                return let (name = new Ast::Identifier(String(new decimal(lexeme())), [[Ast::noNS]])) next(), name;

            default:
                // FIXME: support ns::id here
                if (Token::isReserved(hd ()))
                    return let (name = new Ast::Identifier(lexeme(), [[Ast::noNS]])) next(), name;
                return nonAttributeQualifiedName ();
            }
        }

        function arrayLiteral (alpha: ALPHA) : Ast::EXPR {
            let pos = position();   // Record source location of initial left bracket
            eat (Token::LeftBracket);
            let elts = elementList ();
            if (elts.length == 1 && hd() == Token::For) {
                // FIXME
                Parse::internalError(this, "Array comprehension not implemented");
            }
            else {
                eat (Token::RightBracket);
                let t = null;
                if (alpha == allowColon && match(Token::Colon)) {
                    t = typeExpression();
                    if (!(t is Ast::ArrayType))
                        Parse::syntaxError(this, "Array type required");
                }
                else
                    t = new Ast::ArrayType([]);
                return new Ast::LiteralArray (elts, t, pos);
            }
        }

        function elementList () : Ast::EXPR {
            let elts = [];

            let elt = null;
        loop:
            for (;;) {
                switch (hd()) {
                case Token::RightBracket:
                case Token::For:
                    break loop;
                case Token::Comma:
                    let pos = position();
                    next();
                    if (elt == null)
                        elt = new Ast::LiteralUndefined(pos);
                    elts.push(elt);
                    elt = null;
                    break;
                default:
                    if (elt != null)
                        eat(Token::Comma);
                    elt = assignmentExpression(allowColon, allowIn);
                    break;
                }
            }
            if (elt != null)
                elts.push(elt);

            return elts;
        }

        function primaryExpression(alpha: ALPHA, beta:BETA) : Ast::EXPR {
            if (hd() == Token::BREAK)
                regexp();

            let pos = position();   // Record the source location before consuming the token
            switch (hd ()) {
            case Token::Null:
                return let (expr = new Ast::LiteralNull(pos)) next(), expr;

            case Token::True:
            case Token::False:
                return let (expr = new Ast::LiteralBoolean(hd() == Token::True, pos)) next(), expr;

            case Token::IntLiteral:
                return let (expr = new Ast::LiteralInt(parseInt(lexeme()), pos)) next(), expr;

            case Token::UIntLiteral:
                return let (expr = new Ast::LiteralUInt(parseInt(lexeme()), pos)) next(), expr;

            case Token::DoubleLiteral:
                return let (expr = new Ast::LiteralDouble(parseFloat(lexeme()), pos)) next(), expr;

            case Token::DecimalLiteral:
                return let (expr = new Ast::LiteralDecimal(new decimal(lexeme()), pos)) next(), expr;

            case Token::StringLiteral:
                return let (expr = new Ast::LiteralString (lexeme(), pos)) next(), expr;

            case Token::RegexpLiteral:
                return let (expr = new Ast::LiteralRegExp(lexeme(), pos)) next(), expr;

            case Token::This:
                return let (expr = new Ast::ThisExpr()) next(), expr;

            case Token::LeftParen:
                return parenExpression();

            case Token::LeftBracket:
                return arrayLiteral (alpha);

            case Token::LeftBrace:
                return objectLiteral (alpha);

            case Token::Function:
                return functionExpression ();

            default: {
                // FIXME: Can't call it expr here, compiler bug (has to do with let renaming?)
                let expr77 = primaryName ();
                // FIXME: not good for package qualified refs
                if (expr77 is Ast::UnresolvedPath)
                    return new Ast::ObjectRef (resolvePath (expr77.Ast::path, null), expr77.Ast::ident); 
                return new Ast::LexicalRef (expr77, pos);
            }
            }
        }

        function resolvePath (path: [Ast::IDENT], expr: Ast::EXPR) : Ast::EXPR {
            if (path.length === 0)
                return expr;

            let base;
            if (expr === null) 
                base = new Ast::LexicalRef (makeIdentifier (path[0],cx.pragmas.openNamespaces));
            else 
                base = new Ast::ObjectRef (expr, makeIdentifier (path[0],cx.pragmas.openNamespaces));
            return resolvePath (path.slice (1, path.length), base);
        }

        function superExpression(alpha: ALPHA) {
            eat(Token::Super);
            return new Ast::SuperExpr(null);
        }

        function propertyOperator (obj: Ast::EXPR) : [Ast::EXPR] {
            switch (hd ()) {
            case Token::Dot:
                switch (hd2 ()) {
                case Token::LeftParen:
                    Parse::internalError(this, "Filter operator not implemented");

                default:
                    eat(Token::Dot);
                    return new Ast::ObjectRef (obj, propertyName ());
                }
            case Token::LeftBracket:
                return new Ast::ObjectRef (obj, new Ast::ExpressionIdentifier (brackets(), cx.pragmas.openNamespaces));

            case Token::DoubleDot:
                Parse::internalError(this, "descendents operator not implemented");

            default:
                Parse::internalError("propertyOperator: " + hd());
            }
        }

        function argumentList () {
            let args = [];

            eat(Token::LeftParen);
            if (hd() != Token::RightParen) {
                do 
                    args.push(assignmentExpression(allowColon, allowIn));
                while (match(Token::Comma));
            }
            eat(Token::RightParen);

            return args;
        }

        /*

        MemberExpression(beta)
            PrimaryExpression(beta)
            new  MemberExpression(beta)  Arguments
            SuperExpression  PropertyOperator
            MemberExpression(beta)  PropertyOperator

        Refactored:

        MemberExpression(beta)
            PrimaryExpression(beta) MemberExpressionPrime(beta)
            new MemberExpression(beta) Arguments MemberExpressionPrime(beta)
            SuperExpression  PropertyOperator  MemberExpressionPrime(beta)

        MemberExpressionPrime(beta)
            PropertyOperator MemberExpressionPrime(beta)
            empty

        Note: member expressions always have balanced new and (). The LHS parser is
        responsible for dispatching extra 'new' or '()' to 

        */

        function memberExpression (alpha:ALPHA, beta:BETA) : Ast::EXPR {
            switch (hd ()) {
            case Token::New:
                next();
                let object_expr = memberExpression (alpha, beta);
                let argument_exprs = argumentList ();
                return memberExpressionPrime (alpha, beta, new Ast::NewExpr (object_expr,argument_exprs));
            case Token::Super:
                let super_expr = superExpression (alpha);
                let property_expr = propertyOperator (super_expr);
                return memberExpressionPrime (alpha, beta, property_expr);
            default:
                let expr = primaryExpression (alpha, beta);
                return memberExpressionPrime (alpha, beta, expr);
            }
        }

        function memberExpressionPrime (alpha: ALPHA, beta:BETA, expr: Ast::EXPR) : Ast::EXPR {
            switch (hd ()) {
            case Token::LeftBracket:
            case Token::Dot:
            case Token::DoubleDot:
                return memberExpressionPrime (alpha, beta, propertyOperator (expr));
            default:
                return expr;
            }
        }

        function callExpression (alpha:ALPHA, beta:BETA) : Ast::EXPR {
            let pos = position();
            let object_expr = memberExpression (alpha, beta);
            let argument_exprs = argumentList ();

            return callExpressionPrime (beta, new Ast::CallExpr (object_expr,argument_exprs,pos));
        }

        // shared among many
        function callExpressionPrime (beta:BETA, call_expr: Ast::EXPR) : Ast::EXPR {
            switch (hd ()) {
            case Token::LeftParen:
                let pos = position();
                let argument_exprs = argumentList ();
                return callExpressionPrime (beta, new Ast::CallExpr (call_expr, argument_exprs, pos));
            case Token::LeftBracket:
            case Token::Dot:
            case Token::DoubleDot:
                return callExpressionPrime (beta, propertyOperator (call_expr));
            default:
                return call_expr;
            }
        }

        function newExpression (alpha:ALPHA, beta:BETA, new_count=0) : Ast::EXPR {
            let call_expression, is_new;

            if (is_new = match(Token::New))
                call_expression = newExpression (alpha, beta, new_count+1);
            else
                call_expression = memberExpression(alpha, beta);

            if (hd() == Token::LeftParen) {   // No more new exprs so this paren must start a call expr
                let pos = position();
                let argument_exprs = argumentList ();
                if (new_count > 0)
                    return new Ast::NewExpr (call_expression, argument_exprs);
                return callExpressionPrime (beta, new Ast::CallExpr (call_expression, argument_exprs, pos));
            }

            if (new_count > 0)
                return new Ast::NewExpr (call_expression, []);

            if (is_new)
                return memberExpressionPrime (alpha, beta, call_expression);
            return call_expression;
        }

        function leftHandSideExpression (alpha: ALPHA, beta:BETA) : Ast::EXPR {
            let operator = (hd() == Token::New) ? newExpression (alpha, beta, 0) : memberExpression (alpha, beta);
            if (hd () == Token::LeftParen) {
                let pos = position();
                let args = argumentList ();
                return callExpressionPrime (beta, new Ast::CallExpr (operator, args, pos));
            }
            return operator;
        }

        function postfixExpression (alpha: ALPHA, beta:BETA) : Ast::EXPR {
            let expr = leftHandSideExpression (alpha, beta);
            if (noNewline()) {
                if (match(Token::PlusPlus))
                    return new Ast::UnaryExpr (Ast::postIncrOp, expr);
                if (match(Token::MinusMinus))
                    return new Ast::UnaryExpr (Ast::postDecrOp, expr);
            }

            return expr;
        }

        function unaryExpression (alpha: ALPHA, beta: BETA) : Ast::EXPR {
            let t;

            switch (t = hd ()) {
            case Token::Delete:
            case Token::PlusPlus:
            case Token::MinusMinus:
                next();
                return new Ast::UnaryExpr(Ast::tokenToOperator[t + 1000], postfixExpression (alpha, beta));

            case Token::Void:
            case Token::TypeOf:
            case Token::Plus:
            case Token::Minus:
            case Token::BitwiseNot:
            case Token::Not:
                next();
                return new Ast::UnaryExpr(Ast::tokenToOperator[t + 1000], unaryExpression (alpha, beta));

            case Token::Type:
                next();
                return new Ast::TypeExpr(nullableTypeExpression ());

            default:
                return postfixExpression(alpha, beta);
            }
        }

        function multiplicativeExpression (alpha, beta:BETA) : Ast::EXPR {
            let expr = unaryExpression (alpha, beta);
            let t;

            while (Token::isMultiplicative(t = hd()) || t == Token::BREAK) {
                if (t == Token::BREAK) {
                    div();
                    if (!Token::isMultiplicative(t = hd()))
                        break;
                }
                next();
                expr = new Ast::BinaryExpr(Ast::tokenToOperator[t], expr, unaryExpression(alpha, beta));
            }

            return expr;
        }

        function additiveExpression (alpha: ALPHA, beta: BETA) : Ast::EXPR {
            let expr = multiplicativeExpression (alpha, beta);
            let t;

            while (Token::isAdditive(t = hd())) {
                next();
                expr = new Ast::BinaryExpr(Ast::tokenToOperator[t], expr, multiplicativeExpression(alpha, beta));
            }
            return expr;
        }

        function shiftExpression (alpha: ALPHA, beta: BETA) : Ast::EXPR {
            let expr = additiveExpression (alpha, beta);
            let t;

            while (Token::isShift(t = hd())) {
                next();
                expr = new Ast::BinaryExpr(Ast::tokenToOperator[t], expr, additiveExpression(alpha, beta));
            }

            return expr;
        }

        // FIXME: Why would we distinguish between BinaryTypeExpr and
        // BinaryExpr here, as long as the operator gives it away
        // anyhow?  (Note we must still dispatch on the subparser).

        function relationalExpression (alpha: ALPHA, beta: BETA) : Ast::EXPR {
            let expr = shiftExpression (alpha, beta);
            let t;

            while (Token::isRelational(t = hd(), beta != noIn)) {
                next();
                if (Token::isRelationalType(t))
                    expr = new Ast::BinaryTypeExpr(Ast::tokenToOperator[t], expr, typeExpression());
                else 
                    expr = new Ast::BinaryExpr(Ast::tokenToOperator[t], expr, shiftExpression(alpha, beta));
            }

            return expr;
        }

        function equalityExpression (alpha: ALPHA, beta: BETA) : Ast::EXPR {
            let expr = relationalExpression (alpha, beta);
            let t;

            while (Token::isEquality(t = hd())) {
                next();
                expr = new Ast::BinaryExpr (Ast::tokenToOperator[t], expr, relationalExpression (alpha, beta));
            }

            return expr;
        }

        function bitwiseAndExpression (alpha: ALPHA, beta: BETA) : Ast::EXPR {
            let expr = equalityExpression (alpha, beta);

            while (match(Token::BitwiseAnd))
                expr = new Ast::BinaryExpr (Ast::bitwiseAndOp, expr, equalityExpression (alpha, beta));

            return expr;
        }

        function bitwiseXorExpression (alpha: ALPHA, beta: BETA) : Ast::EXPR {
            let expr = bitwiseAndExpression (alpha, beta);

            while (match(Token::BitwiseXor))
                expr = new Ast::BinaryExpr (Ast::bitwiseXorOp, expr, bitwiseAndExpression (alpha, beta));

            return expr;
        }

        function bitwiseOrExpression (alpha: ALPHA, beta: BETA) : Ast::EXPR {
            let expr = bitwiseXorExpression (alpha, beta);

            while (match(Token::BitwiseOr)) 
                expr = new Ast::BinaryExpr (Ast::bitwiseOrOp, expr, bitwiseXorExpression (alpha, beta));

            return expr;
        }

        function logicalAndExpression (alpha: ALPHA, beta: BETA) : Ast::EXPR {
            let expr = bitwiseOrExpression (alpha, beta);

            while (match(Token::LogicalAnd)) 
                expr = new Ast::BinaryExpr (Ast::logicalAndOp, expr, bitwiseOrExpression (alpha, beta));

            return expr;
        }

        function logicalXorExpression (alpha: ALPHA, beta: BETA) : Ast::EXPR {
            let expr = logicalAndExpression (alpha, beta);

            while (match(Token::LogicalXor))
                expr = new Ast::BinaryExpr (Ast::logicalXor, expr, logicalAndExpression (alpha, beta));

            return expr;
        }

        function logicalOrExpression (alpha: ALPHA, beta: BETA) : Ast::EXPR {
            let expr = logicalXorExpression (alpha, beta);

            while (match(Token::LogicalOr)) 
                expr = new Ast::BinaryExpr (Ast::logicalOrOp, expr, logicalXorExpression (alpha, beta));

            return expr;
        }

        function letExpression(alpha:ALPHA, beta: BETA) {
            eat(Token::Let);
            eat(Token::LeftParen);
            let [fixtures, inits] = variableBindingList(beta, Ast::noNS, Ast::letInit, false);
            eat(Token::RightParen);
            let expr = commaExpression(alpha, beta);  // FIXME: grammar says assignmentExpression
            return new Ast::LetExpr(new Ast::Head(fixtures, inits), expr);
        }

        function yieldExpression(alpha: ALPHA, beta: BETA) {
            cx.topFunction().uses_yield = true;
            eat(Token::Yield);
            let expr = assignmentExpression(alpha, beta);  // FIXME: according to spec, but probably optimizing the wrong good
            return new Ast::YieldExpr(expr);
        }

        function nonAssignmentExpression (alpha: ALPHA, beta: BETA) : Ast::EXPR {
            switch (hd ()) {
            case Token::Let:
                return letExpression (alpha, beta);

            case Token::Yield:
                return yieldExpression (alpha, beta);

            default: {
                let expr = logicalOrExpression (alpha, beta);
                if (match(Token::QuestionMark)) {
                    let consequent = nonAssignmentExpression (noColon, beta);
                    eat (Token::Colon);
                    let alternate = nonAssignmentExpression (alpha, beta);
                    return new Ast::TernaryExpr (expr, consequent, alternate);
                }
                return expr;
            }
            }
        }

        function conditionalExpression (alpha: ALPHA, beta: BETA) : Ast::EXPR {
            switch (hd ()) {
            case Token::Let:
                return letExpression (alpha, beta);

            case Token::Yield:
                return yieldExpression (alpha, beta);

            default: {
                let expr = logicalOrExpression (alpha, beta);
                if (match(Token::QuestionMark)) {
                    let consequent = assignmentExpression (noColon, beta); 
                    eat (Token::Colon);
                    let alternate = assignmentExpression (alpha, beta);
                    return new Ast::TernaryExpr (expr, consequent, alternate);
                }
                return expr;
            }
            }
        }

        function assignmentExpression (alpha: ALPHA, beta: BETA) : Ast::EXPR {
            let lhs = conditionalExpression (alpha, beta);
            let t;

            if ((t = hd()) == Token::Assign || Token::isOpAssign(t)) {
                next();
                let pat = patternFromExpr (lhs);
                if (t != Token::Assign && !(pat is SimplePattern))
                    Parse::internalError(this, "The lhs of op= must be a name or property reference.");
                let rhs = assignmentExpression (alpha, beta);
                let [fxtrs,expr,head] = desugarAssignmentPattern (pat,Ast::anyType,rhs,Ast::tokenToOperator[t]);
                return expr;
            }
            return lhs;
        }

        // FIXME: This looks partial.  There can't be holes in array
        // patterns here.

        function patternFromExpr (e: Ast::EXPR) {
            if (e is Ast::LexicalRef || e is Ast::ObjectRef)
                return new SimplePattern (e);

            if (e is Ast::LiteralArray)
                return new ArrayPattern( Util::map(patternFromExpr, e.Ast::exprs) );

            if (e is Ast::LiteralObject)
                return new ObjectPattern( Util::map(function (f) { return new FieldPattern (f.Ast::ident, f.Ast::expr || f.Ast::ident) }, 
                                                    e.Ast::fields) );

            Parse::internalError(this, "patternFromExpr, unhandled or invalid expression kind " + e);
        }

        function commaExpression(alpha: ALPHA, beta: BETA): Ast::EXPR {
            let expr = assignmentExpression(alpha, beta);
            while (match(Token::Comma))
                expr = new Ast::BinaryExpr(Ast::commaOp, expr, assignmentExpression(alpha, beta));
            return expr;
        }

        // PATTERNS

        function pattern (beta: BETA, gamma: GAMMA) : PATTERN {
            switch (hd ()) {
            case Token::LeftBrace:   return objectPattern (gamma);
            case Token::LeftBracket: return arrayPattern (gamma);
            default:                 return simplePattern (beta, gamma);
            }
        }

        function simplePattern (beta: BETA, gamma: GAMMA) : PATTERN {
            switch (gamma) {
            case noExpr:    return new IdentifierPattern (identifier ());
            case allowExpr: return new SimplePattern (leftHandSideExpression (allowColon, beta));
            }
        }

        function arrayPattern (gamma: GAMMA) : Ast::EXPR {
            eat (Token::LeftBracket);
            let pattern = elementListPattern (gamma);
            eat (Token::RightBracket);

            return new ArrayPattern (pattern);
        }

        function elementListPattern (gamma:GAMMA) : Ast::EXPRS {
            let elements = [], element;

            if (hd () !== Token::RightBracket) {
                switch (hd ()) {
                case Token::Comma:
                    let pos = position();
                    eat(Token::Comma);
                    element = new Ast::LiteralUndefined(pos);
                    break;
                default:
                    element = pattern (allowIn,gamma);
                    break;
                }
                elements.push (element);
                while (hd () === Token::Comma) {
                    eat (Token::Comma);
                    switch (hd ()) {
                    case Token::Comma:
                        element = new Ast::LiteralUndefined(position());
                        break;
                    default:
                        element = pattern (allowIn,gamma);
                        break;
                    }
                    elements.push (element);
                }
            }

            return elements;
        }

        function objectPattern (gamma: GAMMA) : Ast::EXPR {
            eat (Token::LeftBrace);
            let fields = fieldListPattern (gamma);
            eat (Token::RightBrace);

            return new ObjectPattern (fields); 
        }

        function fieldListPattern (gamma:GAMMA) : Ast::EXPR {
            let fields = [];

            if (hd () != Token::RightBrace) {
                do 
                    fields.push (fieldPattern (gamma));
                while (match(Token::Comma));
            }

            return fields;
        }

        function fieldPattern (gamma:GAMMA) : FIELD_PATTERN {
            let field_name = fieldName ();
            let field_pattern;

            if (match(Token::Colon))
                field_pattern = pattern (allowIn,gamma);
            else if (field_name is Ast::Identifier) 
                field_pattern = new IdentifierPattern (field_name.Ast::ident);
            else
                Parse::internalError(this, "unsupported fieldPattern " + field_name);

            return new FieldPattern (field_name, field_pattern);
        }

        function typedPattern (beta: BETA) : [PATTERN,Ast::TYPE_EXPR] {
            let the_pattern = pattern (beta,noExpr);

            if (match(Token::Colon)) 
                return [the_pattern, nullableTypeExpression ()];
            return [the_pattern, Ast::anyType];
        }

        // TYPE EXPRESSIONS

        function nullableTypeExpression () : Ast::TYPE_EXPR {
            let t = typeExpression ();

            if (match(Token::QuestionMark))
                return new Ast::NullableType (t,true);
        
            if (match(Token::Not))
                return new Ast::NullableType (t,false);

            return t;
        }

        function typeExpression () : Ast::TYPE_EXPR {
            switch (hd ()) {
            case Token::Mult:        return next(), Ast::anyType;
            case Token::Null:        return next(), Ast::nullType;
            case Token::Undefined:   return next(), Ast::undefinedType;
            case Token::Function:    return functionType();
            case Token::LeftParen:   return unionType();
            case Token::LeftBrace:   return objectType();
            case Token::LeftBracket: return arrayType();
            default:                 return new Ast::TypeName (primaryName ());
            }
        }

        function unionType () : Ast::TYPE_EXPR {
            eat (Token::LeftParen);
            let type_exprs = typeExpressionListBar ();
            eat (Token::RightParen);

            return new Ast::UnionType (type_exprs);
        }

        function objectType () : Ast::TYPE_EXPR {
            eat (Token::LeftBrace);
            let field_types = fieldTypeList ();
            eat (Token::RightBrace);

            return new Ast::ObjectType (field_types);
        }

        function fieldTypeList () : [Ast::FIELD_TYPE] {
            let types = [];
            if (hd () !== Token::RightBrace) {
                do 
                    types.push (fieldType ());
                while (match(Token::Comma));
            }
            return types;
        }

        function fieldType () : Ast::FIELD_TYPE {
            let name = fieldName ();
            eat (Token::Colon);
            let t = nullableTypeExpression ();

            return new Ast::FieldType(name, t);
        }

        function arrayType () : Ast::TYPE_EXPR {
            eat (Token::LeftBracket);
            let elements = elementTypeList ();
            eat (Token::RightBracket);

            return new Ast::ArrayType (elements);
        }

        function elementTypeList () : [Ast::ELEMENT_TYPE] {
            let element_types = [];

            if (hd () !== Token::RightBracket) {
                let pos = position();
                switch (hd ()) {
                case Token::Comma:
                    eat(Token::Comma);
                    element_types.push( new Ast::LiteralUndefined(pos) );
                    break;
                default:
                    element_types.push( nullableTypeExpression () );
                    break;
                }

                while (match(Token::Comma)) {
                    let pos = position();
                    switch (hd ()) {
                    case Token::Comma:
                        element_types.push( new Ast::LiteralUndefined(pos) );
                        break;
                    default:
                        element_types.push( nullableTypeExpression () );
                        break;
                    }
                }
            }

            return element_types;
        }

        function typeExpressionList () : [Ast::TYPE_EXPR]
            listOfNullableTypeExpressions(Token::Comma);

        function typeExpressionListBar () : [Ast::TYPE_EXPR]
            listOfNullableTypeExpressions(Token::BitwiseOr);

        function listOfNullableTypeExpressions(separator) {
            let types = [];
            do 
                types.push (nullableTypeExpression ()); 
            while (match(separator))
            return types;
        }

        // STATEMENTS

        function statement (tau: TAU, omega: OMEGA) : Ast::Stmt {
            switch (hd()) {
            case Token::LeftBrace: return new Ast::BlockStmt( block(tau) );
            case Token::Break:     return let (stmt = breakStatement ()) semicolon(omega), stmt;
            case Token::Continue:  return let (stmt = continueStatement ()) semicolon(omega), stmt;
            case Token::Do:        return let (stmt = doStatement(omega)) semicolon(omega), stmt;
            case Token::For:       return forStatement(omega);
            case Token::If:        return ifStatement(omega);
            case Token::Return:    return let (stmt = returnStatement()) semicolon(omega), stmt;
            case Token::Switch:    return (hd2() == Token::Type) ? switchTypeStatement() : switchStatement();
            case Token::Throw:     return let (stmt = throwStatement()) semicolon(omega), stmt;
            case Token::Try:       return tryStatement ();
            case Token::While:     return whileStatement(omega);
            case Token::With:      return withStatement (omega);
            default:
                if (hd() == Token::Identifier && hd2() == Token::Colon)
                    return labeledStatement ();
                return let (stmt = expressionStatement ()) semicolon(omega), stmt;
            }
        }

        function substatement (omega: OMEGA) : Ast::Stmt {
            if (match(Token::SemiColon))
                return new Ast::EmptyStmt;
            return statement (localBlk,omega);
        }

        function semicolon (omega: OMEGA) {
            switch (omega) {
            case fullStmt:
                switch (hd ()) {
                case Token::SemiColon:
                    next();
                    return;
                case Token::EOS:
                case Token::RightBrace:
                    // Inserting it
                    return;
                default:
                    if (!newline ())
                        Parse::syntaxError(this, "Expecting semicolon");
                    // Inserting it
                    return;
                }

            case abbrevStmt:  // Abbrev, ShortIf
                switch (hd ()) {
                case Token::SemiColon:
                    next();
                    return;
                default:
                    // Inserting it
                    return;
                }

            default:
                Parse::internalError(this, "Unhandled statement mode " + omega);
            }
        }

        function noNewline() {
            switch (hd ()) {
            case Token::EOS:
            case Token::SemiColon:
            case Token::RightBrace:
                return false;
            default:
                if (newline())
                    return false;
                return true;
            }
        }

        function labeledStatement () : Ast::Stmt {
            let label = identifier ();
            eat(Token::Colon);

            let stmt = substatement(fullStmt);

            let s = stmt;
            while (s is Ast::LabeledStmt)
                s = s.stmt;
            if (s is ILabelSet)
                s.Ast::labels.push(label);

            return new Ast::LabeledStmt(label, stmt);
        }

        function expressionStatement () : Ast::Stmt
            new Ast::ExprStmt (commaExpression (allowColon, allowIn));

        function returnStatement () : Ast::Stmt {
            eat (Token::Return);
            return new Ast::ReturnStmt (noNewline() ? commaExpression(allowColon, allowIn) : null);
        }

        function breakStatement () : Ast::Stmt
            new Ast::BreakStmt( breakOrContinueLabel(Token::Break) );

        function continueStatement () : Ast::Stmt
            new Ast::ContinueStmt( breakOrContinueLabel(Token::Continue) );

        function breakOrContinueLabel(tok) {
            eat(tok);
            return noNewline() ? identifier() : null;
        }

        function ifStatement (omega) : Ast::Stmt {
            eat(Token::If);
            let test = parenExpression();
            let consequent = substatement(omega);
            let alternate = null;
            if (match(Token::Else))
                alternate = substatement(omega);

            return new Ast::IfStmt(test, consequent, alternate);
        }

        function whileStatement (omega) : Ast::Stmt {
            eat(Token::While);
            let test = parenExpression();
            let body = substatement(omega); 
 
            return new Ast::WhileStmt (test, body);
        }

        function doStatement (omega) : Ast::Stmt {
            eat(Token::Do);
            let body = substatement(omega); 
            eat(Token::While);
            let test = parenExpression ();
 
            return new Ast::DoWhileStmt (test, body);
        }

        function forStatement (omega: OMEGA) : Ast::Stmt {
            cx.enterLetBlock ();

            eat (Token::For);
            let is_each = match(Token::Each);
            eat (Token::LeftParen);
            let init = forInitialiser ();
            if (match(Token::In)) {
                let objexpr = commaExpression (allowColon, allowIn);
                eat (Token::RightParen);
                let body = substatement (omega); 
                let head = cx.exitLetBlock ();
                
                return new Ast::ForInStmt (head,init,objexpr,body,is_each);
            }
            else {
                if (is_each)
                    Parse::syntaxError(this, "'for each' requires 'in' style loop");
                eat (Token::SemiColon);
                let test = hd() == Token::SemiColon ? null : commaExpression(allowColon, allowIn);
                eat (Token::SemiColon);
                let update = hd() == Token::RightParen ? null : commaExpression(allowColon, allowIn);
                eat (Token::RightParen);
                let body = substatement (omega); 
                let head = cx.exitLetBlock ();
                
                return new Ast::ForStmt (head,init,test,update,body);
            }
        }

        function forInitialiser () : Ast::EXPR? {
            switch (hd ()) {
            case Token::SemiColon:
                return null;

            case Token::Const:
            case Token::Let:
            case Token::Var:
                return bindingForInitialiser();

            default:
                return commaExpression (allowColon, noIn);
            }
        }

        function bindingForInitialiser() {
            let [it, ro] = variableDefinitionKind(localBlk);
            let in_allowed = true;
            let init_expr = null;

            for (;;) {
                let [pattern, annotation] = typedPattern (noIn);
                let initializer = null;

                if (match(Token::Assign))
                    initializer = assignmentExpression (allowColon, noIn);
                else
                    initializer = new Ast::LiteralUndefined();  // Probably wrong, see comment below

                if (hd() != Token::Comma && hd() != Token::SemiColon)
                    break;

                let [fixtures,init] = desugarBindingPattern (pattern, annotation, initializer, cx.pragmas.defaultNamespace, it, ro);

                if (it == Ast::varInit)
                    cx.addVarFixtures(fixtures);
                else 
                    cx.addLetFixtures(fixtures);

                init_expr = init_expr ? new Ast::BinaryExpr(Ast::commaOp, init_expr, init) : init;

                if (hd() == Token::SemiColon)
                    return init_expr;

                in_allowed = false;
                eat(Token::Comma);
            }

            if (hd() != Token::In)
                Parse::syntaxError("'in' is required here");

            if (!in_allowed)
                Parse::syntaxError("'in' does not allow multiple binding clauses in the loop head.");

            // FIXME: More elaborate handling of patterns here.
            // A destructuring pattern is legal in a for-in head
            // even if it does not have an initializing expression.
            if (!(pattern is IdentifierPattern))
                Parse::internalError(this, "Identifier pattern required by for-in binding (for now)");

            if (initializer == null) {
                // FIXME: This is wrong, because it may overwrite a hoisted var that 
                // should not be overwritten if the loop body is never entered.
                initializer = new Ast::LiteralUndefined();
            }

            let [fixtures,init] = desugarBindingPattern (pattern,annotation,initializer,cx.pragmas.defaultNamespace,it,ro);
            if (init == null)
                Parse::syntaxError(this, "forInitialiser: " + init);
            
            if (it == Ast::varInit)
                cx.addVarFixtures(fixtures);
            return init;
        }

        function switchStatement () : Ast::Stmt {
            eat (Token::Switch);
            let expr = parenExpression ();

            eat (Token::LeftBrace);
            let cases = null;
            if (hd () == Token::Case || hd() == Token::Default) 
                cases = caseElements ();
            eat (Token::RightBrace);

            return new Ast::SwitchStmt (expr, cases);
        }

        function caseElements () : Ast::CASES {
            let cases = [];
            let hasDefault = false;
        loop:
            for (;;) {
                switch (hd ()) {
                case Token::RightBrace:
                    break loop;
                case Token::Default:
                    eat(Token::Default);
                    eat(Token::Colon);
                    if (hasDefault)
                        Parse::syntaxError(this, "Duplicate 'default' clause in 'switch'");
                    hasDefault = true;
                    cases.push(new Ast::Case(null, []));
                    break;
                case Token::Case: {
                    eat(Token::Case);
                    let item = commaExpression(allowColon, allowIn);
                    eat(Token::Colon);
                    cases.push (new Ast::Case (item,[]));
                    break;
                }
                default: {
                    let items = directive (localBlk,fullStmt);            // 'abbrev' is handled by RightBrace check in head
                    Util::pushOnto(cases[cases.length-1].Ast::stmts, items);
                    break;
                }
                }
            }

            return cases;
        }

        function throwStatement () : Ast::Stmt {
            eat (Token::Throw);
            return new Ast::ThrowStmt( commaExpression (allowColon, allowIn) );
        }

        function tryStatement () : Ast::Stmt {
            eat (Token::Try);

            let tryblock = block (localBlk);
            let catchblocks = catches ();
            let finallyblock = null;

            if (match(Token::Finally)) {
                cx.topFunction().uses_finally = true;
                finallyblocks = block (localBlk);
            }

            return new Ast::TryStmt (tryblock,catchblocks,finallyblock);
        }

        function catches () : Ast::CATCHES {
            let catchblocks = [];

            while (match(Token::Catch))
                catchblocks.push (catchClause ());

            cx.topFunction().uses_catch = catchblocks.length > 0;
            return catchblocks;
        }

        function catchClause () : Ast::CATCH {
            eat (Token::LeftParen);
            let catchvar = parameter ();
            eat (Token::RightParen);
            let catchblock = block (localBlk);

            let [k,[p,t]] = catchvar;
            let [f,i] = desugarBindingPattern (p, t, new Ast::GetParam (0), Ast::noNS, Ast::varInit, false);
            let head = new Ast::Head (f,[i]);

            return new Ast::Catch (head,catchblock);
        }

        function withStatement (omega: OMEGA) : Ast::Stmt {
            eat (Token::With);
            cx.topFunction().uses_with = true;
            let expr = parenExpression ();
            let body = substatement (omega);
            return new Ast::WithStmt (expr, body);
        }

        function switchTypeStatement () : Ast::Stmt {
            eat (Token::Switch);
            eat (Token::Type);
            let expr  = typedExpression ();
            let [e,t] = expr;
            eat (Token::LeftBrace);
            let typecases = typeCases ();
            eat (Token::RightBrace);

            return new Ast::SwitchTypeStmt (e,t,typecases);
        }

        function typedExpression () : [Ast::EXPR,Ast::TYPE_EXPR] {
            let expr = parenExpression ();
            let texpr = match(Token::Colon) ? nullableTypeExpression () : Ast::anyType;
            return [expr,texpr];
        }

        function typeCases () : Ast::CATCHES {
            let cases = [];
            while (match(Token::Case))
                cases.push(catchClause());
            return cases;
        }

        // DEFINITIONS

        function variableDefinition (beta: BETA, tau: TAU, ns, isPrototype, isStatic) : Ast::EXPR? {
            let [it, ro] = variableDefinitionKind (tau);

            let [fxtrs,exprs] = variableBindingList (beta, ns, it, ro);
            let expr = null;

            if (it == Ast::letInit) {
                cx.addLetFixtures (fxtrs);
                expr = exprListToCommaExpr(exprs);
            }
            else if (tau == classBlk) {
                cx.addVarFixtures (fxtrs, isStatic);
                cx.addVarInits (exprs, isStatic);      // FIXME these aren't inits, they are a kind of settings
            }
            else {
                cx.addVarFixtures (fxtrs);
                expr = exprListToCommaExpr(exprs);
            }

            return expr;
        }

        function variableDefinitionKind (tau: TAU) {
            if (match(Token::Const)) {
                if (match(Token::Function))
                    Parse::syntaxError(this, "'const function' not allowed here");
                return [tau == classBlk ? Ast::varInit : Ast::letInit, true];
            }

            if (match(Token::Var))
                return [Ast::varInit, false];

            if (match(Token::Let))
                return [Ast::letInit, false];

            Parse::syntaxError(this, "Illegal binding keyword " + lexeme());
        }

        function variableBindingList (beta: BETA, ns: Ast::NAMESPACE, it: Ast::INIT_TARGET, ro: boolean ) : [Ast::FIXTURES, Ast::EXPRS] {
            let fixtures = [], exprs = [];
            do
                variableBinding(beta, ns, it, ro, fixtures, exprs);
            while (match(Token::Comma));
            return [fixtures, exprs];
        }

        function variableBinding (beta, ns, it, ro, fixtures, exprs) {
            let [pattern, annotation] = typedPattern (beta);
            let initializer;
            let f, i;

            if (match(Token::Assign))
                initializer = assignmentExpression (allowColon, beta);
            else if (!(pattern is IdentifierPattern)) 
                Parse::syntaxError(this, "destructuring pattern without initializer");
            else
                initializer = new Ast::LiteralUndefined();
            [f,i] = desugarBindingPattern (pattern,annotation,initializer,ns,it,ro);

            Util::pushOnto(fixtures, f);
            exprs.push(i);
        }

        function functionDefinition (tau: TAU, omega: OMEGA, attrs: ATTRS) : Ast::STMTS {
            eat (Token::Function);

            cx.enterFunction(attrs);

            let name = functionName ();
            let signature = functionSignature ();
            let body;

            cx.enterVarBlock ();
            if (attrs.native || tau == interfaceBlk) {
                semicolon(fullStmt);
                body = new Ast::Block(null, []);
            }
            else 
                body = functionBody (allowIn, omega);

            let vars = cx.exitVarBlock ();
            let attr = cx.exitFunction ();

            if (tau == interfaceBlk)
                checkLegalAttributes(attrs, {});

            let {params, defaults, resultType, thisType, numparams} = signature;
            let func = new Ast::Func (name, body, params, numparams, vars, defaults, resultType, attr);

            let name = new Ast::PropName (new Ast::Name(attrs.ns, name.ident));
            let fxtr = new Ast::MethodFixture (func, Ast::anyType, true, attrs.override, attrs.final);
            switch (tau) {
            case classBlk:
                cx.addVarFixtures ([[name,fxtr]], attrs.static);
                break;
            default:
                cx.addVarFixtures ([[name,fxtr]]);
                break;
            }

            return [];
        }

        function functionExpression () : Ast::EXPR {
            let name = null;

            cx.enterFunction(defaultAttrs());

            let pos = position();
            eat (Token::Function);
            if (hd () == Token::Identifier)
                name = functionName ();

            let signature = functionSignature ();

            cx.enterVarBlock ();
            let body = functionBody (allowIn, fullStmt);
            let vars = cx.exitVarBlock ();

            let attr = cx.exitFunction();

            let {params, numparams, defaults, resultType} = signature;
            let fnexpr = new Ast::LiteralFunction(new Ast::Func(name, 
                                                                body, 
                                                                params, 
                                                                numparams,
                                                                vars, 
                                                                defaults, 
                                                                resultType,
                                                                attr),
                                                  pos);

            return fnexpr;
        }

        function constructorDefinition (omega, ns, attrs) : Ast::STMTS {
            eat (Token::Function);

            cx.enterFunction(attrs);

            let name = identifier ();
            let signature = constructorSignature ();
            let body;

            cx.enterVarBlock ();
            if (attrs.native) {
                semicolon(fullStmt);
                body = new Ast::Block(null, []);
            }
            else
                body = functionBody (allowIn, omega);

            let vars = cx.exitVarBlock ();
            let attr = cx.exitFunction();

            let {params, defaults, settings, superArgs, numparams} = signature;

            let func = new Ast::Func (new Ast::FuncName(Ast::ordinaryFunction, name), 
                                      body, 
                                      params, 
                                      numparams, 
                                      vars, 
                                      defaults, 
                                      Ast::voidType, 
                                      attr);
            let ctor = new Ast::Ctor (settings,superArgs,func);

            if (cx.ctor !== null)
                Parse::syntaxError(this, "Constructor already defined");

            cx.ctor = ctor;

            return [];
        }

        type CTOR_SIG = 
          { typeParams : [Ast::IDENT]
          , params : Ast::HEAD
          , paramTypes : [Ast::TYPE_EXPR]
          , defaults : [Ast::EXPR]
          , settings : [Ast::EXPR]
          , superArgs: [Ast::EXPR] }

        type FUNC_SIG = 
          { typeParams : [Ast::IDENT]
          , params : Ast::HEAD
          , paramTypes : [Ast::TYPE_EXPR]
          , defaults : [Ast::EXPR]
          , resultType : Ast::TYPE_EXPR
          , thisType : Ast::TYPE_EXPR? }

        function constructorSignature () : CTOR_SIG {
            eat (Token::LeftParen);
            let ps = parameters ();
            eat (Token::RightParen);
            let [settings,superArgs] = constructorInitialiser ();

            // Translate bindings and init steps into fixtures and inits (HEAD)
            let { fixtures, inits, exprs, types, numparams } = ps;

            return { typeParams: []
                     , params: new Ast::Head (fixtures,inits)
                     , paramTypes: types
                     , numparams: numparams
                     , defaults: exprs
                     , settings: settings
                     , superArgs: superArgs };
        }

        /*

        constructor initializers are represented by two lists. the first
        list represents the initializers and will consist of set exprs or
        let exprs (if there are temps for destructuring). the second list
        represents the arguments to the call the the super constructor

        */

        function constructorInitialiser () : [[Ast::EXPR], [Ast::EXPR]] {
            let settings=[];
            let superargs=null;

            if (match(Token::Colon)) {
                while (hd() != Token::Super) {
                    settings.push(setting());
                    if (!match(Token::Comma))
                        break;
                }
                if (match(Token::Super))
                    superargs = argumentList();
            }

            return [settings, superargs || []];
        }

        /*

        Setting
            Pattern(noIn, allowExpr)  VariableInitialisation(allowIn)


            function A (a) : [q::x,r::y] = a { }


            let ($t0 = a) q::x = $t0[0], r::y = $t0[1]

            let ($t0 = a)
                init (This,q,[x,$t0[0]),
                init (This,r,[y,$t0[1])


        */

        function setting () : Ast::EXPR {
            let lhs = pattern (allowIn,allowExpr);
            eat (Token::Assign);
            let rhs = assignmentExpression (allowColon, allowIn);

            let [fxtrs,inits] = desugarBindingPattern (lhs, Ast::anyType, rhs, null, Ast::instanceInit, null);
            // FIXME: assert fxtrs is empty

            return inits;
        }

        // FIXME: needs to handle plain functions called 'get' and 'set'
        // FIXME: needs to handle catchalls

        function functionName () : Ast::FUNC_NAME {
            let kind;

            if (match(Token::Get))      kind = Ast::getterFunction;
            else if (match(Token::Set)) kind = Ast::setterFunction;
            else                        kind = Ast::ordinaryFunction;

            let ident = identifier();
            return new Ast::FuncName(kind, ident);
        }

        /*

        FunctionSignature
            TypeParameters  (  Parameters  )  ResultType
            TypeParameters  (  this  :  PrimaryIdentifier  )  ResultType
            TypeParameters  (  this  :  PrimaryIdentifier  ,  NonemptyParameters  )  ResultType

        there are two differences between a BINDING_IDENT and a FIXTURE_NAME: the namespace on
        properties, and the offset on parameter indicies.

        */

        function functionSignature () : FUNC_SIG {
            let type_params = typeParameters (); // FIXME: not used

            eat (Token::LeftParen);
            if (match(Token::This))
                // FIXME implement this
                Parse::internalError(this, "No support for 'this' annotation in parameter list");
            let ps = parameters();
            eat (Token::RightParen);
            let restype = resultType ();

            // Translate bindings and init steps into fixtures and inits (HEAD)
            let { fixtures, inits, exprs, types, numparams } = ps;
            return  { typeParams: []
                    , params: new Ast::Head (fixtures,inits)
                    , paramTypes: types
                    , numparams: numparams
                    , defaults: exprs
                    , ctorInits: null
                    , resultType: restype
                    , thisType: null };
        }

        function typeParameters () : [Ast::IDENT] {
            if (match(Token::LeftDotAngle))
                return let (tparams = typeParameterList()) eat(Token::GreaterThan), tparams;
            return [];
        }

        function typeParameterList () : [Ast::IDENT] {
            let tparams = [];
            do
                tparams.push(identifier ());
            while(match(Token::Comma));
            return tparams;
        }

        type PARAMETERS = {
            fixtures: [Ast::FIXTURE_BINDING],
            inits: [Ast::EXPR],
            exprs: [Ast::EXPR],
            types: [Ast::TYPE_EXPR],
            numparams: *
        };

        function parameters () : PARAMETERS {
            let params = { fixtures: [], inits: [], exprs: [], types: [], numparams: 0 };
            let initRequired = false;

            while (hd() != Token::RightParen) {
                if (hd() == Token::TripleDot) {
                    cx.topFunction().uses_rest = true;
                    restParameter (params);
                    break;
                }

                initRequired = parameterInit(params, initRequired);
                params.numparams++;

                if (!match(Token::Comma))
                    break;
            }
            return params;
        }

        function restParameter(params) {
            eat(Token::TripleDot);
            let [f, i] =
                desugarBindingPattern(simplePattern(noIn, noExpr), 
                                      Ast::anyType, 
                                      new Ast::GetParam(params.numparams), 
                                      Ast::noNS, 
                                      Ast::letInit, 
                                      false);
            f.push([new Ast::TempName(params.numparams), new Ast::ValFixture(Ast::anyType,false)]);

            Util::pushOnto(params.fixtures, f);
            params.inits.push(i);
            params.types.push(Ast::anyType);
        }

        function parameterInit (params, initRequired) {
            let param = parameter ();
            let initexpr = null;

            if (match(Token::Assign))
                initexpr = nonAssignmentExpression(allowColon, allowIn);
            else if (initRequired)
                Parse::syntaxError(this, "Expecting default value expression");

            let [k,[p,t]] = param;
            let [f,i] = desugarBindingPattern (p, t, new Ast::GetParam (params.numparams), Ast::noNS, Ast::letInit, false);
            f.push ([new Ast::TempName (params.numparams), new Ast::ValFixture (t,false)]); // temp for desugaring

            Util::pushOnto(params.fixtures, f);
            params.inits.push(i);
            if (initexpr)
                params.exprs.push(initexpr);
            params.types.push(t);

            return initexpr != null;
        }

        function parameter () : [Ast::VAR_DEFN_TAG, [PATTERN, Ast::TYPE_EXPR]] {
            let kind = match(Token::Const) ? Ast::constTag : Ast::varTag;
            var pattern = typedPattern (allowIn);
            return [kind,pattern];
        }

        function resultType () : [Ast::IDENT] {
            if (match(Token::Colon)) {
                if (match(Token::Void))
                    return Ast::voidType;
                return nullableTypeExpression ();
            }
            return Ast::anyType;
        }

        function functionBody (beta: BETA, omega) : Ast::BLOCK {
            if (hd () == Token::LeftBrace)
                return block (localBlk);

            let expr = assignmentExpression (allowColon, beta);
            semicolon (omega);
            return new Ast::Block (new Ast::Head ([],[]),[new Ast::ReturnStmt (expr)]);
        }

        function classDefinition (ns: Ast::NAMESPACE, attrs) : Ast::STMTS {
            eat (Token::Class);
            checkLegalAttributes(attrs, {dynamic:true, final:true});

            let classid = identifier ();
            let signature = typeSignature ();       // FIXME: not used yet
            let superclass = extendsClause();
            let superinterfaces = interfaceList(Token::Implements);
            let protectedNs = new Ast::ProtectedNamespace (classid);

            currentClassName = classid;
            cx.enterVarBlock();                     // Class
            cx.enterVarBlock ();                    // Instance
            cx.pushNamespace(protectedNs);

            cx.ctor = null;                         // updated by constructorDefinition()
            let body = classBody ();                // Class initialization block

            let ihead = cx.exitVarBlock ();         // Instance
            let chead = cx.exitVarBlock ();         // Class
            currentClassName = "";

            let ctor = cx.ctor;
            if (ctor === null)
                ctor = makeDefaultCtor(classid);
            
            let baseName;
            if (superclass == null)
                baseName = new Ast::Name(Ast::noNS, "Object");
            else 
                baseName = superTypeToName(superclass);

            let interfaceNames = Util::map(superTypeToName, superinterfaces);

            let cname = new Ast::Name(ns, classid);
            let ctype = Ast::anyType;
            let itype = Ast::anyType;
            let cls = new Ast::Cls (cname,
                                    baseName,
                                    interfaceNames,
                                    protectedNs,
                                    ctor,
                                    chead,
                                    ihead,
                                    ctype,
                                    itype,
                                    body,
                                    attrs.dynamic,
                                    attrs.final);

            let fxtrs = [[new Ast::PropName(cname),new Ast::ClassFixture (cls)]];
            cx.addVarFixtures (fxtrs);

            return [];
        }

        function interfaceDefinition(ns: Ast::NAMESPACE, attrs) : Ast::STMTS {
            checkLegalAttributes(attrs, {});
            eat (Token::Interface);
            
            let interfaceid = identifier ();
            let signature = typeSignature ();       // FIXME: not used yet
            let superinterfaces = interfaceList(Token::Extends);

            currentClassName = "";

            cx.enterVarBlock ();                    // Instance
            interfaceBody ();
            let ihead = cx.exitVarBlock ();         // Instance

            let interfaceNames = Util::map(superTypeToName, superinterfaces);
            
            let iname = new Ast::Name(ns, interfaceid);
            let iface = new Ast::Interface(iname, interfaceNames, ihead);

            let fxtrs = [[new Ast::PropName(iname), new Ast::InterfaceFixture (iface)]];
            cx.addVarFixtures (fxtrs);

            return [];
        }

        function makeDefaultCtor(classname) {
            cx.enterFunction(defaultAttrs());
            let ctorbody = new Ast::Block (new Ast::Head([],[]),[]);
            let params = new Ast::Head([],[]);
            let numparams = 0;
            let vars = new Ast::Head([],[]);
            let defaults = [];
            let ty = Ast::anyType;
            let attr = cx.exitFunction();
            let func = new Ast::Func (new Ast::FuncName(Ast::ordinaryFunction, classname),
                                      ctorbody,
                                      params,
                                      numparams,
                                      vars,
                                      defaults,
                                      ty,
                                      attr);
            return new Ast::Ctor ([],[], func);
        }

        function superTypeToName(s) {
            switch type (s) {
            case (x:Ast::Identifier) {
                return new Ast::Name(Ast::noNS, x.Ast::ident);
            }
            case (x:*) {
                Parse::internalError(this, "Can't handle this base type name " + s);
            }
            }
        }

        function typeSignature () : [[Ast::IDENT], boolean] {
            let type_params = typeParameters ();
            let is_nonnullable = match(Token::Not);

            return [type_params, is_nonnullable];
        }

        function extendsClause() {
            if (match(Token::Extends))
                return primaryName ();
            return null;
        }

        function interfaceList(tok) {  // tok is Token::Extends or Token::Implements
            let names = [];
            if (match(tok)) {
                do 
                    names.push(primaryName());
                while (match(Token::Comma));
            }
            return names;
        }

        function classBody () : Ast::BLOCK
            block (classBlk);

        function interfaceBody() {
            eat(Token::LeftBrace);
            directives(interfaceBlk);
            eat(Token::RightBrace);
        }

        function namespaceDefinition (omega: OMEGA, ns: Ast::NAMESPACE ) : Ast::STMTS {
            eat (Token::Namespace);
            let name = identifier ();
            let initializer = null;

            if (match(Token::Assign)) {
                if (hd () == Token::StringLiteral)
                    initializer = let (nsstring = lexeme()) next(), nsstring;
                else
                    initializer = cx.resolveNamespaceFromIdentExpr (primaryName ());
            }
            semicolon (omega);

            let nsVal;
            if (initializer === null) 
                nsVal = new Ast::AnonymousNamespace (getAnonymousName(name));
            else 
                nsVal = new Ast::UserNamespace (initializer);

            let fxname = new Ast::PropName (new Ast::Name(ns, name));
            let fxtr = new Ast::NamespaceFixture (nsVal);
            cx.addVarFixtures ([[fxname,fxtr]]);

            return [];
        }

        // FIXME: not what you want, and anyhow it looks completely
        // ill-defined since any property name is possible in the 
        // global object.

        function getAnonymousName (seedStr) {
            return seedStr;
        }

        function typeDefinition (omega: OMEGA, ns: Ast::NAMESPACE) : Ast::STMTS {
            eat (Token::Type);
            let typename = identifier ();
            eat (Token::Assign);
            let typedef = nullableTypeExpression ();
            semicolon (omega);

            var name = new Ast::PropName (new Ast::Name(ns, typename));
            var fxtr = new Ast::TypeFixture (typedef);
            cx.addVarFixtures ([[name,fxtr]]);

            return [];
        }

        // DIRECTIVES

        function directives (tau: TAU) : [Ast::PRAGMAS, Ast::STMTS] {
            switch (hd ()) {
            case Token::RightBrace:
            case Token::EOS:
                return [[],[]];
            default:
                return directivesPrefix (tau);
            }
        }

        /*

          DirectivesPrefix (tau)
              empty
              Pragmas
              DirectivesPrefix(tau) Directive(tau,full)

          right recursive:

          DirectivesPrefix(tau)
              empty
              Pragmas DirectivePrefix'(tau)

          DirectivesPrefix'(tau)
              empty
              Directive(tau,full) DirectivesPrefix'(tau)

          add var fixtures to the vhead and let fixtures to the bhead. the
          context provides a reference to the current vhead and bhead, as
          well as the whole environment, for convenient name addition and
          lookup.


        */

        function directivesPrefix (tau: TAU) : [Ast::PRAGMAS, Ast::STMTS] {
            if (hd() == Token::Use || hd() == Token::Import) {
                // This test could be debated, esp for 'import'
                if (tau == interfaceBlk)
                    Parse::syntaxError(this, "Pragma not allowed in interfaces");
                pragmas (); 
            }
            return directivesPrefixPrime (tau);
        }

        function directivesPrefixPrime (tau: TAU) : Ast::STMTS {
            let directives = [];

            while (hd () != Token::RightBrace && hd () != Token::EOS)
                Util::pushOnto(directives, directive(tau,fullStmt));

            return directives;
        }

        function isCurrentClassName (lexeme) : boolean
            lexeme === currentClassName;

        function directive (tau: TAU, omega: OMEGA) : Ast::STMTS {
            switch (hd()) {
            case Token::SemiColon:
                if (tau == interfaceBlk)
                    Parse::syntaxError(this, "Statement not allowed in interfaces");
                eat(Token::SemiColon);
                return [new Ast::EmptyStmt];

            case Token::Let: // FIXME might be function
            case Token::Var:
            case Token::Const:
                if (tau == interfaceBlk)
                    Parse::syntaxError(this, "Variable bindings not allowed in interfaces");
                let e = variableDefinition (allowIn
                                            , tau
                                            , cx.pragmas.defaultNamespace
                                            , false
                                            , false);
                return let (stmts = e == null ? [] : [new Ast::ExprStmt(e)]) semicolon (omega), stmts;

            case Token::Function:
                if (isCurrentClassName (lexeme2())) 
                    return constructorDefinition (omega, cx.pragmas.defaultNamespace, defaultAttrs());
                return functionDefinition (tau, omega, defaultAttrs());

            case Token::Interface:
                if (tau != globalBlk)
                    Parse::syntaxError(this, "Interface definition not allowed here");
                return interfaceDefinition (cx.pragmas.defaultNamespace, defaultAttrs());

            case Token::Class:
                if (tau != globalBlk)
                    Parse::syntaxError(this, "Class definition not allowed here");
                return classDefinition (cx.pragmas.defaultNamespace, defaultAttrs());

            case Token::Namespace:
                if (tau == interfaceBlk)
                    Parse::syntaxError(this, "Namespace bindings not allowed in interfaces");
                return namespaceDefinition (omega, cx.pragmas.defaultNamespace);

            case Token::Type:
                if (tau == interfaceBlk)
                    Parse::syntaxError(this, "Type bindings not allowed in interfaces");
                return typeDefinition (omega, cx.pragmas.defaultNamespace);

            case Token::LeftBrace:
            case Token::Break:
            case Token::Continue:
            case Token::Default:
            case Token::Do:
            case Token::For:
            case Token::If:
            case Token::Let:
            case Token::Return:
            case Token::Switch:
            case Token::Throw:
            case Token::Try:
            case Token::While:
            case Token::With:
                if (tau == interfaceBlk)
                    Parse::syntaxError(this, "Statement not allowed in interfaces");
                return [statement (tau,omega)];

            case Token::Dynamic:
            case Token::Final:
            case Token::Native:
            case Token::Override:
            case Token::Prototype:
            case Token::Static:
            case Token::Public:
            case Token::Private:
            case Token::Protected:
            case Token::Internal:
            case Token::Intrinsic:
                let attr = attribute (tau,defaultAttrs());
                return annotatableDirective (tau,omega,attr);

            case Token::Identifier:
                // FIXME: This should be some variant of directive (), in order to catch "var".
                // But not every directive can be labeled, so just do the simple thing for the
                // time being.
                if (hd2 () == Token::Colon) {
                    if (tau == interfaceBlk)
                        Parse::syntaxError(this, "Statement not allowed in interfaces");
                    return [statement (tau,omega)];
                }
                // Fall through if not label

            default:  // label, attribute, or expr statement
                let e = commaExpression (allowColon, allowIn);
                switch (hd ()) {
                case Token::SemiColon:
                    if (tau == interfaceBlk)
                        Parse::syntaxError(this, "Statement not allowed in interfaces");
                    eat(Token::SemiColon);
                    return [new Ast::ExprStmt (e)];

                case Token::RightBrace:
                case Token::EOS:
                    if (tau == interfaceBlk)
                        Parse::syntaxError(this, "Statement not allowed in interfaces");
                    return [new Ast::ExprStmt (e)];

                default:
                    if (newline ()) { // stmt
                        if (tau == interfaceBlk)
                            Parse::syntaxError(this, "Statement not allowed in interfaces");
                        return [new Ast::ExprStmt (e)];
                    }
                    else {
                        switch (hd ()) {
                        case Token::Dynamic:
                        case Token::Final:
                        case Token::Native:
                        case Token::Override:
                        case Token::Prototype:
                        case Token::Static:
                        case Token::Let:
                        case Token::Var:
                        case Token::Const:
                        case Token::Function:
                        case Token::Class:
                        case Token::Interface:
                        case Token::Namespace:
                        case Token::Type:
                            if (!(e is Ast::LexicalRef))
                                Parse::syntaxError(this, "Namespace or attribute name required here, not " + e);
                            // FIXME check that the namespace part was not present.
                            // FIXME there has got to be a cleaner way
                            let ie = e.Ast::ident;  
                            let attrs = defaultAttrs ();
                            attrs.ns = cx.evalIdentExprToNamespace (ie);
                            return annotatableDirective (tau,omega,attrs);

                        default:
                            Parse::internalError(this, "directive should never get here: " + lexeme());
                        }
                    }
                }
            }
        }

        // FIXME: this is wrong, it needs to have the same checks as 'directive' has.

        function annotatableDirective (tau: TAU, omega: OMEGA, attrs) : Ast::STMTS {
            switch (hd()) {
            case Token::Let: // FIXME might be function
            case Token::Var:
            case Token::Const:
                let e = variableDefinition (allowIn, tau
                                            , attrs.ns
                                            , attrs.prototype
                                            , attrs.static);
                return let (stmts = e == null ? [] : [new Ast::ExprStmt(e)]) semicolon (omega), stmts;

            case Token::Function:
                if (isCurrentClassName (lexeme2())) 
                    return constructorDefinition (omega, attrs.ns, attrs);
                return functionDefinition (tau, omega, attrs);

            case Token::Interface:
                return interfaceDefinition (attrs.ns, attrs);

            case Token::Class:
                return classDefinition (attrs.ns, attrs);

            case Token::Namespace:
                return namespaceDefinition (omega, attrs.ns);

            case Token::Type:
                return typeDefinition (omega, attrs.ns);

            default:  // label, attribute, or expr statement
                let xattrs = attribute (tau,attrs);
                if (newline ()) 
                    Parse::syntaxError(this, "error unexpected newline before " + lexeme());
                return annotatableDirective (tau,omega,xattrs);
            }
        }

        type ATTRS = Object;  // FIXME object type

        function defaultAttrs () : ATTRS {
            return { ns: cx.pragmas.defaultNamespace
                   , 'true': false
                   , 'false': false
                   , dynamic: false
                   , final: false
                   , native: false
                   , override: false
                   , prototype: false
                   , static: false }
        }

        function checkLegalAttributes(attrs, names) {
            test("true");
            test("false");
            test("dynamic");
            test("final");
            test("native");
            test("override");
            test("prototype");
            test("static");

            function test(x) {
                if (attrs[x] && !names[x])
                    Parse::syntaxError(this, "Attribute " + x + " not allowed here.");
            }
        }

        // Modifies "attrs" and returns it too.
        function attribute (tau: TAU, attrs: ATTRS) {
            switch (tau) {
            case classBlk:  return classAttr(attrs), attrs;
            case globalBlk: return globalAttr(attrs), attrs;
            case localBlk:  return attrs;
            default:        Parse::internalError(this, "error attribute tau " + tau);
            }
        }

        function classAttr(attrs: ATTRS) {
            switch (hd ()) {
            case Token::Final:
                eat(Token::Final);
                attrs.final = true;
                break;
            case Token::Native:
                eat(Token::Native);
                attrs.native = true;
                break;
            case Token::Override:
                eat(Token::Override);
                attrs.override = true;
                break;
            case Token::Prototype:
                eat(Token::Prototype);
                attrs.prototype = true;
                break;
            case Token::Static:
                eat(Token::Static);
                attrs.static = true;
                break;
            case Token::Public:
            case Token::Private:
            case Token::Protected:
            case Token::Internal:
            case Token::Intrinsic:
                attrs.ns = reservedNamespace ();
                break;
            default:
                attrs.ns = cx.evalIdentExprToNamespace (primaryName ());
                break;
            }
        }

        function globalAttr(attrs: ATTRS) {
            switch (hd ()) {
            case Token::True:
                eat(Token::True);
                attrs['true'] = true;  // ESC BUG
                break;
            case Token::False:
                eat(Token::False);
                attrs['false'] = false;  // ESC BUG
                break;
            case Token::Dynamic:
                eat(Token::Dynamic);
                attrs.dynamic = true;
                break;
            case Token::Final:
                eat(Token::Final);
                attrs.final = true;
                break;
            case Token::Native:
                eat(Token::Native);
                attrs.native = true;
                break;
            case Token::Public:
            case Token::Internal:
            case Token::Intrinsic:
                attrs.ns = reservedNamespace ();
                break;
            default:
                attrs.ns = cx.evalIdentExprToNamespace (primaryName ());
                break;
            }
        }

        // PRAGMAS

        // Works by side effect on cx.
        function pragmas () {
            while (hd () === Token::Use || hd () === Token::Import) {
                pragma ();
                semicolon (fullStmt);
            }
        }

        function pragma () {
            if (match(Token::Use)) 
                pragmaItems ();
            else if (match(Token::Import)) 
                importName ();
        }

        function pragmaItems () {
            do {
                if (match(Token::Namespace))
                    cx.openNamespace (primaryName ());
                else if (match(Token::Default)) {
                    eat(Token::Namespace);
                    let name = primaryName ();
                    cx.defaultNamespace (name);
                    cx.openNamespace (name);
                }
                else if (match(Token::Strict)) {
                    // FIXME: turn on strict mode
                }
                else
                    Parse::syntaxError(this, "Unknown token following 'use'");
            } while (match(Token::Comma));
        }

        function importName () {
            let idents = [identifier ()];
            while (hd () === Token::Dot) {
                eat(Token::Dot);
                // FIXME: This looks anything but safe, it'll accept anything
                idents.push(lexeme());
                next();
            }

            let ns = namespaceFromPath (idents);
            cx.openNamespace (ns);

            return;

            function namespaceFromPath (path) 
            {
                let str = "";
                for (let i=0; i<path.length-1; ++i) { // -1 to skip last ident
                    if (i!=0) 
                        str = str + ".";
                    str = str + path[i];
                }

                return new Ast::ReservedNamespace (new Ast::PublicNamespace (str));  // FIXME ReservedNamespace is a misnomer
            }

        }

        // BLOCKS and PROGRAMS

        function block (tau: TAU) : Ast::BLOCK {
            eat (Token::LeftBrace);
            cx.enterLetBlock ();
            let directive_list = directives (tau);
            let head = cx.exitLetBlock ();
            eat (Token::RightBrace);

            return new Ast::Block (head, directive_list);
        }

        function packages () : [PACKAGE] {
            let pkgs = [];

            // FIXME: "internal".
            do {
                let ids = [];
                eat (Token::Package);
                ids.push( identifier() );
                while (hd() == Token::Dot) {
                    eat(Token::Dot);
                    ids.push( identifier() );
                }
                pkgs.push( new Ast::Package(ids, block(globalBlk)) );
            } while (hd () == Token::Package);
            return pkgs;
        }

        // Synthesize attributes in f.

        function computeAttributes(f: FuncAttr) {
            let reify_activation = false;

            // If there's eval then variable lookup will be by name;
            // new names may be added at run-time.

            reify_activation = reify_activation || f.uses_eval;

            // If there's with then variable lookup will be by name;
            // the set of names in the object is generally unknown.
            // (It's possible to do better for objects of known
            // non-dynamic types, but that's unlikely to be a common
            // case.)

            reify_activation = reify_activation || f.uses_with;

            // If there's a nested function definition or function expression 
            // then it will close over its reified scope.
            //
            // FIXME: If the nested function has no free variables, or its
            // free variables have constant values, then optimizations are
            // possible.

            reify_activation = reify_activation || f.children.length > 0;

            // Catch/finally both imply the use of newcatch/pushscope
            // in a simplistic model.
            //
            // FIXME: In actuality, newcatch/pushscope and lookup by
            // name of the catch var are not necessary unless there is
            // a method in the catch handler that captures the
            // environment, so we can do better -- similar to "let"
            // optimization.

            reify_activation = reify_activation || (f.uses_catch || f.uses_finally); 

            // Even assuming having a nested function does not require
            // reifying the activation, do reify it if the child
            // requires its activation to be reified.

            for ( let i=0, limit=f.children.length ; i < limit ; i++ ) {
                let c = f.children[i];
                computeAttributes(c);
                reify_activation = reify_activation || c.reify_activation;
            }
            f.reify_activation = reify_activation;
        }

        function program () : Ast::PROGRAM {
            start();

            cx.enterVarBlock ();
            let publicNamespace = new Ast::ReservedNamespace (new Ast::PublicNamespace (""));
            cx.openNamespace (publicNamespace);
            cx.defaultNamespace (publicNamespace);

            let package_list;
            if (hd () == Token::Internal || hd () == Token::Package)
                package_list = packages ();
            else
                package_list = [];

            // FIXME, right now this breaks everything because of bugs elsewhere...
            currentPackageName = "" /* + Math.random() + Math.random() + Math.random() */;
            currentClassName = "";

            cx.enterLetBlock ();
            let global_directives = directives (globalBlk);
            let bhead = cx.exitLetBlock ();
            let vhead = cx.exitVarBlock ();

            if (hd () != Token::EOS)
                Parse::syntaxError(this, "extra tokens after end of program.");

            computeAttributes(cx.topFunction());

            return new Ast::Program ( package_list,
                                      new Ast::Block (bhead, global_directives),
                                      vhead,
                                      cx.topFunction(),
                                      filename );
        }
    }
}
