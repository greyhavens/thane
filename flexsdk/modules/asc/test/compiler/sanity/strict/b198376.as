class a
{ 
            const f;
            function a(v)
            {
                        this[v] = 1
                        print("success")
            }
}
class b extends a 
{
	const g;
	function b(v)
	{
		super(v)	
	}
}
new b("f") // should work
new b("g") // shouldn't work
