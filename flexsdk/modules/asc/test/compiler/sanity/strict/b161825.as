function f()
{
try
{
try
{
return print("returning");
}
catch(e)
{
throw e;
}
finally
{
    print("innner-finally");
}
}
finally
{
print("finally");
}
}
f();