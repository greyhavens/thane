package
{

namespace N1 = "http://blarg";
namespace NS = N1;

class C
{
    N1 function test() { print("C.N1::test()"); }    
}

var s = new C;

s.NS::test();

use namespace NS;
s.test();



}