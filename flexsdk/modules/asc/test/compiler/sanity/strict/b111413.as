package {
	public interface Test {
	function add():void;
	}

	public interface I extends Test {
		function something():void;
	}

	trace("this is a test...");
}

