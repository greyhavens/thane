package P
{
public class C
{
public function C() { trace("P.C") }
}
}

package Q
{
public class C
{
public function C() { trace("Q.C") }
}
}

import P.*;
import Q.*;
c = new C();
