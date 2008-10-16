// Should be able to resolve the return type of the function at compile time
with ( {} )
{
    function q () : String
    {
        return "blah";
    }
}
