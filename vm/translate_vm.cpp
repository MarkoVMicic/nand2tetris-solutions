#include "translate_vm.h"
#include "vm_tokens.h"


internal void WriteToASMOutput(vm_string *ASMInstructions,
                               vm_string *ASMOutputString,
                               uint32 *ASMOutputIndex)
{
    ASMOutputString->CurrentLength += ASMInstructions->CurrentLength;
    // NOTE(Marko): Check if the buffer has become too small
    if(ASMOutputString->CurrentLength >= ASMOutputString->MemorySize)
    {
        GrowVMString(ASMOutputString);
    }
    CopyVMString(ASMInstructions->Contents,
                 ASMInstructions->CurrentLength,
                 &ASMOutputString->Contents[*ASMOutputIndex],
                 ASMOutputString->CurrentLength - *ASMOutputIndex);

    // NOTE(Marko): Advance the ASMOutputIndex to refer to the next free slot 
    *ASMOutputIndex += ASMInstructions->CurrentLength;
}


internal void TranslateLine(vm_string *VMInputString, 
                            uint32 *InputIndex, 
                            vm_string *ASMOutputString, 
                            uint32 *OutputIndex)
{

    vm_tokens *VMTokens = AllocateVMTokens(MAX_VM_TOKEN_COUNT, 
                                           DEFAULT_INITIAL_VM_STRING_SIZE);
    TokenizeLine(VMInputString, InputIndex, VMTokens);

    vm_string *ASMInstructions = 
        AllocateVMString(DEFAULT_INITIAL_VM_STRING_SIZE);
    ParseTokensToASM(VMTokens, ASMInstructions);

    WriteToASMOutput(ASMInstructions, ASMOutputString, OutputIndex);

    FreeVMTokens(VMTokens);
    FreeVMString(ASMInstructions);

}


void TranslateVMInstructionsToASM(vm_string *VMInputString, 
                                  vm_string *ASMOutputString)
{
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
    ASMOutputString->Contents[ASMOutputString->CurrentLength] = '\0';
}
