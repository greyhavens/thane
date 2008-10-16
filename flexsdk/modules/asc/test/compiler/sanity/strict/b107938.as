var o = [1]
print(o[0]) // 1
var n = 0
o[n++] *= 2
print(n) // 1
print(o[0]) // 2
print(o[1]) // undefined
