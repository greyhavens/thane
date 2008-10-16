/*

More than one switch default

*/



package TwoDefaultSwitchErrPackage{

    var switchExpression:int = 2;

    switch (switchExpression) {
        case 0:
            var a;
            break;
        case 1:
            var b;
            break;
        case 2:
            var c;
            break;
        default:
            var d;
        default:
            var e; //error
    }
}
