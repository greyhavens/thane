package P
{

public function C() { trace("P.C") }
}

package Q
{
public function C() { trace("Q.C") }

}

import P.*;
import Q.*;
C();
