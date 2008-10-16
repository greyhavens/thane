// Three somewhat unrelated fixes in this sanity test

var junk:String = "10";
var xmlObj:XML = XML ('<elem attr1="firstAttribute"></elem>');
try
{
	trace (xmlObj.(@nonExistentAttribute == "nonExistent"));
}
catch(e)
{
	trace (e.toString().substring(0, 27));
}

try
{
	trace (xmlObj.(@junk == "10"));
}
catch(e2)
{
	trace (e2.toString().substring(0, 27));
}


// General objects don't support attributes
var o:Object = new Object();
try
{
	o.@foo = "10";
}
catch(e3)
{
	trace (e3.toString().substring(0, 27));
}
try
{
	var a = o.@foo;
}
catch(e4)
{
	trace (e4.toString().substring(0, 27));
}


Number.prototype.splice = Array.prototype.splice;
Number.prototype.concat = Array.prototype.concat;
Number.prototype.pop = Array.prototype.pop;
Number.prototype.reverse = Array.prototype.reverse;
Number.prototype.shift = Array.prototype.shift;
Number.prototype.unshift = Array.prototype.unshift;
Number.prototype.slice = Array.prototype.slice;
Number.prototype.join = Array.prototype.join;
Number.prototype.sort = Array.prototype.sort;
Number.prototype.sortOn = Array.prototype.sortOn;
var x:Number = 10;

trace("splice");
x.splice(1, 2);
trace("concat");
trace (x);
x.concat("a");
trace("pop");
x.pop();
trace("reverse");
x.reverse();
trace("shift");
x.shift();
try
{
	trace("unshift");
	x.unshift();
}
catch (e5)
{
	trace("unshift throws an error");
}
trace("slice");
x.slice();
try
{
	trace("join");
	x.join();
}
catch (e6)
{
	trace("join throws an error");
}
trace("sort");
x.sort();
trace("sortOn");
x.sortOn("foo");
