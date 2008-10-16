// various tests to handle our integer key usage 

// test array access
var a = [];
a["0"] = "a";
a["-0"] = "b";
a["+0"] = "c";
a["000"] = "d";
a["0x00"] = "e";
trace (a["0"]);
trace (a["-0"]);
trace (a["+0"]);
trace (a["000"]);
trace (a["0x00"]);
trace (a[0]); // should match ["0"];
a[0] = "a";
trace (a[0]); 
a[-0] = "b";
trace (a[0]); 
a[+0] = "c";
trace (a[0]); 
a[000] = "d";
trace (a[0]); 
a[0x00] = "e";
trace (a[0]); 
a[1] = "one";
trace (a["1"]);
a[0x0FFFFFFF] = "last one";
trace (a[0x0FFFFFFF]);
trace (a["268435455"]);
a[0x10000000] = "first int that will get interned as a string";
trace (a[0x10000000]);
trace (a["268435456"]);
a[-1] = "negative one"; // interned using "-1" 
trace (a["-1"]);
trace (a[-1]);
trace (a["-00001"]); // not a match
a["4294967295"] = "max int";
trace (a["4294967295"]);
trace (a[4294967295]);
a[4294967295] = "max int 2";
trace (a["4294967295"]);
trace (a[4294967295]);

// test object access using same values
a = {};
a["0"] = "a";
a["-0"] = "b";
a["+0"] = "c";
a["000"] = "d";
a["0x00"] = "e";
trace (a["0"]);
trace (a["-0"]);
trace (a["+0"]);
trace (a["000"]);
trace (a["0x00"]);
trace (a[0]); // should match ["0"];
a[0] = "a";
trace (a[0]); 
a[-0] = "b";
trace (a[0]); 
a[+0] = "c";
trace (a[0]); 
a[000] = "d";
trace (a[0]); 
a[0x00] = "e";
trace (a[0]); 
a[1] = "one";
a[1] = "one";
trace (a["1"]);
a[0x0FFFFFFF] = "last one";
trace (a[0x0FFFFFFF]);
trace (a["268435455"]);
a[0x10000000] = "first int that will get interned as a string";
trace (a[0x10000000]);
trace (a["268435456"]);
a[-1] = "negative one"; // interned using "-1" 
trace (a["-1"]);
trace (a[-1]);
trace (a["-00001"]); // not a match
a["4294967295"] = "max int";
trace (a["4294967295"]);
trace (a[4294967295]);
a[4294967295] = "max int 2";
trace (a["4294967295"]);
trace (a[4294967295]);
