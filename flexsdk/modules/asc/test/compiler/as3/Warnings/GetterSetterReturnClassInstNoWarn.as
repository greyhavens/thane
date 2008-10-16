//This test case should not give warnings.  See bug 125146 for details

package p
{
public class A { }

public class B
{
public function get someClass():Class { return A; }
public function f():void { var a:A = new someClass(); }
}
}