var n:Number = 1.1;
var n2:Number = NaN;
var n3:Number = 0;
var i:int = -1;
var u:uint = 1;
var s:String = "string";
var s2:String = "22";
var s3:String = "false";
var s4:String = "Infinity";
var b:Boolean = false;
var b2:Boolean = true;
var d:Date = new Date("4/1/2006");
var a:Array = [1,2,3];
var nu:* = null
var un:* = undefined;

var nV:Number;
var iV:int;
var uV:uint;
var sV:String;
var bV:Boolean;
var dV:Date;
var aV:Array;
class aClass { };
class bClass { };
// cn: remove the instance of aClass from the valArray.  The error printing code will tack on the ptr value of
//     this instance to any error string
var valArray = [n,n2,n3,i,u,s,s2,s3,s4,b,b2,d,a,new aClass(),nu,un];
var typeNames = ["Number", "Number", "Number", "int", "uint", "String", "String", "String", "String", "Boolean", "Boolean", "Date", "Array", "Class instance", "null", "undefined"];
var c;
var v;

function formatStringRep(v):String
{
   if (v === null)
		return "null";
   else if (v === undefined)
		return "undefined"
   else if (v instanceof String)
   {
		var truncIndex = v.indexOf("GMT");
		if (truncIndex != -1)
			return "the timezone dependant result of Date.prototype.toString"

		return "'" + v + "'";
	}
   else if (v instanceof Array)
   {
        var nuString = "[ ";
		for(var x=0; x<v.length; x++)
		{
		   nuString += formatStringRep(v[x]);
		   if (x != v.length-1)
				nuString += ", "
		}
		return nuString + " ]"
	}
   else if (v instanceof Date)
		return "{Date value}"
   else if (v is int)
   {
		var dateValueInSeconds = d.valueOf();
		var dateValueInSeconds_int = int(dateValueInSeconds);
		if (dateValueInSeconds_int == v)
			return "{large int result of int(Date.prototype.valueOf())}";
		return v;
   }
   else if (v is uint)
   {
		var dateValueInSeconds = d.valueOf();
		var dateValueInSeconds_uint = uint(dateValueInSeconds);
		if (dateValueInSeconds_uint == v)
			return "{large uint result of uint(Date.prototype.valueOf())}";
		return v;
   }
   else if (v is Number)
   {
		var dateValueInSeconds = d.valueOf();
		if (dateValueInSeconds == v)
			return "{large number result of Date.prototype.valueOf()}";
		return v;
   }

   else
		return v.toString();
} 

