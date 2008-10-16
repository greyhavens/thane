/*
 *  Expected Error
 * 
 *	Interfaces cannot be instantiated with the new operator
 */


package Errors {
	interface I { }
	var x = new I;
	var y = I;
}