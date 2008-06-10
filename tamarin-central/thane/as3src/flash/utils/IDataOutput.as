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