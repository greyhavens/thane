x = 1
var y = 2
print(delete x)
print(delete y)

var o = { a:1, b:2, c:3 }
print(o.a)
print(o.b)
print(o.c)

delete o.b

print(o.a)
print(o.b)
print(o.c)

delete o["c"]

print(o.a)
print(o.b)
print(o.c)
