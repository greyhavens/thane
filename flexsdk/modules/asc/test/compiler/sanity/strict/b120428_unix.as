var x1 = 
<>
<alpha attr1="value1">
<bravo attr2="value2">
one
<charlie>two</charlie>
</bravo>
</alpha>
<alpha attr1="value3">
<bravo attr2="value4">
three
<charlie>four</charlie>
</bravo>
</alpha>
</>;

trace("x1.@attr1: " + x1.@attr1);

trace("x1.bravo.@attr2: " + x1.bravo.@attr2);

