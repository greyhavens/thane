/**
 *	Class NativeExtClassErr2
 *
 *  	Accesses default function of default class
 *  	from public method of a native sub class
 *
 *	Modified: 03/23/2005 (gasingh@macromedia.com) 
 *	Changed the package name from pack to Errors.
 *	Changed the class name to reflect the file name.
 *	Changed print statement to a return statement.
 *	Changed the Import definition to reflect the new one of import.<file>.*;
 */

package Errors {

	class NativeExtClassErr2 {
		
		function foo() {
			return true;
			// print("Base class");
		}
	 }

	 native class B extends NativeExtClassErr2 {

	      	public function goo() {
	       		foo();
	      	}
	 }
}

import Errors.*;

var b = new B();
b.goo();

 // print( "FAILED, native class should not be allowed" );