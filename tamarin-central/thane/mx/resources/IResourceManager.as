package mx.resources {

/** This mimics the interface provided by flex in order to make narya compile. 
 *  Unused methods are commented out. */
public interface IResourceManager
{
    //function get localeChain () :Array;
    //function addResourceBundle (
    //    resourceBundle :IResourceBundle) :void;
    //function dispatchEvent (
    //    event :Event) :Boolean;
    //function findResourceBundleWithResource (
    //    bundleName :String, 
    //    resourceName :String) :IResourceBundle;
    //function getBoolean (
    //    bundleName :String, 
    //    resourceName :String, 
    //    locale :String = null) :Boolean;
    //function getBundleNamesForLocale (
    //    locale :String) :Array;
    //function getClass (
    //    bundleName :String, 
    //    resourceName :String, 
    //    locale :String = null) :Class;
    //function getInt (
    //    bundleName :String, 
    //    resourceName :String, 
    //    locale :String = null) :int;
    function getLocales () :Array;
    //function getNumber (
    //    bundleName :String, 
    //    resourceName :String, 
    //    locale :String = null) :Number;
    //function getObject (
    //    bundleName :String, 
    //    resourceName :String, 
    //    locale :String = null) :*;
    function getResourceBundle (
        locale :String, 
        bundleName :String) :IResourceBundle;
    function getString (
        bundleName :String, 
        resourceName :String, 
        parameters :Array = null, 
        locale :String = null) :String;
    //function getStringArray (
    //    bundleName :String, 
    //    resourceName :String, 
    //    locale :String = null) :Array;
    //function getUint (
    //    bundleName :String, 
    //    resourceName :String, 
    //    locale :String = null) :uint;
    //function hasEventListener (
    //    type :String) :Boolean;
    //function installCompiledResourceBundles (
    //    applicationDomain :ApplicationDomain, 
    //    locales :Array, 
    //    bundleNames :Array) :void;
    //function loadResourceModule (
    //    url :String, 
    //    update :Boolean = true, 
    //    applicationDomain :ApplicationDomain = null, 
    //    securityDomain :SecurityDomain = null) :IEventDispatcher;
    //function removeEventListener (
    //    type :String, 
    //    listener :Function, 
    //    useCapture :Boolean = false) :void;
    //function removeResourceBundle (
    //    locale :String, 
    //    bundleName :String) :void;
    //function removeResourceBundlesForLocale (
    //    locale :String) :void;
    //function unloadResourceModule (
    //    url :String, 
    //    update :Boolean = true) :void;
    //function update () :void;
}
}
