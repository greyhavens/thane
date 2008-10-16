package P
{
public class A
{
public var v:*;
}
public class B extends A
{
private var v:*;
public function B()
{
trace(v.foo);
trace(v.bar);
}
}
}