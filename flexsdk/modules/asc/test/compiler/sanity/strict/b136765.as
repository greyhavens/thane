package A
{
public class C
{ 
public function C()
{
    print( "A.C.C()" ); 
}
}

}

package B
{
import A.*;
public class C extends A.C
{
    public function C()
    {
        print("B.C.C()");
    }
}
}

import B.*;
var s = new C();

