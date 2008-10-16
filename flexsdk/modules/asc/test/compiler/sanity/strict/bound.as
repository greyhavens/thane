var x=1
var g=this
g.x
print(g.x)
g.x = 2
print(x)
this.x = g.x + 1
print(x)
/*
function f() { return this }
x=f;
x();
new x;
x=new x();
x.a=1;
x.a;
x.a=f;
x.a();
new x.a;
new x.a();
*/

