package mx.resources {

/** This mimics the interface provided by flex in order to make narya compile. 
 *  Unused methods are commented out. */
public interface IResourceBundle
{
    //function get bundleName () :String;
    function get content () :Object;
    //function get locale () :String;
}

}

