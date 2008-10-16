// showed bug with sparse array and treating empty element special in ArraySort
var a = new Array();
a[0] = 'c';
a[1] = 'b'
a[2] = 'a';
a[4] = 'd';
a.sort();
trace (a);

dynamic class MyClass
{
	public var length:int;
	function MyClass()
	{
		length = 5;
		this[0] = 'a';
		this[1] = 'b';
		this[2] = 'c';
		this[3] = 'd';
		this[4] = 'e';
	}

	function toString():String
	{
		var s:String = "";
		for (var i:int = 0; i < length; i++)
		{
			s += this[i]
			if (i < length - 1)
				s += ",";
		}
		s += " (MyClass: " + length + ")";;
		return s;
	}
}

MyClass.prototype.concat = Array.prototype.concat
MyClass.prototype.every = Array.prototype.every
MyClass.prototype.filter = Array.prototype.filter
MyClass.prototype.forEach = Array.prototype.forEach
MyClass.prototype.indexOf = Array.prototype.indexOf
MyClass.prototype.join = Array.prototype.join
MyClass.prototype.lastIndexOf = Array.prototype.lastIndexOf
MyClass.prototype.map = Array.prototype.map
MyClass.prototype.slice = Array.prototype.slice
MyClass.prototype.some = Array.prototype.some

// these set length value
MyClass.prototype.splice = Array.prototype.splice;
MyClass.prototype.pop = Array.prototype.pop
MyClass.prototype.push = Array.prototype.push
MyClass.prototype.reverse = Array.prototype.reverse
MyClass.prototype.shift = Array.prototype.shift
MyClass.prototype.sort = Array.prototype.sort
MyClass.prototype.sortOn = Array.prototype.sortOn
MyClass.prototype.slice = Array.prototype.slice
MyClass.prototype.unshift = Array.prototype.unshift

function dumpArray(a:Array)
{
	var s:String = a.toString();
	s += " (Array: " + a.length + ")";
	return s;
}

var a = new MyClass();
var b = ['a', 'b', 'c', 'd', 'e'];
trace (a.toString());
trace (dumpArray(b));

a.reverse();
b.reverse();
trace ("reverse: " + a.toString());
trace ("reverse: " + dumpArray(b));

a.pop();
b.pop();
trace ("pop: " + a.toString());
trace ("pop: " + dumpArray(b));

a.push(10);
b.push(10);
trace ("push: " + a.toString());
trace ("push: " + dumpArray(b));

a.sort();
b.sort();
trace ("sort: " + a.toString());
trace ("sort: " + dumpArray(b));

a.unshift('f');
b.unshift('f');
trace ("unshift: " + a.toString());
trace ("unshift: " + dumpArray(b));

var c = a.shift();
var d = b.shift();
trace ("shift: " + a.toString() + ", result: " + c);
trace ("shift: " + dumpArray(b) + ", result: " + d);

// Some tests for field sorting with options
var data:Array = [{value: 3}, {value: 2}, {value: 1}];
for(var k = 0; k<3; k++){
trace(data[k].value)
}

var ret = data.sortOn("value", Array.UNIQUESORT);
trace("ret1 : " + ret);
for(var k = 0; k<3; k++){
trace(data[k].value)
}

var ret = data.sortOn("value", Array.UNIQUESORT | Array.DESCENDING);
trace("ret2 : " + ret);
for(var k = 0; k<3; k++){
trace(data[k].value)
}

var ret = data.sortOn("value", Array.UNIQUESORT | Array.RETURNINDEXEDARRAY);
trace("ret3 : " + ret);
for(var k = 0; k<3; k++){
trace(data[ret[k]].value)
}
data.push ({value:3});
var ret = data.sortOn("value", Array.UNIQUESORT);
trace("ret4 : " + ret);
for(var k = 0; k<3; k++){
trace(data[k].value)
}
