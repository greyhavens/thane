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
SET F=%1
SET FF=%F:\=/%
SET R=%F:.as=%
..\cpp\win32\release\ascap.exe -i %FF% > %FF%.cpp.out
echo CPP DONE
cp %R%.il %R%.cpp.il
cp %R%.abc %R%.cpp.abc
..\..\bin\asc.exe  -i %F% > %F%.java.out
echo JAVA DONE
cp %R%.il %R%.java.il
cp %R%.abc %R%.java.abc
echo DIFFING
diff -E -b -w -B %R%.cpp.il %R%.java.il > %R%.il.diff
diff -E -b -w -B %F%.cpp.out %F%.java.out > %F%.out.diff
diff --binary %R%.cpp.abc %R%.java.abc > %R%.abc.diff

