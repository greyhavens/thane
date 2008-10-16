package  {
  import flash.display.Sprite
  import flash.text.TextField
  public class swf extends Sprite {
     function hello():void {
       var t:TextField=new TextField()
       t.text='hello'
       t.x=5
       t.y=5
       addChild(t)
     }
  }
}