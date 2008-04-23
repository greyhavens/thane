#!/usr/bin/env python
#
# ***** BEGIN LICENSE BLOCK ***** 
# Version: MPL 1.1/GPL 2.0/LGPL 2.1 
#
# The contents of this file are subject to the Mozilla Public License Version 1.1 (the 
# 'License'); you may not use this file except in compliance with the License. You may obtain 
# a copy of the License at http://www.mozilla.org/MPL/ 
#
# Software distributed under the License is distributed on an 'AS IS' basis, WITHOUT 
# WARRANTY OF ANY KIND, either express or implied. See the License for the specific 
# language governing rights and limitations under the License. 
#
# modified by dschaffe@adobe.com for use with tamarin tests
# original file created by christine@netscape.com
#
# Alternatively, the contents of this file may be used under the terms of either the GNU 
# General Public License Version 2 or later (the 'GPL'), or the GNU Lesser General Public 
# License Version 2.1 or later (the 'LGPL'), in which case the provisions of the GPL or the 
# LGPL are applicable instead of those above. If you wish to allow use of your version of this 
# file only under the terms of either the GPL or the LGPL, and not to allow others to use your 
# version of this file under the terms of the MPL, indicate your decision by deleting provisions 
# above and replace them with the notice and other provisions required by the GPL or the 
# LGPL. If you do not delete the provisions above, a recipient may use your version of this file 
# under the terms of any one of the MPL, the GPL or the LGPL. 
# 
# ***** END LICENSE BLOCK ***** */
#
# simple script that executes tamarin certification tests.  you have to build the
# stand-alone, avmplus executable.  
# see http://developer.mozilla.org/en/docs/Tamarin_Build_Documentation 
#
# this is just a quick-n-dirty script.  for full reporting, you need to run
# the test driver, which requires java and is currently not available on
# mozilla.org.
#
# this test looks for an executable avmplus shell in
# %MOZ_SRC/mozilla/js/tamarin/platform/,
#

import os, os.path, sys, getopt, datetime, pipes, glob, itertools, tempfile, string, re, platform
from os.path import *
from os import getcwd,environ,walk
from datetime import datetime
from glob import glob
from sys import argv, exit
from getopt import getopt
from itertools import count
from subprocess import PIPE,STDOUT
from util.killableprocess import Popen

verbose = False
timestamps = True
forcerebuild = False
runESC = False
runSource = False # Run the source file (.as, .js) instead of .abc
sourceExt = '.as' # can be changed to .js, .es ...
testTimeOut = -1 #by default tests will NOT timeout
debug = False

globs = { 'avm':'', 'asc':'', 'globalabc':'', 'exclude':[],
          'config':'', 'ascargs':'', 'vmargs':'', 'escbin':''}

# default value for escbin
globs['escbin'] = '../../esc/bin/'

if 'AVM' in environ:
  globs['avm'] = environ['AVM'].strip()
if 'ASC' in environ:
  globs['asc'] = environ['ASC'].strip()
if 'GLOBALABC' in environ:
  globs['globalabc'] = environ['GLOBALABC'].strip()
if 'CVS' in environ:
  globs['exclude'] = ['CVS'].strip()
if 'CONFIG' in environ:
  globs['config'] = environ['CONFIG'].strip()
if 'ASCARGS' in environ:
  globs['ascargs'] = environ['ASCARGS'].strip()
if 'VMARGS' in environ:
  globs['vmargs'] = environ['VMARGS'].strip()
if 'ESCBIN' in environ:
  globs['escbin'] = environ['ESCBIN'].strip()

def verbose_print(m, start='', end=''):
  if verbose:
    js_print(m, start, end)

def err_print(m):
  js_print(m, '<font color=#990000>', '</font><br/>')

def js_print(m, start_tag='<p><tt>', end_tag='</tt></p>'):
  print m
  if js_output:
    js_output_f.write('%s %s %s\n' % (start_tag, m, end_tag))
    js_output_f.flush()

