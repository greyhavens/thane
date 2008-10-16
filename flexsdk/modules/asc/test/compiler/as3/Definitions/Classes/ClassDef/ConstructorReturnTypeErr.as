/**
 *  Bug #108719
 *  http://flashqa.macromedia.com/bugapp/detail.asp?id=108719
 *	
 *	Constructors are allowed to have return values
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
	
		public function Bar():Bar{}
	}
}