interface A
{
	function X()
}

interface B
{
	function X()
}

class C implements A, B
{
	public function X() { print("C::X") }
}

var c:C = new C();

c.A::X();
c.B::X();
c.X();

