/**
 *	Class IllegalAccSpecifiersInStatClassErr
 *	
 *  	Attempts to define a static class property/methods
 *      with illegal access specifiers.
 */

package Errors {

	public class IllegalAccSpecifiersInStatClassErr {
	
	  static class Inner {
	
	 	pub var a;
	 	priv var b;
	 	fin var c;
	 	stat var d;
	 	pub stat var e;
	 	
	 	publi function foo1() {return 1;} 
	 	priv function foo2() {return 1;}
	 	fin function foo3() {return 1;}
	 	stat function foo4() {return 1;}
	 	pub stat function foo5() {return 1;}
	  }
	}
	
}

