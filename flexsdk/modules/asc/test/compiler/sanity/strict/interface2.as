/*    package
    {
        public interface ChildrenCollection
        {
            function f():void;
        }
    }
    
    package flash.display
    {
        public class Sprite 
        {
        }
    }
 
    package mx.core
    {
        import flash.display.*;
 
        public class UIObject extends Sprite implements ChildrenCollection
        {
             public function f():void;
             public UIObject() {}
        }
    }
*/ 
    package flash.display
    {
        public class Sprite 
        {
        }
    }
 
    package mx.core
    {
        public interface ChildrenCollection
        {
            function f():void;
        }
    }
 
    package mx.core
    {
        import flash.display.*;
 
        public class UIObject extends Sprite implements ChildrenCollection
        {
            public function f():void { }
        }
    }
 
