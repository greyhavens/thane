x1 =
<alpha>
<bravo attr1="value1" ns:attr1="value3" xmlns:ns="http://someuri">
<charlie attr1="value2" ns:attr1="value4"/>
</bravo>
</alpha>

n = new Namespace("http://someuri");

q = new QName(n, "attr1");
print(x1..@[q]);

