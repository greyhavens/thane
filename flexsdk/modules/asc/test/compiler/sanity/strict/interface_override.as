class B
{
	public function f()
	{
		print("B::f");
	}
}

interface I
{
	function f()
}

class E extends B implements I
{
	override public function f()
	{
		print("E::f")
	}
}

var e:E = new E()

var i:I = e

var b:B = e

e.f();
i.f();
b.f();
