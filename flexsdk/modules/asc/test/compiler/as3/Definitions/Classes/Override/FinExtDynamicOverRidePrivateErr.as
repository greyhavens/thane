// mtilburg
// cannot override a private method
package Errors{
	dynamic class DefaultClass {
		private function foo(){}
	}

	final class FinExtDynamicOverRidePrivateErr extends DefaultClass {
		override private function foo(){}
	}
}

