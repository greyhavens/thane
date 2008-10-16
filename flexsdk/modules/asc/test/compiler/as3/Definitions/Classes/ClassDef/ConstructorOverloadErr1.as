/**
 *  Bug #108718
 *  http://flashqa.macromedia.com/bugapp/detail.asp?id=108718
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
		
		public function Bar(){}
		
		public function Bar(s:String){}
	}
}