#!/usr/bin/env python
#
# ***** BEGIN LICENSE BLOCK ***** 
# Version: MPL 1.1/GPL 2.0/LGPL 2.1 
#
# The contents of this file are subject to the Mozilla Public License Version 1.1 (the 
# "License"); you may not use this file except in compliance with the License. You may obtain 
# a copy of the License at http://www.mozilla.org/MPL/ 
#
# Software distributed under the License is distributed on an "AS IS" basis, WITHOUT 
# WARRANTY OF ANY KIND, either express or implied. See the License for the specific 
# language governing rights and limitations under the License. 
#
# created by dschaffe@adobe.com for use with tamarin tests
#
# Alternatively, the contents of this file may be used under the terms of either the GNU 
# General Public License Version 2 or later (the "GPL"), or the GNU Lesser General Public 
# License Version 2.1 or later (the "LGPL"), in which case the provisions of the GPL or the 
# LGPL are applicable instead of those above. If you wish to allow use of your version of this 
# file only under the terms of either the GPL or the LGPL, and not to allow others to use your 
# version of this file under the terms of the MPL, indicate your decision by deleting provisions 
# above and replace them with the notice and other provisions required by the GPL or the 
# LGPL. If you do not delete the provisions above, a recipient may use your version of this file 
# under the terms of any one of the MPL, the GPL or the LGPL. 
# 
# ***** END LICENSE BLOCK ***** */
#
# the script is a faster way of recompiling tamarin tests.
# prerequisites: must install pexpect http://pexpect.sourceforge.net/ otherwise import pexpect will fail
# the script uses ash (actionscript interactive compiler shell) to compile tests.  there is 
# significant time savings by compiling using ash since it uses a single processes and uses
# a memory cache.
# 
# requirements: java must be in your path
#               set environment variables: ASC and GLOBALABC to the asc.jar path and the global.abc path
# usage: ./rebuildtests.py . or directory to rebuild
# 
# 
from os.path import *
from os import environ,walk,remove
from sys import argv
from glob import glob
from getopt import getopt
from datetime import datetime
import sys, string, time
import pexpect


globs = { 'asc':'', 'globalabc':''}
if 'ASC' in environ:
    globs['asc'] = environ['ASC'].strip()
if 'GLOBALABC' in environ:
	globs['globalabc'] = environ['GLOBALABC'].strip()

def usage(c):
	print 'usage: %s [options] ' % basename(argv[0])
	print ' -a --asc           compiler to use'
	print ' -g --globalabc     location of global.abc'
	print ' -h --help          display help and exit'
	print ''
	exit(c)

try:
	opts, args = getopt(argv[1:], 'a:g:h', ['asc=','globalabc=','help'])
except:
	opts = [('', '')]
	args = ['.']

if not args:
  args = ['.']
for o, v in opts:
	if o in ('-h', '--help'):
		usage(0)
	elif o in ('-a', '--asc'):
		globs['asc'] = v
	elif o in ('-g', '--globalabc'):
		globs['globalabc'] = v
     

if not isfile(globs['asc']):
	usage('ERROR: cannot build %s, ASC environment variable or --asc must be set to asc.jar' % globs['asc'])
	
if not isfile(globs['globalabc']):
	usage('ERROR: global.abc %s does not exist, GLOBALABC environment variable or --globalabc must be set to builtin.abc' % globs['globalabc'])

def istest(f):
	return f.endswith(".as") and basename(f) != "shell.as" and not f.endswith("Util.as")

def parents(d):
	while d != "":
		yield d
		d = dirname(d)
	yield d

tests = [a for a in args if isfile(a) and istest(a)]
for a in [d for d in args if isdir(d)]:
	for d, dirs, files in walk(a):
		tests += [join(d,f) for f in files if istest(f)]
		utils = [d for d in dirs if d+".as" in files]
		for x in [x for x in utils if x in dirs]:
			dirs.remove(x)

child = pexpect.spawn("java -classpath %s macromedia.asc.embedding.Shell" % globs['asc'])
child.logfile = sys.stdout
child.expect("\(ash\)")
child.expect("\(ash\)")

start_time = datetime.today()
print("starting compile of %d tests at %s" % (len(tests),start_time))
total=len(tests)
counter=0
for test in tests:
	cmd = "asc -import " + globs['globalabc']
	(dir, file) = split(test)
	#print("   compiling %s" % file)
	for p in parents(dir):
		shell = join(p,"shell.as")
		if isfile(shell):
			cmd += " -in " + shell
			break
	(testdir, ext) = splitext(test)
	deps = glob(join(testdir,"*.as"))
	deps.sort()
	for util in deps + glob(join(dir,"*Util.as")):
		cmd += " -in %s" % string.replace(util, "$", "\$")
	cmd += " %s" % test

	if exists(testdir+".abc"):
		remove(testdir+".abc")

	child.sendline(cmd)
	child.expect("\(ash\)")
	if exists(testdir+".abc")==False:
		print("ERROR abc files %s.abc not created" % (testdir))
	counter += 1;
	print("%d remaining, %s" % (total-counter,cmd))
end_time = datetime.today()
print("finished compile of %d tests at %s elapsed time is %s" % (len(tests),start_time,end_time-start_time))
