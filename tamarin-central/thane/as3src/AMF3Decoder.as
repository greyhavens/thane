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
import flash.utils.getDefinitionByName;
import flash.utils.ByteArray;
import flash.utils.Dictionary;
import flash.utils.IDataInput;

/**
 * Partial AMF3 decoder, based on
 *      http://download.macromedia.com/pub/labs/amf/amf3_spec_121207.pdf
 */
public class AMF3Decoder
{
    public static function decode (bytes :IDataInput) :*
    {
        _ctx = new DecoderContext(bytes);
        return decodeValue();
    }

    private static function decodeValue () :*
    {
        var marker :int = _ctx.bytes.readByte();
        switch(marker) {
        default:
            throw new Error("Unexpected marker in AMF3: " + marker);

        case AMF3.MARK_XML:
        case AMF3.MARK_XML_DOC:
            throw new Error("XML deserialization not supported");

        case AMF3.MARK_DATE:
            // TODO: this is just lazy, it's super trivial
            throw new Error("Date deserialization not supported");

        case AMF3.MARK_UNDEFINED:
            return undefined;

        case AMF3.MARK_NULL:
            return null;

        case AMF3.MARK_FALSE:
            return false;

        case AMF3.MARK_TRUE:
            return true;

        case AMF3.MARK_INTEGER:
            return decodeInteger();

        case AMF3.MARK_DOUBLE:
            return _ctx.bytes.readDouble();

        case AMF3.MARK_STRING:
            return decodeString();

        case AMF3.MARK_ARRAY:
            return decodeArray();

        case AMF3.MARK_OBJECT:
            return decodeObject();

        case AMF3.MARK_BYTE_ARRAY:
            return decodeByteArray();

        }
    }

    private static function decodeInteger () :int
    {
        var b :uint = _ctx.bytes.readUnsignedByte();
        var n :uint = (b & 0x7f);
        if ((b & 0x80) != 0) {
            b = _ctx.bytes.readUnsignedByte();
            n = (n << 7) | (b & 0x7f);

            if ((b & 0x80) != 0) {
                b = _ctx.bytes.readUnsignedByte();
                n = (n << 7) | (b & 0x7f);

                if ((b & 0x80) != 0) {
                    b = _ctx.bytes.readUnsignedByte();
                    n = (n << 8) | b;

                    if (n >= (1 << 28)) {
                        // adjust for the fact that the 29th bit was the sign bit
                        return n - (1 << 29);
                    }
                }
            }
        }
        return n;
    }

    private static function decodeString () :String
    {
        var code :uint = decodeInteger();

        // is it a reference?
        if ((code & 0x01) == 0) {
            return _ctx.sRef[code >> 1];
        }

        // if not, read N bytes
        if ((code >> 1) > 0) {
            var str :String = _ctx.bytes.readUTFBytes(code >> 1);
            _ctx.sRef.push(str);
            return str;
        }
        return "";
    }

    private static function decodeArray () :Array
    {
        var code :uint = decodeInteger();

        // is it a reference?
        if ((code & 0x01) == 0) {
            return _ctx.oRef[code >> 1];
        }

        // if not, create our array & note its reference
        var result :Array = [ ];
        _ctx.oRef.push(result);

        // then first read associative pairs
        var key :String;
        while ((key = decodeString()).length > 0) {
            result[key] = decodeValue();
        }

        // finally read N dense values
        var N :uint = code >> 1;
        for (var ii :int = 0; ii < N; ii ++) {
            result[ii] = decodeValue();
        }
        return result;
    }

    private static function decodeObject ():Object
    {
        var code :uint = decodeInteger();

        // is it an instance reference?
        if ((code & 0x01) == 0) {
            return _ctx.oRef[code >> 1];
        }

        var traits :Traits;
        // is it a trait reference?
        if ((code & 0x02) == 0) {
            traits = _ctx.tRef[code >> 2];

        } else if ((code & 0x04) == 0) {
            // it's a new trait, read class name & N sealed variable names
            var alias :String = decodeString();
            var className :String = (alias.length > 0) ? AMF3.getClassNameByAlias(alias) : null;
            var N :uint = code >> 4;
            var vars :Array = new Array(N);
            for (var ii :int = 0; ii < N; ii ++) {
                vars[ii] = decodeString();
            }
            traits = new Traits(className, (code & 0x08) != 0, vars);
            _ctx.tRef.push(traits);

        } else {
            throw new Error("IExternalizable serialization not supported");
        }

        // instantiate the object
        var obj :Object;
        if (traits.className != null) {
            var classObject :Class = getDefinitionByName(traits.className);
            obj = new classObject();
        } else {
            obj = { };
        }

        // register the reference
        _ctx.oRef.push(obj);

        // read the sealed values
        for (var ii :int = 0; ii < traits.vars.length; ii ++) {
            obj[traits.vars[ii]] = decodeValue();
        }

        // if this object was marked dynamic, read key/value pairs until ""
        if (traits.isDynamic) {
            var key :String;
            while ((key = decodeString()).length > 0) {
                obj[key] = decodeValue();
            }
        }
        return obj;
    }

    private static function decodeByteArray () :ByteArray
    {
        var code :uint = decodeInteger();
        // is it an instance reference?
        if ((code & 0x01) == 0) {
            return _ctx.oRef[code >> 1];
        }

        // if not, instantiate
        var bytes :ByteArray = new ByteArray();

        // register
        _ctx.oRef.push(bytes);

        // and read the bytes
        _ctx.bytes.readBytes(bytes, 0, code >> 1);
        return bytes;
    }

    private static var _ctx :DecoderContext;
}
}


import flash.utils.IDataInput;
import flash.utils.Dictionary;

class DecoderContext
{
    public var bytes :IDataInput;

    public var oRef :Array = new Array();
    public var sRef :Array = new Array();
    public var tRef :Array = new Array();

    public function DecoderContext (bytes :IDataInput)
    {
        this.bytes = bytes;
    }
}

class Traits
{
    public var className :String;
    public var isDynamic :Boolean;
    public var vars :Array;

    public function Traits (className :String, isDynamic :Boolean, vars :Array)
    {
        this.className = className;
        this.isDynamic = isDynamic;
        this.vars = vars;
    }
}
