function prime(n : Number) {
    var m : Number = n
    var limit : Number = m/2
    for (var i:Number=2;i<=limit;++i) {
        if(!(m%i)) {
            return false;
        }
    }
    return true;
}
for(var j = 1000;j;--j) 
{
	if( prime(j) ) 
	{
        print(j);
    }
}
true
