function f() { return "first" };
print(f()) // 'second', because fun defs take effect at the beginning of the script.
function f() { return "second" };
print(f())
print(f()=="second")

