class A
{
	function A() { x = 10 }
	function get x() { return A.x }	
	function set x(v) { A.x=v }	
	static var x
}
print(new A().x)
