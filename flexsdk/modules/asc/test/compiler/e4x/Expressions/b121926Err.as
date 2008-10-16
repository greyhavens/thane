print("-------------------------------------");

class AClass { var node:XML; var constructors:XML }
var r = new AClass();
r.node = <asClass>a</asClass>;
r.constructors = <constructors></constructors>;
r.constructors.constructor += <constructor>a<constructor>; // improper terminator
r.constructors.constructor += <constructor>b<constructor>; // also improper

r.node.constructors = r.constructors;
print("$$$$" + r.node.constructors );
//print("$$$$" + r.node.constructors.toXMLString() ); // same error as below
r.constructors = r.node.constructors;