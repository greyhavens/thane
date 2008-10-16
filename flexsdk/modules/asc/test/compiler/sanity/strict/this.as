function c() {
}
function f() {
    print(this);
}
var x = new c
print(x)
x.f = f
x.f()
