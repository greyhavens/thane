
    package flash.display
    {
        public class DisplayObject 
        {
			public function f():void { print("DisplayObject::f") }
        }
    }
 
    package mx.core
    {
        public interface ChildrenCollection
        {
            function f():void
        }
    }
 
    package mx.core
    {
        import flash.display.*;
 
        public class UIObject extends DisplayObject implements ChildrenCollection
        {
			// DisplayObject::f satisfies ChildrenCollection?
        }

		var u:UIObject = new UIObject
		u.f()
		var c:ChildrenCollection = new UIObject
		c.f()
    }
 
