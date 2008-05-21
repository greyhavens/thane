package {
    public class Tim { }
}

import avmplus.*;

import flash.events.Event;

import com.adobe.net.URI;
import org.httpclient.*;
import org.httpclient.events.*;
import org.httpclient.http.*;

var client :HttpClient = new HttpClient();

client.addEventListener(Event.CLOSE, event);
client.addEventListener(Event.COMPLETE, event);
client.addEventListener(Event.CONNECT, event);
client.addEventListener(HttpDataEvent.DATA, dataEvent);
client.addEventListener(HttpErrorEvent.ERROR, event);
client.addEventListener(HttpStatusEvent.STATUS, event);

function event (evt :Event) :void
{
    trace("Got an event: " + evt);
}

function dataEvent (evt :HttpDataEvent) :void
{
    trace("Got data: creating new Domain...");
    var newDomain :Domain = new Domain(Domain.currentDomain);
    trace("Compiling bytecode into new Domain...");
    newDomain.loadBytes(evt.bytes);
    trace("Successfully loaded! Testing...");
    var tinyClass :Class;
    try {
        tinyClass = Domain.currentDomain.getClass("Tiny");
    } catch (e :Error) { }
    trace("Class 'Tiny' in original domain (should be null): " + tinyClass);

        tinyClass = newDomain.getClass("Tiny");

    trace("Class 'Tiny' in new domain (should be non-null): " + tinyClass);
    if (tinyClass != null) {
        trace("Instantiating and calling Tiny...");
        var tiny :Object = new tinyClass();
        tiny.probe();
    }
}

client.request(new URI(System.argv[0]), new Get());
