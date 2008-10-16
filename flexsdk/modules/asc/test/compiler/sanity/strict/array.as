var a=new Array('a','b','c')
print(a[0])
print(a[1])
print(a[2])
a = [,,'foo',,]
var b = [,,'foo',,undefined]
print(a.length)
for(var i = a.length; i >= 0; --i )
{
    print(a[i])
}
print(b.length)
for( i = b.length; i >= 0; --i )
{
    print(b[i])
}

// some simple splice tests
var c = [1, 2, 3];
var d = c.splice();
print (c);
print (d == undefined);

c = [1, 2, 3];
d = c.splice(2);
print (c);
print (d);

c = [1, 2, 3];
d = c.splice(1,1);
print (c);
print (d);

c = [1, 2, 3];
d = c.splice("2");
print (c);
print (d);

c = [1, 2, 3];
d = c.splice("1","1");
print (c);
print (d);

// convert to int failures should get treated as 0
c = [1, 2, 3];
d = c.splice("foo");
print (c);
print (d);

c = [1, 2, 3];
d = c.splice("foo", "bar");
print (c);
print (d);

// sort tests
c = [3, 2, 1]
c.sort();
print (c)

c = [1, 2, 3]
c.sort(2); // descending sort
print (c)

function foo(a, b)
{
	return a > b;
}

c = [3, 2, 1]
c.sort(foo);
print (c)

c = [3, 2, 1]
c.sort(foo, 2);
print (c)

// test sparse splicing that deletes
var a = [];
a.push("bobby");
a.push("sam");
a.push("willy");
a.length = 7;
a.splice(0, 1);
print (a);
print (a.length);

var a = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'];
a.splice (0, 0, 1, 2, 3, 4); // a splice that inserts
print (a);
print (a.length);

a = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'];
a.splice(0, 4, 1, 2); // a splice that deletes
print (a);
print (a.length);

// Some attempts to verify sparse to dense conversion.
// They are not really testing the ordered iteration
// of dense array because sparse array will do ordered
// iteration most of the time because of our new integer HT keys.

// Should convert our mixed-mode array (0/2/3/4) to a dense array when 1 prop is hit
var a:Array = new Array(10);
a[4] = '4';
a[3] = '3';
a[2] = '2';
a[0] = '0';
a[1] = '1';
for each (var x in a)
	trace (x);

// Should convert our sparse array (1/2/3/4) to a dense array when shift is called
a = new Array();
a[4] = '4';
a[3] = '3';
a[2] = '2';
a[1] = '1';
a.shift();
for each (x in a)
	trace (x);

// mixed-mode set length call
// deletes props from dense part as well as HT
a = new Array();
a[0] = '0';
a[1] = '1';
a[2] = '2';
a[5] = '5';
a[8] = '8';
a.length = 2;
for each (x in a)
	trace (x);

// multiple sparse-to-dense conversion
a = new Array();
a[0] = '0';
a[2] = '2';
a[4] = '4';
a[6] = '6';
a[1] = '1'; // causes one conversion
a[3] = '3'; // causes another
a[5] = '5'; // and another
trace (a);
