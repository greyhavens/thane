function A() {
	this.x = 10
}
var a = new A;
function B() {
}
B.prototype = a
var b = new B
print('a:',a)
print('b:',b)
print('a.x:',a.x)
print('b.x:',b.x)
b.x = 20
print('a.x:',a.x)
print('b.x:',b.x)
print(true)

// tests for fixed bugs

var myvar = MyObject
myvar.toLocaleString = Object.prototype.toLocaleString
print("164168 " + myvar.toLocaleString())
function MyObject( value ) {
    this.value = function() { return this.value; }
    this.toLocaleString = function() { return this.value+''; }

    this.value = function() {return this.value;}
    this.toLocaleString = function() {return this.value+'';}
}

var strobj = new Object("JSScript");
print("161857 " + strobj.hasOwnProperty("split"));

print("161834 " + Number.prototype.valueOf());


String.prototype.toString = Object.prototype.toString
var s = new String( "test" );
print("136662 " + s.toString() );

Number.prototype.toString = Object.prototype.toString
var n = 6
print("136662 " + n.toString())

var s2 = "test"
print("136847 " + s2.hasOwnProperty( "split" ) );

print("155025 " + Function.prototype())

print("162564 " + String.prototype.toString());

var obj = function() {}
try {
	obj.toString = Object.prototype.toString
	print("136677 pass")
} catch (e) {
	print("136677 fail")
}


