// This tests conversion of string to number for numbers at the edges of double precision
//  as well as Math.pow where the result is near those edges.
//  The edges are not symmetric about 0, min_value is magnitude e-324 while max_value is
//  magnitude e308.  A pow() optimization  for negative exponents is to divide by 
//  the -exponent.  This was causing problems (on Mac and Linux only for some reason)
//  when the exponent was between -308 and -324, because the -exponent would overflow.
//  pow() needs to special case such exponents.
//  pow() is used during String -> Number conversion, so it was affected as well.
//  cn 9/22/06

trace(Number(String(Number.MIN_VALUE)));
trace(Number(String(Number.MAX_VALUE)));
trace(Number("4.0e-325")); // should round to 0
trace(Math.pow(2,-1074));  // should be MIN_VALUE
trace(Math.pow(2,-1075));  // should round to 0
trace(Math.pow(10,308));   // near MAX_VALUE
trace(Math.pow(10,309));   // should be Infinity, overflows MAX_VALUE
