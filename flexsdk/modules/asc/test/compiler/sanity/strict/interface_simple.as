interface I
{
	function f();
}

class A implements I
{
	public function f()
	{
		print("Hello from f");
	}
}

var a:A = new A();
a.f();
a.I::f();

var i:I = a;
i.f();
i.I::f();
