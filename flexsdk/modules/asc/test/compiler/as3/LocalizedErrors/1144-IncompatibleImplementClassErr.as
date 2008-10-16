/*

  Error message: Interface method '_' in namespace '_' implemented with incompatible signature in class '_'

*/

package IncompatibleImplementPackage{
    interface MyInterface{
        function myfunction():String;
    }
    public class IncompatibleImplementClassErr implements MyInterface{
        public function myfunction(a):String{}
    }
}