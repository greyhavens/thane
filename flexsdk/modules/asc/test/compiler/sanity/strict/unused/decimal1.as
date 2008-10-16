// basic desimal sanity tests

var z1:decimal = 1.0;
var z2:decimal = 2.0;
print("basic arithmetic\n");
print ("5 + 2 is " + (5m + 2m));
print ("5 - 2 is " + (5m - 2m));
print ("5 * 2 is " + (5m * 2m));
print ("5 / 2 is " + (5m / 2m));
print ("5 % 2 is " + (5m % 2m));

print ("5.00 / 2 is " + (5.00m / 2));
print ("5.000 / 2 is " + (5.000m / 2));
print ("5.0000 / 2 is " + (5.0000m / 2));
print ("5.00000 / 2 is " + (5.00000m / 2));
print ("5.000000 / 2 is " + (5.000000m / 2));

print ("1 / 3 is " + z1/3);
print ("2 / 3 is " + z2/3);

print("");


print ("rounding and precision\n");
{ use precision 4; 
 {use rounding CEILING;
 print ("1/3 to 4 places (ceiling) is " + z1/3);
 print ("2/3 to 4 places (ceiling) is " + z2/3);
 print ("-1/3 to 4 places (ceiling) is " + -z1/3);
 print ("-2/3 to 4 places (ceiling) is " + -z2/3);
 }
 print("");
 {use rounding UP
 print ("1/3 to 4 places (up) is " + z1/3);
 print ("2/3 to 4 places (up) is " + z2/3);
 print ("-1/3 to 4 places (up) is " + -z1/3);
 print ("-2/3 to 4 places (up) is " + -z2/3);
 }
 print("");
 {use rounding HALF_UP
 print ("1/3 to 4 places (half_up) is " + z1/3);
 print ("2/3 to 4 places (half_up) is " + z2/3);
 print ("3.1415 + 0 to 4 places is (half_up) " + (3.1415m + 0) );
 print ("-1/3 to 4 places (half_up) is " + -z1/3);
 print ("-2/3 to 4 places (half_up) is " + -z2/3);
 }
 print("");
 {use rounding HALF_EVEN
 print ("1/3 to 4 places (half_even) is " + z1/3);
 print ("2/3 to 4 places (half_even) is " + z2/3);
 print ("3.1415 + 0 to 4 places is (half_even) " + (3.1415m + 0) );
 print ("-1/3 to 4 places (half_even) is " + -z1/3);
 print ("-2/3 to 4 places (half_even) is " + -z2/3);
 }
 print("");
 {use rounding HALF_DOWN
 print ("1/3 to 4 places (half_down) is " + z1/3);
 print ("2/3 to 4 places (half_down) is " + z2/3);
 print ("3.1415 + 0 to 4 places (half_down) is " + (3.1415m + 0) );
 print ("-1/3 to 4 places (half_down) is " + -z1/3);
 print ("-2/3 to 4 places (half_down) is " + -z2/3);
 }
 print("");
 {use rounding DOWN
 print ("1/3 to 4 places (down) is " + z1/3);
 print ("2/3 to 4 places (down) is " + z2/3);
 print ("-1/3 to 4 places (down) is " + -z1/3);
 print ("-2/3 to 4 places (down) is " + -z2/3);
 }
 print("");
 {use rounding FLOOR
 print ("1/3 to 4 places (floor) is " + z1/3);
 print ("2/3 to 4 places (floor) is " + z2/3);
 print ("-1/3 to 4 places (floor) is " + -z1/3);
 print ("-2/3 to 4 places (floor) is " + -z2/3);
 }
}
print ("\nuse <numbertype> pragma\n");
{use Number;
 print ("1/3 (use Number) = " + 1 / 3);
}
{use double;
 print ("1/3 (use double) = " + 1 / 3);
}
{use int;
 print ("1/3 (use int) = " + 1 / 3);
}
{use uint;
 print ("1/3 (use uint) = " + 1 / 3);
}
{use decimal;
 print ("1/3 (use decimal) = " + 1 / 3);
}
print("\nfunctions\n");
print("abs(-5) is " + decimal.abs(-5));
print("ceil(2.1) is " + decimal.ceil(2.1));
print("exp(1) is " + decimal.exp(1));
print("floor(2.9) is " + decimal.floor(2.9));
print("log(10) is " + decimal.log(10));
print("log10(2) is " + decimal.log10(2));
print("round(2.5, decimal.ROUND_HALF_EVEN) is " + decimal.round(2.5, decimal.ROUND_HALF_EVEN));
print("round(3.5, decimal.ROUND_HALF_EVEN) is " + decimal.round(3.5, decimal.ROUND_HALF_EVEN));
print("sqrt(2) is " + decimal.sqrt(2));
print("3.0^^4 is " + decimal.pow(3.0m, 4));
print("max(3m, 4m) is " + decimal.max(3m, 4m));
print("min(-3m, -4m) is " + decimal.min(-3m, -4m));
print("");
print ("1m / 0m is " + (1m / 0m));
print ("-1m / 0m is " + (-1m / 0m));
print ("0m / 0m is " + (0m / 0m));
print("");
print("2 + NaN is " + (2m + decimal.NaN));
print("2 - NaN is " + (2m - decimal.NaN));
print("2 * NaN is " + (2m * decimal.NaN));
print("2 / NaN is " + (2m / decimal.NaN));
print("2 % NaN is " + (2m % decimal.NaN));

