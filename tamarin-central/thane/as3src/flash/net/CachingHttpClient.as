package flash.net {

import flash.events.Event;
import flash.events.EventDispatcher;

import flash.utils.ByteArray;
import flash.utils.Dictionary;
import flash.utils.setTimeout;

import org.httpclient.events.HttpDataEvent;
import org.httpclient.events.HttpErrorEvent;

public class CachingHttpClient extends EventDispatcher
{
    public static function noLongerLoading (resource :String) :void
    {
        delete _loading[resource];
    }

    public static function cache (resource :String, data :ByteArray) :void
    {
        _cached[resource] = data;
    }

    public function request (resource :String) :void
    {
        // did we already load this?
        var data :ByteArray = _cached[resource];
        if (data != null) {
            setTimeout(function () :void {
                dispatchEvent(new HttpDataEvent(data));
                dispatchEvent(new Event(Event.COMPLETE));
            }, 0);
            return;
        }

        // else, is some instance already loading it?
        var loader :HttpLoader = _loading[resource];
        if (!loader) {
            // if not, start loading
            loader = new HttpLoader(resource);

            // and remember that we are
            _loading[resource] = loader;
        }

        // finally make sure we hear about the loader's events
        loader.addEventListener(Event.COMPLETE, dispatchEvent); // pass through
        loader.addEventListener(HttpDataEvent.DATA, dispatchEvent); // pass through
        loader.addEventListener(HttpErrorEvent.ERROR, dispatchEvent); // pass through
    }

    private static var _cached :Dictionary = new Dictionary();
    private static var _loading :Dictionary = new Dictionary();
}
}

import flash.net.CachingHttpClient;

import flash.utils.ByteArray;
import flash.events.Event;
import flash.events.EventDispatcher;

import org.httpclient.HttpClient;
import org.httpclient.events.HttpDataEvent;
import org.httpclient.events.HttpErrorEvent;
import org.httpclient.http.Get;

import com.adobe.net.URI;

class HttpLoader extends EventDispatcher
{
    public function HttpLoader (resource :String) :void
    {
        var client :HttpClient = new HttpClient();
        client.addEventListener(Event.COMPLETE, handleComplete);
        client.addEventListener(HttpDataEvent.DATA, handleData);
        client.addEventListener(HttpErrorEvent.ERROR, handleError);

        _bytes = new ByteArray();
        _resource = resource;

        client.request(new URI(resource), new Get());
    }

    protected function handleData (evt :HttpDataEvent) :void
    {
        _bytes.writeBytes(evt.bytes);
        // we deliberately don't dispatch progress events until we're done
    }

    protected function handleComplete (evt :Event) :void
    {
        CachingHttpClient.noLongerLoading(_resource);

        // deliberate execute these events in quick succession
        dispatchEvent(new HttpDataEvent(_bytes));
        dispatchEvent(new Event(Event.COMPLETE));

        CachingHttpClient.cache(_resource, _bytes);
        _bytes = null;
    }

    protected function handleError (evt :HttpErrorEvent) :void
    {
        CachingHttpClient.noLongerLoading(_resource);

        dispatchEvent(evt);
    }

    private var _resource :String;
    private var _bytes :ByteArray;
}


