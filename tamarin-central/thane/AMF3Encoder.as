package {

/**
 * Partial AMF3 encoder, based on
 *      http://download.macromedia.com/pub/labs/amf/amf3_spec_121207.pdf
 */
public class AMF3Encoder
{
    public static function encode (bytes :ByteArray, value :*) :void
    {
        encodeValue(new Context(bytes), bytes, value);
    }

    private static function encodeValue (context :Context, value :*) :void
    {
        if (value === undefined) {
            context.bytes.writeByte(0x00);

        } else if (value === null) {
            context.bytes.writeByte(0x01);

        } else if (value === false) {
            context.bytes.writeByte(0x02);

        } else if (value === true) {
            context.bytes.writeByte(0x03);            

        } else if (value is int || value is uint || value is Number) {
            // figure out if this number could be represented as an AMF3 int
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
                context.bytes.writeByte(0x05);
                encodeDouble(value);
            } else {
                context.bytes.writeByte(0x04);
                encodeInteger(uint(value));
            }

        } else if (value is String) {
            context.bytes.writeByte(0x06);
            encodeString(value as String);

        } else if ((value is XMLNode) || (value is XML) || (value is XMLList)) {
            // 0x07 & 0x0b
            throw new Error("XML serialization not supported");

        } else if (value is Date) {
            // 0x08
            throw new Error("Date support not supported");

        } else if (value is Array) {
            context.bytes.writeByte(0x09);
            encodeArray(value as Array);

        } else {
            var className :String = getQualifiedClassName(value);
            if (className == "flash.utils.ByteArray") {
                context.bytes.writeByte(0x0c);
                encodeByteArray(context, value as ByteArray);
                return;
            }

            if (value is IExternalizable) {
                throw new Error("IExternalizable serialization not supported");
            }

            context.bytes.writeByte(0x0a);
            encodeObject(context, value as Object);
        }
    }

    private static function encodeInteger (context :Context, n :uint) :void
    {
        if (n < 0x80) {
            context.bytes.writeByte(0x00 | n);

        } else if (n < 0x4000) {
            context.bytes.writeByte(0x80 | (n >> 7));
            context.bytes.writeByte(0x00 | (n & 0x7F));

        } else if (n < 0x200000) {
            context.bytes.writeByte(0x80 | (n >> 14));
            context.bytes.writeByte(0x80 | ((n >> 7) & 0x7F));
            context.bytes.writeByte(0x00 | (n & 0x7F));

        } else {
            context.bytes.writeByte(0x80 | (n >> 21));
            context.bytes.writeByte(0x80 | ((n >> 14) & 0x7F));
            context.bytes.writeByte(0x80 | ((n >> 7) & 0x7F));
            context.bytes.writeByte(0x00 | (n & 0x7F));

        }
    }

    private static function encodeDouble (n :Number) :void
    {
        context.bytes.writeDouble(n);
    }

    private static function encodeString (context :Context, str :String) :void
    {
        if (context.sRef.table[str] !== undefined) {
            encodeInteger(0 | ref.table[str] << 1);
            return;
        }

        encodeInteger(1 | (str.length << 1));
        if (str.length > 0) {
            context.bytes.writeUTFBytes(str);
            // spec says we do not add the empty string to the reference table
            context.sRef.table[str] = context.sRef.ix ++;
        }
    }

    private static function encodeArray (context :Context, arr :Array) :void
    {
        if (context.oRef.table[value] !== undefined) {
            encodeInteger(context.oRef.table[value] << 1);
            return;
        }

        var denseEnd :int = 0;
        while (denseEnd < arr.size && arr[denseEnd] === undefined) {
            denseEnd ++;
        }
        encodeInteger(1 | (denseEnd << 1));

        for (var prop :Object in arr) {
            if ((prop is int || prop is uint) && prop >= 0 && prop < denseEnd) {
                continue;
            }
            encodeString(String(prop));
            encodeValue(context, arr[prop]);
        }
        encodeString("");

        for (var ii :int = 0; ii < denseEnd; ii ++) {
            encodeValue(context, arr[ii]);
        }

        context.oRef.table[value] = context.oRef.ix ++;
    }

    private static function encodeObject (context :Context, object :Object) :void
    {
        if (context.oRef.table[object] !== undefined) {
            encodeInteger(context.oRef.table[object] << 1);
            return;
        }

        // enumerate the sealed types
        var vars :Array = Domain.currentDomain.getVariables(type);
        for (var ii :int = 0; ii < vars.length; ii ++) {
            varSet[vars[ii]] = true;
        }

        // then whatever's left, if anything, is dynamic
        for (var prop :String in type) {
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
            encodeValue(context, object[vars[ii]]);
        }

        // end with the dynamic keys
        for (ii = 0; ii < dynVars.length; ii ++) {
            encodeString(dynVars[ii]);
        }
        // and values
        for (ii = 0; ii < dynVars.length; ii ++) {
            encodeValue(context, object[dynVars[ii]]);
        }

        // if there were any dynamic variables (which we signaled with a bit above),
        // end it all with an empty string
        if (dynVars.length > 0) {
            encodeString("");
        }

        context.oRef.table[object] = context.oRef.ix ++;
    }


    private static function encodeByteArray (context :Context, bytes :ByteArray) :void
    {
        if (context.oRef.table[object] !== undefined) {
            encodeInteger(context.oRef.table[object] << 1);
            return;
        }

        // a byte array consists of a 1 bit, followed by the byte count
        encodeInteger(1 | (bytes.length << 1));
        // and then just... all the bytes
        context.bytes.writeBytes(bytes);

        context.oRef.table[object] = context.oRef.ix ++;
    }
}
}

class Context
{
    public var bytes :ByteArray;

    public var oRef :References = new References();
    public var sRef :References = new References();
    public var tRef :References = new References();

    public function EncodingContext (bytes :ByteArray)
    {
        this.bytes = bytes;
    }
}

class References
{
    public var table :Dictionary = new Dictionary();
    public var ix :uint = 0;
}
