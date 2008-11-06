package {

import avmplus.*;
import flash.utils.ByteArray;
import flash.utils.Dictionary;
import flash.utils.IDataOutput;
import flash.utils.IExternalizable;

/**
 * Partial AMF3 encoder, based on
 *      http://download.macromedia.com/pub/labs/amf/amf3_spec_121207.pdf
 */
public class AMF3Encoder
{
    public static function encode (bytes :IDataOutput, value :*) :void
    {
        _ctx = new Context(bytes);
        encodeValue(value);
    }

    private static function encodeValue (value :*) :void
    {
        if (value === undefined) {
            _ctx.bytes.writeByte(AMF3.MARK_UNDEFINED);

        } else if (value === null) {
            _ctx.bytes.writeByte(AMF3.MARK_NULL);

        } else if (value === false) {
            _ctx.bytes.writeByte(AMF3.MARK_FALSE);

        } else if (value === true) {
            _ctx.bytes.writeByte(AMF3.MARK_TRUE);

        } else if (value is int && value >= -(1 << 28) && value < (1 << 28)) {
            if (value < 0) {
                value = (1 << 29) + value;
            }
            _ctx.bytes.writeByte(AMF3.MARK_INTEGER);
            encodeInteger(value);

        } else if (value is Number) {
            _ctx.bytes.writeByte(AMF3.MARK_DOUBLE);
            encodeDouble(value);

        } else if (value is String) {
            _ctx.bytes.writeByte(AMF3.MARK_STRING);
            encodeString(value as String);

        /* } else if (value is XMLNode) { ... } // we don't even define the legacy XML types */

        } else if (value is Date) {
            throw new Error("Date serialization not supported");

        } else if (value is Array) {
            _ctx.bytes.writeByte(AMF3.MARK_ARRAY);
            encodeArray(value as Array);

        } else if ((value is XML) || (value is XMLList)) {
            throw new Error("XML serialization not supported");

        } else {
            if (value is IExternalizable) {
                throw new Error("IExternalizable not supported");
            }
            if (Domain.currentDomain.getClassName(value) == "flash.utils::ByteArray") {
                _ctx.bytes.writeByte(AMF3.MARK_BYTE_ARRAY);
                encodeByteArray(value as ByteArray);
                return;
            }

            _ctx.bytes.writeByte(AMF3.MARK_OBJECT);
            encodeObject(value as Object);
        }
    }

    private static function encodeInteger (n :uint) :void
    {
        if (n < 0x80) {
            _ctx.bytes.writeByte(0x00 | n);

        } else if (n < 0x4000) {
            _ctx.bytes.writeByte(0x80 | (n >> 7));
            _ctx.bytes.writeByte(0x00 | (n & 0x7F));

        } else if (n < 0x200000) {
            _ctx.bytes.writeByte(0x80 | (n >> 14));
            _ctx.bytes.writeByte(0x80 | ((n >> 7) & 0x7F));
            _ctx.bytes.writeByte(0x00 | (n & 0x7F));

        } else if (n < 0x3FFFFFFF) {
            _ctx.bytes.writeByte(0x80 | (n >> 22));
            _ctx.bytes.writeByte(0x80 | ((n >> 15) & 0x7F));
            _ctx.bytes.writeByte(0x80 | ((n >> 8) & 0x7F));
            _ctx.bytes.writeByte(0x00 | (n & 0xFF));

        } else {
            throw new Error("Internal error - numerical overflow: " + n);
        }
    }

    private static function encodeDouble (n :Number) :void
    {
        _ctx.bytes.writeDouble(n);
    }

    private static function encodeString (str :String) :void
    {
        if (_ctx.sRef.table[str] !== undefined) {
            encodeInteger(0 | _ctx.sRef.table[str] << 1);
            return;
        }

        if (str.length == 0) {
            // spec says we do not add the empty string to the reference table
            encodeInteger(1);
            return;
        }

        // encode the string as UTF bytes
        var encoded :ByteArray = new ByteArray();
        encoded.writeUTFBytes(str);

        // mark the number of bytes (not characters) we're about to write
        encodeInteger(1 | (encoded.length << 1));
        _ctx.bytes.writeBytes(encoded);
        _ctx.sRef.table[str] = _ctx.sRef.ix ++;
    }

    private static function encodeArray (arr :Array) :void
    {
        if (_ctx.oRef.table[arr] !== undefined) {
            encodeInteger(_ctx.oRef.table[arr] << 1);
            return;
        }
        _ctx.oRef.table[arr] = _ctx.oRef.ix ++;

        // arrays consist of a dense part (zero-based uninterrupted numerical indices)
        // and an associate part; find the dense first --
        var denseEnd :int = 0;
        while (denseEnd < arr.length && arr[denseEnd] !== undefined) {
            denseEnd ++;
        }
        // and jot down in the bytestream how much of that there is
        encodeInteger(1 | (denseEnd << 1));

        // then see if there's any associative bits
        for (var prop :Object in arr) {
            if ((prop is int || prop is uint) && prop >= 0 && prop < denseEnd) {
                continue;
            }
            // if so, just stream them out
            encodeString(String(prop));
            encodeValue(arr[prop]);
        }
        encodeString("");

        // finally send the dense values
        for (var ii :int = 0; ii < denseEnd; ii ++) {
            encodeValue(arr[ii]);
        }
    }

    private static function encodeObject (object :Object) :void
    {
        if (_ctx.oRef.table[object] !== undefined) {
            encodeInteger(_ctx.oRef.table[object] << 1);
            return;
        }
        _ctx.oRef.table[object] = _ctx.oRef.ix ++;

        // enumerate the sealed types
        var vars :Array = Domain.currentDomain.getVariables(object);
        var varSet :Dictionary = new Dictionary();
        for (var ii :int = 0; ii < vars.length; ii ++) {
            varSet[vars[ii]] = true;
        }

        var dynVars;
        if (Domain.currentDomain.isDynamic(object)) {
            dynVars = [ ];
            for (var prop :String in object) {
                if (varSet[prop] === undefined) {
                    dynVars.push(prop);
                }
            }

        } else {
            dynVars = null;
        }

        // figure the class name
        var className :String = Domain.currentDomain.getClassName(object);

        // see if we've transmitted an instance of this class before
        if (_ctx.tRef.table[className] !== undefined) {
            // bit 0 set (not an instance ref), bit 1 clear (trait ref), rest is trait ref num
            encodeInteger(1 | _ctx.tRef.table[className] << 2);

        } else {
            // bits 0 and 1 set, 2 clear, 3 determines dynamic or not
            encodeInteger(1 | 2 | (dynVars !== null ? 8 : 0) | (vars.length << 4));

            var alias :String = AMF3.getAliasByClassName(className);
            encodeString(alias != null ? alias : "");

            // then send all the sealed member names
            for (ii = 0; ii < vars.length; ii ++) {
                encodeString(vars[ii]);
            }

            // and store the refernce
            _ctx.tRef.table[className] = _ctx.tRef.ix ++;
        }

        // send the sealed member values
        for (ii = 0; ii < vars.length; ii ++) {
            encodeValue(object[vars[ii]]);
        }

        if (dynVars !== null) {
            // potentially do the dynamic keys
            for (ii = 0; ii < dynVars.length; ii ++) {
                encodeString(dynVars[ii]);
                encodeValue(object[dynVars[ii]]);
            }
            // end with an empty string
            encodeString("");
        }

    }


    private static function encodeByteArray (bytes :ByteArray) :void
    {
        if (_ctx.oRef.table[bytes] !== undefined) {
            encodeInteger(_ctx.oRef.table[bytes] << 1);
            return;
        }
        _ctx.oRef.table[bytes] = _ctx.oRef.ix ++;

        // a byte array consists of a 1 bit, followed by the byte count
        encodeInteger(1 | (bytes.length << 1));
        // and then just... all the bytes
        _ctx.bytes.writeBytes(bytes);
    }

    private static var _ctx :Context;
}
}

import flash.utils.IDataOutput;
import flash.utils.Dictionary;

class Context
{
    public var bytes :IDataOutput;

    public var oRef :References = new References();
    public var sRef :References = new References();
    public var tRef :References = new References();

    public function Context (bytes :IDataOutput)
    {
        this.bytes = bytes;
    }
}

class References
{
    public var table :Dictionary = new Dictionary();
    public var ix :uint = 0;
}
