//
// $Id: $

package flash.geom {

public class Matrix
{ 
    /**
     * One of the values that affect the positioning of pixels along the x axis when scaling
     * or rotating an image. 
     */
    public var a :Number;

    /**
     * One of the values that affect the positioning of pixels along the y axis when scaling
     * or rotating an image. 
     */
    public var b :Number;

    /**
     * One of the values that affect the positioning of pixels along the x axis when scaling
     * or rotating an image. 
     */
    public var c :Number;

    /**
     * One of the values that affect the positioning of pixels along the y axis when scaling
     * or rotating an image. 
     */
    public var d :Number;

    /**
     * The distance by which to translate each point along the x axis.
     */
    public var tx :Number;

    /**
     * The distance by which to translate each point along the x axis.
     */
    public var ty :Number;

    /**
     * Creates a new Matrix object with the specified parameters.
     */
    public function Matrix (a :Number = 1, b :Number = 0, c :Number = 0, d :Number = 1,
                            tx :Number = 0, ty :Number = 0)
    {
        configure(a, b, c, d, tx, ty);
    }

    /**
     * Concatenates a matrix with the current matrix, effectively combining the geometric
     * effects of the two. In mathematical terms, concatenating two matrixes is the same as
     * combining them using matrix multiplication.
     */
    public function concat (m :Matrix) :void
    {
        configure(a*m.a + b*m.c, a*m.b + b*m.d,
                  c*m.a + d*m.c, c*m.b + d*m.d,
                  a*m.tx + b*m.ty, c*m.tx + d*m.ty);
    }

    /**
     * Includes parameters for scaling, rotation, and translation. When applied to a matrix
     * it sets the matrix's values based on those parameters. 
     */
    public function createBox (scaleX :Number, scaleY :Number, rotation :Number = 0,
                               tx :Number = 0, ty :Number = 0) :void
    {
        // let's implement this bizarre function the easy way
        identity();
        rotate(rotation);
        scale(scaleX, scaleY);
        translate(tx, ty);
    }

    /**
     * Given a point in the pretransform coordinate space, returns the coordinates of that
     * point after the transformation occurs. Unlike the standard transformation applied
     * using the transformPoint() method, the deltaTransformPoint() method's transformation
     * does not consider the translation parameters tx and ty.
     */
    public function deltaTransformPoint (point: Point) :Point     
    {
        return new Point(a*point.x + b*point.y, c*point.x + d*point.y);
    }

    /**
     * Sets each matrix property to a value that causes a null transformation. An object
     * transformed by applying an identity matrix will be identical to the original. 
     */
    public function identity() :void
    {
        configure(1, 0, 0, 1, 0, 0);
    }

    /**
     * Performs the opposite transformation of the original matrix. You can apply an
     * inverted matrix to an object to undo the transformation performed when applying
     * the original matrix.
     */
    public function invert () :void
    {
        throw new Error("not implemented");
    }

    /**
     * Applies a rotation transformation to the Matrix object.
     */
    public function rotate (angle :Number) :void
    {
        var cs :Number = Math.cos(angle);
        var sn :Number = Math.sin(angle);
        configure(a*cs - b*sn, a*sn + b*cs, c*cs - d*sn, c*sn + d*cs, tx, ty);
    }

    /**
     * Applies a scaling transformation to the matrix. The x axis is multiplied by sx,
     * and the y axis it is multiplied by sy.
     */
    public function scale (sx :Number, sy :Number) :void
    {
        configure(a*sx, b*sy, c*sx, d*sy, tx, ty);
    }

    /**
     * Returns the result of applying the geometric transformation represented by the
     * Matrix object to the specified point.
     */
    public function transformPoint (point :Point) :Point
    {
        return new Point(a*point.x + b*point.y + tx, c*point.x + d*point.y + ty);
    }

    /**
     * Translates the matrix along the x and y axes, as specified by the dx and dy parameters.
     */
    public function translate (dx :Number, dy :Number) :void
    {
        configure(a, b, c, d, tx + dx, ty + dy);
    }

    /**
     * Returns a new Matrix object that is a clone of this matrix, with an exact
     * copy of the contained object. 
     */
    public function clone () :Matrix
    {
        return new Matrix(a, b, c, d, tx, ty);
    }

    /**
     * Returns a text value listing the properties of the Matrix object. 
     */
    public function toString () :String
    {
        return "(a=" + a + ", b=" + b + ", c=" + c + ", d=" + d + ", tx=" + tx + ", ty=" + ty + ")";
    }

    protected function configure (a :Number, b :Number, c :Number, d :Number,
                                  tx :Number, ty :Number) :void
    {
        this.a = a;
        this.b = b;
        this.c = c;
        this.d = d;
        this.tx = tx;
        this.ty = ty;
    }
}
}
