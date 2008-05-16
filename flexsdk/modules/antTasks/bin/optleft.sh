#!/bin/bash

# Set this variable to the location of flexBuild in the local client

flexBuildHome="/c/perforceDepot/depot/skunkworks/seliopou/flexBuild"

srcDir="$flexBuildHome/src"
sedFile="$flexBuildHome/bin/options.sed"

willNotImplement=3
# -dump-config
# -help
# -version

multipleMatches=2
# -externs  matches -load-externs
# -debug    matches -debug-password

options=`mxmlc -help advanced | sed -f $sedFile`

# Compute the number of options mxmlc accepts

optCount=`echo "$options" | wc -l`

# Compute the number of options that are present in the source directory

completeCount=`echo "$options" | xargs -I {} grep -R {} $srcDir | grep "new OptionSpec" | wc -l`

echo $(($optCount-$willNotImplement-$multipleMatches-$completeCount))
