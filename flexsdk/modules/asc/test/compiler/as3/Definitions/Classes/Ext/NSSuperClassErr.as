package NameSpacePackage{
     namespace NS;
     NS class NSClass{
     }
}
package Errors{
     import NameSpacePackage.*
     public class PublicClass extends NSClass{}
     class DefaultClass extends NSClass{}
     final class FinalClass extends NSClass{}
     internal class InternalClass extends NSClass{}
     dynamic class DynamicClass extends NSClass{}
     explicit class ExplicitClass extends NSClass{}
}