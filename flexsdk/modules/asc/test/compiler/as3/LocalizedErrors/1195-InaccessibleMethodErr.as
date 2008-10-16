/**
 *
 *    Author:     jliao
 *    Error 1195: Attempted access of inaccessible method _ through a reference with static type Class".
 *
 */
    
package myPackage{
    public class myClass {
        private static function myFunction():void {
            var i:uint = 1;
            i++;
        }
    }
}

import myPackage.*;

myClass.myFunction();