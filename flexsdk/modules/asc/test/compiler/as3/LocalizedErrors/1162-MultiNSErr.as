/*
    Error Message: Only one namespace attribute may be used per definition

*/

package MultiNSErrPackage{
    public class MultiNSErr{
        namespace N1;
        namespace N2;
        N1 N2 var myvar:int = 0;
   }
}