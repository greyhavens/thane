/*

Error Message: Attempted access of inaccessible property '_' through a reference with static type '_'"

*/

package InAccPropRefErrPackage{

    public class InAccPropRefErr{
        private static var myvar:Number=10;
    }
    InAccPropRefErr.myvar
}