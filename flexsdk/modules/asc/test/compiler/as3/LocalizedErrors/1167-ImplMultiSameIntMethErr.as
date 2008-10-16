/*

  Error message: Class '_' implements interface '_' multiple times

*/

package ImplMultiSameIntMethErrPackage{
    interface I{
        function MyInt():int;
    }
    public class ImplMultiSameIntMethErr implements I,I{
        public function MyInt():int{}
        
    }
}