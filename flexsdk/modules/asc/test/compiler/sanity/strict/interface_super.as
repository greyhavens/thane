interface Super
{
	function f();
}

interface Sub extends Super
{
	function g();
}

class MyClass implements Super, Sub
{
	public function f()
	{
		print("Class::f");
	}
	public function g()
	{
		print("Class::g");
	}
}

var instance:MyClass = new MyClass();
instance.f();
instance.g();
instance.Super::f();
instance.Sub::g();

var superReference:Super = instance;
superReference.f();
superReference.Super::f();

var subReference:Sub = instance;
subReference.f();
subReference.Super::f();
subReference.g();
subReference.Sub::g();
