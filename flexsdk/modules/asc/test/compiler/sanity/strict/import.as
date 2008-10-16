package p.q.r {
public var x = 10
public function f() { print("p.q.r.f()") }
var _y = 10
public function get y() { return _y }
public function set y(v) { _y = v }
}
package s {
import p.q.r.*
print(x)
x = 20
print(x)
f()
print(y)
y = 20
print(y)
}