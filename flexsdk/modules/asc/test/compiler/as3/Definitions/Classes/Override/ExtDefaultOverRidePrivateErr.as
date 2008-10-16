// mtilburg
// cannot override a private method
package Errors{
	class DefaultClass {
		private function foo(){}
	}

	class ExtDefaultOverRidePrivateErr extends DefaultClass {
		override private function foo(){}
	}
}

