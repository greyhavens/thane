/*
1036	The static keyword cannot be used outside of a class definition.
*/
package a {
	static var a;
	function f() {
		static var b;
	}
}
static var c;
