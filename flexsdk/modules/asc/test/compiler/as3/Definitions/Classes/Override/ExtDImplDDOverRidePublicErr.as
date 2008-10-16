/**
 *  	Class ExtDImplDDOverRidePublicErr
 *
 *  	This class overrides methods of the parent class
 *  	with public methods in the sub class
 *
 *	Modified: 03/30/2005 (gasingh@macromedia.com) 
 *	Added the package Errors to encapsulate the test case code.
 *	Changed the class name to reflect the file name.
 */

package Errors {

	dynamic class DynamicClass {

	    var boolean:Boolean;
	    public var pubBoolean:Boolean;
		private var privArray:Array;
	    static var statBoolean:Boolean;

	    function getBoolean() : Boolean { return boolean; }
	    public function setPubBoolean() : Boolean { return pubBoolean; }
	    private function setPrivBoolean() : Boolean { return privBoolean; }
	    static function setPrivBoolean() : Boolean { return privBoolean; }

	}

	interface DefaultIntDef {
	     function foo();
	}

	interface DefaultInt {
	     function goo();
	}

	class ExtDImplDDOverRidePublicErr extends DynamicClass implements DefaultIntDef, DefaultInt {

	    // interface definitions
	    function foo() {}
	    function goo() {}

	    // *******************************
	    // override a default method
	    // with a default method
	    // *******************************

	    public function getBoolean() : Boolean { return true; }

	    // *******************************
	    // override a public method
	    // with a default method
	    // *******************************

	    public function setPubBoolean( b:Boolean ) { return b; }

	    // *******************************
	    // override a private method
	    // with a default method
	    // *******************************

	    public function setPrivBoolean( b:Boolean ) { return b; }

	    // *******************************
	    // override a static method
	    // with a default method
	    // *******************************

	    public function setStatBoolean( b:Boolean ) { return b; }

	}

	var out = new ExtDImplDDOverRidePublicErr();

	out.iBaseClass();

	// print( "FAILED, should give compile time error for overriding" );
}
