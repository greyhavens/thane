package {
class MyTest {

public function test() {
var x = new Object();
x.a = 1;
return x;
}

static function test2() {
return [1,2,3];
}
}

var mz:MyTest = new MyTest();
var try1 = new mz.test();
var try2 = new MyTest.test2();

}
