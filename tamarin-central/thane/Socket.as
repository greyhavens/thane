//
// $Id: $

package flash.net
{

import flash.errors.IOError;

import flash.events.Event;
import flash.events.EventDispatcher;

import flash.utils.ByteArray;
import flash.utils.IDataInput;
import flash.utils.IDataOutput;

public class Socket extends EventDispatcher
    implements IDataInput, IDataOutput
{
    public function Socket (host :String = null, port :int = 0)
    {
        if (_theSocket != null) {
            throw new Error("You may not create Sockets");
        }
        _theSocket = this;

        _state = ST_VIRGIN;
        Thane.requestHeartbeat(heartbeat);

        if (host != null && port > 0) {
            connect(host, port);
        }
    }

    public function connect (host: String, port: int) :void
    {
        if (host != "127.0.0.1") {
            throw new Error("You may only connect to '127.0.0.1'");
        }

        if (_state == ST_WAITING) {
            return;
        }

        if (_state == ST_CONNECTED) {
            close();
        }

        _port = port;

        _state = ST_WAITING;
    }

    public function close () :void
    {
        if (_state != ST_CONNECTED) {
            throw new IOError("Socket was not open");
        }
        nb_disconnect();
    }

    protected function heartbeat () :void
    {
        switch(_state) {
        case ST_VIRGIN:
            break;

        case ST_CONNECTED:
            nb_read(_iBuf);
            if (_oBuf.bytesAvailable > 0) {
                var pos :int = _oBuf.position;
                var n :int = nb_write(_oBuf);
                _oBuf.position = pos + n;
            }

            _iBuf = massageBuffer(_iBuf);
            _oBuf = massageBuffer(_oBuf);

            break;

        case ST_WAITING:
            if (nb_connect(_port)) {
                _state = ST_CONNECTED;
                dispatchEvent(new Event(Event.CONNECT));
            }
            break;
        }
    }

    private native function nb_disconnect() :void;

    /** Returns -1 for error, 0 for keep trying, 1 for success. */
    private native function nb_connect (port :int) :int;

    /** Returns -1 for error, else the number of bytes read. */
    private native function nb_read (iBuf :ByteArray) :int;

    /** Returns -1 for error, else the number of bytes written. */
    private native function nb_write (oBuf :ByteArray) :int;

    public function flush () :void
    {
        // TODO: we should probably respect flush somehow...
    }

	public function get endian(): String
    {
        return _iBuf.endian;
    }

	public function set endian(type: String) :void
    {
        _iBuf.endian = type;
        _oBuf.endian = type;
    }

    public function get bytesAvailable () :uint
    {
        return _iBuf.bytesAvailable;
    }

    public function set objectEncoding (encoding :uint) :void
    {
        throw new Error("not supported");
    }

    public function get objectEncoding () :uint
    {
        return ObjectEncoding.AMF3;
    }

    // IDataInput
    public function readBytes(bytes :ByteArray, offset :uint=0, length :uint=0) :void
    {
        return _iBuf.readBytes(bytes, offset, length);
    }

    public function readBoolean() :Boolean
    {
        return _iBuf.readBoolean();
    }

    public function readByte() :int
    {
        return _iBuf.readByte();
    }

    public function readUnsignedByte() :uint
    {
        return _iBuf.readUnsignedByte();
    }

    public function readShort() :int
    {
        return _iBuf.readShort();
    }

    public function readUnsignedShort() :uint
    {
        return _iBuf.readUnsignedShort();
    }

    public function readInt() :int
    {
        return _iBuf.readInt();
    }

    public function readUnsignedInt() :uint
    {
        return _iBuf.readUnsignedInt();
    }

    public function readFloat() :Number
    {
        return _iBuf.readFloat();
    }

    public function readDouble() :Number
    {
        return _iBuf.readDouble();
    }

    public function readUTF() :String
    {
        return _iBuf.readUTF();
    }

    public function readUTFBytes(length :uint) :String
    {
        return _iBuf.readUTFBytes(length);
    }

    // IDataOutput
    public function writeBytes(bytes :ByteArray, offset :uint = 0, length :uint = 0) :void
    {
        _oBuf.writeBytes(bytes, offset, length);
    }

    public function writeBoolean(value :Boolean) :void
    {
        _oBuf.writeBoolean(value);
    }

    public function writeByte(value :int) :void
    {
        _oBuf.writeByte(value);
    }

    public function writeShort(value :int) :void
    {
        _oBuf.writeShort(value);
    }

    public function writeInt(value :int) :void
    {
        _oBuf.writeInt(value);
    }

    public function writeUnsignedInt(value :uint) :void
    {
        _oBuf.writeUnsignedInt(value);
    }

    public function writeFloat(value :Number) :void
    {
        _oBuf.writeFloat(value);
    }

    public function writeDouble(value :Number) :void
    {
        _oBuf.writeDouble(value);
    }

    public function writeUTF(value :String) :void
    {
        _oBuf.writeUTF(value);
    }

    public function writeUTFBytes(value :String) :void
    {
        _oBuf.writeUTFBytes(value);
    }

    protected function massageBuffer (buffer :ByteArray) :ByteArray
    {
        // we only switch buffers if we're wasting 25% and at least 64k
        if (buffer.position < 0x10000 || 4*buffer.position < buffer.length) {
            return buffer;
        }
        var newBuffer :ByteArray = new ByteArray();

        buffer.readBytes(newBuffer, buffer.position);
        return newBuffer;
    }

    private var _state :int;
    private var _port :int;

    private var _iBuf :ByteArray = new ByteArray();
    private var _oBuf :ByteArray = new ByteArray();

    private static var _theSocket :Socket = null;

    private static const ST_VIRGIN :int = 1;
    private static const ST_WAITING :int = 2;
    private static const ST_CONNECTED :int = 3;
}
}
