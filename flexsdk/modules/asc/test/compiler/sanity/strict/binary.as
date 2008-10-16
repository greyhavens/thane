print(1==1)
var x = {}
print(x===x)
print("foo"=="foo")
print("foo"==="foo")
print(1+2==3)
print(7-3==4)
print(1+2+3+4+5+6+7+8+9+10-9-8-7-6-5-4-3-2-1==10)
print((2||3)==2);    // expect: 2
print((4&&5)==5);    // expect: 5
print(true==true)
print("is","foo" is String)

// test that the ConstantEvaluator optimization for adding a string and a number is not
//  used for numbers larger than the int/uint range.
print( 123456789012345671.9999 +""); // expect 123456789012345660 due to rounding.  
print( 1e2000 +'' );  // expect 'infinity' due to rounding.

