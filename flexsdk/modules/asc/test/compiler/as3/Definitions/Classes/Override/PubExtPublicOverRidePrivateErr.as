// mtilburg
// cannot override a private method
package Errors{

	public class PubExtPublicOverRidePrivateErr {
		private function foo(){}
	}

	public class DefaultClass extends PubExtPublicOverRidePrivateErr {
		override private function foo(){}
	}

}

