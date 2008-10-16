const x = 20/2
print(x)

class A
{
    const x
    const y
    function A() {
        x = 20
        this['y'] = 30 // should work from within constructor
    }

}
var a:A = new A
print(a.x)		// 20
print(a.y)		// 30
a['y'] = 40		// rt error
print(a.y)		// never reached
