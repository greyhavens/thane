//
// $Id$
//
// Thane - an enhancement of Mozilla/Adobe's Tamarin project with partial Flash Player compatibility
// Copyright (C) 2008-2009 Three Rings Design, Inc.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted
// provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice, this list of
//    conditions and the following disclaimer in the documentation and/or other materials provided
//    with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

package {

import avmplus.*;

import flash.utils.Dictionary;
import flash.utils.getQualifiedClassName;

public class AMF3
{
    public static const MARK_UNDEFINED :uint    = 0x00;
    public static const MARK_NULL :uint         = 0x01;
    public static const MARK_FALSE :uint        = 0x02;
    public static const MARK_TRUE :uint         = 0x03;
    public static const MARK_INTEGER :uint      = 0x04;
    public static const MARK_DOUBLE :uint       = 0x05;
    public static const MARK_STRING :uint       = 0x06;
    public static const MARK_XML_DOC :uint      = 0x07;
    public static const MARK_DATE :uint         = 0x08;
    public static const MARK_ARRAY :uint        = 0x09;
    public static const MARK_OBJECT :uint       = 0x0a;
    public static const MARK_XML :uint          = 0x0b;
    public static const MARK_BYTE_ARRAY :uint   = 0x0c;

    public static function getClassNameByAlias (aliasName :String) :String
    {
        if (aliasName == null) {
            throw new TypeError("null argument to getClassNameByAlias");
        }
        return _aliasToClass[aliasName];
    }

    public static function getAliasByClassName (className :String) :String
    {
        if (className == null) {
            throw new TypeError("null argument to getAliasByClassName");
        }
        return _classToAlias[className];
    }

    public static function registerClassAlias (aliasName :String, classObject :Class) :void
    {
        // TODO: protect against user code
        if (aliasName == null || classObject == null) {
            throw new TypeError("null argument to registerClassAlias");
        }
        var className :String = getQualifiedClassName(classObject);
        _aliasToClass[aliasName] = className;
        _classToAlias[className] = aliasName;
    }

    private static var _aliasToClass :Dictionary = new Dictionary();
    private static var _classToAlias :Dictionary = new Dictionary();
}
}
