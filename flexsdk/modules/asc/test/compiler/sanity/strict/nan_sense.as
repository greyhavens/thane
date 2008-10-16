// cn: this tests platform specific problems with NaN handling.

package {
    // first make sure any comparision involving NaN is false.
	trace( NaN < 0);  
	trace( NaN > 0);
	trace( NaN == qq );
	trace( NaN != qq );
	
	// now make sure NaN converts to int as 0, not MAX_INT
	var qq:Number = NaN;
	trace(qq);
	var ee:int = qq;
	trace(ee);

	// finally, make sure apis that take a Number index as argument handle NaN/undefined correctly.
	var obj2 = new Object();
	obj2.slice = Array.prototype.slice;
	obj2.length = 10;
	obj2[0] = 0;
	obj2[1] = 1;
	obj2[2] = 2;
	obj2[3] = 3;
	obj2[4] = 4;
	obj2[5] = 5;
	obj2[6] = 6;
	obj2[7] = 7;
	obj2[8] = 8;
	obj2[9] = 9;
	trace(obj2.slice(undefined,5)); // undefined coerces to Number as NaN, NaN should be clamped to 0
	trace(obj2.slice(1,undefined)); // undefined coerces to Number as NaN, NaN should be clamped to 0
	
	

	trace(obj2[qq]);
}