# yet another way to specify asc,avm,globalabc ...from a file
pf = 'runtests.properties'
if os.path.exists(pf):
  verbose_print( 'reading properties from %s' % (pf) )
  fd = open(pf,'r')
  for l in fd:
    setting = l.strip().split('=')
    if l.startswith('#') or len(setting) < 2 or len(setting[1]) <= 0:
      continue
    val = setting[1].strip()
    option = setting[0].split('.')  # see if we have x.y = z
    nm = option[0].strip()
    if len(option) > 1:
      val = globs[nm] + ' ' + val  # concat
    globs[nm] = val
  fd.close()

def usage(c):
  print 'usage: %s [options] [tests]' % basename(argv[0])
  print ' -v --verbose       enable additional output'
  print ' -E --avm           avmplus command to use'
  print ' -a --asc           compiler to use'
  print ' -g --globalabc     location of global.abc'
  print ' -x --exclude       comma separated list of directories to skip'
  print ' -h --help          display help and exit'
  print ' -t --notime        do not generate timestamps (cleaner diffs)'
  print ' -f --forcerebuild  force rebuild all test files'
  print ' -c --config        sets the config string [default OS-tvm]'
  print '    --esc           run esc instead of avm'
  print '    --escbin        location of esc/bin directory - defaults to ../../esc/bin'
  print '    --ext           set the testfile extension (defaults to .as)'
  print '    --ascargs       args to pass to asc on rebuild of test files'
  print '    --vmargs        args to pass to vm'
  print '    --timeout       max time to let a test run, in sec (default -1 = never timeout)'
  exit(c)

try:
  opts, args = getopt(argv[1:], 'vE:a:g:x:htfc:d', ['verbose','avm=','asc=','globalabc=',
                'exclude=','help','notime','forcerebuild','config=','ascargs=','vmargs=',
                'ext=','timeout=','esc','escbin='])
except:
  usage(2)

if not args:
  args = ['.']
for o, v in opts:
  if o in ('-v', '--verbose'):
    verbose = True
  elif o in ('-h', '--help'):
    usage(0)
  elif o in ('-E', '--avm'):
    globs['avm'] = v
  elif o in ('-a', '--asc'):
    globs['asc'] = v
  elif o in ('-g', '--globalabc'):
    globs['globalabc'] = v
  elif o in ('-x', '--exclude'):
    globs['exclude'] += v.split(',')
  elif o in ('-t', '--notime'):
    timestamps = False
  elif o in ('-f', '--forcerebuild'):
    forcerebuild = True
  elif o in ('-c', '--config'):
    globs['config'] = v
  elif o in ('--ascargs'):
    globs['ascargs'] = v
  elif o in ('--vmargs'):
    globs['vmargs'] = v
  elif o in ('--ext'):
    sourceExt = v
  elif o in ('--timeout'):
    testTimeOut=int(v)
  elif o in ('--esc'):
    runESC = True
  elif o in ('--escbin'):
    globs['escbin'] = v
  elif o in ('-d'):
    debug = True
      

exclude = globs['exclude']

def istest(f):
  return f.endswith(sourceExt) and basename(f) != ('shell'+sourceExt) and not f.endswith('Util'+sourceExt)

tests = [a for a in args if isfile(a) and istest(a)]
for a in [d for d in args if isdir(d) and not (basename(d) in exclude)]:
  for d, dirs, files in walk(a):
    tests += [join(d,f) for f in files if istest(f)]
    utils = [d for d in dirs if d+sourceExt in files]
    for x in [x for x in exclude+utils if x in dirs]:
      dirs.remove(x)

# set the output file name.  let's base its name on the date and platform,
# and give it a sequence number.

now = datetime.today()
for i in count(1):
  js_output = '%d-%s-%s.%d.html' % (now.year, str(now.month).zfill(2), str(now.day).zfill(2), i)
  if not isfile(js_output):
    break

print 'Writing results to %s' % js_output
js_output_f = open(js_output, 'w')

