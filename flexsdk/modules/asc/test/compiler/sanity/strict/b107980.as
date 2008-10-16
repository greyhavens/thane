var x = <x><![CDATA[1 2 3]]></x>
var y = <y><!-- A B C -->A B C</y>
var z = <z><?A B C?>A B C</z>
print(x.toXMLString())
print(y.toXMLString())
print(z.toXMLString())