
var iter:int = 10//000000
var count:int

interface I
{
	function f(i:int,j:int);
	function g1();
	function g2();
	function g3();
	function g4();
	function g5();
}

class C implements I
{
	public function f(i:int,j:int)
	{
		count++
	}

	public function g1() { print("g1") }
	public function g2() { print("g2") }
	public function g3() { print("g3") }
	public function g4() { print("g4") }
	public function g5() { print("g5") }
}

function test()
{
	var c:C = new C
	var t:int = getTimer()
	for (var i:int=iter; i > 0; i--)
		c.f(i,t)
	trace("class call ", count);

	var ifc:I = c

	// test calling every method
	ifc.g1()
	ifc.g2()
	ifc.g3()
	ifc.g4()
	ifc.g5()

	count = 0
	t = getTimer()
	for (var i:int=iter; i > 0; i--)
		ifc.f(i,t)
	trace("interface call ", count);
}

test()
