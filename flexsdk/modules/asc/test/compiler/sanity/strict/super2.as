class A {
	function get x() { print('get a') }
	function set x(v) { print('set a') }
	function f() { print('call a') }
}
class B extends A {
	override function get x() { print('get b') }
	override function set x(v) { print('set b') }
	override function f() { print('call b') }
	
	function test() {
		var t

	    // get
		t = this.x
		t = this['x']
		t = super.x
		t = super['x']

		// set
		this.x = t
		this['x'] = t
		super.x = t
		super['x'] = t

		// call
		this.f()
		this['f']()
		super.f()
		super['f']()
	}
}
var b:B = new B
b.test()
