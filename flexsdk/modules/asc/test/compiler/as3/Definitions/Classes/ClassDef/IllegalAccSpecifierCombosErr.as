package Errors {
	// whilst internal public and explicit are all legal on
	// their own, any combination of them should be a syntax
	// error:
	public internal class pi { }
	public internal explicit class pie { }
	public explicit class pe { }
	public explicit internal class pei { }
	internal public class ip { }
	internal public explicit class ipe { }
	internal explicit class ie { }
	internal explicit public class iep { }
	explicit internal class ei { }
	explicit internal public class eip { }
	explicit public class ep { }
	explicit public internal class epi { }
}
