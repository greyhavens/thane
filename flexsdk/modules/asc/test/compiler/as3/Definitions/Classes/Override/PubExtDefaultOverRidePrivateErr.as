// mtilburg
// cannot override a private method
package Errors{
	class DefaultClass {
		private function foo(){}
	}

	public class PubExtDefaultOverRidePrivateErr extends DefaultClass {
		override private function foo(){}
	}
}

