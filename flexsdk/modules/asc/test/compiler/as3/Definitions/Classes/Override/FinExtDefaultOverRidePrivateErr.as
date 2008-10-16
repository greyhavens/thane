// mtilburg
// cannot override a private method
package Errors{
	class DefaultClass {
		private function foo(){}
	}

	final class FinExtDefaultOverRidePrivateErr extends DefaultClass {
		override private function foo(){}
	}
}

