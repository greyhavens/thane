var t = "how you doin?"
function test() 
{ 
    x = function() { return t; }

    print(x.call());

}

test();