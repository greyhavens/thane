Thane ActionScript Engine
-------------------------

Thane is a lightly modified version of Tamarin (http://www.mozilla.org/projects/tamarin/) with
a partial Flash Player compatibility layer written mostly in ActionScript with a bit of native
glue.

 * The engine itself was modified to run on Unix platforms under x86_64 architecture. The existing
   Windows/64 support was extended for the Unix ABI (http://www.x86-64.org/documentation.html).
   JIT mode is currently not available for this architecture: a future versions of Tamarin will
   be updated for the most recent Tamarin code base and its enhanced code generation subsystem.

 * The compatibility layer adds many of the abilities of the Flash Player library that make
   sense in a server environment: sockets, event dispatching, timers, geometry classes, etc.


Fair Warning
------------

 * This code is in mid-development and anything and everything in it is likely to change without
   notice. We will document the changes, but provide no backward compatibility support.

 * As suggested above, our code is out of sync with upstream Tamarin and merging with their
   changes is a priority -- currently pending investigation into the quality of the Unix/64
   support in the new LIR code generation engine.

License and Distribution
------------------------

Thane is released under a BSD license. This means you are free to use Thane on any project,
open source or proprietary, but that our copyright notice and liability disclaimer must be
retained. See the file LICENSE in this directory for more information.

