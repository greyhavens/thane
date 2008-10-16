

function f(p) { 
	function g() { 
		print(p,l) 
		p='goodbye'
	} 
	var l = "world!";
	g()
	print(p) 
}
f('hello')