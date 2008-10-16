 //class a { };
 //class b extends a { };

 var a:Number = 4.35; // shouldn't this be a compile error?
 var b:uint = uint(a); // crash! If the 'var a' declaration is removed, no crash.
 print(b);
