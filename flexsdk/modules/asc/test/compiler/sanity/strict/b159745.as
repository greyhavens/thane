var x:XML = <abc/>
x.@text = "hello"; // produces compiler error
trace(x.toXMLString()); // traces 'hello'




