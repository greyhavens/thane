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

package flash.events {

public class ProgressEvent extends Event
{
    public static const SOCKET_DATA :String = "socketData";
    public static const PROGRESS :String = "progress";

    public function ProgressEvent (
        type :String, bubbles :Boolean = false, cancelable :Boolean = false,
        bytesLoaded :Number = 0, bytesTotal :Number = 0)
    {
        _bytesLoaded = bytesLoaded;
        _bytesTotal = bytesTotal;

        super(type, bubbles, cancelable);
    }

    public function get bytesLoaded () :Number
    {
        return _bytesLoaded;
    }

    public function set bytesLoaded (value :Number) :void
    {
        _bytesLoaded = value;
    }

    public function get bytesTotal () :Number
    {
        return _bytesTotal;
    }

    public function set bytesTotal (value :Number) :void
    {
        _bytesTotal = value;
    }

    public override function clone () :Event
    {
        return new ProgressEvent(type, bubbles, cancelable, bytesLoaded, bytesTotal);
    }

    public override function toString () :String
    {
        return formatToString("ProgressEvent", "type", "bubbles", "cancelable",
                              "bytesLoaded", "bytesTotal");
    }

    protected var _bytesLoaded :Number;
    protected var _bytesTotal :Number;
}
}
