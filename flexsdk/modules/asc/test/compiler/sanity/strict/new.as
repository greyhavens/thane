/*
Object();
new Object;
new Object();
Object(a,b,c);
new Object(a,b,c);
new x.Object;
new x.Object(a,b,c);
*/
/*
new Object();
new this.Object();
*/						

function c() {
return this;
}

print(new c);
print(c());
var x = new c
x.a = 10
print(x.a)

//c(a,b);
//o.c(a,b);
//new o.c(a,b);
//x = new c(1,2,3);
//x.value;
