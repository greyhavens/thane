package
{
public var f = function factorial (x) {
if (x < 1)
{
	return 1; 
}
else 
{
	return x * factorial(x-1);
}
}
print(f(8));

try
{
	// should be an error since the name factorial should only be in scope
	// inside the factorial method, and no where else
	print(factorial(8));
}
catch(e:ReferenceError)
{
	print("Caught ReferenceError - factorial is not visible");
}

}

