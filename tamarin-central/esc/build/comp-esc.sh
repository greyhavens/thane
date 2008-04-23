# Recompile the compiler using the compiler in ../bin, compiling everything.
# This typically takes a time; in many cases, running "make" is faster (but not always what you want).
#
# Please update Makefile if you add or remove files below.

./esc.sh ../src/debug.es
./esc.sh ../src/util.es
./esc.sh ../src/util-tamarin.es
./esc.sh ../src/lex-char.es
./esc.sh ../src/lex-token.es
./esc.sh ../src/lex-scan.es
./esc.sh ../src/ast.es
./esc.sh ../src/ast-decode.es
./esc.sh ../src/ast-encode.es
./esc.sh ../src/parse.es
./esc.sh ../src/bytes-tamarin.es
./esc.sh ../src/asm.es
./esc.sh ../src/abc.es
./esc.sh ../src/abc-encode.es
./esc.sh ../src/abc-encode-cpp.es
./esc.sh ../src/abc-decode.es
./esc.sh ../src/abc-parse.es
./esc.sh ../src/emit.es
./esc.sh ../src/cogen.es
./esc.sh ../src/cogen-stmt.es
./esc.sh ../src/cogen-expr.es
./esc.sh ../src/esc1-t.es
./esc.sh ../src/esc2-t.es
./esc.sh ../src/esc3-t.es
./esc.sh ../src/abcenc.es
./esc.sh ../src/abcdec.es
./esc.sh ../src/esc.es
./esc.sh ../src/main.es

cp ../src/debug.es.abc ../bin
cp ../src/util.es.abc ../bin
cp ../src/util-tamarin.es.abc ../bin
cp ../src/lex-char.es.abc ../bin
cp ../src/lex-token.es.abc ../bin
cp ../src/lex-scan.es.abc ../bin
cp ../src/ast.es.abc ../bin
cp ../src/ast-decode.es.abc ../bin
cp ../src/ast-encode.es.abc ../bin
cp ../src/parse.es.abc ../bin
cp ../src/bytes-tamarin.es.abc ../bin
cp ../src/asm.es.abc ../bin
cp ../src/abc.es.abc ../bin
cp ../src/abc-encode.es.abc ../bin
cp ../src/abc-encode-cpp.es.abc ../bin
cp ../src/abc-decode.es.abc ../bin
cp ../src/abc-parse.es.abc ../bin
cp ../src/emit.es.abc ../bin
cp ../src/cogen.es.abc ../bin
cp ../src/cogen-stmt.es.abc ../bin
cp ../src/cogen-expr.es.abc ../bin
cp ../src/esc1-t.es.abc ../bin
cp ../src/esc2-t.es.abc ../bin
cp ../src/esc3-t.es.abc ../bin
cp ../src/abcenc.es.abc ../bin
cp ../src/abcdec.es.abc ../bin
cp ../src/esc.es.abc ../bin
cp ../src/main.es.abc ../bin

