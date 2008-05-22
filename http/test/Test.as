package {
    public class Tim {
        public static function probe () :void
        {
            trace("TIM.AS:: This is probe() executing statically in Tim.");
        }
    }
}

import avmplus.*;

import flash.events.Event;
import flash.utils.ByteArray;

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
    trace("Got data: running test with a child domain...");
    runTest(evt.bytes, new Domain(Domain.currentDomain));
    trace("Next, running test with a free-standing domain...");
    runTest(evt.bytes, new Domain());
}

function runTest (bytes :ByteArray, domain :Domain)
{
    trace("Compiling bytecode into new Domain...");
    domain.loadBytes(bytes);
    trace("Successfully loaded! Testing...");
    var tinyClass :Class;
    try {
        tinyClass = Domain.currentDomain.getClass("Tiny");
    } catch (e :Error) { }
    trace("Class 'Tiny' in original domain (should be null): " + tinyClass);

    tinyClass = domain.getClass("Tiny");
    trace("Class 'Tiny' in new domain (should be non-null): " + tinyClass);
    if (tinyClass != null) {
        trace("Setting Tiny's lifeline...");
        tinyClass.lifeline = lifelineEndpoint;

        trace("Instantiating and calling Tiny...");
        var tiny :Object = new tinyClass();
        tiny.probe();
    }
}

client.request(new URI(System.argv[0]), new Get());

function lifelineEndpoint () :void
{
    trace("TEST.AS:: Executing lifeline endpoint!");
}