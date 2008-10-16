package p
{
namespace ns1;
namespace ns2;
namespace ns3;
namespace ns4;
class A {
ns1 function foo():String { return "A.ns1:foo"; }
ns2 function foo():String { return "A.ns2:foo"; }
ns3 function foo():String { return "A.ns3:foo"; }
ns4 function foo():String { return "A.ns4:foo"; }


ns1 function get bar():String { return "A.ns1:get foo"; }
ns2 function get bar():String { return "A.ns2:get foo"; }
ns3 function get bar():String { return "A.ns3:get foo"; }
ns4 function get bar():String { return "A.ns4:get foo"; }

ns1 function set bar(a:String):void { print ( "A.ns1:set foo" ); }
ns2 function set bar(a:String):void { print ( "A.ns2:set foo" ); }
ns3 function set bar(a:String):void { print ( "A.ns3:set foo" ); }
ns4 function set bar(a:String):void { print ( "A.ns4:set foo" ); }

}
class B extends A {
override ns2 function foo():String { return "B.ns2:foo"; }
override ns3 function foo():String { return "B.ns3:foo"; }

override ns2 function get bar():String { return "B.ns2:get foo"; }
override ns3 function get bar():String { return "B.ns3:get foo"; }

override ns2 function set bar(a:String):void { print ( "B.ns2:set foo" ); }
override ns3 function set bar(a:String):void { print ( "B.ns3:set foo" ); }

}

var x:B = new B();
print("Methods");
print( "x.ns1::foo() = " + x.ns1::foo() );
print( "x.ns2::foo() = " + x.ns2::foo() );
print( "x.ns3::foo() = " + x.ns3::foo() );
print( "x.ns4::foo() = " + x.ns4::foo() );
print("---");
print("Getters");
print( "x.ns1::get foo() = " + x.ns1::bar );
print( "x.ns2::get foo() = " + x.ns2::bar );
print( "x.ns3::get foo() = " + x.ns3::bar );
print( "x.ns4::get foo() = " + x.ns4::bar );
print("---");
print("Setters");
print( "x.ns1::set foo() = " ); x.ns1::bar="hi";
print( "x.ns2::set foo() = " ); x.ns2::bar="hi"
print( "x.ns3::set foo() = " ); x.ns3::bar="hi";
print( "x.ns4::set foo() = " ); x.ns4::bar="hi";

}
