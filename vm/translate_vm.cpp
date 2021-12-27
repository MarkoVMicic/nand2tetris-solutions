#include "translate_vm.h"


internal void TranslateLine(vm_string *VMInputString, 
                            uint32 *InputIndex, 
                            vm_string *ASMOutputString, 
                            uint32 *OutputIndex)
{

}


void TranslateVMInstructionsToAsm(vm_string *VMInputString, 
                                  vm_string *ASMOutputString)
{
    // TODO(Marko): Read each line until newline, then split by whitespace.
    // TODO(Marko): Create table for commands
    // TODO(Marko): BONUS OBJECTIVE: Create error struct and pass it into 
    //                               here. Then write errors to the error 
    //                               struct as they are encountered, recording 
    //                               the line numbers and error type. 

    uint32 LineCount = 1;
    uint32 OutputIndex = 0;
    for(uint32 InputIndex = 0; 
        InputIndex < VMInputString->CurrentLength; 
        InputIndex++)
    {
        switch(VMInputString->Contents[InputIndex])
        {
            case COMMENT_SLASH:
            {
                if((InputIndex < VMInputString->CurrentLength-1) && 
                   (VMInputString->Contents[InputIndex+1] == COMMENT_SLASH))
                {
                    while(VMInputString->Contents[InputIndex] != NEWLINE)
                    {
                        InputIndex++;
                    }
                    // NOTE(Marko): Since we've seeked to the newline, we want 
                    //              to back up once so that the for loop can 
                    //              increment us and then the switch statement 
                    //              can process the newline. 
                    InputIndex--;
                }
                else
                {
                    printf("Floating slash found on line %d\n", LineCount);
                    return;
                }
            };

            case NEWLINE:
            {
#if VM_DEBUG
                printf("Line %d processed\n", LineCount);
#endif
                LineCount++;
            } break;

            default:
            {
                TranslateLine(VMInputString, 
                              &InputIndex, 
                              ASMOutputString, 
                              &OutputIndex);
            }
        }
    }

}
