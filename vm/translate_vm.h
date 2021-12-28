#if !defined(TRANSLATE_VM_H)

#include "vm_string.h" 
#include <stdio.h>


void TranslateVMInstructionsToASM(vm_string *VMInputString, 
                                  vm_string *ASMOutputString);

#define TRANSLATE_VM_H
#endif
