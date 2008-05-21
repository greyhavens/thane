package flash.utils {

public interface IDataInput
{
    function readBytes(bytes :ByteArray, offset :uint=0, length :uint=0) :void;
    function readBoolean() :Boolean;
    function readByte() :int;
    function readUnsignedByte() :uint;
    function readObject () :*
    function readShort() :int;
    function readUnsignedShort() :uint;
    function readInt() :int;
    function readUnsignedInt() :uint;
    function readFloat() :Number;
    function readDouble() :Number;
    function readUTF() :String;
    function readUTFBytes(length :uint) :String;
    function get bytesAvailable() :uint;
    function get endian() :String;
    function set endian(type :String) :void;
}

}