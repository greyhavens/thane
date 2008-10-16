package a.b
{
    public class Foo
    {
        public final function lable(i : String) : String
        {
            return "hi";
        }
        
        public var x : Foo;
        
        public function get readOnly () : int
        {
            return 5;
        }
        
        protected function set writeOnly (a : Object) :void
        {
        }
        
        private function get readWrite( ) : Date
        {
            return new Date();
        }
        
        function set readWrite( a : Date )
        {
        }
    }
    
    public class Bar extends Foo
    {
        protected override function set writeOnly (a : Object) : void
        {
        }
    }
}