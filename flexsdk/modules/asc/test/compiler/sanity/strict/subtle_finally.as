function g() { print("return"); }
function f()
{
	try {
		return g();
	} finally {
		print("finally");
	}
}
f();

		
  
