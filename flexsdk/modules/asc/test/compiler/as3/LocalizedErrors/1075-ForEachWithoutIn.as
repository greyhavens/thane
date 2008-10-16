/*
1075	The each keyword is not allowed without an in operator.
*/
class A {
    function f() {
        for each (var i = 0;;) {
            var j = 0;
        }
    }
}
