package {

import avmplus.*;

import flash.utils.Dictionary;

public class AMF3
{
    public static function getClassNameByAlias (aliasName :String) :Class
    {
        if (aliasName == null) {
            throw new TypeError("null argument to getClassNameByAlias");
        }
        return _aliasToClass[aliasName];
    }

    public static function getAliasByClassName (className :String) :String
    {
        if (className == null) {
            throw new TypeError("null argument to getAliasByClassName");
        }
        return _classToAlias[className];
    }

    public static function registerClassAlias (aliasName :String, classObject :Class) :void
    {
        // TODO: protect against user code
        if (aliasName == null || classObject == null) {
            throw new TypeError("null argument to registerClassAlias");
        }
        var className :String = Domain.currentDomain.getClassName(classObject);
        _aliasToClass[aliasName] = className;
        _classToAlias[className] = aliasName;
    }

    private static var _aliasToClass :Dictionary = new Dictionary();
    private static var _classToAlias :Dictionary = new Dictionary();
}
}
