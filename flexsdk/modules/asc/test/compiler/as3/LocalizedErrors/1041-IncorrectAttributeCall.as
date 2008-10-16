/*
1041	Attributes are not callable.
*/
package {
	class myTest {
		function myTest() {
			var x:XML = <one nast="foo"><two nee="22"/></one>;
//			print(x.@nast());
			x.@nast();
		}
	}
}
