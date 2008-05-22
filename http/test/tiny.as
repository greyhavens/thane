package {

public class Tiny
{
    public static var lifeline :Function;

    public function probe () :void
    {
        trace ("TINY.AS:: This is probe() calling my lifeline...");
        lifeline();
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

var timClass :Class
try {
    timClass = Domain.currentDomain.getClass("Tim");
    trace("TINY.AS:: Tim class successfully located from the Tiny domain: " + timClass);
} catch (e: Error) {
    trace("TINY.AS:: Tim class NOT found from the Tiny domain: " + e);
}

if (timClass != null) {
    try {
        trace("TINY.AS:: Calling static probe() in Tim...");
        timClass.probe.apply();
        trace("TINY.AS:: Success! Wait, that's bad.");
    } catch (e: Error) {
        trace("TINY.AS:: Hurray, error executing the call: " + e);
    }
}
