 
 //cn: this tests that we correctly hoist var declarations that are held within LabeledStatementNodes (see bug 181292)
 
 function foo():String { return "hello"; }
 
 function test(x):void
{
	someLabel: var str:String = foo();
	print(str);
	
	try { // force need for activation object
		str += str;
	} catch(e) { }
	
	if (x)
	{
		someOtherLabel: var str2:String = foo();  // in bug 181292, this var is never hoisted and the initializer is never run
		print(str2);							  // prints null pre fix.
	}
	while(x)
	{
		someOtherLabel3: var str3:String = foo();
		x--;
	}
	print(str3);
	
	switch(str)
	{
		case "hellohello": 
			someOtherLAbel4: for(var q=3; q>x; q--) // this unreferenced label was causing a compiler crash
			{
				print(q);
			}
			break;
	}
	print(q);

}
test(1); // prints undefined. Remove "someLabel" and it prints "hello".  