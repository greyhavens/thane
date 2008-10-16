package {
  public class Test {
    public function Test() {
      var bar:String = "valOfBarLocal";
      var myObject:Object = {bar: "valOfBarInObject"};
      with (myObject) {
      // displays valOfBarLocal with -warnings on
      // displays valOfBarInObject with -warnings off
      // should display valOfBarInObject
      // so result with -warnings turned on is WRONG
        trace(bar);
      }
    }
  }
}

var t:Test = new Test();