o = {a:10,b:10,c:10}
var n = 0
for(p in o) {
    ++n
    print(o[p])
}
print(n)
n = 0
for(var r in o) {
    ++n
    print(o[r])
}
print(n)
