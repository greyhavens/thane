/*
 * Regression for bug #112582.
 */
class TestObject_112582
{
	public function doIt():*
	{
		throw new RangeError("testing");
		return null;
	}
	public function toString():String
	{
		return "TestObject_112582";
	}
	public var name:String;
}

class TestExceptions_112582
{
	public static function runIt():void
	{
		var a = new TestExceptions_112582();
		try
		{
			a.runTest();
		}
		catch(e)
		{
			trace("here we are in the catch block");
			throw e;
		}
		finally
		{
			trace("Test #7: PASSED");
		}
	}

	public function runTest():void
	{
		var b = new TestObject_112582();
		b.name = "test";
		trace("Test #1:", (b.toString() == "TestObject_112582" ? "PASSED" : "FAILED")); 
		try
		{
			b = new TestObject_112582();
			b.name = "gary";
			b.doIt();
		}
		catch(e)
		{
			trace("Test #2:", (b.toString() == "TestObject_112582" ? "PASSED" : "FAILED")); 			
			trace("Test #3:", e instanceof RangeError ? "PASSED" : "FAILED");
		}
		trace("Test #4: PASSED");
		trace("Test #5:", (b.toString() == "TestObject_112582" ? "PASSED" : "FAILED"));
		trace("Test #6:", (b.name == "gary" ? "PASSED" : "FAILED"));
	}
}

trace("Regression #112582:");
try
{
	TestExceptions_112582.runIt();
}
catch(e)
{
	trace("Failure");
}

/*
 * Regression for bug #120410
 */
class Event_120410 {}

class catchBug_120410 
{
	public function catchBug_120410()
	{
		var myEvent_120410:Event_120410;
		
		try
		{
			trace("About to throw TypeError...");
			trace(myEvent_120410.type);
		}
		catch (typeError:TypeError)
		{
			trace("PASSED");
		}
	}

	public function method()
	{
		try
		{
			trace("No exception thrown here");
		}
		catch (typeError)
		{
			trace("FAILED");
		}
	}
}

trace("");
trace("Regression #120410:");
var instance_120410 = new catchBug_120410();

/*
 * Variant of #120410 that uses a method instead of constructor.
 * When #120410 was fixed, the constructor had a stack balance problem because there
 * wasn't a Pop after iinit was invoked.
 */
instance_120410.method();

/*
 * Regression #122367:
 * Once we started adding edges from every throwing instruction in the try
 * block, we started to run into problems with temps.  Temps may have
 * various types over the course of a try block, so incompatible type merging
 * may occur in the catch block state.  The fix was to detect OP_killreg to
 * kill the temp regs in the catch block states.  This is some code from
 * the Flex framework that failed to verify until we made the fix.
 */
class DataProvider
{
	//--------------------------------------------------------------------------
	//
	//  Constructor
	//
	//--------------------------------------------------------------------------

	/**
	 *  @private
	 *  Constructor.
	 */
	var data:Array;
	
	public function DataProvider(dataArg:Array = null)
	{
		trace("PASSED: No VerifyError on #122367 killreg case");
		
		data = dataArg ? dataArg : [];
		if (data.length && typeof(data[0]) == "object")
		{
			var l:int = data.length;
			for (var i:int = 0; i < l; i++)
			{
				try 
				{
					if (data[i].uid == null)
					{
						var t:int = uid++;
						data[i].uid = t.toString();
					}
				}
				catch (e:Error) {}
			}
		}
	}
}

new DataProvider();

/*
 * Edge case: Empty try block.
 */
try
{
}
catch (e)
{
	trace("FAILED: You should never see me.");
}

/*
 * Try and finally without a catch block.  finally is supported by inserting
 * dummy catch blocks.  The compiler, at one point, was not inserting
 * a dummy catch block if there were no other catch blocks.
 */
try
{
	trace("Testing finally with no catch clause");
}
finally
{
	trace("PASSED: finally with no catch clause executed");
}

/*
 * Regression for bug #122524.  The following code should not cause
 * a VerifyError.
 */
TryWith( new Object() );

function TryWith( object ) {
	try {
		with ( object ) {
			result = check();
		}
	} catch ( e ) {
		result = e;
	}
}

/*
 * Regression for bug #120639
 */
class SubError_120639 extends Error
{
	public function SubError_120639(msg:String="")
	{
		super(msg);
	}
}

class bug_120639
{
	public function bug_120639()
	{
		try
		{
			throw new Error("RegError");
		}
		catch (sub : SubError_120639)
		{
			trace('FAILED, caught regular Error in suberror handler');
		}
		
		trace('FAILED, if you see me the thrown error was swallowed silently!');
	}
}

trace("");
trace("Regression #120639:");
new bug_120639();

