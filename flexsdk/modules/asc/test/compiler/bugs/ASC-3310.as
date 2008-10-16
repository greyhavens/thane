package
{
	import flash.display.*;
	import flash.events.*;
	import flash.text.*;

	public class MainPocket extends Sprite
	{
		public function MainPocket()
		{
			rebuildStage();
		}
			
		public function rebuildStage()
		{
			var l:int = numChildren;
			var p:Number=0;
			var i:int=0;
			var o:DisplayObject;
			
			//Sort
			trace("test");
			if(false){
				for(i=0;i<l;i++){
					o=getChildAt(i);
					if(o==null)continue;
				}
			}
		}
	}
}
