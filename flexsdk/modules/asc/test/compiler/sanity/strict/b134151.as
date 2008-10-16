
function mySlice(a, from, to)
{
var from2 = from;
var to2 = to;
var returnArray = [];
var i;

if ((to2 > from2)&&(to2 > 0)&&(from2 < a.length))
{
for (i = from2; i < to2; ++i) returnArray.push(a[i]);
}
return returnArray;
}

var b = [1,2,3,4,5,6,7,8,9,0];


function exhaustiveSliceTest(testname, a)
{
var x = 0;
var y = 0;
for (x = -(2 + a.length); x <= (2 + a.length); x++)
for (y = (2 + a.length); y >= -(2 + a.length); y--)
{
var c = mySlice(a,x,y);
print("mySlice(",a,",",x,",",y,") = ",c)
}
//return testCase;
}


exhaustiveSliceTest("exhaustive slice test 2", b);
