class A { 
public var x
public function f() { print(x) }
var y
function g() { print(y) }
protected var z
protected function h() { print(z) }
}

var a = new A()
var xn = 'x'
var fn = 'f'
a.public::[xn] = 10
print(a.public::[xn])
a.public::[fn]()

a.public::[([[xn]])] = 20
print(a.public::[([[xn]])])

var yn = 'y'
var gn = 'g'
a.internal::[yn] = 20
print(a.internal::[yn])
a.internal::[gn]()
delete a.internal::[yn]
//a.internal::[yn]++
//print(a.internal::[yn])

class B extends A {
private var v
private function e() { print(v) }

function test()
{
var zn = 'z'
var hn = 'h'
protected::[zn] = 30
print(protected::[zn])
protected::[hn]()

var vn = 'v'
var en = 'e'
private::[vn] = 40
print(private::[vn])
private::[en]()
}
}

new B().test()
