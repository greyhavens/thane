package {

import avmplus.*;
import flash.utils.ByteArray;
import flash.utils.Dictionary;

/**
 * Partial AMF3 decoder, based on
 *      http://download.macromedia.com/pub/labs/amf/amf3_spec_121207.pdf
 */
public class AMF3Decoder
{
    public static function decode (bytes :ByteArray) :*
    {
        _ctx = new Context(bytes);
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
        var b :uint = _ctx.bytes.readByte();
        var n :uint = (b & 0x7f);
        if ((b & 0x80) != 0) {
            b = _ctx.bytes.readByte();
            n = (n << 7) | (b & 0x7f);

            if ((b & 0x80) != 0) {
                b = _ctx.bytes.readByte();
                n = (n << 7) | (b & 0x7f);

                if ((b & 0x80) != 0) {
                    b = _ctx.bytes.readByte();
                    n = (n << 8) | b;

                    if (n >= (1 << 28)) {
                        // adjust for the fact that the 29th bit was the sign bit
                        return (1 << 29) - n;
                    }
                    return n;
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
        var str :String = _ctx.bytes.readUTFBytes(code >> 1);
        _ctx.sRef.table[str] = _ctx.sRef.ix ++;
        return str;
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
        _ctx.oRef.table[result] = _ctx.oRef.ix ++;

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
            var className = (alias.length > 0) ? AMF3.getClassNameByAlias(alias) : "";
            var N :uint = code >> 4;
            var vars :Array = new Array(N);
            for (var ii :int = 0; ii < N; ii ++) {
                vars[ii] = decodeString();
            }
            traits = new Traits(className, (code & 0x08) != 0, vars);
            _ctx.tRef.table[traits] = _ctx.tRef.ix ++;

        } else {
            throw new Error("IExternalizable serialization not supported");
        }

        // instantiate the object
        var obj :Object;
        if (traits.className != null) {
            obj = Domain.currentDomain.getClass(traits.className);
        } else {
            obj = { };
        }

        // register the reference
        _ctx.oRef.table[obj] = _ctx.oRef.ix ++;

        // read the sealed values
        for (var ii :int = 0; ii < traits.vars.length; ii ++) {
            obj[vars[ii]] = decodeValue();
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
        _ctx.oRef.table[bytes] = _ctx.oRef.ix ++;

        // and read the bytes
        _ctx.bytes.readBytes(bytes, 0, code >> 1);
        return bytes;
    }

    private static var _ctx :Context;
}
}


import flash.utils.ByteArray;
import flash.utils.Dictionary;

class Context
{
    public var bytes :ByteArray;

    public var oRef :References = new References();
    public var sRef :References = new References();
    public var tRef :References = new References();

    public function Context (bytes :ByteArray)
    {
        this.bytes = bytes;
    }
}

class References
{
    public var table :Dictionary = new Dictionary();
    public var ix :uint = 0;
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
