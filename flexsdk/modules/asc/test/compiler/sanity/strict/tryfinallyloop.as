try
{
    // this loop should execute twice, then the finally once
    var i: int = 0;
	for(i = 0; i < 2; ++i)
	{
        trace("whee");
		continue;
        trace("whee2");
	}
}
finally
{
	trace("finally");
}


// This loop should execute twice, and the finally should
// execute twice as welll since the continue jumps out of
// the try block
for(i = 0; i < 2; ++i)
{
    try
    {
        trace("whee");
		continue;
        trace("whee2");
	}
    finally
    {
    	trace("finally");
    }
}
