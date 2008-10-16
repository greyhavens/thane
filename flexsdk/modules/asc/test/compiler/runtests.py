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
# created by dschaffe@adobe.com
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

import os, os.path, sys, getopt, datetime, pipes, glob, itertools, re, tempfile, string
from os.path import *
from os import getcwd,environ,walk
from datetime import datetime
from glob import glob
from sys import argv, exit
from getopt import getopt
from itertools import count


verbose = False
timestamps = True

# needed for pipe
fd,tmpfile = tempfile.mkstemp()
os.close(fd)

globs = { 'avm':'', 'asc':'', 'globalabc':'', 'playerglobalabc':'', 'exclude':[], 'tmpfile':tmpfile, 'config':'asc-smokes', 'failmsgs':[],'expfailmsgs':[],
          'unpassmsgs':[],'allfails':0,'allexpfails':0,'allunpass':0,'allpasses':0,'allskips':0, 'regexOutput':False, 'toplevelabc':'', 'full':False}

globs['toplevelabc'] = abspath(abspath(dirname(sys.argv[0]))+'/../../abc/toplevel.abc')

if 'AVM' in environ:
  globs['avm'] = environ['AVM']
if 'ASC' in environ:
  globs['asc'] = environ['ASC']
if 'GLOBALABC' in environ:
  globs['globalabc'] = environ['GLOBALABC']
if 'PLAYERGLOBALABC' in environ:
  globs['playerglobalabc'] = environ['PLAYERGLOBALABC']
if 'CVS' in environ:
  globs['exclude'] = ['CVS']
if 'CONFIG' in environ:
  globs['config'] = environ['CONFIG']
if 'TOPLEVELABC' in environ:
  globs['toplevelabc'] = environ['TOPLEVELABC']

def verbose_print(m, start="", end=""):
  if verbose:
    js_print(m, start, end)

def err_print(m):
  js_print(m, "<font color=#990000>", "</font><br/>")

def js_print(m, start_tag="<p><tt>", end_tag="</tt></p>"):
  print m
  if js_output:
    js_output_f.write("%s %s %s\n" % (start_tag, m, end_tag))
    js_output_f.flush()

# yet another way to specify asc,avm,globalabc ...from a file
pf = 'runtests.properties'
if os.path.exists(pf):
  verbose_print( "reading properties from file '%s'" % (pf) )
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
  print "usage: %s [options] [tests]" % basename(argv[0])
  print " -v --verbose         enable additional output"
  print " -E --avm             avmplus command to use"
  print " -a --asc             compiler to use"
  print " -g --globalabc       location of global.abc"
  print " -p --playerglobalabc location of playerglobal.abc"
  print "    --toplevelabc     location of toplevel.abc"
  print " -f --full            do a full coverage pass (all switches and options exercised"
  print " -x --exclude         comma separated list of directories to skip"
  print " -h --help            display help and exit"
  print " -i --intermediate    create and compare intermediate code/AVM assembly/parse tree"
  print " -t --notime          do not generate timestamps (cleaner diffs)"
  print " -c --config          sets the config string [default qvm]"
  print " -r --regex           output err.actual files that are regex corrected"
  exit(c)

try:
  opts, args = getopt(argv[1:], "vE:a:g:x:htc:p:rf", ["verbose","avm=","asc=","globalabc=","exclude=","help","notime",
                                "config=","playerglobalabc=","regex","toplevelabc=","full"])
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
  elif o in ("-a", "--asc"):
    globs['asc'] = v
  elif o in ("-g", "--globalabc"):
    globs['globalabc'] = v
  elif o in ("-x", "--exclude"):
    globs['exclude'] += v.split(",")
  elif o in ("-t", "--notime"):
    timestamps = False
  elif o in ("-c", "--config"):
    globs['config'] = v
  elif o in ("-p", "--playerglobalabc"):
    globs['playerglobalabc'] = v
  elif o in ("-r","--regex"):
    globs['regexOutput'] = True
  elif o in ("--toplevelabc"):
    globs['toplevelabc'] = v
  elif o in ("-f", "--full"):
    globs['full'] = True

exclude = globs['exclude']

def istest(f):
  return f.endswith(".as") and basename(f) != "shell.as" and not f.endswith("Util.as")
