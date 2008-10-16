class A {
	static var a:A = new A();
	print("A.prototype.f being set");
	A.prototype.f = function()  { return "hello, world!" }
	print("set it.");
	print("value of A.prototype.f:");
	print(A.prototype.f);
	print("running a.f:");
	print(a.f());
	public function A()
	{
		print("A constructor");
	}
}
