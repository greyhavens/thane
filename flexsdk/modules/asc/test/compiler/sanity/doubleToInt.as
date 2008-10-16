// tests double to integer truncation, (see ES3 spec, 9.5)

var a = 2147483646;
for(var x=0; x<4; x++) {
   print(a + ' | ' + a + " = " + (a|a));
   a++
}
var a = 4294967294;
for(var x=0; x<4; x++) {
   print(a + ' | ' + a + " = " + (a|a));
   a++
}
a = (a-2)*10 - 2;
for(var x=0; x<4; x++) {
   print(a + ' | ' + a + " = " + (a|a));
   a++
}
var a = -2147483646;
for(var x=0; x<4; x++) {
   print(a + ' | ' + a + " = " + (a|a));
   a--
}
a = (a+2)*10 - 2;
for(var x=0; x<4; x++) {
   print(a + ' | ' + a + " = " + (a|a));
   a++
}

// WLS - 21sep05 - added some more tests
var z1:Number = 1000000000.0;
print ("uint(1000000000) is " + (uint(z1)));
z1 = 2000000000.0;
print ("uint(2000000000) is " + (uint(z1)));
z1 = 3000000000.0;
print ("uint(3000000000) is " + (uint(z1)));
z1 = 4000000000.0;
print ("uint(4000000000) is " + (uint(z1)));
z1 = 5000000000.0;
print ("uint(5000000000) is " + (uint(z1)));
z1 = -1.0;
print ("uint(-1) is " + (uint(z1).toString(16)));
z1 = -1000000000.0;
print ("uint(-1000000000) is " + (uint(z1).toString(16)));
z1 = -2000000000.0;
print ("uint(-2000000000) is " + (uint(z1).toString(16)));
z1 = -3000000000.0;
print ("uint(-3000000000) is " + (uint(z1).toString(16)));
z1 = -4000000000.0;
print ("uint(-4000000000) is " + (uint(z1).toString(16)));

z1 = 1000000000.0;
print ("int(1000000000) is " + (int(z1)));
z1 = 2000000000.0;
print ("int(2000000000) is " + (int(z1)));
z1 = 3000000000.0;
print ("int(3000000000) is " + (int(z1)));
z1 = 4000000000.0;
print ("int(4000000000) is " + (int(z1)));
z1 = 5000000000.0;
print ("int(5000000000) is " + (int(z1)));
z1 = -1.0;
print ("int(-1) is " + (int(z1)));
z1 = -1000000000.0;
print ("int(-1000000000) is " + (int(z1)));
z1 = -2000000000.0;
print ("int(-2000000000) is " + (int(z1)));
z1 = -3000000000.0;
print ("int(-3000000000) is " + (int(z1)));
z1 = -4000000000.0;
print ("int(-4000000000) is " + (int(z1)));

// rounding cases
print ("-1.4 >>> 0 is " + (-1.4 >>> 0).toString(16));
print ("-1.6 >>> 0 is " + (-1.6 >>> 0).toString(16));
print ("1.4 >>> 0 is " + (1.4 >>> 0).toString(16));
print ("1.6 >>> 0 is " + (1.6 >>> 0).toString(16));
print ("-1.4 >> 0 is " + (-1.4 >> 0).toString(16));
print ("-1.6 >> 0 is " + (-1.6 >> 0).toString(16));
print ("1.4 >> 0 is " + (1.4 >> 0).toString(16));
print ("1.6 >> 0 is " + (1.6 >> 0).toString(16));

print (uint(Number.NaN));
print (int(Number.NaN));
print (uint(Number.NaN));
print (int(Number.POSITIVE_INFINITY));
print (uint(Number.POSITIVE_INFINITY));
print (int(Number.NEGATIVE_INFINITY));
print (uint(Number.NEGATIVE_INFINITY));
