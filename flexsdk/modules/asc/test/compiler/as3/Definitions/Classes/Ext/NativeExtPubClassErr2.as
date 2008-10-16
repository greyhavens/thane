/**
 *	Class NativeExtPubClassErr2
 *  	Accesses default property of public class
 *  	from final method of a native sub class
 *
 *	Modified: 03/23/2005 (gasingh@macromedia.com) 
 *	Changed the package name from pack to Errors.
 *	Changed the class name to reflect the file name.
 *	Changed print statement to a return statement.
 *	Changed the Import definition to reflect the new one of import.<file>.*;
 */

package Errors {

	public class NativeExtPubClassErr2 {
 		var a = 10;
 	}

 	native class B extends NativeExtPubClassErr2 {
     
      		public final function foo() {
       			a = 20;
      		}
     
 	}

 
}

import Errors.*;

var b = new B();
b.foo();

 // print( "FAILED, native class should not be allowed" );