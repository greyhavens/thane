package {

import avmplus.*;

import flash.utils.Dictionary;
import flash.utils.getQualifiedClassName;

public class AMF3
{
    public static const MARK_UNDEFINED :uint    = 0x00;
    public static const MARK_NULL :uint         = 0x01;
    public static const MARK_FALSE :uint        = 0x02;
    public static const MARK_TRUE :uint         = 0x03;
    public static const MARK_INTEGER :uint      = 0x04;
    public static const MARK_DOUBLE :uint       = 0x05;
    public static const MARK_STRING :uint       = 0x06;
    public static const MARK_XML_DOC :uint      = 0x07;
    public static const MARK_DATE :uint         = 0x08;
    public static const MARK_ARRAY :uint        = 0x09;
    public static const MARK_OBJECT :uint       = 0x0a;
    public static const MARK_XML :uint          = 0x0b;
    public static const MARK_BYTE_ARRAY :uint   = 0x0c;

    public static function getClassNameByAlias (aliasName :String) :String
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
        var className :String = getQualifiedClassName(classObject);
        _aliasToClass[aliasName] = className;
        _classToAlias[className] = aliasName;
    }

    private static var _aliasToClass :Dictionary = new Dictionary();
    private static var _classToAlias :Dictionary = new Dictionary();
}
}
