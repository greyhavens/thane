// "correct" version
package
{
	public interface BaseInterfaceMethod
	{
		function interface_method(a:String, newarg:String):void;
	}

	public interface BaseInterfaceGetter
	{
		function get interface_prop():Object;
	}

	public interface BaseInterfaceSetter
	{
		function set interface_prop(s:Object):void;
	}

	public class BaseClass 
	{
		public function class_method(a:String, newarg:String):void
		{
			print("New: " + a + " " + newarg);
		}
	}
}