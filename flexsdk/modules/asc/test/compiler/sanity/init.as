// without defaults

class i1 {
var a // anonymous
var o:*
var n:Number
var i:int
var u:uint
var ns:Namespace
var udt:i1
}

i = new i1
print(i.a)
print(i.o)
print(i.n)
print(i.i)
print(i.u)
print(i.ns)
print(i.udt)

// with defaults

namespace NS = "myNS"

class i2 {
var a = 1 // anonymous
var o:* = Math
var n:Number = 1.423
var i:int = -123
var u:uint = 123
var ns:Namespace = NS
var udt:i2 = null
var x:int;
var y:int;
var z:int = x = y = 1;
// added this to catch problem exposed by flex
var f = Object(new Object())
}

i = new i2
print(i.a)
print(i.o)
print(i.n)
print(i.i)
print(i.u)
print(i.ns)
print(i.udt)

// check activation init

function f()
{
var a // anonymous
var o:*
var n:Number
var i:int
var u:uint
var udt:i1

var a2 = 3 // anonymous
var o2:* = 1
var n2:Number = 1.3243
var i2:int = -3421
var u2:uint = 1045
var udt2:i1 = null
	function f1() 
	{
print(a)
print(o)
print(n)
print(i)
print(u)
print(udt)
print(a2)
print(o2)
print(n2)
print(i2)
print(u2)
print(udt2)
	}
f1()
}

f()

// extend a base class with slots from another abc
class esub extends Error
{
	var f:int = 1
}
new esub()
