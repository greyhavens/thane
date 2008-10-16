LabelTest(0, 0);
LabelTest(1, 1)
LabelTest(-1, 1000);
LabelTest(false, 0);
LabelTest(true, 1);

function LabelTest( limit, expect) {
woo: for ( var result = 0; result < 1000; result++ ) { if (result == limit) { break
woo; } else { continue; } };
print(result);
}
