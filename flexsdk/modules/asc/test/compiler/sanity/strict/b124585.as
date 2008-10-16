interface I
{
	function interfaceMethod();
}

class A implements I
{
	public function classMethod() {
		print("A.classMethod");
	}
	public function interfaceMethod() {
		print("A.interfaceMethod");
	}
}

class Test
{
	function Test()
	{
		var a:A = new A();
		var i:I = a;
		
		a.classMethod();
		a.interfaceMethod();
		i.interfaceMethod();
	}
}

new Test();
