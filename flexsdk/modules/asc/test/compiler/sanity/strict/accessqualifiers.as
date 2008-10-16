package p {
public class A { 
  public function A() { 
    print("p.A") 
  }
}
}
package q {
class A { 

  function A() { 
    print("internal::A") 
  } 

  static protected var z = 10
  private var x = 20
  protected var y = 30
  static protected var y = -30  // shadowed by instance y in an instance context

  print(protected::z)   // 10

  function m() {
    print(private::x)   // 20
    print(protected::y) // 30
  }
}
}
package q {
import p.A
function f() {
  new p.A   // public p.A
  new internal::A  // internal A in q
  new internal::A().m()
}
f()
}



