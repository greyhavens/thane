ASC has similar requirements as flex for building/testing.

It requires the following software that is not under source control:

    jdk 1.5 or greater (see http://java.sun.com/products/archive/j2se/5.0_13/index.html)
    
    Ant 1.7.0 (see http://archive.apache.org/dist/ant/binaries/)

To build asc:

   cd build/java
   ant clean main
   

Running tests

Prerequisite setup:
1) cygwin
	- python 2.5
2) pexpect http://www.noah.org/wiki/Pexpect
	http://pexpect.sourceforge.net/pexpect-2.3.tar.gz
	- install into python
3) mercurial http://www.selenic.com/mercurial/wiki/
	http://www.selenic.com/mercurial/release/mercurial-1.0.tar.gz
	- install into python
4) clone the tamarin repository
	hg clone http://hg.mozilla.org/tamarin-central/


Tamarin Acceptance Suite:
1) update tamarin repository
            hg pull -u
2) delete all old abc files
            cd tamarin-central/test/acceptance
            find ./ -name "*.abc" -exec rm {} \;
3) rebuild the abc files
            export ASC==<svnROOT>/trunk/modules/asc/lib/asc.jar
            export GLOBALABC=<svnROOT>/trunk/modules/asc/abc/builtin.abc
            ./rebuildtests.py
4) run the testsuite
            ./runtests.py --avm=<svnROOT>/trunk/modules/asc/bin/windows/avmplus_s.exe

ASC Errors and Warnings			
There is an Ant target called ‘asc-tests’ in the build.xml. 
Use this target to run the ASC error test suite, which is in SVN under modules/asc/test/compiler. 
There is one prerequisite, you must have the Cygwin version of Python 2.5 or greater installed for these tests to run properly. 

