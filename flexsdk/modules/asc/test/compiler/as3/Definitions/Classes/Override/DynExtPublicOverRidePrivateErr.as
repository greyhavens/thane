// mtilburg
// cannot override a private method
package Errors{

	public class DynExtPublicOverRidePrivateErr {
		private function foo(){}
	}

	dynamic class DefaultClass extends DynExtPublicOverRidePrivateErr {
		override private function foo(){}
	}

}

