/***************************************************************************************************************************************************************************************
Warning coach mode arguments.caller is removed.  You can achieve the same functionality by passing arguments.callee from the "caller" function as an argument to the "callee" function.***********************************************************************************
*******************************************************************************************/




package warnings{
    import flash.util.trace;
    CallingFunction();

    public function CallingFunction():void{
        MyCalledFunction();
    }
    public function MyCalledFunction():void{
        if (arguments.caller!=CallingFunction){
            trace("Error!");
        }else{
            trace("No error!");
        }
    }

}
     