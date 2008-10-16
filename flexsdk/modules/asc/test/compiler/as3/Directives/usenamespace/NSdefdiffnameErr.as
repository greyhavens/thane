
/*
Compiler Error:  Define a namespace, then call use namespace with a 
                 different namespace that is not defined
*/

namespace thisNSisdefined;
  
use namespace thisNSisnotdefined;
