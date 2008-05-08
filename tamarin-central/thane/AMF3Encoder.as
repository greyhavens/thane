package {

import avmplus.*;
import flash.utils.getQualifiedClassName;
import flash.utils.ByteArray;
import flash.utils.Dictionary;

/**
 * Partial AMF3 encoder, based on
 *      http://download.macromedia.com/pub/labs/amf/amf3_spec_121207.pdf
 */
public class AMF3Encoder
{
    public static function encode (bytes :ByteArray, value :*) :void
    {
        _ctx = new Context(bytes);
        encodeValue(value);
    }

    private static function encodeValue (value :*) :void
    {
        if (value === undefined) {
            _ctx.bytes.writeByte(0x00);

        } else if (value === null) {
            _ctx.bytes.writeByte(0x01);

        } else if (value === false) {
            _ctx.bytes.writeByte(0x02);

        } else if (value === true) {
            _ctx.bytes.writeByte(0x03);            

        } else if (value is int || value is uint || value is Number) {
            // figure out if this number could be represented as an AMF3 int
            var doublify :Boolean;
            if (value is int) {
                // signed integers must lie within -2^28 and 2^28-1
                doublify = (value < -(1 << 28)) || (value >= (1 << 28));
            } else if (value is uint) {
                // unsigned between 0 and 2^29-1
                doublify = (value >= (1 << 29));
            } else {
                // general Numbers are always doubles
                doublify = true;
            }

            if (doublify) {
                _ctx.bytes.writeByte(0x05);
                encodeDouble(value);
            } else {
                _ctx.bytes.writeByte(0x04);
                encodeInteger(uint(value));
            }

        } else if (value is String) {
            _ctx.bytes.writeByte(0x06);
            encodeString(value as String);

        /* } else if (value is XMLNode) { ... } // we don't even define the legacy XML types */

        } else if (value is Date) {
            // 0x08
            throw new Error("Date serialization not supported");

        } else if (value is Array) {
            _ctx.bytes.writeByte(0x09);
            encodeArray(value as Array);

        } else if ((value is XML) || (value is XMLList)) {
            // 0x0b
            throw new Error("XML serialization not supported");

        } else {
            var className :String = getQualifiedClassName(value);
            if (className == "flash.utils.ByteArray") {
                _ctx.bytes.writeByte(0x0c);
                encodeByteArray(value as ByteArray);
                return;
            }

            // Tamarin/Thane doesn't even have an IExternalizable type to test for

            _ctx.bytes.writeByte(0x0a);
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
            _ctx.bytes.writeByte(0x80 | (n >> 21));
            _ctx.bytes.writeByte(0x80 | ((n >> 14) & 0x7F));
            _ctx.bytes.writeByte(0x80 | ((n >> 7) & 0x7F));
            _ctx.bytes.writeByte(0x00 | (n & 0x7F));

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

        encodeInteger(1 | (str.length << 1));
        if (str.length > 0) {
            _ctx.bytes.writeUTFBytes(str);
            // spec says we do not add the empty string to the reference table
            _ctx.sRef.table[str] = _ctx.sRef.ix ++;
        }
    }

    private static function encodeArray (arr :Array) :void
    {
        if (_ctx.oRef.table[arr] !== undefined) {
            encodeInteger(_ctx.oRef.table[arr] << 1);
            return;
        }

        // arrays consist of a dense part (zero-based uninterrupted numerical indices)
        // and an associate part; find the dense first --
        var denseEnd :int = 0;
        while (denseEnd < arr.size && arr[denseEnd] === undefined) {
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

        _ctx.oRef.table[arr] = _ctx.oRef.ix ++;
    }

    private static function encodeObject (object :Object) :void
    {
        if (_ctx.oRef.table[object] !== undefined) {
            encodeInteger(_ctx.oRef.table[object] << 1);
            return;
        }

        // enumerate the sealed types
        var vars :Array = Domain.currentDomain.getVariables(object);
        var varSet :Dictionary = new Dictionary();
        for (var ii :int = 0; ii < vars.length; ii ++) {
            varSet[vars[ii]] = true;
        }

        // then whatever's left, if anything, is dynamic
        var dynVars :Array = [ ];
        for (var prop :String in object) {
            if (varSet[prop] === undefined) {
                dynVars.push(prop);
            }
        }

        // bits 0 and 1 set, 2 clear, 3 determines dynamic or not
        encodeInteger(1 | 2 | (dynVars.length > 0 ? 8 : 0) | (vars.length << 4));

        // encode the class name
        var alias :String = getClassAlias(className);
        encodeString(alias != null ? alias : "");

        // then send all the sealed member names
        for (ii = 0; ii < vars.length; ii ++) {
            encodeString(vars[ii]);
        }
        // and associated values
        for (ii = 0; ii < vars.length; ii ++) {
            encodeValue(object[vars[ii]]);
        }

        // end with the dynamic keys
        for (ii = 0; ii < dynVars.length; ii ++) {
            encodeString(dynVars[ii]);
        }
        // and values
        for (ii = 0; ii < dynVars.length; ii ++) {
            encodeValue(object[dynVars[ii]]);
        }

        // if there were any dynamic variables (which we signaled with a bit above),
        // end it all with an empty string
        if (dynVars.length > 0) {
            encodeString("");
        }

        _ctx.oRef.table[object] = _ctx.oRef.ix ++;
    }


    private static function encodeByteArray (bytes :ByteArray) :void
    {
        if (_ctx.oRef.table[bytes] !== undefined) {
            encodeInteger(_ctx.oRef.table[bytes] << 1);
            return;
        }

        // a byte array consists of a 1 bit, followed by the byte count
        encodeInteger(1 | (bytes.length << 1));
        // and then just... all the bytes
        _ctx.bytes.writeBytes(bytes);

        _ctx.oRef.table[bytes] = _ctx.oRef.ix ++;
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
