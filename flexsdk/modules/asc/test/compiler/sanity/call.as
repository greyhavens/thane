class A{ var x; function A(x) { this.x=x } }
function f(a:A) { print(a.x) }
f.public::call(null, new A(1))
f.AS3::call(null, new A(2))
f.call(null, new A(3))
