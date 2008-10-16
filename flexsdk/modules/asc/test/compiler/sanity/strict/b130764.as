//#130764 should not have any errors while executing

interface A {
   function one():void;
}

interface B extends A {
   function two():void;
}

class C implements B
{
   public function one():void { print("one"); }
   public function two():void { print("two"); }
}

var d:B = new C();
d.one();
d.two();
