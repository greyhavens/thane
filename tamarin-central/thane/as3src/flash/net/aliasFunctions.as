package flash.net {

import avmplus.Domain;

public function getClassByAlias (aliasName :String) :Class
{
    var className :String = AMF3.getClassNameByAlias(aliasName);
    if (className == null) {
        throw new VerifyError("Alias not registered.");
    }
    return Domain.currentDomain.getClass(className);
}

public function registerClassAlias (aliasName :String, classObject :Class) :void
{
    // TODO: protect against user code
    AMF3.registerClassAlias(aliasName, classObject);
}
}
