function test()
{
    var firstFunc = function firstFunc()
        {
            var itemCount = 500;
            callFunc(
                function secondFunc()
                {
                    print(itemCount);
                }    
            );
            
        }

    callFunc( firstFunc );
    firstFunc();
}

function callFunc( f )
{
    f();
}

test();
test();        
test();
test();

function test2()
{
    var f = function recurse( x : int)
    {
        trace(x);
        if( x <=0 )
            return;
        else
            recurse(x-1);
    } 
    
    f(4);
}
print("Testing recursive anonymous function");

test2();