tests = [a for a in args if isfile(a) and istest(a)]
for a in [d for d in args if isdir(d) and not (basename(d) in exclude)]:
  for d, dirs, files in walk(a):
    tests += [join(d,f) for f in files if istest(f)]
    utils = [d for d in dirs if d+".as" in files]
    for x in [x for x in exclude+utils if x in dirs]:
      dirs.remove(x)

# set the output file name.  let's base its name on the date and platform,
# and give it a sequence number.

now = datetime.today()
for i in count(1):
  js_output = "%d-%s-%s.%d.html" % (now.year, str(now.month).zfill(2), str(now.day).zfill(2), i)
  if not isfile(js_output):
    break

print "Writing results to %s" % js_output
js_output_f = open(js_output, "w")

def verbose_print(m, start="", end=""):
  if verbose:
    js_print(m, start, end)

def err_print(m,pr=True):
  js_print(m, "<font color=#990000>", "</font><br/>")

def js_print(m, start_tag="<p><tt>", end_tag="</tt></p>",pr=True):
  if pr:
    print m
  if js_output:
    js_output_f.write("%s %s %s\n" % (start_tag, m, end_tag))
    js_output_f.flush()

if timestamps:
  # get the start time
  start_time = datetime.today()
  js_print("asc tests started: %s" % start_time)

#
# run the tests
#

def parents(d):
  while d != "":
    yield d
    d = dirname(d)
  yield d

# run a command and return its output
def run_pipe(cmd):
  t = pipes.Template()
  t.append("%s 2>&1" % cmd, "--")
  verbose_print(cmd)
  return t.open(globs['tmpfile'], "r")

def run_avm(abc):
  output=[]
  avmargs = ''
  if globs['settings'].has_key('avm.args'):
    avmargs = globs['settings']['avm.args']
  cmd="%s %s %s" % (globs['avm'],avmargs,abc)
  try:
    f=run_pipe(cmd)
    for line in f:
      line = line.strip()
      if line:
        if not (line[0:3] == 'at ' and line[-1] == ')'):  #don't record error location, so we strip it out
          output.append(line)
  finally:
    f.close()
  return output
  
def compile_test(as):
  asc, globalabc, playerglobalabc = globs['asc'], globs['globalabc'], globs['playerglobalabc']
  if not isfile(asc):
    exit("ERROR: cannot build %s, ASC environment variable or --asc must be set to asc.jar" % as)
  if not isfile(globalabc):
    exit("ERROR: global.abc %s does not exist, GLOBALABC environment variable or --globalabc must be set to global.abc" % globalabc)
  
  if asc.endswith(".jar"):
    cmd = "java -jar " + asc
  else:
    cmd = asc
  # sub out correct locations for global abc files
  if globs['settings'].has_key('asc.args'):
    ascargs=globs['settings']['asc.args']
    if playerglobalabc:
      ascargs=re.sub(' playerglobal\.abc', ' '+playerglobalabc, ascargs)
    ascargs=re.sub(' global\.abc', ' '+globalabc, ascargs)
    ascargs=re.sub(' builtin\.abc', ' '+globalabc, ascargs)
    ascargs=re.sub(' toplevel\.abc', ' '+globs['toplevelabc'], ascargs)
    ascargs=re.sub(' avmshell', ' '+globs['avm'], ascargs)
    cmd+= " "+ascargs
  else:
    cmd += " -import " + globalabc

  (dir, file) = split(as)
  verbose_print("   compiling %s" % file)
  output=[]
  try:
    if globs['full']: # compile test w/ multiple command line options
      ignore = run_pipe("%s %s %s" % (cmd,"-f" ,as))
      ignore = run_pipe("%s %s %s" % (cmd,"-i" ,as))
      ignore = run_pipe("%s %s %s" % (cmd,"-m" ,as))
      ignore = run_pipe("%s %s %s" % (cmd,"-p" ,as))
      ignore = run_pipe("%s %s %s" % (cmd,"-d" ,as))
    f = run_pipe("%s %s" % (cmd,as))
    for line in f:
      line = line.strip()
      if line:
        output.append(line)
  finally:
    f.close()
  return output

# Unused function?
#def fail(abc, msg, failmsgs):
#  msg = msg.strip()
#  err_print("   %s" % msg)
#  globs['failmsgs'] += ["%s : %s" % (abc, msg)]

