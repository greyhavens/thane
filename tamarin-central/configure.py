#!/usr/bin/env python
# -*- Mode: Python; indent-tabs-mode: nil -*-
# vi: set ts=4 sw=4 expandtab:
# ***** BEGIN LICENSE BLOCK *****
# Version: MPL 1.1/GPL 2.0/LGPL 2.1
#
# The contents of this file are subject to the Mozilla Public License Version
# 1.1 (the "License"); you may not use this file except in compliance with
# the License. You may obtain a copy of the License at
# http://www.mozilla.org/MPL/
#
# Software distributed under the License is distributed on an "AS IS" basis,
# WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
# for the specific language governing rights and limitations under the
# License.
#
# The Original Code is [Open Source Virtual Machine].
#
# The Initial Developer of the Original Code is
# Adobe System Incorporated.
# Portions created by the Initial Developer are Copyright (C) 2005-2006
# the Initial Developer. All Rights Reserved.
#
# Contributor(s):
#
# Alternatively, the contents of this file may be used under the terms of
# either the GNU General Public License Version 2 or later (the "GPL"), or
# the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
# in which case the provisions of the GPL or the LGPL are applicable instead
# of those above. If you wish to allow use of your version of this file only
# under the terms of either the GPL or the LGPL, and not to allow others to
# use your version of this file under the terms of the MPL, indicate your
# decision by deleting the provisions above and replace them with the notice
# and other provisions required by the GPL or the LGPL. If you do not delete
# the provisions above, a recipient may use your version of this file under
# the terms of any one of the MPL, the GPL or the LGPL.
#
# ***** END LICENSE BLOCK *****

#
# This script runs just like a traditional configure script, to do configuration
# testing and makefile generation.

import os.path
import sys

thisdir = os.path.dirname(os.path.abspath(__file__))

# Look for additional modules in our build/ directory.
sys.path.append(thisdir)

from build.configuration import *
import build.getopt

o = build.getopt.Options()

config = Configuration(thisdir, options = o,
                       sourcefile = 'core/avmplus.h')

buildTamarin = o.getBoolArg('tamarin', True)
if buildTamarin:
    config.subst("ENABLE_TAMARIN", 1)

buildShell = o.getBoolArg("shell", False)
if (buildShell):
    config.subst("ENABLE_SHELL", 1)
buildThane = o.getBoolArg("thane", False)
if (buildThane):
    config.subst("ENABLE_THANE", 1)


APP_CPPFLAGS = ""
APP_CXXFLAGS = ""
OPT_CXXFLAGS = "-O3 "
OPT_CPPFLAGS = ""
DEBUG_CPPFLAGS = "-DDEBUG -D_DEBUG "
DEBUG_CXXFLAGS = ""
DEBUG_LDFLAGS = ""
OS_LIBS = []
OS_LDFLAGS = ""
MMGC_CPPFLAGS = ""
AVMSHELL_CPPFLAGS = ""
AVMSHELL_LDFLAGS = ""
MMGC_DEFINES = {'SOFT_ASSERTS': None}
NSPR_INCLUDES = ""
NSPR_LDOPTS = ""

selfTest = o.getBoolArg("selftests", False)
if selfTest:
    APP_CPPFLAGS += "-DAVMPLUS_SELFTEST "

memoryProfiler = o.getBoolArg("memory-profiler", False)
if memoryProfiler:
    APP_CPPFLAGS += "-DMMGC_MEMORY_PROFILER "

MMGC_INTERIOR_PTRS = o.getBoolArg('mmgc-interior-pointers', False)
if MMGC_INTERIOR_PTRS:
    MMGC_DEFINES['MMGC_INTERIOR_PTRS'] = None

MMGC_DYNAMIC = o.getBoolArg('mmgc-shared', False)
if MMGC_DYNAMIC:
    MMGC_DEFINES['MMGC_DLL'] = None
    MMGC_CPPFLAGS += "-DMMGC_IMPL "

MMGC_THREADSAFE = o.getBoolArg('threadsafe-mmgc', False)
if MMGC_THREADSAFE:
    MMGC_DEFINES['MMGC_THREADSAFE'] = None
    NSPR_INCLUDES = o.getStringArg('nspr-includes')
    MMGC_CPPFLAGS += NSPR_INCLUDES + " "
    APP_CPPFLAGS += NSPR_INCLUDES + " "

    NSPR_LDOPTS = o.getStringArg('nspr-ldopts')
    OS_LDFLAGS += " " + NSPR_LDOPTS

os, cpu = config.getTarget()

