/* 
1113	Circular type reference was detected in _.
*/

class A extends B { }
class B extends A { }
class C extends C {
  
}
