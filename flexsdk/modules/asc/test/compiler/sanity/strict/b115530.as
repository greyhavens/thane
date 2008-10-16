package P {
public interface fletch {
function lives();
}

public class erwin implements fletch { 
public function lives(){return 'PASSED';}
}
}

import P.*;

var p = new erwin();
print(p.lives());

