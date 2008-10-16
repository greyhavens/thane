function f()
{
	var e = 3.14;

	print("value of e is " + e);
	
	try
	{
		throw new Error();
	}
	catch (e:Error)
	{
		print("caught Error: " + e);
	}
	catch (e:TypeError)
	{
		print("caught TypeError: " + e);
	}

	try
	{
		throw new TypeError();
	}
	catch (e:ReferenceError)
	{
		print("caught ReferenceError: " + e);
	}
	catch (e:TypeError)
	{
		print("caught TypeError: " + e);
	}

	print("value of e is " + e);
}

f();
