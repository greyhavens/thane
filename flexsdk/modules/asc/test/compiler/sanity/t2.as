class A {
static var x = 10
static function sm() { return x }
static function test() { print(sm()) }
}
print(A.sm())
var smc = A.sm
print(smc())
print(A.x)
A.test()