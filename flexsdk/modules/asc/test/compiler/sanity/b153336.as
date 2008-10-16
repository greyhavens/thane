function f()
{
try
{
throw new Error();
}
catch (e)
{
	print("in catch: " + e)
}
finally
{
	print("in finally")
	print(e)
}
}
f();
