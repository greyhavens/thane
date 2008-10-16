// mtilburg
// cannot override a private method
package Errors{
	dynamic class DefaultClass {
		private function foo(){}
	}

	class ExtDynamicOverRidePrivateErr extends DefaultClass {
		override private function foo(){}
	}
}