def verbose_print(m, start='', end=''):
  if verbose:
    js_print(m, start, end)

def err_print(m):
  js_print(m, '<font color=#990000>', '</font><br/>')

def js_print(m, start_tag='<p><tt>', end_tag='</tt></p>'):
  print m
  if js_output:
    js_output_f.write('%s %s %s\n' % (start_tag, m, end_tag))
    js_output_f.flush()

if timestamps:
  # get the start time
  start_time = datetime.today()
  js_print('Tamarin tests started: %s' % start_time)
#
# run the tests
#

allpasses=0
allfails=0
allunpass=0
allexpfails=0
allexceptions=0
allskips=0
alltimeouts=0
failmsgs=[]
expfailmsgs=[]
unpassmsgs=[]
timeoutmsgs=[]

#setup absolute path of base dir to not have parents go beyond that
absArgPath = abspath(args[0])

def parents(d):
  while d != absArgPath and d != '':
    yield d
    d = dirname(d)
  yield d

# run a command and return its output
def run_pipe(cmd):
  if debug:
    print('cmd: %s' % cmd)
  p = Popen((cmd), shell=True, stdout=PIPE, stderr=STDOUT)
  output = p.stdout.readlines()
  exitCode = p.wait(testTimeOut) #abort if it takes longer than 60 seconds
  if exitCode < 0:  # process timed out
    return 'timedOut'
  return output
  
def list_match(list,test):
  for k in list:
    if re.search(k,test):
      return True
  return False
  
def dict_match(dict,test,value):
  for k in dict.keys():
    if re.search(k,test):
      if dict[k].has_key(value):
        return dict[k][value]

def compile_test(as):
  asc, globalabc, ascargs = globs['asc'], globs['globalabc'], globs['ascargs']
  if not isfile(asc):
    exit('ERROR: cannot build %s, ASC environment variable or --asc must be set to asc.jar' % as)
  if not isfile(globalabc):
    exit('ERROR: global.abc %s does not exist, GLOBALABC environment variable or --globalabc must be set to global.abc' % globalabc)
  
  if asc.endswith('.jar'):
    cmd = 'java -jar ' + asc
  else:
    cmd = asc
  cmd += ' ' + ascargs
  cmd += ' -import ' + globalabc
  (dir, file) = split(as)
  verbose_print('   compiling %s' % file)
  for p in parents(dir):
    shell = join(p,'shell'+sourceExt)
    if isfile(shell):
      cmd += ' -in ' + shell
      break
  (testdir, ext) = splitext(as)
  deps = glob(join(testdir,'*'+sourceExt))
  deps.sort()
  for util in deps + glob(join(dir,'*Util'+sourceExt)):
    cmd += ' -in %s' % string.replace(util, '$', '\$')
  try:
    f = run_pipe('%s %s' % (cmd,as))
    for line in f:
      verbose_print(line.strip())
  except:
    print 'Exception'


def fail(abc, msg, failmsgs):
  msg = msg.strip()
  err_print('   %s' % msg)
  failmsgs += ['%s : %s' % (abc, msg)]

vmargs = globs['vmargs']
avm = globs['avm']
if not avm: # or not isfile(avm.split()[0]): /* isfile() fails for alias on OSX */
  exit('ERROR: cannot run %s, AVM environment variable or --avm must be set to avmplus' % avm)
js_print('Executing %d tests against vm: %s' % (len(tests), avm));

# Are we running esc - depends on a valid avm
if runESC:
  runSource = True
  # generate the executable cmd for esc
  #escAbcs = [f for f in os.listdir(globs['escbin']) if f.endswith('.abc')] #not all abcs are used for esc
  escAbcs = ['debug','util','bytes-tamarin','util-tamarin','lex-char','lex-token',
       'lex-scan','ast','ast-decode','parse','asm','abc','emit','cogen',
       'cogen-stmt','cogen-expr','esc-core','eval-support','esc-env','main']
  if not globs['escbin'].endswith('/'):
    globs['escbin'] += '/'
  for f in escAbcs:
    avm += ' %s%s.es.abc' % (globs['escbin'], f)
  avm += ' -- '
  avm += ' %s../test/spidermonkey-prefix.es' % globs['escbin']  #needed to run shell harness


