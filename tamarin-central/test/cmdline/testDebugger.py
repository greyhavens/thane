#!/usr/bin/env python

#!/usr/bin/env python

#* ***** BEGIN LICENSE BLOCK *****
#* Version: MPL 1.1/GPL 2.0/LGPL 2.1
#*
#* The contents of this file are subject to the Mozilla Public License Version
#* 1.1 (the "License"); you may not use this file except in compliance with
#* the License. You may obtain a copy of the License at
#* http://www.mozilla.org/MPL/
#*
#* Software distributed under the License is distributed on an "AS IS" basis,
#* WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
#* for the specific language governing rights and limitations under the
#* License.
#*
#* The Original Code is [Open Source Virtual Machine.].
#*
#* The Initial Developer of the Original Code is
#* Adobe System Incorporated.
#* Portions created by the Initial Developer are Copyright (C) 2009
#* the Initial Developer. All Rights Reserved.
#*
#* Contributor(s):
#*   Adobe AS3 Team
#*
#* Alternatively, the contents of this file may be used under the terms of
#* either the GNU General Public License Version 2 or later (the "GPL"), or
#* the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
#* in which case the provisions of the GPL or the LGPL are applicable instead
#* of those above. If you wish to allow use of your version of this file only
#* under the terms of either the GPL or the LGPL, and not to allow others to
#* use your version of this file under the terms of the MPL, indicate your
#* decision by deleting the provisions above and replace them with the notice
#* and other provisions required by the GPL or the LGPL. If you do not delete
#* the provisions above, a recipient may use your version of this file under
#* the terms of any one of the MPL, the GPL or the LGPL.
#*
#* ***** END LICENSE BLOCK ***** */
#


from cmdutils import *
import os

def run():
    r=RunTestLib()
    if r.avmrd==None:
        print "environment variable 'AVMRD' is not set"
        print "debugger FAILED"
        return

    r.run_test('debugger basic','%s -d testdata/debug.abc'%r.avmrd,input='quit\n',expectedout=['40:  public class debug','(asdb)'])
    r.run_test('debugger list','%s -d testdata/debug.abc'%r.avmrd,input='list\nlist 40\nquit\n',expectedout=['44:     public function pubfunc','46:[ ]+privfunc\(\);']);
    r.run_test('debugger break','%s -d testdata/debug.abc'%r.avmrd,input='break 13\ninfo break\nquit\n',expectedout=['Breakpoint 1: file debug.as, 13.','1 at debug.as:13']);
    r.run_test('debugger stacktrace','%s -d testdata/debug.abc'%r.avmrd,input='bt\nquit\n',expectedout=['#0   global.*global\$init']);
    r.run_test('debugger break2','%s -d testdata/debug.abc'%r.avmrd,input='break 13\ndelete 1\nbreak 10\ninfo break\nquit\n',expectedout=['Breakpoint 1 deleted.','2 at debug.as:10']);
    r.run_test('debugger next','%s -d testdata/debug.abc'%r.avmrd,input='break 13\ncontinue\nnext\nstep\nquit\n',expectedout=['13:.*public function locals','15:.*var local2:int','16:.*local2']);
    r.run_test('debugger locals','%s -d testdata/debug.abc'%r.avmrd,input='break 13\ncontinue\nnext\nnext\nprint local2\ninfo locals\nquit\n',expectedout=['16:.*local2=15']);
    r.run_test('debugger exception','%s -d testdata/debug.abc'%r.avmrd,input='continue\nquit\n',expectedout=['Exception has been thrown:']);
    r.run_test('debugger local exception','%s -d testdata/debug.abc'%r.avmrd,input='break 17\ncontinue\nset local2 = 5\nprint local2\nquit\n',expectedout=['17:']);

if __name__ == '__main__':
    r=RunTestLib()
    r.compile("testdata/debug.as",None,"-d")
    run()
