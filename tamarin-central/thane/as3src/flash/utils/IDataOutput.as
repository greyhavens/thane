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

package flash.utils {

public interface IDataOutput
{
    function writeBytes(bytes :ByteArray, offset :uint = 0, length :uint = 0) :void;
    function writeBoolean(value :Boolean) :void;
    function writeByte(value :int) :void;
    function writeObject (object :*) :void 
    function writeShort(value :int) :void;
    function writeInt(value :int) :void;
    function writeUnsignedInt(value :uint) :void;
    function writeFloat(value :Number) :void;
    function writeDouble(value :Number) :void;
    function writeUTF(value :String) :void;
    function writeUTFBytes(value :String) :void;
    function get endian() :String;
    function set endian(type :String) :void;
    function get objectEncoding () :uint;
    function set objectEncoding (encoding :uint) :void;
}

}