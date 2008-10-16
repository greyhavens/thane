// mtilburg
// cannot override a private method
package Errors{
	class DefaultClass {
		private function foo(){}
	}

	dynamic class DynExtDefaultOverRidePrivateErr extends DefaultClass {
		override private function foo(){}
	}
}

