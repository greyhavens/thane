// [Coach] Warning #1038: 'Number()' would return 'NaN' in AS2 when the argument was '' or contained whitespace.  In AS3 whitespace is ignored and 0 is returned for ''.
package Warnings {
   num = new Number("12");
   num = new Number(" ");
   num = new Number();
}

var num:Number = new Number("12");
num = new Number(" ");
num = new Number();
