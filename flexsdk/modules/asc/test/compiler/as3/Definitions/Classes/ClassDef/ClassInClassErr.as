package Errors {
	class outer1 {
		static class inner1 { }
		static var innerObj = new inner1()
	}
	class outer2 {
		class inner2 { }
		static var innerObj = new inner2()
	}
	class outer3 {
		private static class inner1 { }
		static var innerObj = new inner1()
	}
	class outer4 {
		private class inner2 { }
		static var innerObj = new inner2()
	}
	class outer5 {
		public static class inner1 { }
		static var innerObj = new inner1()
	}
	class outer6 {
		public class inner2 { }
		static var innerObj = new inner2()
	}
	class outer7 {
		dynamic static class inner1 { }
		static var innerObj = new inner1()
	}
	class outer8 {
		dynamic class inner2 { }
		static var innerObj = new inner2()
	}
	class outer9 {
		final static class inner1 { }
		static var innerObj = new inner1()
	}
	class outer10 {
		final class inner2 { }
		static var innerObj = new inner2()
	}
	class outer11 {
		explicit static class inner1 { }
		static var innerObj = new inner1()
	}
	class outer12 {
		explicit class inner2 { }
		static var innerObj = new inner2()
	}
	class outer13 {
		internal static class inner1 { }
		static var innerObj = new inner1()
	}
	class outer14 {
		internal class inner2 { }
		static var innerObj = new inner2()
	}
}
