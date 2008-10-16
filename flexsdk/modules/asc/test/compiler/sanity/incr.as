x=0;
print(++x)
print(x++)
print(x)
print(--x)
print(x--)
print(x)

var y=0;
print(++y)
print(y++)
print(y)
print(--y)
print(y--)
print(y)

var o = {a:0}
print(++o.a)
print(o.a++)
print(o.a)
print(--o.a)
print(o.a--)
print(o.a)

o = {a:0}
var p = 'a'
print(++o[p])
print(o[p]++)
print(o[p])
print(--o[p])
print(o[p]--)
print(o[p])
function f() {
var z=0;
print(++z)
print(z++)
print(z)
print(--z)
print(z--)
print(z)
return z;
}
f()==0;

o = "foo";
print(o++);
print(o);
o = "foo";
print(++o);
print(o);
o = "foo";
print(o--);
print(o);
o = "foo";
print(--o);
print(o);

o = "33.3";
print(o++);
print(o);
o = "33.3";
print(++o);
print(o);
o = "33.3";
print(o--);
print(o);
o = "33.3";
print(--o);
print(o);
