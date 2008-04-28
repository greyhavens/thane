#!/bin/sh
#

if [ -z "$1" ]; then
    echo "Usage: $0 <path to asc.jar>"
    exit 255
fi

ASC=$1
ASC_CLASS=macromedia.asc.embedding.ScriptCompiler
ASC_OPTS="-strict -abcfuture -AS3 -outdir ."

BUILTIN=../core/builtin.abc

SOURCES="Thane.as toplevel.as Domain.as StringBuilder.as flash/util/ByteArray.as Endian.as \
         ../extensions/Dictionary.as thane_core.as thane_net.as thane_geom.as \
         flash/net/Socket.as flash/util/Timer.as \
         as3ds/Heap.as as3ds/Collection.as as3ds/Iterator.as"

java -ea -DAS3 -DAVMPLUS -DAS3 -classpath $ASC $ASC_CLASS $ASC_OPTS  \
     -import $BUILTIN -out thane $SOURCES
