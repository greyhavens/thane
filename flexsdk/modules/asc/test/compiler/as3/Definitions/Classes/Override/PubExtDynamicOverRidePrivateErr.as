// mtilburg
// cannot override a private method
package Errors{
	dynamic class DefaultClass {
		private function foo(){}
	}

	public class PubExtDynamicOverRidePrivateErr extends DefaultClass {
		override private function foo(){}
	}
}

