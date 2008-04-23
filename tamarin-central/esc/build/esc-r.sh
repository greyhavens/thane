echo esc-r $1
cp $1 esc-tmp.es
sml @SMLload=../bin/es4-dump.heap.x86-darwin -e ../src/es4ri/debug.es ../src/es4ri/ast.es ../src/es4ri/ast-encode.es ../src/es4ri/ast-decode.es ../src/es4ri/lex-char.es ../src/es4ri/lex-token.es ../src/es4ri/lex-scan.es ../src/es4ri/util.es ../src/es4ri/parse-util.es ../src/es4ri/parse-ident.es ../src/es4ri/parse-lhsexpr.es ../src/es4ri/parse-assignexpr.es ../src/es4ri/parse-ptrn.es ../src/es4ri/parse-type.es ../src/es4ri/parse-stmt.es ../src/es4ri/parse-defn.es ../src/es4ri/parse-prgm.es ../src/es4ri/util-es4ri.es ../src/es4ri/bytes-es4ri.es ../src/es4ri/asm.es ../src/es4ri/abc.es ../src/es4ri/emit.es ../src/es4ri/cogen.es ../src/es4ri/cogen-stmt.es ../src/es4ri/cogen-expr.es ../src/es4ri/esc-r.es

cp esc-tmp.abc $1.abc
rm esc-tmp.es
