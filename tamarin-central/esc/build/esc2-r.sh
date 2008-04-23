### compile .ast file to a .abc file using the RI

echo esc2-r $1.ast

cp $1.ast esc-tmp.ast
sml @SMLload=../bin/es4-dump.heap.x86-darwin -e ../src/debug.es ../src/ast.es ../src/ast-decode.es ../src/util.es ../src/util-es4ri.es ../src/bytes-es4ri.es ../src/asm.es ../src/abc.es ../src/emit.es ../src/cogen.es ../src/cogen-stmt.es ../src/cogen-expr.es ../src/abc-encode.es ../src/esc2-r.es
cp esc-tmp.asm $1.asm
rm esc-tmp.asm
