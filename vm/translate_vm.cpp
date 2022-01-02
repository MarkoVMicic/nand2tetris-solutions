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
                            uint32 *OutputIndex,
                            instruction_counts *InstructionCounts,
                            vm_string *ASMInstructions,
                            vm_tokens *VMTokens,
                            vm_error_list *ErrorList)
{

    TokenizeLine(VMInputString, InputIndex, VMTokens, ErrorList);

    ParseTokensToASM(VMTokens, ASMInstructions, InstructionCounts);

    WriteToASMOutput(ASMInstructions, ASMOutputString, OutputIndex);

    // NOTE(Marko): Reset VMTokens by setting each of the vm_strings to have 0 
    //              length
    for(uint32 TokenIndex = 0; 
        TokenIndex < VMTokens->VMTokenCount; 
        TokenIndex++)
    {
        VMTokens->VMTokens[TokenIndex].CurrentLength = 0;
    }
    // NOTE(Marko): Reset ASMInstructions by setting its length to 0
    ASMInstructions->CurrentLength = 0;
}


void TranslateVMInstructionsToASM(vm_string *VMInputString, 
                                  vm_string *ASMOutputString,
                                  instruction_counts *InstructionCounts,
                                  vm_string *ASMInstructions,
                                  vm_tokens *VMTokens,
                                  vm_error_list *ErrorList)
{
    // TODO(Marko): BONUS OBJECTIVE: Create error struct and pass it into 
    //                               here. Then write errors to the error 
    //                               struct as they are encountered, recording 
    //                               the line numbers and error type. 

    ErrorList->CurrentLineCount = 1;
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
                    vm_string Error = ConstructVMStringFromCString("Floating slash found.");
                    AddErrorToErrorList(ErrorList, &Error);
                }
            } break;

            case NEWLINE:
            {
#if VM_DEBUG
                printf("Line %d processed\n", ErrorList->CurrentLineCount);
#endif
                ErrorList->CurrentLineCount++;
            } break;

            default:
            {
                TranslateLine(VMInputString, 
                              &InputIndex, 
                              ASMOutputString, 
                              &OutputIndex,
                              InstructionCounts,
                              ASMInstructions,
                              VMTokens,
                              ErrorList);
                // NOTE(Marko): Since we've seeked to the newline, we want 
                //              to back up once so that the for loop can 
                //              increment us and then the switch statement 
                //              can process the newline. 
                InputIndex--;
            }
        }
    }
    ASMOutputString->Contents[ASMOutputString->CurrentLength] = '\0';
}
