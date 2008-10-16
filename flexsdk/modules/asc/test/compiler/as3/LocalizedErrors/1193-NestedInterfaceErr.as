/**
 *
 *    Author:     jliao
 *    Error 1193: Interface definitions must not be nested within class or other interface definitions.
 *
 */
    
package myPackage{
    public class myClass {
        public interface myInterface {
            function myFunction();
        }
    }
}