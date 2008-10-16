for( var x : int = 0; x < 10; ++x )
{
     print(x)
}

if(true) 
{
    var x : int = 10
    print(x)
}
else
{
    var x : int = 20
}

if(false) 
{
    var x : int = 10
}
else
{
    var x : int = 20
    print(x)
}

while( true ) 
{
    var x : int = 30
    print(x)
    break;
}

do 
{
    var x : int = 40
    print(x)
} while(false)

{
    var x : int = 50
    print(x)
}

try 
{
    var x : int = 60
    print(x)
    throw 70
}
catch( x : int )
{
    print(x)
}
finally
{
    var x : int = 80
    print(x)
}

with({})
{
    var x : int = 90
    print(x)
}

