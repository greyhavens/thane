/*
 *  Expected Error
 *  Interface method hello in namespace public  not implemented by class Errors$internal:MyClass
 */

package Errors {
	interface IInterface {
		public function hello()
	}
	
	class MyClass implements IInterface {
	
	}

}
	