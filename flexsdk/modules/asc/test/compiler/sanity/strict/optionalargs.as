
class A {
	static function f1(x=1) { print(x) }
	static function f2(x=-1) { print(x) }
	static function f3(x=-1.2) { print(x) }
	static function f4(x="s") { print(x) }
	static function f5(x=null) { print(x) }
	static function f6(x=void 0) { print(x) } // undefined
	static function f7(x=false) { print(x) }
	static function f8(x=true) { print(x) }
}

A.f1()
A.f2()
A.f3()
A.f4()
A.f5()
A.f6()
A.f7()
A.f8()

function f1(x=1) { print(x) }
function f2(x=-1) { print(x) }
function f3(x=-1.2) { print(x) }
function f4(x="s") { print(x) }
function f5(x=null) { print(x) }
function f6(x=void 0) { print(x) } // undefined
function f7(x=false) { print(x) }
function f8(x=true) { print(x) }

f1()
f2()
f3()
f4()
f5()
f6()
f7()
f8()

function g1(a:uint=0xFFFFFFFF,b:int=0x7FFFFFFF,c:Boolean=true,d:String="",e:Number=1E300) { print(a,b,c,d,e) }
function g2(a:uint=void 0,b:int=void 0,c:Boolean=void 0,d:String=void 0,e:Number=void 0) { print(a,b,c,d,e) }

g1()
g2()

function h1(a:Number=NaN,b:Number=Infinity,c:*=undefined) { print(a,b,c) }
function h2(a:Number=Number.MAX_VALUE,b:Number=Number.MIN_VALUE,c:Number=Number.NaN,
            d:Number=Number.NEGATIVE_INFINITY,e:Number=Number.POSITIVE_INFINITY) { print(a,b,c,d,e) }

h1()
h2()
