/**
 *  	Class ExtDImplAccessDPFPrivStatErr
 *
 *  	Attempts to access a default property from
 *  	a static method of the sub class
 *
 *	Modified: 03/30/2005 (gasingh@macromedia.com) 
 *	Added the package Errors to encapsulate the test case code.
 *	Changed the class name to reflect the file name.
 */

package Errors {

	import Definitions.Classes.DynamicClass;
	import Definitions.Classes.DefaultInt;
	import Definitions.Classes.DefaultIntDef;

	class Definitions.Classes.ExtImpl.Errors.ExtDImplDDAccDPFPrivStatErr extends DynamicClass implements DefaultIntDef, DefaultInt {

	    private static function subGetArray() : Array { return array; }

	    // interface definitions
	    function iGetBoolean() : Boolean {}
	    function iGetPubBoolean() : Boolean {}
	    function iGetNumber() : Number {}
	    function iGetPubNumber() : Number {}
	}
}
