package DupDefInSameNamespaceErrPackage{

    public class DupDefInSameNamespaceErr{
        namespace N1;
 
        N1 var samevar:int = 100;
        
    }

    class samevarclass extends DupDefInSameNamespaceErr{

        use namespace N1;
        N1 var samevar:String = 'mystring';
    }
}