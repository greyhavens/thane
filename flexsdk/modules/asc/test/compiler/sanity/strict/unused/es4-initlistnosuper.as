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
        public function C() : x = 10
        {
            trace("C ctor");
        }
    }    
    
    new C();
}