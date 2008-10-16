/**
 *
 *    Error 1116: A user-defined namespace attribute can only be used at the top level of a class definition.
 *
 */

package myPackage
{
	public namespace ns1;

	public class myClass
	{
		ns1 function myFunction()
		{
			ns1 var myVar:int = 0;
		}
	}
}
