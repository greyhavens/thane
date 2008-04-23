### parse .es files with the RI

echo esc1-r $1
cp $1 esc-tmp.es

sml @SMLload=../bin/es4-dump.heap.x86-darwin -e ../src/debug.es ../src/util.es ../src/util-es4ri.es ../src/ast.es ../src/ast-decode.es ../src/ast-encode.es ../src/lex-char.es ../src/lex-token.es ../src/lex-scan.es ../src/parse-util.es ../src/parse-ident.es ../src/parse-lhsexpr.es ../src/parse-assignexpr.es ../src/parse-ptrn.es ../src/parse-type.es ../src/parse-stmt.es ../src/parse-defn.es ../src/parse-prgm.es ../src/esc1-r.es

cp esc-tmp.ast $1.ast
rm esc-tmp.es
rm esc-tmp.ast
