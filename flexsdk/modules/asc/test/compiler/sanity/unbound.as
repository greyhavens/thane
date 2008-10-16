x=1;
x;
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
print(new x.a())