def fixExpected(expected):
  for i in range(len(expected)):
    expected[i] = expected[i].strip()
    if expected[i].startswith("[Compiler]"):
      expected[i] = "\[Compiler\]" + expected[i][10:]
    if expected[i].startswith("[Coach]"):
      expected[i] = "\[Coach\]" + expected[i][7:]
    if expected[i].endswith(".^"):
      expected[i] = expected[i][:len(expected[i])-1] + "\^"
    if re.match(".*, Ln \d+, Col \d+:",expected[i]):
      expected[i] = ".*" + expected[i]
    if re.match(".*\(\).*",expected[i]):
      p=re.compile("\(\)")
      expected[i] = p.sub("\(\)",expected[i])

def fail_print (s,pr=True,expected=False,unexpectedpass=False):
  if unexpectedpass:
    globs['unpassmsgs'].append(s)
  elif expected:
    s=re.sub('FAILED','EXPECTED FAIL',s)
    globs['expfailmsgs'].append(s)
  else:
    globs['failmsgs'].append(s)
  err_print(s,pr)

def dict_match(dict,test,value):
  for k in dict.keys():
    if re.search(k,test):
      if dict[k].has_key(value):
        return dict[k][value]
        
def regexReplace(match):
  'regex replace function for writeErrActualFile'
  matchDict = match.groupdict()
  for i in matchDict:
    if matchDict['regexchar']:
      return r'\%s' % matchDict['regexchar']
    elif matchDict['byteswritten']:
      return r'\d+ bytes written'
    elif matchDict['errorNumber']:
      return '%s.*$' % matchDict['errorNumber']
    else: #testdir
      return r'.*'


def writeErrActualFile(root, actual):
  'output a regex ready err.actual file'
  fdopen=open(root+'.err.actual','w')
  testDir = os.path.dirname(root)
  if globs['regexOutput']:
    subPattern = r'((?P<testdir>%s.)|(?P<byteswritten>\d+ bytes written)|(?P<errorNumber>^.*Error: Error #[0-9]{4})(.*$)|(?P<regexchar>[\^\$\*\+\?\{\}\[\]\(\)\\]))' % testDir
    for line in actual:
      line = re.sub(subPattern, regexReplace,line)
      fdopen.write(line+"\n")
  else:
    for line in actual:
      fdopen.write(line+"\n")
  fdopen.close()


asc = globs['asc']
if not asc: # or not isfile(avm.split()[0]): /* isfile() fails for alias on OSX */
  print("ERROR: cannot run %s, ASC environment variable or --asc must be set to asc" % avm)
  exit(1)

js_print("Executing %d tests using vm: %s" % (len(tests), asc));
testnum = len(tests)

