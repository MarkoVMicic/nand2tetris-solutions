@echo off
set CommonCompilerFlags= -MTd -Gm- -GR- -WX -nologo -Od -Oi -Zi -DWIN_ASSEMBLER_DEBUG=1
set CommonLinkerFlags=/INCREMENTAL:NO 

IF NOT EXIST .\build mkdir .\build
pushd .\build
del *.pdb > NUL 2> NUL
cl %CommonCompilerFlags% ..\win_assembler.cpp /link %CommonLinkerFlags% 
popd
