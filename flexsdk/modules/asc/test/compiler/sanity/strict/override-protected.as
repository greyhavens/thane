package
{
public class base {
public function test(){foo()}
protected function foo() { print("base.foo()")}
}
public class derived extends base {
protected override function foo() { print("derived.foo()") }
}
var s = new derived();
s.test();
}