print("To String:");
for(c = 0; c < valArray.length; c++)
{
	v = valArray[c];
	print("   from: " + typeNames[c]);
	print("      String( " + formatStringRep(v) + " ) -> " + formatStringRep(String(v)));
	print("      " + formatStringRep(v) + " as String -> " + formatStringRep(v as String));
	try
	{
		sV = v;
		print("      var x:String = " + formatStringRep(v) + "; x -> " + formatStringRep(sV));
	}
	catch(e9)
	{
		print("      var x:String = " + formatStringRep(v) + "; Error! ");
	}
}
print("------------------------------------------------------------------");
print("To Boolean:");
for(c = 0; c < valArray.length; c++)
{
	v = valArray[c];
	print("   from: " + typeNames[c]);
	print("      Boolean( " + formatStringRep(v) + " ) -> " + formatStringRep(Boolean(v)));
	print("      " + formatStringRep(v) + " as Boolean -> " + formatStringRep(v as Boolean));
	try
	{
		bV = v;
		print("      var x:Boolean = " + formatStringRep(v) + "; x -> " + formatStringRep(bV));
	}
	catch(e8)
	{
		print("      var x:Boolean = " + formatStringRep(v) + ";  Error! ");
	}
	
}
print("------------------------------------------------------------------");
print("To Number:");
for(c = 0; c < valArray.length; c++)
{
	v = valArray[c];
	print("   from: " + typeNames[c]);
	print("      Number( " + formatStringRep(v) + " ) -> " + formatStringRep(Number(v)));
	print("      " + formatStringRep(v) + " as Number -> " + formatStringRep(v as Number));
	try
	{
		nV = v;
		print("      var x:Number = " + formatStringRep(v) + "; x -> " + formatStringRep(nV));
	}
	catch(e7)
	{
		print("      var x:Number = " + formatStringRep(v) + "; Error! ");
	}


}
print("------------------------------------------------------------------");
print("To int:");
for(c = 0; c < valArray.length; c++)
{
	v = valArray[c];
	print("   from: " + typeNames[c]);
	print("      int( " + formatStringRep(v) + " ) -> " + formatStringRep(int(v)));
	print("      " + formatStringRep(v) + " as int -> " + formatStringRep(v as int));
	try
	{
		iV = v;
		print("      var x:int = " + formatStringRep(v) + "; x -> " + formatStringRep(iV));
	}
	catch(e6)
	{
		print("      var x:int = " + formatStringRep(v) + "; Error! ");
	}

}
print("------------------------------------------------------------------");
print("To uint:");
for(c = 0; c < valArray.length; c++)
{
	v = valArray[c];
	print("   from: " + typeNames[c]);
	print("      uint( " + formatStringRep(v) + " ) -> " + formatStringRep(uint(v)));
	print("      " + formatStringRep(v) + " as uint -> " + formatStringRep(v as uint));
	try
	{
		uV = v;
		print("      var x:uint = " + formatStringRep(v) + "; x -> " + formatStringRep(uV));
	}
	catch(e5)
	{
		print("      var x:uint = " + formatStringRep(v) + " Error! ");
	}
}

print("------------------------------------------------------------------");
print("to Array:");
for(c = 0; c < valArray.length; c++)
{
	v = valArray[c];
	print("   from: " + typeNames[c]);
	try
	{
		print("      Array( " + formatStringRep(v) + " ) -> " + formatStringRep(Array(v)));
	}
	catch(e4)
	{
		print("      Array( " + formatStringRep(v) + " ) ->  Error! ");
	}
	
	print("      " + formatStringRep(v) + " as Array = " + formatStringRep(v as Array));
	
	try {
		aV = v;
		print("      var x:Array = " + formatStringRep(v) + "; x -> " + formatStringRep(aV));
	}
	catch(e3)
	{
		print("      var x:Array = " + formatStringRep(v) + ";  Error! ");
	}

}

print("------------------------------------------------------------------");
print("To Date:");
for(c = 0; c < valArray.length; c++)
{
	v = valArray[c];
	print("   from: " + typeNames[c]);
	
	try
	{
		print("      Date( " + formatStringRep(v) + " ) -> " + formatStringRep(Date(v)));
	}
	catch(e2)
	{
		print("      Date( " + formatStringRep(v) + " ) ->  Error! ");
	}
	
	
	print("      " + formatStringRep(v) + " as Date -> " + formatStringRep(v as Date));
	
	try {
		dV = v;
		print("      var x:Date = " + formatStringRep(v) + "; x -> " + formatStringRep(dV));
	}
	catch(e1)
	{
		print("      var x:Date = " + formatStringRep(v) + ";  Error! ");
	}
}

print("To Unreleated type bClass:");
var bCV:bClass;

for(c = 0; c < valArray.length; c++)
{
	v = valArray[c];
	print("   from: " + typeNames[c]);
	try
	{
		print("      bClass( " + formatStringRep(v) + " ) -> " + formatStringRep(bClass(v)));
	}
	catch(e10)
	{
		print("      bClass( " + formatStringRep(v) + " ) ->  Error! ");
	}
	print("      " + formatStringRep(v) + " as bClass -> " + formatStringRep(v as bClass));
	try
	{
		bCV = v;
		print("      var x:bClass = " + formatStringRep(v) + "; x -> " + formatStringRep(bCV));
	}
	catch(e9)
	{
		print("      var x:bClass = " + formatStringRep(v) + ";  Error! ");
	}
}




