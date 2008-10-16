// mtilburg
// cannot override a private method
package Errors{
	dynamic class DefaultClass {
		private function foo(){}
	}

	dynamic class DynExtDynamicOverRidePrivateErr extends DefaultClass {
		override private function foo(){}
	}
}

