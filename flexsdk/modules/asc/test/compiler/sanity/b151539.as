// A test to verify that a QName works the same as a namespace::name pair
// The QNameObject constructor needs to use the exact namespace (which is 
// interned) as it gets as an constructor argument.
namespace ns;

class Example {
	ns var foo:String;
}

var o = new Example();

o.ns::foo = "10";
trace (o.ns::foo);
trace (o[new QName(ns, "foo")]);
trace (o[QName(ns, "foo")]);
