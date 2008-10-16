// mtilburg
// cannot override a private method
package Errors{

	public class ExtPublicOverRidePrivateErr {
		private function foo(){}
	}

	class DefaultClass extends ExtPublicOverRidePrivateErr {
		override private function foo(){}
	}

}

