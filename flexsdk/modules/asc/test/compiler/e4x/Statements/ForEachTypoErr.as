var i, s="";

var xmlDoc = "<L><z><A>1</A></z><z><A>2</A></z><z><A>3</A></z></L>";
var x1 = new XML(xmlDoc);

for ech(i in x1.z) {
      s += i.A;
}
