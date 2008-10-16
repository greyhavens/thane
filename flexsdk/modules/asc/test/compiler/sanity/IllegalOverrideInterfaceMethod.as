package
{
	import BaseInterfaceMethod;
	
	class IllegalInterfaceOverride implements BaseInterfaceMethod 
	{
		public function interface_method(a:String):void
		{
			print("interface_method: " + a);
		}
	}
	
	var a:IllegalInterfaceOverride = new IllegalInterfaceOverride();
	a.interface_method("Hello");
}