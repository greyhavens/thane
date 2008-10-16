
// test1: array init order
var a = [1, 2];
trace(a[0] == 1 && a[1] == 2 ? "pass" : "fail");

// test2: function arg order
function test2(a, b, c)
{
	if (a==1 && b==2 && c==3)
		trace("pass");
	else
		trace("fail a="+a+" b="+b+" c="+c);
}
test2(1,2,3);


