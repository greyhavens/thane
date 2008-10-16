package
{
public class foo { function foo() { print("foo") } }
public class bug 
{
public function bad():void
{

//  var f=function ():void 
//  {
//  print("foo")
//  }
  
  badHelper( 
    function ():void 
    {
    var c : Class = foo
    new c
    }
  )
}

public function badHelper(f:Function):void
{
f()
}
}
new bug().bad()
}
