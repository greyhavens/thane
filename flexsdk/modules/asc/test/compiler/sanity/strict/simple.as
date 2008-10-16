package
{
    public class base{  
            public function addFrameScript(... frames):void {print("base.addFrameScript") }
    }
  public class bug extends base{

            public function f() {}

            public function bug() {
                        this.addFrameScript(1, f);

            }

  }

var s : bug = new bug();
}
