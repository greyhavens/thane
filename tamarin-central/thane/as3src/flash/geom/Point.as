//
// $Id$
//
// Thane - an enhancement of Mozilla/Adobe's Tamarin project with partial Flash Player compatibility
// Copyright (C) 2008-2009 Three Rings Design, Inc.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted
// provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice, this list of
//    conditions and the following disclaimer in the documentation and/or other materials provided
//    with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

package flash.geom {

public class Point
{ 
    public static function distance (a :Point, b :Point) :Number
    {
        var dX :Number = a.x - b.x;
        var dY :Number = a.y - b.y;

        return Math.sqrt(dX*dX + dY*dY);
    }

    public static function interpolate (a :Point, b :Point, f :Number) :Point
    {
        f = Math.max(0, Math.min(f, 1));

        // note: bizarrely, we interolate from b to a as f goes from 0 to 1
        return new Point(b.x + f*(a.x - b.x),
                         b.y + f*(a.y - b.y));
    }

    public static function polar (len :Number, angle :Number) :Point
    {
        return new Point(len * Math.cos(angle),
                         len * Math.sin(angle));
    }

    public var x :Number;
    public var y :Number;

    public function Point (x :Number = 0, y :Number = 0)
    {
        this.x = x;
        this.y = y;
    }

    public function get length () :Number
    {
        return Math.sqrt(x*x + y*y);
    }

    public function add (v :Point) :Point
    {
        return new Point(x + v.x, y + v.y);
    }

    public function subtract (v :Point) :Point
    {
        return new Point(x - v.x, y - v.y);
    }

    public function normalize (thickness :Number) :void
    {
        var len :Number = this.length;
        if (len == 0) {
            x = y = 0;

        } else {
            x = (x * thickness) / len;
            y = (y * thickness) / len;
        }
    }

    public function offset (dX :Number, dY :Number) :void
    {
        x += dX;
        y += dY;
    }

    public function clone () :Point
    {
        return new Point(x, y);
    }

    public function equals (p :Point) :Boolean
    {
        return p.x == x && p.y == y;
    }

    public function toString () :String
    {
        return "(x=" + x + ", y=" + y + ")";
    }

}
}
