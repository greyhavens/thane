
package errors {
namespace ns1;

class A {
ns1 function foo(): String { return "A.ns1::foo()"; }
}

class B extends A {
override ns1 function foo(): String {
return super.ns1::foo() + " override";
}
}

class C extends A {
override ns1 function foo(): String {
return super.foo() + " override"; // no namespace ns1 => method not found exception
}
}

var b:B = new B();
print( b.ns1::foo() );

var c:C = new C()
print( c.ns1::foo() )
}

