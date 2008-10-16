package {
  CONFIG const class2=!CONFIG::class1
  CONFIG::class1 class A {
    function getMessage():String { return "class1" }
  }
  CONFIG::class2 class A {
    function getMessage():String { return "class2" }
  }
  class B {
    CONFIG::class1 function getMessage():String {
      return "class1"
    }
    CONFIG::class2 function getMessage():String {
      return "class2"
    }
  }
  class C {
    CONFIG::class1 var var1="class1"
    CONFIG::class2 var var1="class2"
    function getMessage():String {
      return var1
    }
  }
  var a=new A()
  print(a.getMessage())
  var b=new B()
  print(b.getMessage())
  var c=new C()
  print(c.getMessage())
}
  