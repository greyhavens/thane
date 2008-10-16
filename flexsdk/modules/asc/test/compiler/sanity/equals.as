print(true==true)
// Special case for xml in Avmcore::eq
var x:XML = new XML("null");
trace (x == null);
trace (null == x);
// Special case for xml in Avmcore::eq
x = new XML("undefined");
trace (x == undefined);
trace (undefined == x);
x = null;
trace (x == null);
trace (null == x);
trace (x == undefined);
trace (undefined == x);
x = undefined;
trace (x == null);
trace (null == x);
trace (x == undefined);
trace (undefined == x);

// Same behavior should happen if var holding XML is not strongly typed
// Special case for xml in Avmcore::eq
var y:Object = 10;
y = new XML("null");
trace (y == null);
trace (null == y);
// Special case for xml in Avmcore::eq
y = new XML("undefined");
trace (y == undefined);
trace (undefined == y);
y = null;
trace (y == null);
trace (null == y);
trace (y == undefined);
trace (undefined == y);
y = undefined;
trace (y == null);
trace (null == y);
trace (y == undefined);
trace (undefined == y);

// These tests will go through our new optimized op_equals/op_convert_b MIR case
var a:Array = [1, 2, 3]
if (a)
	trace ("array is non null");
if (a == null)
	trace ("array is null");
if (a == undefined)
	trace ("array is undefined");
a = null;
if (a)
	trace ("array is non null");
if (a == null)
	trace ("array is null");
if (a == undefined)
	trace ("array is undefined");
a = undefined;
if (a)
	trace ("array is non null");
if (a == null)
	trace ("array is null");
if (a == undefined)
	trace ("array is undefined");

// these test == with different types
var o = { toString: function() { return "a" } }
var a = "a"
print(o == a)
print(a == o)
print(true == 1)
