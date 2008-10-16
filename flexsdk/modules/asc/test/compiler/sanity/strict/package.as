package p {
public var x = 10
public class C {
public static function g() { print('g',x) }
public function m() { print('m',x) }
}
}

package p {
public function f() { print('f',x) }
f()
C.g()
print('p',x)
}

package p {
}

package q.r.s {
public class A {
public function m() { 
print('q.r.s.A.m') 
}
}
}
import p.*
import q.r.s.*
p.f()
p.C.g()
var o : p.C = new p.C
o.m()
print('p',p.x)

var a = new q.r.s.A
a.m()
