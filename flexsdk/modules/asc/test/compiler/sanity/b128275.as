function TEST(n, e, a) {
if (e != a) {
print("Test failed: " + a + ", expected: " + e);
} else {
print("Test passed");
}
}
var content = "<foo name=\"value\">bar</foo>";
x1 = <x><a>{content}</a></x>;
correct = <x/>;
correct.a = content;
TEST(16, correct, x1);
