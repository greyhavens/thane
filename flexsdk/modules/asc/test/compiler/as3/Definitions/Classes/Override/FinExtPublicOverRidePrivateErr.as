// mtilburg
// cannot override a private method
package Errors{

	public class FinExtPublicOverRidePrivateErr {
		private function foo(){}
	}

	final class DefaultClass extends FinExtPublicOverRidePrivateErr {
		override private function foo(){}
	}

}

