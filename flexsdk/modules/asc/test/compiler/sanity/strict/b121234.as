class A { 
function name() { print("A"); return "A"; } 
function test() { 
var x1 = <alpha><name>Foo</name><length>Bar</length></alpha> 
trace(x1.(name=="Foo")); 
} 
} 

var a:A = new A 
a.test()