def build_incfiles(as):
  files=[]
  (dir, file) = split(as)
  for p in parents(dir):
    shell = join(p,'shell'+sourceExt)
    if isfile(shell):
      files.append(shell)
  (testdir, ext) = splitext(as)
  for util in glob(join(testdir,'*'+sourceExt)) + glob(join(dir,'*Util'+sourceExt)):
    files.append(string.replace(util, "$", "\$"))
  return files



# ================================================
# Determine the configruation if it has not been 
# passed into the script:
# {CPU_ARCH}-{OS}-{VM}-{VERSION}-{VMSWITCH}
# ================================================
ostype={'CYGWIN_NT-5.1':'win','CYGWIN_NT-5.2':'win','CYGWIN_NT-5.2-WOW64':'win','CYGWIN_NT-6.0-WOW64':'win','Windows':'win','Darwin':'mac','Linux':'lnx','SunOS':'sol',}[platform.system()]
cputype={'i386':'x86','i686':'x86','i86pc':'x86','Power Macintosh':'ppc','sun4u':'x86'}[platform.machine()]

if globs['config'] == '':
  if not runSource:
    vmtype = 'release'
    f = run_pipe('%s' % avm)
    try:
      for line in f:
        if line.find('[-d]') != -1:
            vmtype = 'releasedebugger'
            break
    except:
      nop = True
      
    globs['config'] = cputype+'-'+ostype+'-tvm-'+vmtype+vmargs
    
js_print('current configuration: %s' % globs['config'])


testnum = len(tests)
for ast in tests:
  if ast.startswith('./'):
    ast=ast[2:]
  testnum -= 1
  lpass = 0
  lfail = 0
  lexpfail = 0
  lunpass = 0
  ltimeout = 0
  dir = ast[0:ast.rfind('/')]
  root,ext = splitext(ast)
  if runSource:
    testName = ast
  else:
    testName = root + '.abc'
  settings=dict()
  names=None
  lines=[]
  includes=None
  if isfile(dir+'/testconfig.txt'):
    lines=open(dir+'/testconfig.txt').read().splitlines()
    for i in range(len(lines)):
      if not lines[i].startswith('#'):
        lines[i] = '%s/%s' %(dir,lines[i])
  if isfile('./testconfig.txt'):
    for line in open('./testconfig.txt').read().splitlines():
      lines.append(line)
  for line in lines:
    if line.startswith('#') or len(line)==0:
      continue
    fields = line.split(',')
    for f in range(len(fields)):
      fields[f]=fields[f].strip()
    while len(fields)<4:
      fields.append('');
    names=fields[0].split(':')
    if len(names)==1:
      names.append('.*')
    rs='^%s$' % names[0]
    if re.search(fields[1],globs['config']) and fields[2]=='include':
      if includes==None:
        includes=[]
      includes.append(fields[0])
    if re.search(rs,root) and re.search('^%s$' % fields[1],globs['config']):
      if not settings.has_key(names[1]):
        settings[names[1]]={}
      settings[names[1]][fields[2]]=fields[3]
  if includes and not list_match(includes,root):
    continue
  js_print('%d running %s' % (testnum, ast), '<b>', '</b><br/>');
  if names and dict_match(settings,names[1],'skip'):
    js_print('  skipping')
    allskips += 1
    continue
  if forcerebuild and isfile(testName):
    os.unlink(testName)
  if not isfile(testName):
    compile_test(ast)
    if not isfile(testName):
      lfail += 1
      fail(testName, 'FAILED! file not found ' + testName, failmsgs)
  if runSource:
    incfiles=build_incfiles(testName)
    for incfile in incfiles:
      testName=incfile+" "+testName
  f = run_pipe('%s %s %s' % (avm, vmargs, testName))
  if f == "timedOut":
    fail(testName, 'Test Timed Out! Time out is set to %s s' % testTimeOut, timeoutmsgs)
    ltimeout += 1
  else:
    try:
      outputLines = []
      for line in f:
        outputLines.append(line)
        verbose_print(line.strip())
        testcase=''
        if len(line)>9:
          testcase=line.strip()
        if dict_match(settings,testcase,'skip'):
          js_print('  skipping %s' % line.strip())
          allskips+=1
          continue
        if 'PASSED!' in line:
          res=dict_match(settings,testcase,'expectedfail')
          if res:
            fail(testName, 'unexpected pass: ' + line.strip() + ' reason: '+res, unpassmsgs)
            lunpass += 1
          else:
            lpass += 1
        if 'FAILED!' in line:
          res=dict_match(settings,testcase,'expectedfail')
          if res:
            fail(testName, 'expected failure: ' + line.strip() + ' reason: '+res, expfailmsgs)
            lexpfail += 1
          else:
            lfail += 1
            fail(testName, line, failmsgs)
    except:
      print 'exception running avm'
      exit(-1)
    if lpass == 0 and lfail == 0 and lunpass==0 and lexpfail==0:
      res=dict_match(settings,'*','expectedfail')
      if res:
        fail(testName, 'expected failure: FAILED contained no testcase messages reason: %s' % res,expfailmsgs)
        lexpfail += 1
      else:
        lfail = 1
        fail(testName, '   FAILED contained no testcase messages - reason: %s' % string.join([l.strip() for l in outputLines], ' | '), failmsgs)
  allfails += lfail
  allpasses += lpass
  allexpfails += lexpfail
  allunpass += lunpass
  alltimeouts += ltimeout
  if lfail or lunpass:
    js_print('   FAILED passes:%d fails:%d unexpected passes: %d expected failures: %d' % (lpass,lfail,lunpass,lexpfail), '', '<br/>')
  else:
    js_print('   PASSED passes:%d fails:%d unexpected passes: %d expected failures: %d' % (lpass,lfail,lunpass,lexpfail), '', '<br/>')

