var a = 5;
var b = 3;
var c = "x";
var x = <{c} a={a + " < " + b + " is " + (a < b)}>{a + " < " + b + " is " + (a < b)}</{c}>;
var x2 = "<x a=\"5 &lt; 3 is false\">5 &lt; 3 is false</x>"; 
print(x.toXMLString() == x2);

