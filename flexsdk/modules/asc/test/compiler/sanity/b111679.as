interface I {
function get foo():Boolean;
}

class MyIImplemenation implements I {

public function get foo():Boolean {
return(false);
}
}

var it:I = new MyIImplemenation();

