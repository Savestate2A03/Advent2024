echo SETTING UP MSVC ENV...
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x86_amd64
echo COMPILING PROGRAM...
cl advent9.c /Zi /EHsc /nologo /TC
echo RUNNING!
advent9.exe