package warnings{
    import flash.util.trace;
    import flash.display.Sprite;
    //import flash.events.*;
    import flash.events.EventDispatcher;

    public class MyClass extends EventDispatcher{
    public var myListener1:* = new Object();
    public var myBroadCaster:*= new Object();
    public var someEventDispatcher:EventDispatcher=new EventDispatcher();
    new AsBroadcaster();
    public function MyClass(){
        super();    
    }

    public function MyListenerFunction():void{
    myListener1.someEvent = function():void {          // Creates listener method.
    trace("myListener1 received someEvent");
    }
    AsBroadcaster.initialize(myBroadCaster);
    myBroadCaster._listeners.length=2;
    myBroadCaster.addListener(myListener1);
    trace(myBroadCaster.broadcastMessage("someEvent"));
    myBroadCaster.removeListener(myListener1);
    }
    }
    var MCL:MyClass = new MyClass();
    MCL.MyListenerFunction();

}
