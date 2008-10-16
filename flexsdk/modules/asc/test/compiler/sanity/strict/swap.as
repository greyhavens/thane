function SwapTest()
{
trace("start");
var y1: Number = 1;
var y2: Number = 2;
var y3: Number = 3;
var y4: Number = 4;
var y5: Number = 5;
var y6: Number = 6;
var y7: Number = 7;
var y8: Number = 8;
var y9: Number = 9;

trace( [ y1, y2, y3, y4, y5, y6, y7, y8, y9 ] );

var tmpf: Number;

if( y1 < y2 )
{
	trace('a')
	tmpf = y1
	y1 = y2
	y2 = y3
	y3 = y4
	y4 = y5
	y5 = y6
	y6 = y7
	y7 = y8
	y8 = y9
	y9 = tmpf
}

trace( [ y1, y2, y3, y4, y5, y6, y7, y8, y9 ] );
}

SwapTest()
