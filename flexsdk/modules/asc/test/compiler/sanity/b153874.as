package foo {
  public var pi = 3.14159;
  var x = 1;
  internal var y = 2;
}
import foo.*;
trace(pi); //should succeed
try {
  trace(x);  //ReferenceError: Error #1065: Variable x is not defined at global$init()
} catch (e:Error) {
  trace(e);
}
trace(y);  //ReferenceError: Error #1065: Variable y is not defined at global$init()
