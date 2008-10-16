class A2 { }
class CC { }
A2 = null;	// const error
A2 = CC;	// ditto
const b:Class = A; // no error
b = CC;		// error
var c = new b();

Number = String;			// error, need to test imported defs too
Number.NEGATIVE_INFINITY=0; // ditto

class A {}
class B {}
const C : Class 

var x:Boolean
if( x ) 
{
    C = A
}
else
{
    C = B  // no error, this is the second assignment but only one branch of the if statement could succeed.
}

