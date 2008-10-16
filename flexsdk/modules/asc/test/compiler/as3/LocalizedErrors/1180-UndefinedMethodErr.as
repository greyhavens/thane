/**
 *
 *    Author:     jliao
 *    Error 1180: Interface definitions must not be nested within class or other interface definitions.
 *
 */
    
package myPackage{
    public class myClass {
        private function myFunction() {
        }
    }
}

import myPackage.*;

var c1 = new MyClass();
c1.myFunction1();