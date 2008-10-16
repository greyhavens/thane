package Errors {
 
 class PrivOverLoadFuncErr {

    private function thisFunction () {}
    private function thisFunction (x) {}
    private function thisFunction (x,y) {}
 }
}
