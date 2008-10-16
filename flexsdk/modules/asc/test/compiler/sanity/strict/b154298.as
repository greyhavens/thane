class A
{
	function get foo():int { return _foo; }
	function set foo(value:int) { _foo = value }
	private var _foo:int = 3

	var x:Number=0

	public function A()
	{
		x++;
		x--;
	}
}

var a:A = new A()
a.foo++
a.foo--
print(a.foo)
