
// make sure we dont mess up uint values with our 29-bit atoms

var o:* = new Object();
var a:uint = 0x1fffffff;
o.a = a;
o.b = -1;
trace(o.a == o.b); // true, but should be false

