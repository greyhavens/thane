import avmplus.*;

import flash.events.Event;
import flash.events.EventDispatcher;

import flash.utils.ByteArray;

import com.adobe.net.URI;
import org.httpclient.*;
import org.httpclient.events.*;
import org.httpclient.http.*;

var client :HttpClient = new HttpClient();

client.addEventListener(Event.CLOSE, event);
client.addEventListener(Event.COMPLETE, handleComplete);
client.addEventListener(Event.CONNECT, event);
client.addEventListener(HttpDataEvent.DATA, handleData);
client.addEventListener(HttpErrorEvent.ERROR, event);
client.addEventListener(HttpStatusEvent.STATUS, event);

trace("Temporary hack: " + Date.UTC(2008, 5, 29, 15, 16, 22, 43));

var bytes :ByteArray = new ByteArray();

function event (evt :Event) :void
{
    trace("Got an event from the HTTP client: " + evt.type);
}

function handleData (evt :HttpDataEvent) :void
{
    trace("Read " + evt.bytes.length + " bytes...");
    bytes.writeBytes(evt.bytes);
}

function handleComplete (evt :Event) :void
{
    trace("Compiling bytecode into new Domain...");

    var bridge = new EventDispatcher();

    var domain :Domain = Domain.spawnDomain("UserCode", bridge);
    domain.loadBytes(bytes);

    trace("Successfully loaded! Testing...");
    var gameServerClass :Class =  domain.getClass("zell.coolgame.MyCoolGameServer");
    trace("Server class in new domain: " + gameServerClass);

    var loaderInfo :Class = domain.getClass("flash.display.LoaderInfo");
    trace("LoaderInfo class in new domain: " + loaderInfo);

    bridge.addEventListener("controlConnect", handleUserCodeConnect);
    try {
        new gameServerClass();
        trace("New server instantiated!");
    } catch (err :Error) {
        trace("Error instantiating server class: " + err);
    }
    loaderInfo.__currentBridge = null;

}

function handleUserCodeConnect (evt :Event) :void
{
    var props :Object =  Object(evt).props;
    trace("Got connect props: " + props);
    props.hostProps = { "foo" : 1 };
}


client.request(new URI(System.argv[0]), new Get());
