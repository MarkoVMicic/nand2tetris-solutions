#if !defined(TRANSLATE_VM_H)

#include "vm_string.h" 
#include "vm_tokens.h"
#include <stdio.h>


void TranslateVMInstructionsToASM(vm_string *VMInputString, 
                                  vm_string *ASMOutputString,
                                  instruction_counts *InstructionCounts,
                                  vm_string *ASMInstructions,
                                  vm_tokens *VMTokens);

#define TRANSLATE_VM_H
#endif
