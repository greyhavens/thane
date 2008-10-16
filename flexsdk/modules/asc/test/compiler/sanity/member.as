var x;
x;
x=1;
x=function(){}
x();
new x;
new x();
x.a;
x.a=1;
x.a=function(){}
x.a();
new x.a;
new x.a();
//y;
y=1;
y=function(){}
y();
new y;
new y();
y.a;
y.a=1;
y.a=function(){}
y.a();
new y.a;
new y.a();
function f() {
function g() {
var x;
x;
x=1;
x=function(){}
x();
new x;
new x();
x.a;
x.a=1;
x.a=function(){}
x.a();
new x.a;
new x.a();
y;
y=1;
y=function(){}
y();
new y;
new y();
y.a;
y.a=1;
y.a=function(){}
y.a();
new y.a;
new y.a();
}
g()
}
f()
print(true)
