
// simple test to make sure builtin .length and .prototype getters
// are doing the right thing

function f(a) {}
print(f.length)

print(f.prototype.constructor === f)
print(Function.prototype.constructor === Function)
print(XML.prototype.constructor === XML)