for ast in tests:
  if ast.startswith('./'):
    ast=ast[2:]
  testnum -= 1
  js_print("%d running %s" % (testnum, ast), "<b>", "</b><br/>");
  lpass = 0
  lfail = 0
  lexpfail = 0
  lunpass = 0
  dir = ast[0:ast.rfind('/')]
  root,ext = splitext(ast)
  tname = root[root.rfind('/')+1:]
  asc_args=None
  settings=dict()
  lines=[]
  if isfile(dir+'/testconfig.txt'):
    lines=open(dir+'/testconfig.txt').read().splitlines()
    for i in range(len(lines)):
      if len(lines[i])>0 and not lines[i].startswith('#'):
        lines[i]=dir+'/'+lines[i]
  if isfile('./testconfig.txt'):
    for line in open('./testconfig.txt').read().splitlines():
      lines.append(line)
  # process testconfig.txt lines
  for line in lines:
    if line.startswith("#"):
      continue
    fields=line.split(',',3) #maxsplit=3 so that any commas in last field are preserved
    for f in range(len(fields)):
      fields[f]=fields[f].strip()
    while len(fields)<4:
      fields.append('<empty>')
    if re.match('^%s$' % fields[0], root) and re.match(fields[2],globs['config']):
      settings[fields[1]]=fields[3]
  globs['settings']=settings
  if settings.has_key('skip'):
    js_print('  skipped : %s' % settings['skip'])
    globs['allskips'] += 1
    continue
  if os.path.isfile("%s.abc" % root):
    os.unlink("%s.abc" % root)
  actual=compile_test(ast)
  if os.path.isfile("%s.abc" % root):
    run_output = run_avm("%s.abc" % root)
    for line in run_output:
      actual.append(line)
  for line in actual:
    verbose_print(line)
  expected=[]
  if os.path.isfile("%s.err" % root):
    fileobject=open("%s.err" % root)
    for line in fileobject:
      line = line.strip()
      if line:
        expected.append(line)
  elif os.path.isfile("%s.out" % root):
    fileobject=open("%s.out" % root)
    for line in fileobject:
      line = line.strip()
      if line:
        expected.append(line)
  else:
    expected=["%s.abc, [\d]+ bytes written" % tname]
  if len(expected) != len(actual):
    if settings.has_key('expectedfailure'):
      expectFail=True
      globs['allexpfails'] += 1
    else:
      expectFail=False
      globs['allfails'] += 1
    #fail_print("[[KEY]]: %s" % ast, expected=expectFail)
    fail_print(ast,expected=expectFail, pr=False)
    fail_print("  FAILED: number of lines actual vs expected does not match",expected=expectFail)
    fail_print("  expected:[",expected=expectFail)
    for line in expected:
      fail_print("    "+line,expected=expectFail)
    fail_print("  ]",expected=expectFail)
    fail_print("  actual  :[",expected=expectFail)
    for line in actual:
      fail_print("    "+line,expected=expectFail)
    fail_print("  ]",expected=expectFail)
    writeErrActualFile(root,actual)
  else:
    fixExpected(expected)
    result=True
    expectFail=settings.has_key("expectedfailure")
    for i in range(len(expected)):
      try:
        if not re.match(expected[i],actual[i]):
          if result:
            fail_print(as,pr=False,expected=expectFail)
            result=False
          fail_print( "  FAILED matching line %d\n  expected:[%s]\n  actual  :[%s]" % (i+1,expected[i],actual[i]),expected=expectFail)
      except:
        if result:
          fail_print(ast,pr=False,expected=expectFail)
          result=False
        fail_print( "  FAILED matching line %d\n  expected:[%s]\n  actual  :[%s]" % (i+1,expected[i],actual[i]),expected=expectFail)
    if result:
      if expectFail:
        fail_print(" FAILED unexpected passing", unexpectedpass=True)
        globs['allunpass'] += 1
      else:
        js_print('  PASSED')
        globs['allpasses'] += 1
    else:
      if expectFail:
        globs['allexpfails'] += 1
      else:
        globs['allfails'] += 1
      writeErrActualFile(root, actual)

#
# cleanup
#

if globs['failmsgs']:
  js_print("\nFAILURES:", "", "<br/>")
  for m in globs['failmsgs']:
    js_print("  %s" % m, "", "<br/>")

if globs['expfailmsgs']:
  js_print("\nEXPECTED FAILURES:", "", "<br/>")
  for m in globs['expfailmsgs']:
    js_print("  %s" % m, "", "<br/>")

if globs['unpassmsgs']:
  js_print("\nUNEXPECTED PASSES:", "", "<br/>")
  for m in globs['unpassmsgs']:
    js_print("  %s" % m, "", "<br/>")

if not globs['allfails'] and not globs['allunpass']:
  js_print("\ntest run PASSED!")
else:
  js_print("\ntest run FAILED!")

if timestamps:
  end_time = datetime.today()
  js_print("Tests complete at %s" % end_time, "<hr><tt>", "</tt>")
  js_print("Start Date: %s" % start_time, "<tt><br>", "")
  js_print("End Date  : %s" % end_time, "<br>", "")
  js_print("Test Time : %s" % (end_time-start_time), "<br>", "")
js_print("passes               : %d" % globs['allpasses'], "<br>", "")
js_print("failures             : %d" % globs['allfails'], "<br>", "")
js_print("unexpected passes    : %d" % globs['allunpass'], "<br>", "")
js_print("expected failures    : %d" % globs['allexpfails'], "<br>", "")

logfile = open('result.properties', 'w')
if globs['allfails']>0:
  logfile.write("failures=%d" % globs['allfails'])
else:
  logfile.write("no failures")

if globs['allskips']>0:
  js_print("skips                : %d" % globs['allskips'], "<br>", "")

print "Results were written to %s" % js_output
