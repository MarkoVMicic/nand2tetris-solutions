@echo off
set CommonCompilerFlags= -MTd -Gm- -GR- -WX -nologo -Od -Oi -Zi -DVM_DEBUG=1
set CommonLinkerFlags=/INCREMENTAL:NO 
set CompiledFiles=..\vm_main.cpp ..\vm_string.cpp ..\translate_vm.cpp ..\vm_tokens.cpp

IF NOT EXIST .\build mkdir .\build
pushd .\build
del *.pdb > NUL 2> NUL
cl %CommonCompilerFlags% %CompiledFiles% /link %CommonLinkerFlags% 
popd