if config.getCompiler() == 'GCC':
    APP_CXXFLAGS = "-fstrict-aliasing -Wextra -Wall -Wno-reorder -Wno-switch -Wno-invalid-offsetof -Wsign-compare -Wunused-parameter -fmessage-length=0 -fno-rtti -fno-exceptions "
    if config.getDebug():
        APP_CXXFLAGS += ""
    else:
        APP_CXXFLAGS += "-Wuninitialized "
    DEBUG_CXXFLAGS += "-g "
elif config.getCompiler() == 'VS':
    if cpu == "arm":
        APP_CXXFLAGS = "-W4 -WX -wd4291 -wd4201 -wd4189 -wd4740 -wd4127 -fp:fast -GF -GS- -Zc:wchar_t- "
        OS_LDFLAGS += "-MAP "
        if config.getDebug():
            DEBUG_CXXFLAGS = "-Od "
            APP_CXXFLAGS += "-GR- -fp:fast -GS- -Zc:wchar_t- -Zc:forScope "
        else:
            OPT_CXXFLAGS = "-O2 -GR- "
    else:
        APP_CXXFLAGS = "-W4 -WX -wd4291 -GF -fp:fast -GS- -Zc:wchar_t- "
        OS_LDFLAGS += "-SAFESEH:NO -MAP "
        if config.getDebug():
            DEBUG_CXXFLAGS = "-Od -EHsc "
        else:
            OPT_CXXFLAGS = "-O2 -Ob1 -GR- "
        if memoryProfiler:
            OPT_CXXFLAGS += "-Oy- -Zi "
    DEBUG_CXXFLAGS += "-Zi "
    DEBUG_LDFLAGS += "-DEBUG "
elif config.getCompiler() == 'SunStudio':
    OPT_CXXFLAGS = "-xO5 "
    DEBUG_CXXFLAGS += "-g "
else:
    raise Exception('Unrecognized compiler: ' + config.getCompiler())

zlib_include_dir = o.getStringArg('zlib-include-dir')
if zlib_include_dir is not None:
    AVMSHELL_CPPFLAGS += "-I%s " % zlib_include_dir

zlib_lib = o.getStringArg('zlib-lib')
if zlib_lib is not None:
    AVMSHELL_LDFLAGS = zlib_lib
else:
    AVMSHELL_LDFLAGS = '$(call EXPAND_LIBNAME,z)'


if os == "darwin":
    AVMSHELL_LDFLAGS += " -exported_symbols_list "  + thisdir + "/platform/mac/shell/exports.exp"
    MMGC_DEFINES.update({'TARGET_API_MAC_CARBON': 1,
                         'DARWIN': 1,
                         '_MAC': None,
                         'AVMPLUS_MAC': None,
                         'TARGET_RT_MAC_MACHO': 1})
    APP_CXXFLAGS += "-fpascal-strings -faltivec -fasm-blocks "
    if cpu == 'x86_64' or cpu == 'ppc64' or o.getBoolArg("leopard"):
        # use --enable-leopard to build for 10.5 or later; this is mainly useful for enabling
        # us to build with gcc4.2 (which requires the 10.5 sdk), since it has a slightly different
        # set of error & warning sensitivities. Note that we don't override CC/CXX here, the calling script
        # is expected to do that if desired (thus we can support 10.5sdk with either 4.0 or 4.2)
        APP_CXXFLAGS += "-mmacosx-version-min=10.5 -isysroot /Developer/SDKs/MacOSX10.5.sdk "
        config.subst("MACOSX_DEPLOYMENT_TARGET",10.5)
    else:
        APP_CXXFLAGS += "-mmacosx-version-min=10.4 -isysroot /Developer/SDKs/MacOSX10.4u.sdk "
        config.subst("MACOSX_DEPLOYMENT_TARGET",10.4)
elif os == "freebsd":
    MMGC_DEFINES.update({
            'LINUX' :None,
            'HAVE_PTHREAD_NP_H' :None,
            'UNIX': None,
            'AVMPLUS_UNIX' :None })
    OS_LIBS.append('pthread')
    APP_CPPFLAGS += '-DAVMPLUS_CDECL '
elif os == "windows" or os == "cygwin":
    MMGC_DEFINES.update({'WIN32': None,
                         '_CRT_SECURE_NO_DEPRECATE': None})
    OS_LDFLAGS += "-MAP "
    if cpu == "arm":
        APP_CPPFLAGS += "-DARM -D_ARM_ -DARMV5 -DUNICODE -DUNDER_CE=1 -DMMGC_ARM -QRarch5t "
        OS_LIBS.append('mmtimer corelibc coredll')
    else:
        APP_CPPFLAGS += "-DWIN32_LEAN_AND_MEAN -D_CONSOLE "
        OS_LIBS.append('winmm')
        OS_LIBS.append('shlwapi')
