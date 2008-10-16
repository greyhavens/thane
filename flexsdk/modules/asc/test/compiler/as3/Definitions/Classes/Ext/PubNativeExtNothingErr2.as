/**
 *	Class PubNativeExtNothingErr2
 *
 * 	Public native class is not allowed
 *	
 *	Modified: 03/23/2005 (gasingh@macromedia.com) 
 *	Changed the package name from pack to Errors.
 *	Changed the class name to reflect the file name.
 *	Changed print statement to a return statement.
 *	Changed the Import definition to reflect the new one of import.<file>.*;
 */

package Errors {

 	public native class PubNativeExtNothingErr2 {
     
      		public function foo() {
       			a = 20;
      		}
 	}
}

import Errors.*;

var b = new PubNativeExtNothingErr2();
b.foo();

 // print( "FAILED, public native class should not be allowed" );