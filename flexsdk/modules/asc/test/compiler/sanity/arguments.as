function f(x,y,z)
{
    print(x,y,z)
    function g(x,y,z)
    {
        print(arguments[0],arguments[1],arguments[2])
        print(arguments.length)
        print(arguments[0]==x&&arguments[1]==y&&arguments[2]==z)
    }
    g(x,y,z)
}

f(10,20,30)

function h( x, arguments )
{
   print(x,arguments)
   var arguments = 30
   print(x,arguments)
}
h(10,20)