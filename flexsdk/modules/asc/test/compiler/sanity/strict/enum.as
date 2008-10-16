print("1:");

Array.prototype.foo = function () {}
var a = [1,2,3]
for (var x in a)
{
	print(x)
}

print("2:");

for (var k in 3)
{
	print(k)
}

print("3:");

Number.prototype.xyz = function () {}

for (var k in 3)
{
	print(k)
}

print("4:");

Number.prototype.setPropertyIsEnumerable("xyz", false);

for (var k in 3)
{
	print(k)
}

print("5:");

Number.prototype.setPropertyIsEnumerable("xyz", true);

for (var k in 3)
{
	print(k)
}

