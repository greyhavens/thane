function TEST(desc, e, a) {
if (e == a) {
trace("PASSED: " + a);
} else {
trace("FAILED: " + a + ", expected: " + e);
}
}

x1 = 
<>
<alpha attr1="value1">
<bravo>one</bravo>
<charlie>
two
three
<echo>four</echo>
</charlie>
<delta />
</alpha>
<delta>
<echo>five</echo>
</delta>
</>;

// Returns true for elements/attributes
TEST(2, true, x1.hasOwnProperty("bravo"));
TEST(3, true, x1.hasOwnProperty("@attr1"));
TEST(4, false, x1.hasOwnProperty("foobar"));
TEST(5, true, x1.hasOwnProperty("echo"));

