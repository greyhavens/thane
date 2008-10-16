package Errors {
	// whilst internal public and explicit are all legal on
	// their own, any combination of them should be a syntax
	// error:

	// also check that illegal combos on properties are flagged:
	class test {
		public private var pp1;
		public protected var pp2;
		public internal var pi3;
		private protected var pp4;
		private internal var pi5;
		protected internal var pp6;
		static public private var spp1;
		static public protected var spp2;
		static public internal var spi3;
		static private protected var spp4;
		static private internal var spi5;
		static protected internal var spp6;
		public private function fpp1() { }
		public protected function fpp2() { }
		public internal function fpi3() { }
		private protected function fpp4() { }
		private internal function fpi5() { }
		protected internal function fpp6() { }
		static public private function fspp1() { }
		static public protected function fspp2() { }
		static public internal function fspi3() { }
		static private protected function fspp4() { }
		static private internal function fspi5() { }
		static protected internal function fspp6() { }
	}
}
