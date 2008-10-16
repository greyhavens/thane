
class A
{
	static var x = "pass"
	function f()
	{
		return x
	}
	function g()
	{
	    try {
		   return x
		} finally {
		}
	}
}
class B extends A
{
	var x = "fail"
}

var a:A = new B 
print(a.f())
print(a.g())
