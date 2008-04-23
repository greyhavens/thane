/* ECMAScript support for ESC */

{
    ESC var eval_counter = 0;
    ESC var eval_hook;         // will be assigned the newly compiled function

    /* The wrapper around ESC that compiles and runs code to be eval'd.
     *
     * "scopes" must contain all scopes except the global one, which
     * is implicit.
     */
    ESC function evaluateInScopeArray(args, scopes, scopedesc) {
        use namespace Ast;

        if (args.length > 0) {
            if (args[0] is String) {
                let id = ESC::eval_counter++;
                let file = "(EVAL CODE)";
                let name = "$eval$" + id + "$";
                let parser = new Parse::Parser( args[0], ESC::getTopFixtures(), file );
                let prog = parser.Parse::program();

                // These checks could be moved into the parser, and might be, by and by.

                if (prog.packages.length != 0)
                    Util::syntaxError(file, 0, "Packages not legal.");

                for ( let fs=prog.head.fixtures, limit=fs.length, i=0 ; i < limit ; i++ ) {
                    let f = fs[i];
                    switch type (f[1]) {
                    case (mf:MethodFixture) {
                    }
                    case (vf:ValFixture) {
                    }
                    case (x:*) {
                        Util::syntaxError(file, 0, "Binding form not legal: " + x);
                    }
                    }
                }

                //Asm::listify = true;
                ESC::loadBytes(Gen::cgEval(prog, name, scopedesc).getBytes());
                //Asm::listify = false;

                return ESC::eval_hook(scopes);
            }
            else
                return args[0];
        }
        else
            return undefined;
    }

    // This is the true eval function.  Install this as "eval" in the global object.

    /*
    ESC function eval(...rest)
        ESC::evalCompiler(rest, [], "");
    */
}
