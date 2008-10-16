function f() { return function() { trace("hello"); } }
f()();
g()();
