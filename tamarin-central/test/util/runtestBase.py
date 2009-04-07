#!/usr/bin/env python
#
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
# The Original Code is [Open Source Virtual Machine.].
#
# The Initial Developer of the Original Code is
# Adobe System Incorporated.
# Portions created by the Initial Developer are Copyright (C) 2005-2009
# the Initial Developer. All Rights Reserved.
#
# Contributor(s):
#   christine@netscape.com
#   Adobe AS3 Team
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
# ***** END LICENSE BLOCK ***** */
#
#
# simple script that executes tamarin certification tests.  you have to build the
# stand-alone, avmplus executable.  
# see http://developer.mozilla.org/en/docs/Tamarin_Build_Documentation 
#
# this test looks for an executable avmplus shell in
# %MOZ_SRC/mozilla/js/tamarin/platform/,
#

import os, sys, getopt, datetime, pipes, glob, itertools, tempfile, string, re, platform
import subprocess
from os.path import *
from os import getcwd,environ
from datetime import datetime
from glob import glob
from sys import argv, exit
from getopt import getopt
from itertools import count
from killableprocess import Popen
from time import time

import threadpool
import subProcess

# runtestUtils must be imported after "from os.path import *" as walk is overridden
from runtestUtils import *


try:
    import pexpect
except ImportError:
    pexpect = False
    

