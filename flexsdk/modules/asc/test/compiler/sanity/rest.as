

var f = function (... args)
{
	try
	{
		trace(args);
	}
	catch (e:Error)
	{
		trace("yo")
	}
}

f(1,2,3)
	
