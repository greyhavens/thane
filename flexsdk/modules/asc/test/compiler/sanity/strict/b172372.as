var s = "Sie haben ein grünes Buch.";

// Some basic preliminaries
print(s)
print("length = " + s.length);
print(s.substr(22))

// Test String.search
print ("String.search:");
print ((s.search(/Buch/) == 21) ? "PASS" : "FAIL");

// Test String.replace
print ("String.replace:");
print (s.replace(/[ei]+/g, "_"));
print (s.replace(/ü/, "_"));
print (s.replace(/[^a-z. ]/gi, "_"));
print (s.replace(/./g, function (s) { return s.toUpperCase(); }));

// Test String.match
print ("String.match:");
print (s.match(/Buch/));
print (s.match(/gruünes/));
print (s.match(/grünes/));

// Test String.split
print ("String.split:");
var a = s.split(/\s+/);
for (var i=0; i<a.length; i++) {
	print(i + ": " + a[i]);
}

// Test RegExp.exec, RegExp.lastIndex
var re = /Buch/g;
re.exec(s)
print(re.lastIndex)
