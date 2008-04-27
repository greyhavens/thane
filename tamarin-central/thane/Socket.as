//
// $Id: $

package flash.net
{

import flash.errors.IOError;

import flash.events.Event;
import flash.events.EventDispatcher;
import flash.events.IOErrorEvent;
import flash.events.ProgressEvent;

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
        if (!host) {
            throw new IOError("No host specified in connect()");
        }
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
        trace("Socket:connect() switching to ST_WAITING...");
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
        if (_theSocket == null) {
            return;
        }
        switch(_state) {
        case ST_VIRGIN:
            break;

        case ST_CONNECTED:
            _oBuf.position = _oPos;
            var wrote :int = 0;

            var read :int = nb_read(_iBuf);
            if (_oPos < _oBuf.length) {
                wrote = nb_write(_oBuf);
            }
            if (read > 0 || wrote > 0) {
                trace("Socket:heartbeat() read " + read + ", wrote " + wrote);
            }

            _iBuf = massageBuffer(_iBuf);
            _oBuf = massageBuffer(_oBuf);

            _oPos = _oBuf.position;

            if (read > 0) {
                _bytesTotal += read;
                dispatchEvent(new ProgressEvent(ProgressEvent.SOCKET_DATA));
            }

            break;

        case ST_WAITING:
            switch(nb_connect(_port)) {
            case -1:
                dispatchEvent(new IOErrorEvent(IOErrorEvent.IO_ERROR));
                _state = ST_VIRGIN;
                break;
            case 0:
                trace("Socket:heartbeat() trying to connect...");
                break;
            case 1:
                trace("Socket:heartbeat() connected!");
                _state = ST_CONNECTED;
                dispatchEvent(new Event(Event.CONNECT));
                break;
            }
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

        if (buffer.bytesAvailable > 0) {
            buffer.readBytes(newBuffer, 0, buffer.bytesAvailable);
        }
        return newBuffer;
    }

    private var _state :int;
    private var _port :int;

    private var _iBuf :ByteArray = new ByteArray();
    private var _oBuf :ByteArray = new ByteArray();

    // we have to keep our own position in oBuf because writing changes it (?!)
    private var _oPos :int = 0;

    private var _bytesTotal :int = 0;

    private static var _theSocket :Socket = null;

    private static const ST_VIRGIN :int = 1;
    private static const ST_WAITING :int = 2;
    private static const ST_CONNECTED :int = 3;
}
}
