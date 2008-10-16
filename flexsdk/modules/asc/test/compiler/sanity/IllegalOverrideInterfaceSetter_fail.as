package
{
	import BaseInterfaceSetter;
	
	class IllegalInterfaceOverride implements BaseInterfaceSetter 
	{
		private var foo:String;
		
		public function set interface_prop(s:String):void
		{
			foo = s;
			print("set interface_prop: " + foo);
		}
	}
	
	var a:IllegalInterfaceOverride = new IllegalInterfaceOverride();
	a.interface_prop = "bar";
}