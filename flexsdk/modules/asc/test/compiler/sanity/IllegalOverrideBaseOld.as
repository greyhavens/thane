// "wrong" (outdated) version
package
{
	public interface BaseInterfaceMethod
	{
		function interface_method(a:String):void;
	}

	public interface BaseInterfaceGetter
	{
		function get interface_prop():String;
	}

	public interface BaseInterfaceSetter
	{
		function set interface_prop(s:String):void;
	}

	public class BaseClass 
	{
		public function class_method(a:String):void
		{
			print("Old: " + a);
		}
	}
}