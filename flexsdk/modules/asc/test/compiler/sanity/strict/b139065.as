with (Object)
{
	try
	{
		undeclR = undecl;
	}
	catch (e)
	{
		trace("caught exception " + e)

		with (Object)
		{
			try
			{
				undeclR = undecl;
			}
			catch (ex)
			{
				trace("foo");
				trace("caught exception " + ex)
			}
		}
	}
}
