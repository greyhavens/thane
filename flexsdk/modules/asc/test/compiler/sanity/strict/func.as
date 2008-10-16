f3("hello",", world","!!");
f1("goodbye");
function f1(s1) {
    f2(s1,", world");
}
function f2(s1,s2) {
	f3(s1,s2,"!!");
}
function f3(s1,s2,s3) {
	print(s1,s2,s3);
}
