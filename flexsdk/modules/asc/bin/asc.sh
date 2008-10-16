#!/bin/sh

################################################################################
##
##  ADOBE SYSTEMS INCORPORATED
##  Copyright 2006 Adobe Systems Incorporated
##  All Rights Reserved.
##
##  NOTICE: Adobe permits you to use, modify, and distribute this file
##  in accordance with the terms of the license agreement accompanying it.
##
################################################################################

#
# fdb launch script for unix.  On windows, fdb.exe is used and
# java settings are managed in jvm.config in this directory.
#

case `uname` in
		CYGWIN*)
		   OS="Windows"
		   ;;
		*)
		   OS=Unix
esac

if [ $OS = "Windows" ]; then
	# set FLEX_HOME relative to fdb if not set
	test "$FLEX_HOME" = "" && {
    	FLEX_HOME=`dirname $0`/..
    	FLEX_HOME=`cygpath -m $FLEX_HOME`
	}

	VMARGS="-ea -Dapplication.home=$FLEX_HOME -Xms32m -Xmx384m"

	java $VMARGS -jar $FLEX_HOME/lib/fdb.jar $*
fi

if [ $OS = "Unix" ]; then
	# set FLEX_HOME relative to fdb if not set
	test "$FLEX_HOME" = "" && {
    	FLEX_HOME=`dirname $0`/..
	}

	VMARGS="-ea -Dapplication.home=$FLEX_HOME -Xms32m -Xmx384m"

	java $VMARGS -jar $FLEX_HOME/lib/asc.jar $*
fi
