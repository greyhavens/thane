package
{
	import BaseClass;
	
	class IllegalClassOverride extends BaseClass 
	{
		override public function class_method(a:String):void
		{
			print("Override: " + a);
		}
	}
	
	var a:IllegalClassOverride = new IllegalClassOverride();
	a.class_method("Hello");
}