#!/bin/bash

################################################################################
##
##  ADOBE SYSTEMS INCORPORATED
##  Copyright 2007 Adobe Systems Incorporated
##  All Rights Reserved.
##
##  NOTICE: Adobe permits you to use, modify, and distribute this file
##  in accordance with the terms of the license agreement accompanying it.
##
################################################################################

(set -o igncr) 2>/dev/null && set -o igncr; # comment is needed

export JAVA_HOME="c:/Program Files/Java/jdk1.5.0_09"
export ANT_HOME="c:/tools/apache-ant-1.6.5"
export PATH="/c/Program Files/Java/jdk1.5.0_09/bin:/c/tools/apache-ant-1.6.5/bin:$PATH"
FILEDROP=/w/builds/as

basedir=`pwd`

export ASC=c:/lib/asc.jar
export GLOBALABC=c:/lib/global.abc
export AVM=c:/lib/avmshell.exe
avmshellversion=`$AVM | head -1 | awk '{ print $5 }'`
ascversion=`java -jar $ASC | head -2 | tail -1 | awk '{ printf("%s %s",$4,$5) }'`
echo "avm-version $avmshellversion"
echo "asc-version $ascversion"
cd /c/hg/js/tamarin-buildbot/test
./rebuildtests.py .
./runtests.py .
