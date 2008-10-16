package versions {
    public namespace v1
    public namespace v2
}
package say {
    import versions.*
    public class A {
    static v1 function something() { print('hello') }
    static v2 function something() { print('goodbye') }
    }
}
import versions.*
import say.*
use namespace v2

A.something()
say.A.something()
