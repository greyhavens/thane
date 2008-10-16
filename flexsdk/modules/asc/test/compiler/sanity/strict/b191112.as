// this should be compiled with -! and b191112.abc imported
package
{
	import a.b.c.d.Foo;
	import a.b.Bar;

	function main()
	{
		try
		{
			var f : a.b.c.d.Foo;
		}
		catch (e)
		{
			print("caught exception: "+e);
		}
	}
	main();
}
