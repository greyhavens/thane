var f:Function = function f1(p1,p2) {
	var g = function f2(p3) {
		print(p1,p2)
		print(p3,p2)
		p1 = p3
		p2 = 'jeff!'
		return true
	}
	g('goodbye')
	print(p1,p2)
	return true
}
f('hello','world!')
function h(hx) {
	function i(ix) {
		function j(jx) {
			return k()
		}
		function k() {
			print(hx)		   // should print: 1
			return hx;
		}
		return j(ix+1)
	}
	return i(hx+1)
}
print(h(1) == 1)
