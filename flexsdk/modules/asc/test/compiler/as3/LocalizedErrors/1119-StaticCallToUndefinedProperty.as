/*
1119	Access of possibly undefined property _ through a reference with static type _.
*/

class A
{
    public function A() { super() }
    public static var a:Number = 0;
}

A.b = 1;
