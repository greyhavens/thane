/*
 *	Expected Error: 
 *  Interface members can't be declare as internal
 */
 
package Errors {
	interface IInterface {
		internal function hello()
	}
	
	class MyClass implements IInterface {
	
	}

}
	