
var a = new Object();
a.valueOf = function() { return 33 }
print(a + 22); // 55
print(a - 22); // 11

function foo() { };
foo.prototype.valueOf = function() { return new Object(); }
foo.prototype.toString = function() { return 2; }
print( new foo() + 33 ); // should be 35

print(typeof(new Date() + 22)); // a string
print(typeof(new Date() - 22)); // a number



