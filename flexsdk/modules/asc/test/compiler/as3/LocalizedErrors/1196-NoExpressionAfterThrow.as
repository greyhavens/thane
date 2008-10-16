/**
 *
 *    Author:     jliao
 *    Error 1196: Expecting an expression after throw.
 *
 */
    
package myPackage{
    public class myClass {
        private static function myFunction(myArg):void {
            if (myArg == NULL) {
                throw
            }
        }
    }
}