/**
 *	Class ImplExtErr
 *	
 *	Default class implements default interface and 
 *	extends a public class.
 *
 *	Modified: 03/30/2005 (gasingh@macromedia.com) 
 *	Added the package Errors to encapsulate the test case code.
 *	Changed the class name to reflect the file name.
 */

package Errors {

	public class Simple {}

	interface Ia {}

	class ImplExtErr implements Ia extends Simple {}
}
