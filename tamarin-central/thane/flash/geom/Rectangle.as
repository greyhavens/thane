//
// $Id: $

package flash.geom {

public class Rectangle
{
    public var x :Number;
    public var y :Number;
    public var width :Number;
    public var height :Number;

    public function Rectangle (x :Number = 0, y :Number = 0, width :Number = 0, height :Number = 0)
    {
        this.x = x;
        this.y = y;
        this.width = Math.max(width, 0);
        this.height = Math.max(height, 0);
    }

    public function get bottom () :Number
    {
        return y + height;
    }
    public function set bottom (value :Number) :void
    {
        // TODO: Verify this is what should happen here
        height = Math.max(y, value) - y;
    }

    public function get bottomRight () :Point
    {
        return new Point(right, bottom);
    }
    public function set bottomRight (value :Point) :void
    {
        bottom = value.y;
        right = value.x;
    }

    public function get left () :Number
    {
        return x;
    }
    public function set left (value :Number) :void
    {
        var r :Number = x + width;
        x = Math.min(right, value);
        width = r - x;
    }

    public function get right () :Number
    {
        return x + width;
    }
    public function set right (value :Number) :void
    {
        width = Math.max(0, value - x);
    }

    public function get size () :Point
    {
        return new Point(width, height);
    }
    public function set size (value :Point) :void
    {
        width = value.x;
        height = value.y;
    }

    public function get top () :Number
    {
        return y;
    }
    public function set top (value :Number) :void
    {
        var b :Number = y + height;
        y = Math.min(bottom, value);
        height = b - y;
    }

    public function get topLeft () :Point
    {
        return new Point(x, y);
    }
    public function set topLeft (value :Point) :void 
    {
        top = value.x;
        left = value.y;
    }

    public function contains (x :Number, y :Number) :Boolean
    {
        return x >= left && x <= right && y >= top && y <= bottom;
    }

    public function containsPoint (p :Point) :Boolean
    {
        return contains(p.x, p.y);
    }

    public function containsRect(r :Rectangle) :Boolean
    {
        return r.left >= left && r.right <= right && r.top >= top && r.bottom <= bottom;
    }

    public function inflate (dX :Number, dY :Number) :void
    {
        x -= dX;
        width += 2*dX;
        y -= dY;
        height += 2*dY;
    }

    public function inflatePoint (p :Point) :void
    {
        inflate(p.x, p.y);
    }

    public function intersection (rect :Rectangle) :Rectangle
    {
        if (!intersects(rect)) {
            return new Rectangle(0, 0, 0, 0);
        }

        var l :Number = Math.max(left, rect.left);
        var r :Number = Math.min(right, rect.right);
        var t :Number = Math.max(top, rect.top);
        var b :Number = Math.min(bottom, rect.bottom);

        return new Rectangle(l, t, r-l, b-t);
    }

    public function intersects (r :Rectangle) :Boolean
    {
        return r.top <= bottom && r.bottom >= top && r.left <= right && r.right >= left;
    }

    public function isEmpty () :Boolean
    {
        return width <= 0 && height <= 0;
    }

    public function offset (dX :Number, dY :Number) :void
    {
        x += dX;
        y += dY;
    }

    public function offsetPoint (p :Point) :void
    {
        offset(p.x, p.y);
    }

    public function setEmpty () :void
    {
        x = y = width = height = 0;
    }

    public function union (rect :Rectangle) :Rectangle
    {
        var l :Number = Math.min(left, rect.left);
        var r :Number = Math.max(right, rect.right);
        var t :Number = Math.min(top, rect.top);
        var b :Number = Math.max(bottom, rect.bottom);

        return new Rectangle(l, t, r-l, b-t);
    }

    public function clone () :Rectangle
    {
        return new Rectangle(x, y, width, height);
    }

    public function equals (r :Rectangle) :Boolean
    {
        return r.x == x && r.y == y && r.width == width && r.height == height;
    }

    public function toString () :String
    {
        return "(x=" + x + ", y=" + y + ", width=" + width + ", height=" + height + ")";
    }

}
}
