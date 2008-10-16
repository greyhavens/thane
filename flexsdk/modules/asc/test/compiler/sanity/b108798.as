
class A
{
function A() { trace("ctor") }
}

trace("testing A.constructor()")
A.constructor(A); // nothing happens, same as A
print(A.constructor == A) // false
print(A.constructor == Class.prototype.constructor) // true

trace("testing new A()")
var a:A = new A();

trace("testing a.constructor")
trace(a.constructor); // Object
trace(a.constructor == A.prototype.constructor) // true
trace(a.constructor == A) // true


print(true.constructor)
print(true.constructor == Boolean)
