// Just want to make sure this doesn't produce any verify errors
function test()
{
    var z : int = 50;
    for (var i = 0; i <= 10; i++ ) 
    {
        if (i == 5) 
        {
            z = 20;
            y = 10;
        }
    }
    var y : int = 40;
}

test();