elif os == "linux":
    MMGC_DEFINES.update({'UNIX': None,
                         'AVMPLUS_UNIX': None,
                         'LINUX': None})
    OS_LIBS.append('pthread')
    APP_CPPFLAGS += '-DAVMPLUS_CDECL '
    if cpu == "x86_64":
        # workaround https://bugzilla.mozilla.org/show_bug.cgi?id=467776
        OPT_CXXFLAGS += '-fno-schedule-insns2 '
        # these warnings are too noisy
        APP_CXXFLAGS += ' -Wno-parentheses -Wno-c++0x-compat -Wno-empty-body '
    if config.getDebug():
        OS_LIBS.append("dl")
elif os == "sunos":
    if config.getCompiler() != 'GCC':
        APP_CXXFLAGS = ""
        OPT_CXXFLAGS = "-xO5 "
        DEBUG_CXXFLAGS = "-g "
    MMGC_DEFINES.update({'UNIX': None,
                         'AVMPLUS_UNIX': None,
                         'SOLARIS': None})
    OS_LIBS.append('pthread')
    APP_CPPFLAGS += '-DAVMPLUS_CDECL '
    if config.getDebug():
        OS_LIBS.append("dl")
else:
    raise Exception("Unsupported OS")

if cpu == "i686":
    if config.getCompiler() == 'GCC' and os == 'darwin':
        #only mactel always has sse2
        APP_CPPFLAGS += "-msse2 "
elif cpu == "powerpc":
    # we detect this in core/avmbuild.h and MMgc/*build.h
    None
elif cpu == "ppc64":
    # we detect this in core/avmbuild.h and MMgc/*build.h
    None
elif cpu == "sparc":
    APP_CPPFLAGS += "-DAVMPLUS_SPARC "
elif cpu == "x86_64":
    # we detect this in core/avmbuild.h and MMgc/*build.h
    None
elif cpu == "arm":
    # we detect this in core/avmbuild.h and MMgc/*build.h
    None
else:
    raise Exception("Unsupported CPU")

if o.getBoolArg("selftests"):
    APP_CPPFLAGS += "-DAVMPLUS_SELFTEST "

if o.getBoolArg("debugger"):
    APP_CPPFLAGS += "-DDEBUGGER "

if o.getBoolArg('perfm'):
    APP_CPPFLAGS += "-DPERFM "
    
if o.getBoolArg('disable-nj'):
    APP_CPPFLAGS += '-DAVMPLUS_DISABLE_NJ '

if o.getBoolArg('abc-interp'):
    APP_CPPFLAGS += '-DAVMPLUS_ABC_INTERPRETER '

if o.getBoolArg('selftest'):
    APP_CPPFLAGS += '-DAVMPLUS_SELFTEST '

# We do two things with MMGC_DEFINES: we append it to APP_CPPFLAGS and we also write MMgc-config.h
APP_CPPFLAGS += ''.join(val is None and ('-D%s ' % var) or ('-D%s=%s ' % (var, val))
                        for (var, val) in MMGC_DEFINES.iteritems())

definePattern = \
"""#ifndef %(var)s
#define %(var)s %(val)s
#endif
"""

outpath = "%s/MMgc-config.h" % config.getObjDir()
contents = ''.join(definePattern % {'var': var,
                                    'val': val is not None and val or ''}
                   for (var, val) in MMGC_DEFINES.iteritems())
writeFileIfChanged(outpath, contents)

config.subst("APP_CPPFLAGS", APP_CPPFLAGS)
config.subst("APP_CXXFLAGS", APP_CXXFLAGS)
config.subst("OPT_CPPFLAGS", OPT_CPPFLAGS)
config.subst("OPT_CXXFLAGS", OPT_CXXFLAGS)
config.subst("DEBUG_CPPFLAGS", DEBUG_CPPFLAGS)
config.subst("DEBUG_CXXFLAGS", DEBUG_CXXFLAGS)
config.subst("DEBUG_LDFLAGS", DEBUG_LDFLAGS)
config.subst("OS_LIBS", " ".join(OS_LIBS))
config.subst("OS_LDFLAGS", OS_LDFLAGS)
config.subst("MMGC_CPPFLAGS", MMGC_CPPFLAGS)
config.subst("AVMSHELL_CPPFLAGS", AVMSHELL_CPPFLAGS)
config.subst("AVMSHELL_LDFLAGS", AVMSHELL_LDFLAGS)
config.subst("MMGC_DYNAMIC", MMGC_DYNAMIC and 1 or '')
config.generate("Makefile")

o.finish()

