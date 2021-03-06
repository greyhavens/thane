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

package flash.net {

import flash.errors.IOError;
import flash.events.Event;
import flash.events.EventDispatcher;
import flash.events.HTTPStatusEvent;
import flash.events.IOErrorEvent;
import flash.events.ProgressEvent;

import flash.utils.ByteArray;

import com.adobe.net.URI;

import org.httpclient.HttpClient;
import org.httpclient.http.Get;
import org.httpclient.http.Get;
import org.httpclient.events.HttpDataEvent;
import org.httpclient.events.HttpErrorEvent;
import org.httpclient.events.HttpStatusEvent;

import Thanette;

/**
 * The URLLoader class downloads data from a URL as text, binary data, or URL-encoded variables.
 * It is useful for downloading text files, XML, or other information to be used in a dynamic,
 * data-driven application.
 *
 * A URLLoader object downloads all of the data from a URL before making it available to code
 * in the applications. It sends out notifications about the progress of the download, which you
 * can monitor through the bytesLoaded and bytesTotal properties, as well as through dispatched
 * events.
 */
public class URLLoader extends EventDispatcher
{
    /**
     * Creates a URLLoader object.
     */
    public function URLLoader (request: URLRequest = null)
    {
        if (request != null) {
            load(request);
        }
    }
    
    /**
     * Indicates the number of bytes that have been loaded thus far during the load operation.
     */
    public function get bytesLoaded () :uint
    {
        return _bytes.length;
    }

    /**
     * Indicates the total number of bytes in the downloaded data. This property contains
     * 0 while the load operation is in progress and is populated when the operation is complete.
     * Also, a missing Content-Length header will result in bytesTotal being indeterminate. 
     */
    public function get bytesTotal () :uint
    {
        return _bytesTotal;
    }

    /**
     * The data received from the load operation. This property is populated only when the load
     * operation is complete. Because we only handle the URLLoaderDataFormat.BINARY data format,
     * the return value is always a ByteArray.
     *
     * @see #binaryData
     */
    public function get data () :*
    {
        return _data;
    }

    /**
     * Always returns URLLoaderDataFormat.BINARY.
     */
    public function get dataFormat () :String
    {
        return URLLoaderDataFormat.BINARY;
    }

    /**
     * Setting the data format is not supported in this implementation.
     */
    public function set dataFormat (format :String) :void
    {
        if (format != URLLoaderDataFormat.BINARY) {
            throw new Error("Only the binary data format is implemented");
        }
    }

    /**
     * Closes the load operation in progress. Any load operation in progress is immediately
     * terminated. If no URL is currently being streamed, an invalid stream error is thrown. 
     */
    public function close () :void
    {
        if (_client == null) {
            throw new IOError("No open HTTP request to close");
        }
        reset();
    }

    /**
     * Loads data from the specified URL.
    */
    public function load (request :URLRequest) :void
    {
        if (_client != null) {
            throw new IOError("Already serving a HTTP request");
        }
        reset();

        _client = Thanette.getCachingHttpClient();
        _client.addEventListener(Event.COMPLETE, handleComplete);
        _client.addEventListener(HttpDataEvent.DATA, handleData);
        _client.addEventListener(HttpErrorEvent.ERROR, handleError);
        _client.addEventListener(HttpStatusEvent.STATUS, handleStatus);

        _client.request(request.url);
    }

    /** Deal with a HTTP Error. */
    protected function handleError (evt :HttpErrorEvent) :void
    {
        dispatchEvent(new IOErrorEvent(IOErrorEvent.IO_ERROR));

        reset();
    }

    /** React to the client reading the full HTTP header. */
    protected function handleStatus (evt :HttpStatusEvent) :void
    {
        var length :String = evt.response.header.getValue("Content-Length");
        _bytesTotal = (length != null) ? Number(length) : 0;
        dispatchEvent(new HTTPStatusEvent(
            HTTPStatusEvent.HTTP_STATUS, false, false, int(evt.response.code)));
    }

    /** Receive some data from teh intarnets. */
    protected function handleData (evt :HttpDataEvent) :void
    {
        _bytes.writeBytes(evt.bytes);
        dispatchEvent(new ProgressEvent(
            ProgressEvent.PROGRESS, false, false, _bytes.length, _bytesTotal));
    }

    /** Finished receiving data. */
    protected function handleComplete (evt :Event) :void
    {
        _data = _bytes;
        _data.position = 0;
        _bytes = null;

        dispatchEvent(new Event(Event.COMPLETE));
    }

    protected function reset () :void
    {
        if (_client != null) {
            _client.removeEventListener(Event.COMPLETE, handleComplete);
            _client.removeEventListener(HttpDataEvent.DATA, handleData);
            _client.removeEventListener(HttpErrorEvent.ERROR, handleError);
            _client.removeEventListener(HttpStatusEvent.STATUS, handleStatus);

            _client = null;
        }

        _data = null;
        _bytes = new ByteArray();
        _bytesTotal = 0;
    }

    protected var _request :URLRequest;
    protected var _data :ByteArray;

    protected var _client :CachingHttpClient;

    protected var _bytes :ByteArray;
    protected var _bytesTotal :uint;
}
}
