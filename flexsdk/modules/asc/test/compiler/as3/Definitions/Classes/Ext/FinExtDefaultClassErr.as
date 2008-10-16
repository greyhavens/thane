/**
 *  Class FinExtDefaultClassErr
 *    This test case is for the error case where the same attribute name is used simultaneously
 *    twice in the case method, i.e., public public function FOO() {}.  
 
 *
 *  Written on : 12/21/2004 (gasingh@macromedia.com)
 *
 *  Modified: 03/23/2005 (gasingh@macromedia.com) 
 *  Added the package Errors to encapsulate the test case code.
 *  Changed the class name to reflect the file name.
 *
 
 *  Final is a Member Modifier Attribute.
 *
 *  The virtual and final attributes make M be an instance member of C.
 *  The final attribute prevents subclasses from defining their own members 
 *  with the name M (unless they can’t see this M, in which case they can define 
 *  an independent M). virtual allows subclasses to override M.

 *  
 *  Final Class (negative test cases)
 */

package Errors {
	
	// The package definition has been changed, and now all the packages will be under the 
	// folder Definitions.Classes, and only this will be used and not the subfolders within.
	// This means that we cannot import a single Class file as was required to be done earlier.
	// All the files in the package folder will be compiled and used.

	// IMPORTANT: This might change later, hence only commenting out the import lines 
	//            and not deleting them. This also creates a problem that the compiled file
	//            might become too big to handle.


	class DefaultClass {

	    var array:Array;
	    var boolean:Boolean;
	    var date:Date;
	    var myFunction:Function;
	    var math:Math;
	    var number:Number;
	    var object:Object;
	    var string:String;
	    var simple:Simple;

	    public var pubArray:Array;
	    public var pubBoolean:Boolean;
	    public var pubDate:Date;
	    public var pubFunction:Function;
	    public var pubMath:Math;
	    public var pubNumber:Number;
	    public var pubObject:Object;
	    public var pubString:String;
	    public var pubSimple:Simple;

	    private var privArray:Array;
	    private var privBoolean:Boolean;
	    private var privDate:Date;
	    private var privFunction:Function;
	    private var privMath:Math;
	    private var privNumber:Number;
	    private var privObject:Object;
	    private var privString:String;
	    private var privSimple:Simple;

	    static var statArray:Array;
	    static var statBoolean:Boolean;
	    static var statDate:Date;
	    static var statFunction:Function;
	    static var statMath:Math;
	    static var statNumber:Number;
	    static var statObject:Object;
	    static var statString:String;
	    static var statSimple:Simple;

	    var finArray:Array;
	    var finBoolean:Boolean;
	    var finDate:Date;
	    var finFunction:Function;
	    var finMath:Math;
	    var finNumber:Number;
	    var finObject:Object;
	    var finString:String;
	    var finSimple:Simple;

	    public static var pubStatArray:Array;
	    public static var pubStatBoolean:Boolean;
	    public static var pubStatDate:Date;
	    public static var pubStatFunction:Function;
	    public static var pubStatMath:Math;
	    public static var pubStatNumber:Number;
	    public static var pubStatObject:Object;
	    public static var pubStatString:String;
	    public static var pubStatSimple:Simple;

	    private static var privStatArray:Array;
	    private static var privStatBoolean:Boolean;
	    private static var privStatDate:Date;
	    private static var privStatFunction:Function;
	    private static var privStatMath:Math;
	    private static var privStatNumber:Number;
	    private static var privStatObject:Object;
	    private static var privStatString:String;
	    private static var privStatSimple:Simple;

	    // *****************************
	    // to be overloaded
	    // *****************************

	    var overLoadVar;
	    public var pubOverLoadVar;
	    private var privOverLoadVar;
	    static var statOverLoadVar;
	    public static var pubStatOverLoadVar;
	    private static var privStatOverLoadVar;

	    // ****************
	    // constructor
	    // ****************

	    function DefaultClass() {
	    }

	    // *****************
	    // Default methods
	    // *****************

	    function getArray() : Array { return array; }
	    function getBoolean() : Boolean { return boolean; }
	    function getDate() : Date { return date; }
	    function getFunction() : Function { return myFunction; }
	    function getMath() : Math { return math; }
	    function getNumber() : Number { return number; }
	    function getObject() : Object { return object; }
	    function getString() : String { return string; }
	    function getSimple() : Simple { return simple; }

	    function setArray( a:Array ) { array = a; }
	    function setBoolean( b:Boolean ) { boolean = b; }
	    function setDate( d:Date ) { date = d; }
	    function setFunction( f:Function ) { myFunction = f; }
	    function setMath( m:Math ) { math = m; }
	    function setNumber( n:Number ) { number = n; }
	    function setObject( o:Object ) { object = o; }
	    function setString( s:String ) { string = s; }
	    function setSimple( s:Simple ) { simple = s; }

	    function setAll( a:Array,
			     b:Boolean,
			     d:Date,
			     f:Function,
			     m:Math,
			     n:Number,
			     o:Object,
			     s:String,
			     si:Simple ) {
		array = a;
		boolean = b;
		date = d;
		myFunction = f;
		math = m;
		number = n;
		object = o;
		string = s;
		simple = si;
	    }

	    // *******************
	    // public methods
	    // *******************

	    public function setPubArray( a:Array ) { pubArray = a; }
	    public function setPubBoolean( b:Boolean ) { pubBoolean = b; }
	    public function setPubDate( d:Date ) { pubDate = d; }
	    public function setPubFunction( f:Function ) { pubFunction = f; }
	    public function setPubMath( m:Math ) { pubMath = m; }
	    public function setPubNumber( n:Number ) { pubNumber = n; }
	    public function setPubObject( o:Object ) { pubObject = o; }
	    public function setPubString( s:String ) { pubString = s; }
	    public function setPubSimple( s:Simple ) { pubSimple = s; }

	    public function getPubArray() : Array { return this.pubArray; }
	    public function getPubBoolean() : Boolean { return this.pubBoolean; }
	    public function getPubDate() : Date { return this.pubDate; }
	    public function getPubFunction() : Function { return this.pubFunction; }
	    public function getPubMath() : Math { return this.pubMath; }
	    public function getPubNumber() : Number { return this.pubNumber; }
	    public function getPubObject() : Object { return this.pubObject; }
	    public function getPubString() : String { return this.pubString; }
	    public function getPubSimple() : Simple { return this.pubSimple; }

	    // *******************
	    // private methods
	    // *******************

	    private function getPrivArray() : Array { return privArray; }
	    private function getPrivBoolean() : Boolean { return privBoolean; }
	    private function getPrivDate() : Date { return privDate; }
	    private function getPrivFunction() : Function { return privFunction; }
	    private function getPrivMath() : Math { return privMath; }
	    private function getPrivNumber() : Number { return privNumber; }
	    private function getPrivObject() : Object { return privObject; }
	    private function getPrivString() : String { return privString; }
	    private function getPrivSimple() : Simple { return privSimple; }

	    private function setPrivArray( a:Array ) { privArray = a; }
	    private function setPrivBoolean( b:Boolean ) { privBoolean = b; }
	    private function setPrivDate( d:Date ) { privDate = d; }
	    private function setPrivFunction( f:Function ) { privFunction = f; }
	    private function setPrivMath( m:Math ) { privMath = m; }
	    private function setPrivNumber( n:Number ) { privNumber = n; }
	    private function setPrivObject( o:Object ) { privObject = o; }
	    private function setPrivString( s:String ) { privString = s; }
	    private function setPrivSimple( s:Simple ) { privSimple = s; }

	    // *******************
	    // static methods
	    // *******************

	    static function setStatArray(a:Array) { statArray=a; }
	    static function setStatBoolean( b:Boolean ) { statBoolean = b; }

	    static function getStatArray() { return statArray; }

	    // *******************
	    // final methods
	    // *******************

	    final function setFinArray(a:Array) { finArray=a; }
	    final function getFinArray() { return finArray; }

	    // **************************
	    // public static methods
	    // **************************

	    public static function setPubStatArray(a:Array) { pubStatArray=a; }
	    public static function setPubStatBoolean( b:Boolean ) { pubStatBoolean = b; }

	    public static function getPubStatArray() { return pubStatArray; }

	    // **************************
	    // private static methods
	    // **************************

	    private static function setPrivStatArray(a:Array) { privStatArray=a; }
	    private static function setPrivStatBoolean( b:Boolean ) { privStatBoolean = b; }

	    private static function getPrivStatArray() { return privStatArray; }

	    // ***************************
	    // to be overloaded
	    // ***************************

	    function overLoad() { return "This is the parent class"; }
	    public function pubOverLoad() { return "This is the parent class"; }
	    private function privOverLoad() { return "This is the parent class"; }
	    static function statOverLoad() { return "This is the parent class"; }
	    public static function pubStatOverLoad() { return "This is the parent class"; }
	    private static function privStatOverLoad() { return "This is the parent class"; }
	 }



	final class FinExtDefaultClassErr extends DefaultClass {

		// access default method of parent from public method of sub class
		public public function ppubSubSetArray( a:Array ) { setArray( a ); }
		public public function ppubSubGetArray() : Array { return getArray(); }


		// access static method of parent from static method of sub class
		static static function sstatSubSetArray( a:Array ) { setStatArray( a ); }
		static static function sstatSubGetArray() : Array { return getStatArray(); }


		// access default method of parent from final method of sub class
		final final function ffinSubSetArray( a:Array )  { setArray( a ); }
		final final function ffinSubGetArray() : Array { return getArray(); }


		// access default method of parent from virtual method of sub class
		virtual virtual function vvirSubSetArray( a:Array ) { setArray( a ); }
		virtual virtual function vvirSubGetArray() : Array { return getArray(); }


		// access default method of parent from private method of sub class
		private private function pprivSubSetArray( a:Array ) { setArray( a ); }
		private private function pprivSubGetArray() : Array { return getArray(); }

		// function to test above using a public function within the class
		// this function will be accessible to an object of the class.
		public function ttestPrivSubArray( a:Array ) : Array {
			this.privSubSetArray( a );
			return this.privSubGetArray();
		}
	}


	// Create an instance of the final class in the package.
	// This object will be used for accessing the methods created 
	// within the sub class given above.
	var EXTDCLASS = new FinClassExtDefaultClassErr(); 


	// The public method of the sub class.	
	public function ppubSubSetArray( a:Array )  { EXTDCLASS.ppubSubSetArray( a ); }
	public function ppubSubGetArray() : Array  { return EXTDCLASS.ppubSubGetArray(); }


	// The private method of the sub class. Only one is used as we need to call only the 
	// test function, which in turn calls the actual private methods, as within the class
	// we can access the private methods; but not outside of the class.
	public function ttestPrivSubArray( a:Array ) : Array  { return EXTDCLASS.ttestPrivSubArray( a ); }


	// The static method of the sub class.
	public function sstatSubSetArray( a:Array )  { FinClassExtDefaultClassErr.sstatSubSetArray( a ); }
	public function sstatSubGetArray() : Array  { return FinClassExtDefaultClassErr.sstatSubGetArray(); }

	// The STATIC class cannot be accessed by an object of the class, but has to be accessed
	// by the class name itself. Bug filed for this: 108206
	// public function sstatSubSetArray( a:Array )  { EXTDCLASS.sstatSubSetArray( a ); }
	// public function sstatSubGetArray() : Array  { return EXTDCLASS.sstatSubGetArray(); }


	// The final method of the sub class.
	public function ffinSubSetArray( a : Array )  { EXTDCLASS.ffinSubSetArray( a ); }
	public function ffinSubGetArray() : Array  { return EXTDCLASS.ffinSubGetArray(); }


	// The virtual method of the sub class.
	public function vvirSubSetArray( a : Array )  { EXTDCLASS.vvirSubSetArray( a ); }
	public function vvirSubGetArray() : Array  { return EXTDCLASS.vvirSubGetArray(); }

}
