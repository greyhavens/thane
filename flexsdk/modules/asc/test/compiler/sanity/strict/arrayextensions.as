// Various tests for new Array ECMA extension
var a = new Object();
var b = 10;
var c = "foo";
var emptyArray = [];

var d = [a, b, c, a, b, c];

// indexOf tests
print ("indexOf");
print (d.indexOf (a));
print (d.indexOf (b));
print (d.indexOf (c));
print (d.indexOf (a, 1));
print (d.indexOf (b, 1));
print (d.indexOf (c, 1));
print (d.indexOf (a, -1));
print (d.indexOf (b, -1));
print (d.indexOf (c, -1));

// indexOf tests
print ("lastIndexOf");
print (d.lastIndexOf (a));
print (d.lastIndexOf (b));
print (d.lastIndexOf (c));
print (d.lastIndexOf (a, 1));
print (d.lastIndexOf (b, 1));
print (d.lastIndexOf (c, 1));
print (d.lastIndexOf (a, -1));
print (d.lastIndexOf (b, -1));
print (d.lastIndexOf (c, -1));

function foo (element, index, array)
{
	return (element == 10);
}

function bar (element, index, array)
{
	return (element != 150);
}

///////////////////////////////////////////
// every
print ("every");
print (d.every (foo));
print (d.every (bar));
print (emptyArray.every(bar));

///////////////////////////////////////////
// filter
print ("filter");
var numbers = [1, 4, 9];
function lessthanfive (element, index, array)
{
	return element < 5;
}

var roots = numbers.filter(lessthanfive);
print (numbers);
print (roots);
print (emptyArray.filter(lessthanfive));

///////////////////////////////////////////
// forEach
print ("forEach");
var mysum = new Object();
mysum.total = 0;
function sum (element, index, array)
{
	this.total += element;
}
var numbers = [1, 4, 9];
numbers.forEach (sum, mysum);
print (mysum.total);
emptyArray.forEach(sum);

///////////////////////////////////////////
// map
print ("map");
function mysqrt (element, index, array)
{
	return Math.sqrt(element);
}

var roots = numbers.map(mysqrt);
print (numbers);
print (roots);
print (emptyArray.map(mysqrt));

///////////////////////////////////////////
// some
print ("some");
function some1 (element, index, array)
{
	return (element == 150);
}
print (d.some (foo));
print (d.some (bar));
print (d.some (some1));
print (emptyArray.some(some1));


// these functions should all work in generic objects 


dynamic class MyClass
{

	// class vars are not yet supported for generic Array routines
	//var length;
	function MyClass()
	{
		//length = 3;
	}
}

MyClass.prototype.indexOf = Array.prototype.indexOf;
MyClass.prototype.lastIndexOf = Array.prototype.lastIndexOf;
MyClass.prototype.every = Array.prototype.every;
MyClass.prototype.forEach = Array.prototype.forEach;
MyClass.prototype.filter = Array.prototype.filter;
MyClass.prototype.map = Array.prototype.map;
MyClass.prototype.some = Array.prototype.some;

var z = new MyClass();
z.length = 3;
z[0] = a;
z[1] = b;
z[2] = c;
print ("MyClass");
print (z[0]);
print (z[1]);
print (z[2]);
print (z.length);
print (z.indexOf (a));
print (z.lastIndexOf (a));

print ("object every");
print (z.every (foo));
print (z.every (bar));

z[0] = 1;
z[1] = 4;
z[2] = 9;
print ("object filter");
var roots = z.filter(lessthanfive);
print (z);
print (roots);

print ("object forEach");
var mysum = new Object();
mysum.total = 0;
z.forEach (sum, mysum);
print (mysum.total);

print ("object map");
var roots = z.map(mysqrt);
print (z);
print (roots);

z[0] = a;
z[1] = b;
z[2] = c;
print ("object some");
print (z.some (foo));
print (z.some (bar));
print (z.some (some1));
