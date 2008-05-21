package flash.utils {

import avmplus.Domain;

public function describeType (c :Object) :XML
{
    throw new Error("describeType() Not implemented");
}

public function getDefinitionByName (name :String) :Class
{
    return Domain.currentDomain.getClass(name.replace("::", "."));
}

public function getQualifiedClassName (c :*) :String
{
    return Domain.currentDomain.getClassName(c);
}

public function getQualifiedSuperclassName (c :*) :String
{
    throw new Error("getQualifiedSuperclassName() not implemented");
}


}
