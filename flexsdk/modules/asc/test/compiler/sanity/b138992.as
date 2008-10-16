package mx {
var mx = 10
print(mx)
}

package A {
public class A {

public function A() {
print("A.A instantiator");
}
}
}

package A.A {

public class B {

public function B() {
print("A.A.B instantiator");
}
}
}


import A.*;
import A.A.*

new A.A
new A

A.A()
A()

A.A = 10
A = 10

A.A
A

new A.A.B
A.A.B()
A.A.B = 10
A.A.B
