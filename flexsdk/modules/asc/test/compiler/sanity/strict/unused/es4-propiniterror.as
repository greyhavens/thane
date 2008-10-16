package
{
    public class B
    {
        var x : String = "hi";
    }
    public class C extends B
    {
        var y : String = x;  //should error
        var z : String = "z";
        var a : String = z; //should error
        
        public function C(): y = x, a = z //should error
        {
        }            
    }
}