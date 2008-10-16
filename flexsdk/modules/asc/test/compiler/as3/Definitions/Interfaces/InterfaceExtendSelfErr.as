/*
 * Expected Error
 *
 *  Circular type reference detected in Errors$internal:A
 */
package Errors {
	interface A extends A{
		function f()
	}
			
}