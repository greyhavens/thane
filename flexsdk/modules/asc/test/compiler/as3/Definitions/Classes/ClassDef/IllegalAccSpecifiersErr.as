/**
 *	Class IllegalAccSpecifiersErr
 *	
 *  	Attempts to define a class property/methods
 *      with illegal access specifiers.
 */

package Errors {

	public class IllegalAccSpecifiersErr {
	
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

//import Errors.*;

//var obj = new AccDefMethFOutsideErr_rt();

//obj.foo();