#
# cleanup
#

if timeoutmsgs:
  js_print('\nTIMEOUTS:', '', '<br/>')
  for m in timeoutmsgs:
    js_print('  %s' % m, '', '<br/>')

if failmsgs:
  js_print('\nFAILURES:', '', '<br/>')
  for m in failmsgs:
    js_print('  %s' % m, '', '<br/>')

if expfailmsgs:
  js_print('\nEXPECTED FAILURES:', '', '<br/>')
  for m in expfailmsgs:
    js_print('  %s' % m, '', '<br/>')

if unpassmsgs:
  js_print('\nUNEXPECTED PASSES:', '', '<br/>')
  for m in unpassmsgs:
    js_print('  %s' % m, '', '<br/>')

if not allfails and not allunpass:
  js_print('\ntest run PASSED!')
else:
  js_print('\ntest run FAILED!')

if timestamps:
  end_time = datetime.today()
  js_print('Tests complete at %s' % end_time, '<hr><tt>', '</tt>')
  js_print('Start Date: %s' % start_time, '<tt><br>', '')
  js_print('End Date  : %s' % end_time, '<br>', '')
  js_print('Test Time : %s' % (end_time-start_time), '<br>', '')
js_print('passes               : %d' % allpasses, '<br>', '')
js_print('failures             : %d' % allfails, '<br>', '')
if allunpass>0:
  js_print('unexpected passes    : %d' % allunpass, '<br>', '')
if allexpfails>0:
  js_print('expected failures    : %d' % allexpfails, '<br>', '')
if allskips>0:
  js_print('tests skipped        : %d' % allskips, '<br>', '')
if allexceptions>0:
  js_print('test exceptions      : %d' % allexceptions, '<br>', '')
if alltimeouts>0:
  js_print('test timeouts        : %d' % alltimeouts, '<br>', '')

print 'Results were written to %s' % js_output

