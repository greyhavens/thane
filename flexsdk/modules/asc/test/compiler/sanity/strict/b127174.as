interface IBaseInterface
{
function method1():void;
}


interface ITestInterface extends IBaseInterface
{
function method2():void;
}

class CoerceErrorForInterfaces implements ITestInterface
{
public function CoerceErrorForInterfaces()
{
super();
}

public function method1():void {
}

public function method2():void {
}
}


class InterfaceCorceErrorTest {

public function causeCoerceError(intf:IBaseInterface):void
{
}
}

var t:InterfaceCorceErrorTest = new InterfaceCorceErrorTest();
var k:ITestInterface = new CoerceErrorForInterfaces();
t.causeCoerceError(k);
print("success")
