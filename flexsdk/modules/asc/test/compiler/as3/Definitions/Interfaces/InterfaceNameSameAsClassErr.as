/*
 * Expected Error
 *
 *  Duplicate interface definition: A
 */
package Errors {
	class A {}

	interface A {
		function f()
	}
			
}