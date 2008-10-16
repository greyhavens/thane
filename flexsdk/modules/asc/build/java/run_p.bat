@echo off

rem
rem ADOBE SYSTEMS INCORPORATED
rem Copyright 2007 Adobe Systems Incorporated
rem All Rights Reserved.
rem 
rem NOTICE: Adobe permits you to use, modify, and distribute this file
rem in accordance with the terms of the license agreement accompanying it.
rem 

echo Compiling %1
..\cpp\win32\release\ascap.exe -p3 %1 > %1.cpp.out
move %1.p3 %1.cpp.p3
c:\j2sdk1.5.0\bin\java -ea -DAVMPLUS -DAS2 -classpath  ../../lib/asc.jar macromedia.asc.embedding.Compiler -p3 %1 2> %1.java.out
move %1.p3 %1.java.p3
diff -E -b -w -B %1.cpp.p3 %1.java.p3 > %1.p3.diff
diff -E -b -w -B %1.cpp.out %1.java.out > %1.out.diff