class RuntestBase:
    sourceExt = '.as'
    abcasmExt = '.abs'
    abcasmRunner = 'bash ../../utils/abcasm/abcasm.sh'
    abcasmShell = 'abcasm/abs_helper'
    testconfig = 'testconfig.txt'
    logFileType = 'html'
    
    avm = ''
    asc = ''
    builtinabc = ''
    shellabc = ''
    exclude = []
    config = ''
    ascargs = ''
    vmargs = ''
    escbin = ''
    rebuildtests = False
    includes = None
    settings = None
    ashErrors = []
    options = ''
    longOptions = []
    osName = ''
    vmtype = ''

    js_output = ''
    js_output_f = None
    args = []
    tests = []
    start_time = None
    
    verbose = False
    quiet = False
    htmlOutput = True
    timestamps = True
    forcerebuild = False
    eval = False      # Run the source file (.as, .js) but, do not magically prepend included files
    
    runSource = False # Run the source file (.as, .js) instead of .abc, magically prepend included files
    testTimeOut = -1 #by default tests will NOT timeout
    debug = False
    threads = 1
    
    
    
    def __init__(self):
        # Result Vars
        self.allpasses=0
        self.allfails=0
        self.allunpass=0
        self.allexpfails=0
        self.allexceptions=0
        self.allskips=0
        self.alltimeouts=0
        self.allasserts=0
        self.failmsgs=[]
        self.expfailmsgs=[]
        self.unpassmsgs=[]
        self.timeoutmsgs=[]
        self.assertmsgs=[]
        self.altsearchpath=None
        
        self.run()
        
    def run(self):
        self.setEnvironVars()
        self.loadPropertiesFile()
        self.setOptions()
        self.parseOptions()
        if self.htmlOutput and not self.rebuildtests:
            self.createOutputFile()
        self.setTimestamp()
        if not self.config:
            self.determineConfig()
        
        self.tests = self.getTestsList(self.args)
        # Load the root testconfig file
        self.settings, self.includes = self.parseTestConfig('.')
        self.preProcessTests()
        if self.rebuildtests:
            self.rebuildTests()
        else:
            self.runTests(self.tests)
        self.cleanup()
        
    def setEnvironVars(self):
        if 'AVM' in environ:
            self.avm = environ['AVM'].strip()
        if 'ASC' in environ:
            self.asc = environ['ASC'].strip()
        if 'GLOBALABC' in environ:
            self.builtinabc = environ['GLOBALABC'].strip()
        if 'BUILTINABC' in environ:
            self.builtinabc = environ['BUILTINABC'].strip()
        if 'SHELLABC' in environ:
            self.shellabc = environ['SHELLABC'].strip()
        if 'CVS' in environ:
            self.exclude = ['CVS'].strip()
        if 'CONFIG' in environ:
            self.config = environ['CONFIG'].strip()
        if 'ASCARGS' in environ:
            self.ascargs = environ['ASCARGS'].strip()
        if 'VMARGS' in environ:
            self.vmargs = environ['VMARGS'].strip()
        

    def loadPropertiesFile(self):
        # yet another way to specify asc,avm,builtinabc ...from a file
        pf = 'runtests.properties'
        if exists(pf):
            self.verbose_print( 'reading properties from %s' % (pf) )
            fd = open(pf,'r')
            for l in fd:
                setting = l.strip().split('=')
                if l.startswith('#') or len(setting) < 2 or len(setting[1]) <= 0:
                    continue
                val = setting[1].strip()
                option = setting[0].split('.')  # see if we have x.y = z
                nm = option[0].strip()
                # check if nm is valid
                if nm in self.__class__.__dict__ and not callable(self.__class__.__dict__[nm]):
                    if len(option) > 1:
                        val = self.__class__.__dict__[nm] + ' ' + val  # concat
                    self.__class__.__dict__[nm] = val
            fd.close()

    def usage(self, c):
        print 'usage: %s [options] [tests]' % basename(argv[0])
        print ' -v --verbose       enable additional output'
        print ' -E --avm           avmplus command to use'
        print ' -a --asc           compiler to use'
        print ' -g --globalabc     DEPRECATED but still works - use builtin.abc (used to be location of global.abc)'
        print ' -b --builtinabc    location of builtin.abc'
        print ' -s --shellabc      location of shell_toplevel.abc'
        print ' -x --exclude       comma separated list of directories to skip'
        print ' -e --eval          use run-time compiler'
        print ' -h --help          display help and exit'
        print ' -t --notime        do not generate timestamps (cleaner diffs)'
        print ' -f --forcerebuild  force rebuild all test files'
        print ' -c --config        sets the config string [default OS-tvm]'
        print ' -q --quiet         display minimum output during testrun'
        print '    --rebuildtests  rebuild the tests only - do not run against VM'
        print '    --ascargs       args to pass to asc on rebuild of test files'
        print '    --vmargs        args to pass to vm'
        print '    --timeout       max time to let a test run, in sec (default -1 = never timeout)'
        print '    --nohtml        do not create a html output file' 
        

    def setOptions(self):
        '''set the valid command line options.
            When subclassing, call this method first, then append options to each list'''
        self.options = 'vE:a:g:b:s:x:htfc:dqe'
        self.longOptions = ['verbose','avm=','asc=','globalabc=','builtinabc=','shellabc=',
                   'exclude=','help','notime','forcerebuild','config=','ascargs=','vmargs=',
                   'timeout=', 'rebuildtests','quiet','nohtml','eval']

    def parseOptions(self):
        try:
            opts, self.args = getopt(argv[1:], self.options, self.longOptions )
        except:
            self.usage(2)
        
        if not self.args:
            self.args = ['.']
        
        for o, v in opts:
            if o in ('-v', '--verbose'):
                self.verbose = True
            elif o in ('-h', '--help'):
                self.usage(0)
            elif o in ('-E', '--avm'):
                self.avm = v
            elif o in ('-a', '--asc'):
                self.asc = v
            elif o in ('-g', '--globalabc'):
                self.builtinabc = v
            elif o in ('-b', '--builtinabc'):
                self.builtinabc = v
            elif o in ('-s', '--shellabc'):
                self.shellabc = v
            elif o in ('-x', '--exclude'):
                self.exclude += v.split(',')
            elif o in ('-t', '--notime'):
                self.timestamps = False
            elif o in ('-f', '--forcerebuild'):
                self.forcerebuild = True
            elif o in ('-c', '--config'):
                self.config = v
            elif o in ('-e', '--eval'):
                self.eval = True
            elif o in ('--ascargs',):
                self.ascargs = v
            elif o in ('--vmargs',):
                self.vmargs = v
            elif o in ('--ext',):
                self.sourceExt = v
            elif o in ('--timeout',):
                self.self.testTimeOut=int(v)
            elif o in ('-d',):
                self.debug = True
            elif o in ('--rebuildtests',):
                self.rebuildtests = True
            elif o in ('-q', '--quiet'):
                self.quiet = True
            elif o in ('--nohtml',):
                self.htmlOutput = False
        return opts
                
    def setTimestamp(self):
        if self.timestamps:
            # get the start time
            self.start_time = datetime.today()
            self.js_print('Tamarin tests started: %s' % self.start_time, overrideQuiet=True)
        
    def determineOS(self):
        _os = platform.system()
        ostype = ''
        if re.search('(CYGWIN_NT|Windows)', _os):
            ostype='win'
        if re.search('(Darwin)', _os):
            ostype='mac'
        if re.search('(Linux)', _os):
            ostype='lnx'
        if re.search('(SunOS)', _os):
            ostype='sol'
        
        if ostype == '':
            print("ERROR: os %s is unknown, expected values are (win,mac,lnx,sol), use runtests.py --config x86-win-tvm-release to manually set the configuration" % (platform.system()))
            exit(1)
        
        self.osName = ostype
    
    def determineConfig(self):
        if not self.runSource:
            self.vmtype = 'release'
            (f,err,exitcode) = self.run_pipe('%s' % self.avm)
            try:
                for line in f:
                    if line.find('[-d]') != -1:
                        self.vmtype = 'releasedebugger'
                        break
            except:
                nop = True
          
        # ================================================
        # Determine the configruation if it has not been 
        # passed into the script:
        # {CPU_ARCH}-{OS}-{VM}-{VERSION}-{VMSWITCH}
        # ================================================
        
        self.determineOS()
        
        try:
            # Try and determine CPU architecture of the AVM, if it fails drop back to platform.machine()
            cputype = ''
            (f,err,exitcode) = self.run_pipe('file %s' % (self.avm))
            if re.search('(32-bit|80386|i386)', f[0]):
                cputype='x86'
            if re.search('(64-bit|x86-64|x86_64|Mono/\.Net)', f[0]):
                cputype='x64'
            if re.search('(ppc)', f[0]):
                cputype='ppc'
            if re.search('(ppc64)', f[0]):
                cputype='ppc64'
            if cputype == '':
                raise Exception()
                
        except:
            try:
                cputype={'x86':'x86','i386':'x86','i686':'x86','x86_64':'x64','i86pc':'x86','Power Macintosh':'ppc','sun4u':'x86','':'x86'}[platform.machine()]
            except:
                print("ERROR: cpu_arch '%s' is unknown, expected values are (x86,ppc), use runtests.py --config x86-win-tvm-release to manually set the configuration" % (platform.machine()))
                exit(1)
                
        self.config = cputype+'-'+self.osName+'-tvm-'+self.vmtype+self.vmargs
    
    ### File and Directory functions ###
    def istest(self,f):
        return f.endswith((self.sourceExt,self.abcasmExt)) and basename(f) != ('shell'+self.sourceExt) \
               and not f.endswith('Util'+self.sourceExt)
               
    def getTestsList(self, startDir):
        if self.altsearchpath!=None:
            newstartDir=[]
            for a in startDir:
                newstartDir.append(self.altsearchpath+a)
            startDir=startDir+newstartDir
        for i in range(len(startDir)):
            if startDir[i][-1] == '/':
                startDir[i] = startDir[i][:-1]
        tests = [a for a in startDir if isfile(a) and self.istest(a)]
        for a in [d for d in startDir if isdir(d) and not (basename(d) in self.exclude)]:
            for d, dirs, files in walk(a, followlinks=True):
                tests += [(d+'/'+f) for f in files if self.istest(f)]
                utils = [d for d in dirs if d+self.sourceExt in files]
                for x in [x for x in self.exclude+utils if x in dirs]:
                    dirs.remove(x)
        return tests


    # set the output file name.  let's base its name on the date and platform,
    # and give it a sequence number.
    def createOutputFile(self):
        now = datetime.today()
        for i in count(1):
            self.js_output = '%d-%s-%s.%d.%s' % (now.year, str(now.month).zfill(2), str(now.day).zfill(2), i, self.logFileType)
            if not isfile(self.js_output):
                break
        
        print 'Writing results to %s' % self.js_output
        self.js_output_f = open(self.js_output, 'w')
    
    def js_print(self, m, start_tag='<p><tt>', end_tag='</tt></p>', overrideQuiet=False):
        if self.quiet and not overrideQuiet:
            sys.stdout.write('.')
        else:
            print m
            sys.stdout.flush()
        if self.js_output:
            if self.logFileType == 'html':
                self.js_output_f.write('%s %s %s\n' % (start_tag, m, end_tag))
            else:
                self.js_output_f.write('%s\n' % m)
            self.js_output_f.flush()
            
    def verbose_print(self, m, start='', end=''):
      if self.verbose:
        self.js_print(m, start, end)
    
    def err_print(self, m):
        self.js_print(m, '<font color=#990000>', '</font><br/>')
    
    def quiet_print(self, m, start=None, end=None):
        if self.quiet:
            sys.stdout.write('.')
        else:
            if not start and not end:
                self.js_print(m)
            else:
                self.js_print(m, start, end)
    
    def fail(self, abc, msg, failmsgs):
        msg = msg.strip()
        self.err_print('   %s' % msg)
        failmsgs += ['%s : %s' % (abc, msg)]

    def parents(self, d):
        while d != abspath(self.args[0]) and d != '':
            yield d
            d = dirname(d)
        yield d
    
    # run a command and return its output 
    def run_pipe(self, cmd):
        if self.debug:
            print('cmd: %s' % cmd)
        try:
            p = Popen((cmd), shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            output = p.stdout.readlines()
            err = p.stderr.readlines()
            starttime=time()
            exitCode = p.wait(self.testTimeOut) #abort if it takes longer than 60 seconds
            if exitCode < 0 and self.testTimeOut>-1 and time()-starttime>self.testTimeOut:  # process timed out
                return 'timedOut'
            return (output,err,exitCode)
        except KeyboardInterrupt:
            print '\n\nKeyboardInterrupt detected ... killing process'
            p.kill()
            self.killmyself()
            
    def killmyself(self):
        # destroy this python process and children
        if self.osName == 'win':
            import ctypes
            ctypes.windll.kernel32.TerminateProcess(
                ctypes.windll.kernel32.OpenProcess(1, False, os.getpid()),
                -1)
        else:
            os.killpg(os.getpgrp(),9)
                    
    def parseArgStringToList(self, argStr):
        args = argStr.strip().split(' ')
        # recombine any args that have spaces in them
        argList = []
        for a in args:
            if a == '':
                pass
            elif a[0] == '-':
                argList.append(a)
            else:   # append the space and text to the last arg
                argList[len(argList)-1] += ' ' + a
        return argList
    
    def parseAscArgs(self, ascArgFile, currentdir):
        # reads an .asc_args file and returns a tuple of the arg mode (override or merge) and a list of args
        f = open(ascArgFile,'r')
        while True: # skip comment lines
            ascargs = f.readline()
            if (ascargs[0] != '#'):
                break
        ascargs = ascargs.split('|')
        ascargs[0] = ascargs[0].strip()
        if (len(ascargs) == 1): #treat no keyword as a merge
            ascargs.insert(0,'merge')
        elif (ascargs[0] != 'override') or (ascargs[0] != 'merge'): # default to merge if mode not recognized
            ascargs[0] = 'merge'
        # replace the $DIR keyword with actual directory
        ascargs[1] = string.replace(ascargs[1], '$DIR', currentdir)
        if ascargs[1].find('$SHELLABC') != -1:  
            if not isfile(self.shellabc):   # TODO: not the best place to check for this
                exit('ERROR: shell.abc %s does not exist, SHELLABC environment variable or --shellabc must be set to shell_toplevel.abc' % self.shellabc)
            ascargs[1] = string.replace(ascargs[1], '$SHELLABC', self.shellabc)
        ascargs[1] = self.parseArgStringToList(ascargs[1])
        removeArgList = []
        argList = []
        for a in ascargs[1]:
            if a[0:3] == '-no':
                removeArgList.append(a[3:])
            else:
                argList.append(a)
    
        return ascargs[0], argList, removeArgList
        
    def loadAscArgs(self, arglist,dir,file):
        # It is possible that file is actually a partial path rooted to acceptance,
        # so make sure that we are only dealing with the actual filename
        file = split(file)[1]
        mode = ''
        newArgList = []
        removeArgList = []
        # Loads an asc_args file and modifies arglist accordingly
        if isfile('./dir.asc_args'):
            mode = ''
            mode, newList, removeList = self.parseAscArgs('./dir.asc_args', './')
            newArgList.extend(newList)
            removeArgList.extend(removeList)
            if mode == 'merge':
                arglist.extend(newArgList)
            elif mode == 'override':
                arglist = newArgList
            # remove any duplicate args
            arglist = list(set(arglist))
            if removeArgList:
                for removeArg in removeArgList:
                    try:
                        arglist.remove(removeArg)
                    except:
                        pass
        if isfile(dir+'/dir.asc_args'):
            mode = ''
            mode, newList, removeList = self.parseAscArgs(dir+'/dir.asc_args', dir)
            newArgList.extend(newList)
            removeArgList.extend(removeList)
            if mode == 'merge':
                arglist.extend(newArgList)
            elif mode == 'override':
                arglist = newArgList
            # remove any duplicate args
            arglist = list(set(arglist))
            if removeArgList:
                for removeArg in removeArgList:
                    try:
                        arglist.remove(removeArg)
                    except:
                        pass
        if file and isfile('%s/%s.asc_args' % (dir, file)):  #file takes precendence over directory
            mode = ''
            mode, newList, removeList = self.parseAscArgs('%s/%s.asc_args' % (dir, file), dir)
            newArgList.extend(newList)
            removeArgList.extend(removeList)
            if mode == 'merge':
                arglist.extend(newArgList)
            elif mode == 'override':
                arglist = newArgList
            # remove any duplicate args
            arglist = list(set(arglist))
            if removeArgList:
                for removeArg in removeArgList:
                    try:
                        arglist.remove(removeArg)
                    except:
                        pass
        return arglist
    
        
    def compile_test(self, as_file, extraArgs=[]):
        asc, builtinabc, shellabc, ascargs = self.asc, self.builtinabc, self.shellabc, self.ascargs
        
        # if there is a .build file available (which is an executable script) run that file instead
        # of compiling with asc
        as_base = as_file[0:as_file.rfind('.')]
        if isfile(as_base+'.build'):
            (dir,file)=split(as_base)
            self.verbose_print('    compiling %s running %s%s' % (file, as_base, '.build'))
            (f,err,exitcode) = self.run_pipe('%s%s' % (as_base, '.build'))
            for line in f:
                self.verbose_print(line.strip())
            for line in err:
                self.verbose_print(line.strip())
            return

        if not isfile(asc):
            exit('ERROR: cannot build %s, ASC environment variable or --asc must be set to asc.jar' % as_file)
           
        (dir, file) = split(as_file)
        self.verbose_print('   compiling %s' % file)
        
        # additional .as file compiler args
        if as_file.endswith(self.sourceExt):
            if not isfile(builtinabc):
                exit('ERROR: builtin.abc (formerly global.abc) %s does not exist, BUILTINABC environment variable or --builtinabc must be set to builtin.abc' % builtinabc)
        
            if asc.endswith('.jar'):
                cmd = 'java -jar ' + asc
            else:
                cmd = asc
                
            arglist = self.parseArgStringToList(ascargs)
        
            # look for .asc_args files to specify dir / file level compile args, arglist is passed by ref
            arglist = self.loadAscArgs(arglist, dir, as_file)
            for arg in extraArgs:
                cmd += ' %s' % arg
        
                                  
            cmd += ' -import %s' % builtinabc
            for arg in arglist:
                cmd += ' %s' % arg
            
            for p in self.parents(dir):
                if p=='':
                    p='.'
                shell = join(p,'shell'+self.sourceExt)
                if isfile(shell):
                    cmd += ' -in ' + shell
                    break
            (testdir, ext) = splitext(as_file)
            deps = glob(join(testdir,'*'+self.sourceExt))
            deps.sort()
            for util in deps + glob(join(dir,'*Util'+self.sourceExt)):
                cmd += ' -in %s' % string.replace(util, '$', '\$')
                
        elif as_file.endswith(self.abcasmExt):
            cmd = self.abcasmRunner
            
        try:
            self.verbose_print('%s %s' % (cmd,as_file))
            (f,err,exitcode) = self.run_pipe('%s %s' % (cmd,as_file))
            for line in f:
                self.verbose_print(line.strip())
            for line in err:
                self.verbose_print(line.strip())
            return f+err
        except:
            raise
    
    def rebuildTests(self):
        if self.threads == 1 or platform.system()[:6].upper() == 'CYGWIN':
            self.compileWithAsh(self.tests)
        else: # run using threads
            # split tests into number of threads
            testGroups = splitList(self.tests, self.threads)
            
            # generate threadpool
            requests = threadpool.makeRequests(self.compileWithAsh, testGroups, self.printOutput)
            main = threadpool.ThreadPool(self.threads)
            # que requests
            [main.putRequest(req) for req in requests]
            
            # ...and wait for the results to arrive in the result queue
            # wait() will return when results for all work requests have arrived
            
            try:
              main.wait()
            except KeyboardInterrupt, SystemExit:
                print '\n\nKeyboardInterrupt detected ... killing worker threads'
                main.dismissWorkers(self.threads)
                self.killmyself()
            except Exception, e:
                main.dismissWorkers(self.threads)
                print 'EXCEPTION: %s' % e
                self.killmyself()
        
    
    def compileWithAsh(self, tests):
        start_time = datetime.today()
        #print("starting compile of %d tests at %s" % (len(tests),start_time))
        total=len(tests)
        if not pexpect:
              for test in tests:
                  self.js_print('%d\tcompiling %s' % (total,test))
                  self.compile_test(test)
                  (testdir, ext) = splitext(test)
                  if exists(testdir+".abc")==False:
                      print("ERROR abc files %s.abc not created" % (testdir))
                      self.ashErrors.append("abc files %s.abc not created" % (testdir))
                  total -= 1;
        else:  #pexpect available
            child = pexpect.spawn("java -classpath %s macromedia.asc.embedding.Shell" % self.asc)
            child.logfile = None
            child.expect("\(ash\)")
            child.expect("\(ash\)")
    
            for test in tests:
                if self.debug:
                    print cmd
                else:
                    print "Compiling ", test
                    
                if test.endswith(self.abcasmExt):
                    self.compile_test(test)
                else:
                    arglist = self.parseArgStringToList(self.ascargs)
                
                    (dir, file) = split(test)
                    # look for .asc_args files to specify dir / file level compile args
                    arglist = self.loadAscArgs(arglist, dir, test)
                    
                    cmd = "asc -import %s " % (self.builtinabc)
                    for arg in arglist:
                        cmd += ' %s' % arg
                    
                    for p in self.parents(dir):
                        shell = join(p,"shell.as")
                        if isfile(shell):
                            cmd += " -in " + shell
                            break
                    (testdir, ext) = splitext(test)
                    deps = glob(join(testdir,"*.as"))
                    deps.sort()
                    for util in deps + glob(join(dir,"*Util.as")):
                        cmd += " -in %s" % util #no need to prepend \ to $ when using ash
                    cmd += " %s" % test
                
                    if exists(testdir+".abc"):
                        os.unlink(testdir+".abc")
                    child.sendline(cmd)
                    child.expect("\(ash\)")
                    if not exists(testdir+".abc"):
                        print("ERROR: abc file %s.abc not created, cmd used to compile: %s" % (testdir,cmd))
                        self.ashErrors.append("abc file %s.abc not created, cmd used to compile: %s" % (testdir,cmd))
                    total -= 1;
                    #print("%d remaining, %s" % (total,cmd))
        end_time = datetime.today()
        #print("finished compile of %d tests at %s elapsed time is %s" % (len(tests),start_time,end_time-start_time))

    def build_incfiles(self, as_file):
        files=[]
        (dir, file) = split(as_file)
        for p in self.parents(dir):
            shell = join(p,'shell'+self.sourceExt)
            if isfile(shell):
                files.append(shell)
        (testdir, ext) = splitext(as_file)
        if not self.eval:
            for util in glob(join(testdir,'*'+self.sourceExt)) + glob(join(dir,'*Util'+self.sourceExt)):
                files.append(string.replace(util, "$", "\$"))
        return files

    # TODO: Rename/move to better place
    def preProcessTests(self):
        if (not self.rebuildtests) and (not self.avm): #don't need AVM if rebuilding tests
            exit('ERROR: cannot run %s, AVM environment variable or --avm must be set to avmplus' % self.avm)
            
        self.js_print('current configuration: %s' % self.config, overrideQuiet=True)
        self.js_print('Executing %d tests against vm: %s' % (len(self.tests), self.avm), overrideQuiet=True);
        
        
        # Are we running esc - depends on a valid avm
        if self.runESC:
            runSource = True
            # generate the executable cmd for esc
            #escAbcs = [f for f in os.listdir(self.escbin) if f.endswith('.abc')] #not all abcs are used for esc
            escAbcs = ['debug','util','bytes-tamarin','util-tamarin','lex-char','lex-token',
                       'lex-scan','ast','ast-decode','parse','asm','abc','emit','cogen',
                       'cogen-stmt','cogen-expr','esc-core','eval-support','esc-env','main']
            if not self.escbin.endswith('/'):
                self.escbin += '/'
            for f in escAbcs:
                self.avm += ' %s%s.es.abc' % (self.escbin, f)
            self.avm += ' -- '
            self.avm += ' %s../test/spidermonkey-prefix.es' % self.escbin  #needed to run shell harness

    def printOutput(self,request, outputCalls=None):
        #execute the outputCalls
        if outputCalls:
            for call in outputCalls:
                apply(call[0],call[1])
    
    def runTests(self, testList):
        testnum = len(testList)
        # threads on cygwin randomly lock up
        if self.threads == 1 or platform.system()[:6].upper() == 'CYGWIN':
            for t in testList:
                testnum -= 1
                o = self.runTest((t, testnum))
                self.printOutput(None, o)
        else: # run using threads
            #Assign test numbers
            testsTuples = []
            testsLen = len(testList)
            for i,t in enumerate(testList):
              testsTuples.append([t,testsLen-i])
            # generate threadpool
            requests = threadpool.makeRequests(self.runTest, testsTuples, self.printOutput)
            main = threadpool.ThreadPool(self.threads)
            # que requests
            [main.putRequest(req) for req in requests]
            
            # ...and wait for the results to arrive in the result queue
            # wait() will return when results for all work requests have arrived
            
            try:
              main.wait()
            except KeyboardInterrupt, SystemExit:
                print '\n\nKeyboardInterrupt detected ... killing worker threads'
                main.dismissWorkers(self.threads)
                self.killmyself()
            except Exception, e:
                main.dismissWorkers(self.threads)
                print 'EXCEPTION: %s' % e
                self.killmyself()
        
    def parseTestConfig(self, dir):
        settings={}
        includes=[]
        names=None
        lines=[]
        
        if isfile(join(dir,self.testconfig)):
            if join(dir, '') == './':
                for line in open(join(dir,self.testconfig)).read().splitlines():
                    lines.append(line)
            else:
                # if this is not the root testconfig, append the path before the testname
                for line in open(join(dir,self.testconfig)).read().splitlines():
                    lines.append('%s/%s' %(dir,line))
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
                # remove any trailing extension if specified
                # TODO: add abs to here
                if names[0][-3:] == self.sourceExt:
                    names[0]=names[0][:-3]
                rs='^%s$' % names[0]
                # only add settings for current config
                if re.search('^%s$' % fields[1],self.config):
                    if re.search(fields[1],self.config) and fields[2]=='include':
                        includes.append(fields[0])
                    if not settings.has_key(names[0]):
                        settings[names[0]] = {}
                    if not settings[names[0]].has_key(names[1]):
                        settings[names[0]][names[1]] = {}
                    settings[names[0]][names[1]][fields[2]]=fields[3]
        return settings, includes
    
    def compareAbcAsmOutput(self, file, output):
        # return diff
        try:
            f = open(file[:-4]+'.out', 'r')
            if self.config.find('debugger') != -1:
                if isfile(file[:-4]+'.out.debug'):
                    f.close()
                    f = open(file[:-4]+'.out.debug', 'r')
            if self.config.find('interp') != -1:
                if isfile(file[:-4]+'.out.interp'):
                    f.close()
                    f = open(file[:-4]+'.out.interp', 'r')
            flines = []
            for line in f.readlines():
                line = ''.join(line.split('\r'))
                if line != '\n':
                    flines.append(line)
            f.close()
        except IOError:
            flines = ['IOError Opening .out file']
        if len(output) != len(flines):
            return flines
        # compare lines
        for i in range(0,len(output)):
            if output[i].strip() != flines[i].strip():
                return flines
        return
                                                
    def runTest(self, testAndNum):
        ast = testAndNum[0]
        testnum = testAndNum[1]
        outputCalls = [] #queue all output calls so that output is written in a block
        lpass = 0
        lfail = 0
        lexpfail = 0
        lunpass = 0
        ltimeout = 0
        lassert = 0
        
        if ast.startswith('./'):
            ast=ast[2:]
        
        dir = ast[0:ast.rfind('/')]
        root,ext = splitext(ast)
        if self.runSource or self.eval:
            testName = ast
        else:
            testName = root + '.abc'
            
        includes = self.includes #list
        settings = {}
        
        # get settings for this test
        for k in self.settings.keys():
            if re.search('^'+k+'$', root):
                for k2 in self.settings[k].keys():
                    if k2 in settings:
                        settings[k2].update(self.settings[k][k2])
                    else:
                        settings[k2] = self.settings[k][k2].copy()
        
        if isfile(join(dir,self.testconfig)):
            localIncludes, localSettings = self.parseTestConfig(dir)
            # have a local testconfig, so we create a copy of the global settings to not overwrite
            includes = list(self.includes) #copy list - don't use reference
            includes.extend(localIncludes)
            if localSettings.has_key(root):
                settings.update(localSettings[root])
        
        #TODO: possibly handle includes by building test list?  This works for now...
        if includes and not list_match(includes,root):
            return
        
        outputCalls.append((self.js_print,('%d running %s' % (testnum, ast), '<b>', '</b><br/>')));
        
        # skip entire test if specified
        # TODO: add skip reason to output
        if settings.has_key('.*') and settings['.*'].has_key('skip'):
            outputCalls.append((self.js_print,('  skipping',)))
            self.allskips += 1
            return outputCalls
            
        
        # delete abc if forcerebuild
        if self.forcerebuild and isfile(testName):
            os.unlink(testName)
        if isfile(testName) and getmtime(ast)>getmtime(testName):
        	self.verbose_print("%s has been modified, recompiling" % ast)
        	os.unlink(testName)
        if not isfile(testName):
            compileOutput = self.compile_test(ast)
            if not isfile(testName):
                if ast.endswith(self.abcasmExt):
                    # file didn't compile, compare compile output
                    flines = self.compareAbcAsmOutput(ast, compileOutput)
                    if flines:
                        lfail += 1
                        outputCalls.append((self.fail,(testName, 'FAILED! :\nExpected:\n'+''.join(flines)+'\nGOT:\n'+''.join(compileOutput), self.failmsgs)))
                        outputCalls.append((self.js_print, ('   FAILED passes:%d fails:%d unexpected passes: %d expected failures: %d' % (lpass,lfail,lunpass,lexpfail), '', '<br/>')))
                    else:
                        lpass += 1
                        outputCalls.append((self.verbose_print, ('   PASSED passes:%d fails:%d unexpected passes: %d expected failures: %d' % (lpass,lfail,lunpass,lexpfail), '', '<br/>')))
                    self.allfails += lfail
                    self.allpasses += lpass
                    return outputCalls
                else:
                    lfail += 1
                    outputCalls.append((self.fail,(testName, 'FAILED! file not found ' + testName, self.failmsgs)))
        if self.runSource or self.eval:
            incfiles=self.build_incfiles(testName)
            incfiles.append("shell" + self.sourceExt)
            for incfile in incfiles:
                testName=incfile+" "+testName
        
        if isfile("%s.avm_args" % ast):
            testName = " %s %s" % (string.replace(open("%s.avm_args" % ast).readline(), "$DIR", dir), testName)
        
        if ast.endswith(self.abcasmExt):
            # make sure util file has been compiled
            if not exists(self.abcasmShell+'.abc'):  # compile abcasmShell with no additional args
                self.run_pipe('java -jar %s %s' % (self.asc, self.abcasmShell+'.as'))
            (f,err,exitcode) = self.run_pipe('%s %s %s %s' % (self.avm, self.vmargs, self.abcasmShell+'.abc', testName))
        else:
            (f,err,exitcode) = self.run_pipe('%s %s %s' % (self.avm, self.vmargs, testName))
            
        if f == "timedOut":
            outputCalls.append((self.fail(testName, 'FAILED! Test Timed Out! Time out is set to %s s' % self.testTimeOut, self.timeoutmsgs)))
            ltimeout += 1
        else:
            try:
                outputLines = []
                for line in f:
                    outputLines.append(line)
                    outputCalls.append((self.verbose_print,(line.strip(),)))
                    if 'Assertion failed:' in line:
                        lassert += 1
                        outputCalls.append((self.fail,(testName, line, self.assertmsgs)))
                    testcase=''
                    if len(line)>9:
                        testcase=line.strip()
                    if dict_match(settings,testcase,'skip'):
                        outputCalls.append((self.js_print,('  skipping %s' % line.strip(),)))
                        self.allskips+=1
                        continue
                    if 'PASSED!' in line:
                        res=dict_match(settings,testcase,'expectedfail')
                        if res:
                            outputCalls.append((self.fail,(testName, 'unexpected pass: ' + line.strip() + ' reason: '+res, self.unpassmsgs)))
                            lunpass += 1
                        else:
                            lpass += 1
                    if 'FAILED!' in line:
                        res=dict_match(settings,testcase,'expectedfail')
                        if res:
                            outputCalls.append((self.fail,(testName, 'expected failure: ' + line.strip() + ' reason: '+res, self.expfailmsgs)))
                            lexpfail += 1
                        else:
                            lfail += 1
                            outputCalls.append((self.fail,(testName, line, self.failmsgs)))
            except:
                print 'exception running avm'
                exit(1)
            exitcodeExp=0
            if isfile(root+".exitcode"):
                try:
                    exitcodeExp=int(open(root+".exitcode").read())
                except:
                    print("ERROR: reading exit code file '%s' should contain an integer")
            res=dict_match(settings,'exitcode','expectedfail')
            if exitcode!=exitcodeExp:
                res2=dict_match(settings,'exitcode','skip')
                if res2==None and res:
                    outputCalls.append((self.js_print,(testName, 'expected failure: exitcode reason: %s'%res,self.expfailmsgs)))
                    lexpfail += 1
                elif res2==None:
                    outputCalls.append((self.fail,(testName, 'unexpected exit code expected:%d actual:%d FAILED!' % (exitcodeExp,exitcode), self.failmsgs)))
                    outputCalls.append((self.fail,(testName, 'captured output: %s' % string.join([l.strip() for l in outputLines], ' | '), self.failmsgs)))
                    lfail+= 1
            elif err!=[]:
                outputCalls.append((self.fail,(testName, "unexpected stderr expected:'%s' actual:'%s'" % ('',err), self.failmsgs)))
            elif lpass == 0 and lfail == 0 and lunpass==0 and lexpfail==0:
                res=dict_match(settings,'*','expectedfail')
                if res:
                    outputCalls.append((self.fail,(testName, 'expected failure: FAILED contained no testcase messages reason: %s' % res,self.expfailmsgs)))
                    lexpfail += 1
                else:
                    lfail = 1
                    outputCalls.append((self.fail,(testName, '   FAILED contained no testcase messages - reason: %s' % string.join([l.strip() for l in outputLines], ' | '), self.failmsgs)))
        
        self.allfails += lfail
        self.allpasses += lpass
        self.allexpfails += lexpfail
        self.allunpass += lunpass
        self.alltimeouts += ltimeout
        self.allasserts += lassert
        if lfail or lunpass:
            outputCalls.append((self.js_print, ('   FAILED passes:%d fails:%d unexpected passes: %d expected failures: %d' % (lpass,lfail,lunpass,lexpfail), '', '<br/>')))
        else:
            outputCalls.append((self.verbose_print, ('   PASSED passes:%d fails:%d unexpected passes: %d expected failures: %d' % (lpass,lfail,lunpass,lexpfail), '', '<br/>')))
        return outputCalls
    #
    # cleanup
    #
    def cleanup(self):
        # Turn off quiet to display summary
        if self.quiet:
            self.quiet = False
        if self.timeoutmsgs:
            self.js_print('\nTIMEOUTS:', '', '<br/>')
            for m in self.timeoutmsgs:
                self.js_print('  %s' % m, '', '<br/>')
        
        if self.failmsgs:
            self.js_print('\nFAILURES:', '', '<br/>')
            for m in self.failmsgs:
                self.js_print('  %s' % m, '', '<br/>')
        
        if self.expfailmsgs:
            self.js_print('\nEXPECTED FAILURES:', '', '<br/>')
            for m in self.expfailmsgs:
                self.js_print('  %s' % m, '', '<br/>')
        
        if self.unpassmsgs:
            self.js_print('\nUNEXPECTED PASSES:', '', '<br/>')
            for m in self.unpassmsgs:
                self.js_print('  %s' % m, '', '<br/>')
    
        if self.assertmsgs:
            self.js_print('\nASSERTIONS:', '', '<br/>')
            for m in self.assertmsgs:
                self.js_print('  %s' % m, '', '<br/>')
        
        if self.rebuildtests:
            if self.ashErrors:
                self.js_print('\ntest run FAILED!')
                self.js_print('')
                self.js_print('Compile Errors:')
                for msg in self.ashErrors:
                    self.js_print('\t'+msg)
                self.js_print('')
            else:
                self.js_print('\ntest run PASSED!')
        else:
            if not self.allfails and not self.allunpass:
                self.js_print('\ntest run PASSED!')
            else:
                self.js_print('\ntest run FAILED!')
        
        if self.timestamps:
            end_time = datetime.today()
            self.js_print('Tests complete at %s' % end_time, '<hr><tt>', '</tt>')
            self.js_print('Start Date: %s' % self.start_time, '<tt><br>', '')
            self.js_print('End Date  : %s' % end_time, '<br>', '')
            self.js_print('Test Time : %s' % (end_time-self.start_time), '<br>', '')
            
        if not self.rebuildtests:
            self.js_print('passes               : %d' % self.allpasses, '<br>', '')
            self.js_print('failures             : %d' % self.allfails, '<br>', '')
            if self.allunpass>0:
                self.js_print('unexpected passes    : %d' % self.allunpass, '<br>', '')
            if self.allexpfails>0:
                self.js_print('expected failures    : %d' % self.allexpfails, '<br>', '')
            if self.allskips>0:
                self.js_print('tests skipped        : %d' % self.allskips, '<br>', '')
            if self.allexceptions>0:
                self.js_print('test exceptions      : %d' % self.allexceptions, '<br>', '')
            if self.alltimeouts>0:
                self.js_print('test timeouts        : %d' % self.alltimeouts, '<br>', '')
            if self.allasserts>0:
                self.js_print('assertions           : %d' % self.allasserts, '<br>', '')
                
            if self.js_output:
                print 'Results were written to %s' % self.js_output

        if self.ashErrors:
            exit(1)
            
#if __name__ == '__main__':
#    test = RuntestBase()
