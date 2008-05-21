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
# The Original Code is open source virtual machine.
#
# The Initial Developer of the Original Code is
# dschaffe@adobe.com
# Portions created by the Initial Developer are Copyright (C) 2008
# the Initial Developer. All Rights Reserved.
#
# Contributor(s):
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
# ***** END LICENSE BLOCK ***** 
#
# simple script that executes tests on tamarin-central or tamarin-tracing vm
#
import os, os.path, sys, getopt, datetime, pipes, glob, itertools, tempfile, string, re, platform, time, subprocess, socket
from os.path import *
from os import getcwd,environ,walk
from datetime import datetime
from glob import glob
from sys import argv, exit
from getopt import getopt
from itertools import count
from platform import *
from socket import *

verbose = False
timestamps = True
forcerebuild = False
logresults = False

# needed for pipe
fd,tmpfile = tempfile.mkstemp()
os.close(fd)
js_output_f=False

globs = { 'avm':'','avm2':'', 'asc':'', 'globalabc':'', 'exclude':[], 'tmpfile':tmpfile, 'config':'sunspider', 'ascargs':'', 'vmargs':'', 'vmargs2':'', 'vmname':'unknown', 'vmversion':''}
if 'AVM' in environ:
	globs['avm'] = environ['AVM'].strip()
if 'AVM2' in environ:
	globs['avm2'] = environ['AVM2'].strip()
if 'ASC' in environ:
	globs['asc'] = environ['ASC'].strip()
if 'GLOBALABC' in environ:
	globs['globalabc'] = environ['GLOBALABC'].strip()
if 'ASCARGS' in environ:
	globs['ascargs'] = environ['ASCARGS'].strip()
if 'VMARGS' in environ:
	globs['vmargs'] = environ['VMARGS'].strip()
if 'VMARGS2' in environ:
	globs['vmargs2'] = environ['VMARGS2'].strip()

def log_print(m):
	print(m)
	if (js_output_f):
		js_output_f.write("%s\n" % m)
		js_output_f.flush()


def verbose_print(m):
	if verbose:
		log_print(m)

def usage(c):
	print "usage: %s [options] [tests]" % basename(argv[0])
	print " -v --verbose       enable additional output"
	print " -E --avm           avmplus command to use"
	print " -S --avm2          second avmplus command to use"
	print " -a --asc           compiler to use"
	print " -c --config        configuration to use with testconfig.txt"
	print " -g --globalabc     location of global.abc"
	print " -h --help          display help and exit"
	print " -f --forcerebuild  force rebuild all test files"
	print " -l --log           logs results to a file"
	print " -d --socketlog     logs results to a socket server"
	print " -r --runtime	   name of the runtime VM used, including switch info eg. TTVMi (tamarin-tracing interp)"
	print "    --vmversion	   specify vmversion e.g. 502, use this if cannot be calculated from executable"
	print "    --ascargs	   args to pass to asc on rebuild of test files"
	print "    --vmargs	       args to pass to vm"
	print "    --vmargs2	   args to pass to avm2, if not specified --vmargs will be used"
	exit(c)

try:
	opts, args = getopt(argv[1:], "vE:S:a:g:hfc:ldr:", ["verbose","avm=","asc=","globalabc=","help","forcerebuild","ascargs=","vmargs=","log","socketlog","avm2=","vmargs2=","config=","runtime=","vmversion="])
except:
	usage(2)

if not args:
	args = ["."]
for o, v in opts:
	if o in ("-v", "--verbose"):
		verbose = True
	elif o in ("-h", "--help"):
		usage(0)
	elif o in ("-E", "--avm"):
		globs['avm'] = v
	elif o in ("-S", "--avm2"):
		globs['avm2'] = v
	elif o in ("-a", "--asc"):
		globs['asc'] = v
	elif o in ("-g", "--globalabc"):
		globs['globalabc'] = v
	elif o in ("-x", "--exclude"):
		globs['exclude'] += v.split(",")
	elif o in ("-t", "--notime"):
		timestamps = False
	elif o in ("-f", "--forcerebuild"):
		forcerebuild = True
	elif o in ("-c", "--config"):
		globs['config'] = v
	elif o in ("--ascargs"):
		globs['ascargs'] = v
	elif o in ("--vmargs"):
		globs['vmargs'] = v
	elif o in ("--vmargs2"):
		globs['vmargs2'] = v
	elif o in ("-l","--log"):
		now = datetime.today()
		for i in count(1):
			js_output = "%d-%s-%s.%d.log" % (now.year, str(now.month).zfill(2), str(now.day).zfill(2), i)
			if not isfile(js_output):
				break
		print "Writing results to %s" % js_output
		js_output_f = open(js_output, "w")
	elif o in ("-d", "--socketlog"):
		logresults = True
	elif o in ("-r", "--runtime"):
		globs['vmname'] = v
	elif o in ("--vmversion"):
		globs['vmversion'] = v

def istest(f):
	return f.endswith(".as")

tests = [a for a in args if isfile(a) and istest(a)]
for a in [d for d in args if isdir(d)]:
	for d, dirs, files in walk(a):
		tests += [join(d,f) for f in files if istest(f)]
		utils = [d for d in dirs if d+".as" in files]

def parents(d):
	while d != "":
		yield d
		d = dirname(d)
	yield d

