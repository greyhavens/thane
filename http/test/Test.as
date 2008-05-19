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
    trace("Got data: loading...");
    var dom :Domain = Domain.currentDomain.loadBytes(evt.bytes);
    trace("Bytecode successfully loaded!");
    var tinyClass :Class = Domain.currentDomain.getClass("Tiny");
    trace("Class 'Tiny' located: " + tinyClass);
    if (tinyClass != null) {
        var tiny :Object = new tinyClass();
        tiny.probe();
    }
}

client.request(new URI(System.argv[0]), new Get());
