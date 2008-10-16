class A
{
    static function foo(o:*):void
    {
        o.bar = function() { this.foozle = "something"; }
    }
}

var x = {}
A.foo(x)
x.bar()
print(x.foozle)
