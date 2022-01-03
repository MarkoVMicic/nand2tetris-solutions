@echo off
pushd .\build
vm_main.exe "..\vm_bad_input_files\WrongFileExtension.test" "..\vm_bad_output_files\WrongFileExtension.asm"
popd