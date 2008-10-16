// [Coach] Warning #1058: Unsupported AS2 property.
// this also tests to make sure include statements work correctly, and that error reporting is correct across multiple includes.
class A
{
   _focusrect = false;
   // just for added testing... includes with trailing and leading spaces should still work
   include " warning1058.foo.as.inc "
}
