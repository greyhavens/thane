package {

public class main  {

    public function main() {

        myEmployee = <employee><name><first/><last>Wong</last></name></employee>;

 

        trace(myEmployee.toXMLString());

        trace("first: " + myEmployee.name.first);

        trace("last: " + myEmployee.name.last);

    }

 

    public var myEmployee:XML;

}


var x:main = new main();

}

