/*
 *	Expected Error: 
 *  Interface method hello in namespace Errors$internal  not
 *  implemented by class Errors$internal:MyClass
 */

package Errors {
	interface IInterface {
		function hello()
	}
	
	class MyClass implements IInterface {
	
	}

}
	