/*
 *  Expected Error
 * 
 *	Nested classes/interfaces aren't supported
 *	Bug 145571
 */



package Errors{

	class testA {
		static interface B {}
	}

}