/**
 *  Bug #108718
 *  http://flashqa.macromedia.com/bugapp/detail.asp?id=108718
 *
 *	Modified scorfield 8/1/2005 constructors can only be public / default
 *	so the test should only attempt to overload those combos.
 *	
 *	Overloading a constructor does not throw an error
 *	
 *	Actual Results:
 *	Compiles without error
 *	
 *	Expected Results:
 *  Should be a compiler error.
 *
 */
package Errors{
	
	public class Bar{
	
		public function Bar(){}
		
		function Bar(){}
		
	}
}
