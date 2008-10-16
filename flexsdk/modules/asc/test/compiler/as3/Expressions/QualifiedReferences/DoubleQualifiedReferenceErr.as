package Errors {

	public namespace ns1;
	public namespace ns2;
	public class A
	{
	    ns1 var x:Number = 5;
	}
}

import Errors.*;
var a:A = new A();

// this should throw a compiler error
//print(a.ns2::ns1::x);
a.ns2::ns1::x;
