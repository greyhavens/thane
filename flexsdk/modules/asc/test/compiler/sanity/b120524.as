var xml1 = "<d><colour c='1'>pink</colour><colour c='2'>purple</colour><colour c='3'>orange</colour></d>";
var placeHolder = "c";

x1 = new XML(xml1); 
print(x1.colour[0].@[placeHolder]);

x1.colour[0].@[placeHolder] = "5"; 
print(x1.colour[0].@[placeHolder]);

