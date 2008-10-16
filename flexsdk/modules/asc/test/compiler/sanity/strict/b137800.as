package
{
    public class B
    {
    }
    public class A
    {
        public var blah = true;
        public static function test(q:*):void
        {
            A(q);
        }
    }
    A.test("hi");
}
