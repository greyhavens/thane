x =    <order xmlns:x="x">
	<item id="1" xmlns:x="x2">
		<menuName xmlns:x="x">burger</menuName>
		<price>3.95</price>
	</item>
   </order>;
   
print(x);

var example:XML = <example>
<two>2</two>
</example>;

example = example.insertChildBefore( example.two, <one>1</one> );
print( example );

var example:XML = <example>
<two>2</two>
</example>;

example = example.insertChildAfter( example.two, <one>1</one> );
print( example );

// invalid node types should return undefined
var x:XML = new XML("text node");
print (x.insertChildAfter (example, <one>1</one>));
print (x.insertChildBefore (example, <one>1</one>));

// A 1st parameter without a match should return undefined
print (example.insertChildAfter (x, <one>1</one>));
print (example.insertChildBefore (x, <one>1</one>));
