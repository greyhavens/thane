/* -*- mode: java; tab-width: 4; insert-tabs-mode: nil; indent-tabs-mode: nil -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is [Open Source Virtual Machine.].
 *
 * The Initial Developer of the Original Code is
 * Adobe System Incorporated.
 * Portions created by the Initial Developer are Copyright (C) 2004-2006
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Adobe AS3 Team
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

/* Utilities adapted to the ECMAScript 4 reference implementation */

{
    use default namespace Util;
    
    use namespace intrinsic;

    intrinsic native function explodeDouble(d:double, idx:uint);

    public function explodeNumber(d) {
        d = double(d);
        let hi = explodeDouble(d, 0u);
        let lo = explodeDouble(d, 1u);
        return [lo & 255, (lo >> 8) & 255, (lo >> 16) & 255, (lo >> 24) & 255,
                hi & 255, (hi >> 8) & 255, (hi >> 16) & 255, (hi >> 24) & 255];
    }

    intrinsic native function writeFile(s:string, filename:string);

    public function writeStringToFile(s, filename) {
        writeFile(string(s), string(filename));
    }

    intrinsic native function beginBytes ();
    intrinsic native function pushByte (b:uint);
    intrinsic native function writeBytes(filename:string);

    function dumpABCFile(abcfile, filename) {
        var bytes = abcfile.getBytes();
        Util::assert( bytes.push );  /*FIXME ES4: really "bytes is Array" */

        let s = ""
        let len = bytes.length;
	beginBytes();
        for (let i = 0; i<len; ++i) {
	    pushByte (uint(bytes[i]));
        }

        print (filename, ", writing ",len," bytes");        
        writeBytes(filename);
    }
}