# run a command and return its output
def run_pipe(cmd):
	#verbose_print(cmd)
	#cmds = cmd.split()
	#proc = subprocess.Popen([cmds[0],cmds[1]], stdout=subprocess.PIPE, shell=False)
	#proc.wait()
	#return proc.stdout
	t = pipes.Template()
	t.append("%s 2>&1" % cmd, "--")
	verbose_print(cmd)
	return t.open(globs['tmpfile'], "r")

def compile_test(as):
	asc, globalabc, ascargs = globs['asc'], globs['globalabc'], globs['ascargs']
	if not isfile(asc):
		exit("ERROR: cannot build %s, ASC environment variable or --asc must be set to asc.jar" % as)
	if not isfile(globalabc):
		exit("ERROR: global.abc %s does not exist, GLOBALABC environment variable or --globalabc must be set to global.abc" % globalabc)
	
	if asc.endswith(".jar"):
		cmd = "java -jar " + asc
	else:
		cmd = asc
	cmd += " " + ascargs
	cmd += " -import " + globalabc
	(dir, file) = split(as)
	verbose_print("   compiling %s" % file)
	(testdir, ext) = splitext(as)
	try:
		f = run_pipe("%s %s" % (cmd,as))
		for line in f:
			verbose_print(line.strip())
	finally:
		f.close()


# ================================================
# Determine the version number associated with the 
# VM, will be cyclone if not from build machine
# ================================================
VM_version = ''
VM_name = globs['vmname']

if globs['vmversion']:
	VM_version=globs['vmversion']
else:
    f = run_pipe("%s" % globs['avm'])
    try:
	    for line in f:
		    version = line.split()
		    version = version[len(version)-1]
		    if version.find(":") != -1:		# if this is an actual build we need to remove the hash
			    version = version.split(":")
			    version = version[0]
		    VM_version = version
		    break
    except:
	    nop = True

OS_name = {'CYGWIN_NT-5.1':'WIN','CYGWIN_NT-5.2':'WIN','CYGWIN_NT-6.0-WOW64':'WIN','Windows':'WIN','Darwin':'MAC','Linux':'LNX','SunOS':'SOL',}[system()]
VM_reporting = '%s %s %s' % (VM_name,OS_name,VM_version)
# ================================================
# Prepare the data to be sent through a socket to 
# be recorded to the database
# ================================================
serverHost = '10.60.48.47'
serverPort = 1188
# ===========================================
# If logging is enabled, log results to 
# performance database
# ===========================================
def socketlog(msg):
	if logresults:
		s = socket(AF_INET, SOCK_STREAM)    # create a TCP socket
		s.connect((serverHost, serverPort)) # connect to server on the port
		s.send("%s;exit" % msg)             # send the data
		data = s.recv(1024)
		#print('Sent: %s' % msg)
		#print('Received: %s \n\n' % data)
		s.shutdown(SHUT_RDWR)
		s.close()




skips=[]
if isfile('./testconfig.txt'):
	for line in open('./testconfig.txt').read().splitlines():
		fields = line.split(',')
		for f in range(len(fields)):
			fields[f]=fields[f].strip()
		if len(fields)<3:
			continue
		if re.match(fields[1],globs['config'])==None:
			continue
		if fields[2]=='skip':
			skips.append(fields[0])
vmargs = globs['vmargs']
avm = globs['avm']
avm2 = globs['avm2']
if not avm: # or not isfile(avm.split()[0]): /* isfile() fails for alias on OSX */
	exit("ERROR: cannot run %s, AVM environment variable or --avm must be set to avmplus" % avm)
log_print("Executing tests at %s" % (datetime.now()))
log_print("avm: %s" % avm);
if len(avm2)>0:
	log_print("avm2: %s" % avm2);

if len(avm2)>0:
	log_print("\n\n%-50s %7s %7s\n" % ("test","avm","avm2"));
else:
	log_print("\n\n%-50s %7s\n" % ("test","avm"));
testnum = len(tests)
for ast in tests:
	if ast.startswith("./"):
		ast=ast[2:]
	skip=False
	for sk in skips:
		if re.match("^%s$" % sk,ast):
			verbose_print("skipping %s" % ast)
			testnum-=1
			skip=True
			break
	if skip:
		continue
	testnum -= 1
	dir =ast[0:ast.rfind('/')]
	root,ext = splitext(ast)
	tname = root[root.rfind('/')+1:]
	abc = "%s.abc" % root
	verbose_print("%d running %s" % (testnum, ast));
	if forcerebuild and isfile(abc):
		os.unlink(abc)
	if not isfile(abc):
		compile_test(ast)
		if not isfile(abc):
			log_print("compile FAILED!, file not found " + abc)
	f1 = run_pipe("%s %s %s" % (avm, vmargs, abc))
	if len(avm2)>0:
		if len(vmargs)>0:
			f2 = run_pipe("%s %s %s" % (avm2, vmargs2, abc))
		else:
			f2 = run_pipe("%s %s %s" % (avm2, vmargs, abc))
	try:
		result1="na"
		result2="na"
		for line in f1:
			if "metric" in line:
				result1list=line.rsplit()
				if len(result1list)>2:
					result1=result1list[2]
		if len(avm2)>0:
			for line in f2:
				if "metric" in line:
					result2list=line.rsplit()
					if len(result2list)>2:
						result2=result2list[2]
		if len(avm2)>0:
			log_print("%-50s %7s %7s" % (ast,result1,result2)) 
		else:
			socketlog("addresult::%s::time::%s::%s;" % (ast,result1,VM_reporting))
			log_print("%-50s %7s" % (ast,result1)) 
	except:
		log_print("exception")
		exit(-1)

