function throwException()
{
throw new TypeError("blah");
}

try
{
throwException();
}
catch(e)
{
print("in catch, throwing exception");
throwException();
}
finally
{
print("in finally");
}
print("shouldn't execute");
