package OverrideIT {

	class A{
		function A()
		{
			// constructor
		}

		final function get val():Number
		{
			return 10;
		}
	}

	class B extends A {
		function B ()
		{
			super();
		}

		override final function get val():Number
		{
			return 10;
		}

	}
}
