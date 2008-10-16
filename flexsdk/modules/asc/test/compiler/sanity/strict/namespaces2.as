// namespace
package actors {
    public namespace English
    public namespace French
    public class Greeter {
        English function hello() { 
            print("hello, world")
        }
        French function hello() { 
            print("bonjour, le monde")
        }
    }
}
import actors.*
var greeter : Greeter = new Greeter
use namespace English     // Make the English version the default
greeter.hello()           // Invoke the used (English) version
greeter.French::hello()   // Invoke the French version

