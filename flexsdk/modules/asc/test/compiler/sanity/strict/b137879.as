// test to ensure that parameters to string methods are declared Number, not int.  int coercion uses toInt32(), 
//  which converts Infinity/-Infinity to 0 and any number > 2^32 to a number modulo 2^32.  As spec'ed, String
//  methods should use toInteger(), which  ensures that the double value is a decimal (or Infinity/-Infinity),
//  allowing for proper bounds detection.

var a = "abcdefg";
print("charAt(Infinity) == " + a.charAt(Infinity));
print("a.charAt(4294967296) == " + a.charAt(4294967296));
print("a.charAt(4294967296+1) == " + a.charAt(4294967296+1));
print("a.indexOf('2',4294967296) == " + a.indexOf("2",4294967296));
print("a.charCodeAt(4294967296) == " + a.charCodeAt(4294967296));
print("a.substring(4294967296,4294967296+2) == " + a.substring(4294967296,4294967296+2));
print("a.substring(NaN,Infinity) == " + a.substring(NaN,Infinity));