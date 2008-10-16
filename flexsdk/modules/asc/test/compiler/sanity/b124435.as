namespace ns = "foo"
nsx = new Namespace("foo")
class C
{
ns var x
public function C()
{
  nsx::x = 10
}
}
var s = new C();
print(s.nsx::x)