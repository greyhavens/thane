/*

This test shows that Moz. doesn't update the closed activation object
before returning. Interestingly it does update the closure state if f 
is not called before returning.

*/

var x;
var a;

function g(y) {
	y();
}
function h() {
	x();
}
function makef(i) {				// << create activation for makef
	var f = function _f() { print(i); return i; };		
									// << set f.[[Scope]] to { global, activation(makef) }
	f();							// << update activation and invoke f
	++i;							// 
	g(f);							// << update activation and invoke f
	++i;
	x=f;
	h();
	++i;
	return f;						// << update activation for last time and return
}
a=makef(10)
print(a()==13)


