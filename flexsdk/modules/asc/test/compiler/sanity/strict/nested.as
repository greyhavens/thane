function f1(p1,p2) {
    //print(p1,p2);
	function f2(p3) {
		print(p1,p2)
		print(p3,p2)
		p1 = p3
		p2 = 'jeff!'
		return true
	}
	f2('goodbye')
	print(p1,p2)
	return true
}
f1('hello','world!')
