var x = <x xmlns:ns="foo"><ns:a>10</ns:a><b/><ns:c/></x>
var ns = new Namespace("foo")

var f\u0061 = 3;
print("1:",x.*)
print("2:",x.ns::*)
print("3:",x.ns::a)
x.ns::a
x.ns::a = 20
print("4:",x.ns::a)
//print("5:",x.*::a) // issue: assert
var y = <y xmlns:ns="foo" a="10" b="20" ns:c="30" ns:d="40"/>
print("6:",y.@a)
print("7:",y.@*)     // issue: includes ns:c and ns:d
print("8:",y.@ns::*)
y.@b = 200
print("9:",y.@b)
delete y.@b
print("10:",y.@*)
delete x.ns::a
print("11:",x.*)
var c = <class>foo</class>
print(c.toXMLString())


// compiler error
/*
y.@a()   // Attributes are not callable  
*/