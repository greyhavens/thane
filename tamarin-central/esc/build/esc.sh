# Translate .es to .es.abc using esc on tamarin
#
# You must first build a tamarin executable and store it under the
# name "shell" (or "shell.exe") in the directory $DIR.
#
# usage:
#   esc.sh <filename>
#
# esc.sh must be run from $DIR, or you must change the value of
# $DIR to be the absolute path of the bin directory (that has the
# shell and the abc files for ESC).

DIR=../bin

$DIR/shell \
    $DIR/debug.es.abc \
    $DIR/util.es.abc \
    $DIR/bytes-tamarin.es.abc \
    $DIR/util-tamarin.es.abc \
    $DIR/lex-char.es.abc \
    $DIR/lex-token.es.abc \
    $DIR/lex-scan.es.abc \
    $DIR/ast.es.abc \
    $DIR/parse.es.abc \
    $DIR/asm.es.abc \
    $DIR/abc.es.abc \
    $DIR/emit.es.abc \
    $DIR/cogen.es.abc \
    $DIR/cogen-stmt.es.abc \
    $DIR/cogen-expr.es.abc \
    $DIR/esc-core.es.abc \
    $DIR/eval-support.es.abc \
    $DIR/esc-env.es.abc \
    $DIR/esc.es.abc \
    -- $@
