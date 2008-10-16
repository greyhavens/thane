package
{
    public class B
    {
        public function B()
        {
            trace("B ctor");
        }
    }
    public class C extends B
    {
        var x;
        var y : String;
        var z;
        
        var i = "hi";  
           
        public function C(a) : 
            x = "blah", 
            y="somthing", 
            z=a,    
            super()
        {
            trace("C ctor");
            trace(x);
            trace(y);
            trace(z);
            x = a;
            trace(a);
            trace(x);
        }
    }
    
    new C("arg-value");
}