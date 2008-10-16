class Vector
{
    function Vector()
    {
       this.x = 0;
       this.y = 0;
    }

    function copyVector( v:Vector ):void
    {
        this.x = v.x;
        this.y = v.y;
    }

    function setMembers( x:Number, y:Number ):void
    {
        this.x = x;
        this.y = y;
    }

    function addVector( v:Vector ):void
    {
        this.x += v.x;
        this.y += v.y;
    }


    function subVector( v:Vector ):void
    {
        this.x -= v.x;
        this.y -= v.y;
    }

    function divScalar( i:Number ):void
    {
        this.x /= i;
        this.y /= i;
    }

    function mulScalar( i:Number ):void
    {
        this.x *= i;
        this.y *= i;
    }

    function magnitude():Number
    {
        return Math.sqrt( this.x*this.x + this.y*this.y );
    }

    function magnitude2():Number
    {
        return this.x*this.x + this.y*this.y;
    }

    var x:Number;
    var y:Number;
}

var v : Vector = new Vector
print(v.x)
print(v.y)



