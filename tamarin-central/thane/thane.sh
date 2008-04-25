#!/bin/sh
#

ASC_DIR=/home/zell/asc
ASC_CLASS=macromedia.asc.embedding.ScriptCompiler
ASC_OPTS="-strict -abcfuture -AS3 -outdir ."

BUILTIN=../core/builtin.abc

SOURCES="Thane.as toplevel.as Domain.as StringBuilder.as ByteArray.as Endian.as \
         ../extensions/Dictionary.as thane_core.as thane_net.as Socket.as"

java -ea -DAS3 -DAVMPLUS -DAS3 -classpath $ASC_DIR/lib/asc.jar $ASC_CLASS $ASC_OPTS \
    -import $BUILTIN -out thane $SOURCES
