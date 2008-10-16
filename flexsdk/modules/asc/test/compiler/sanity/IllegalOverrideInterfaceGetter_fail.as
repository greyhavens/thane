package
{
	import BaseInterface;
	
	class IllegalInterfaceOverride implements BaseInterfaceGetter 
	{
		private var foo:String = "foo";
		
		public function get interface_prop():String
		{
			print("get interface_prop: " + foo);
			return foo;
		}
	}
	
	var a:IllegalInterfaceOverride = new IllegalInterfaceOverride();
	var r = a.interface_prop;
	print(r);
}