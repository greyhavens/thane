var s:String = "abcdefghijk";
var t:String = "";
t += s.charAt(1);
t += s.charAt(2);
t += s.charAt(3);
t += s.charAt(4);
// T is a prefix strings whose first prefix is an offset string
trace (t);

// Test number conversion with an offset string
s = "100andthensometext"
t = s.substring (0, 2);
trace (Number(t));
trace (t);


s = "1234"
t = s + "abcd";

// Test substring with a prefix string - some output
// strings will be offset, some will be newly allocated
var u:String = t.substring(0, 2);
trace (u);
u = t.substring(1, 3);
trace (u);
u = t.substring(2, 4);
trace (u);
u = t.substring(4, 6);
trace (u);
u = t.substring(5, 7);
trace (u);
u = t.substring(6, 8);
trace (u);
u = t.substring(7, 9);
trace (u);
// spans two prefixes - will cause a normalization
u = t.substring(3, 5);
trace (u);

s = "1234abcd"
t = s.substring(0,4);
u = "1234";
trace (t == u); // compare an offset string to regular string
trace (u == t); // compare an offset string to regular string
u = "12";
u += s.substring(2,4);
trace (t == u); // compare an offset string to prefix string
trace (u == t); // compare an offset string to prefix string

// toLower/toUpper can work with offset strings directly
s = "1234ABCD";
t = s.substring(4,8);
u = t.toLowerCase();
trace (u);
t = s.substring(4,8);
u = t.toLowerCase();
u = u.toUpperCase();
trace (u);

// charAt/charCodeAt work with offset strings directly
t = s.substring(4,8);
trace (t.charAt(0));
trace (t.charCodeAt(0));
t = s.substring(0,1);
trace (t.charAt(0));
trace (t.charCodeAt(0));

t = s.substring(4,8);
trace (t.indexOf ("C"));
trace (t.lastIndexOf ("C"));

// tricky triple prefix - where substring gets middle prefix but middle
// is not large enough so it forces a normalization
s = "12345";
s += "abcde";
s += "67890";

t = s.substring (5, 13);
trace (s);
trace (t);
trace (t.length);
