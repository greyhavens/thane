package {

public class Tiny
{
    public function probe () :void
    {
        trace ("TINY.AS:: This is probe() executing in an instance of Tiny: " + this);
    }
}
}

import avmplus.Domain;

trace("TINY.AS:: This is statically executing initialization code.");
try {
    var tinyClass :Class = Domain.currentDomain.getClass("Tiny");
    trace("TINY.AS:: Tiny class successfully located within its own execution Domain.");
} catch (e: Error) {
    trace("TINY.AS:: Tiny class NOT found within its own execution Domain: " + e);
}

try {
    var timClass :Class = Domain.currentDomain.getClass("Tim");
    trace("TINY.AS:: Tim class successfully located from the Tiny domain.");
} catch (e: Error) {
    trace("TIY.AS:: Tim class NOT found from the Tiny domain: " + e);
}
