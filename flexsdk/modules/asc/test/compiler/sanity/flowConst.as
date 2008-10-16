// this tests that flow analysis is correctly finding all possible SetExpressionNodes of a variable which reach any given
//  reference.  This logic is used in -strict to find const violations.  

package {
   function f1(y) {
		const x = 55;
		var q;  // var is hoisted to top of func, emptyStatementNode replaces it since there is no initializer
				//  make sure it doesn't clear the rch_bits for nodes which follow it.
		x = 22; // should give a -strict mode error here
		print(x);
	}
	
	function f2(y) {
		const x = 55;
		while (x)
		{
			var q;  // var is hoisted to top of func, emptyStatementNode replaces it since there is no initializer
				//  make sure it doesn't clear the rch_bits for nodes which follow it.
			x = 33; // should give a -strict mode error here
			print(x);
		}
	}
	
	function f3(y) {
		const x = 55;
		const x2 = 11;
		if (x >= y)
		{
			if (x > y)
			{
				y = x;
			}
			else 
			{
				var q2;  // var is hoisted to top of func, emptyStatementNode replaces it since there is no initializer
						//  make sure it doesn't clear the rch_bits for nodes which follow it.
				x = 44; // should give a -strict mode error here
			}
		}
		else if (x <= y)
		{
			if (x < y)
			{
				y = x;
			}
			else
			{
				var q3;  // var is hoisted to top of func, emptyStatementNode replaces it since there is no initializer
						//  make sure it doesn't clear the rch_bits for nodes which follow it.
				x = 55; // should give a -strict mode error here
			}
		}
		
		x2 = 66;
	}
	
	function f4(y) {
		const x = 55;
		try {
			var q;  // var is hoisted to top of func, emptyStatementNode replaces it since there is no initializer
					//  make sure it doesn't clear the rch_bits for nodes which follow it.
			x = 77; // should give a -strict mode error here
			print(x);
		}
		catch(e) {
			var q2;
			x = 88;// should give a -strict mode error here
		}
		finally {
			var q3;
			x = 99;// should give a -strict mode error here
		}
	}
}