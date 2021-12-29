#include "vm_tokens.h"
#include "vm_string.h"


void FreeVMTokens(vm_tokens *VMTokens)
{
    for(uint32 TokenIndex = 0;
        TokenIndex < VMTokens->VMTokenCount; 
        TokenIndex++)
    {
        free(VMTokens->VMTokens[TokenIndex].Contents);
    }
    free(VMTokens->VMTokens);
    free(VMTokens);
}

vm_tokens *AllocateVMTokens(uint32 TokenCount, uint32 TokenVMStringSize)
{
    vm_tokens *Result = (vm_tokens *)malloc(sizeof(vm_tokens));
    Result->VMTokens = (vm_string *)malloc(TokenCount*sizeof(vm_string));
    Result->VMTokenCount = TokenCount;
    for(uint32 TokenIndex = 0; TokenIndex < Result->VMTokenCount; TokenIndex++)
    {
        vm_string *CurrentString = 
            AllocateVMString(TokenVMStringSize);
        Result->VMTokens[TokenIndex] = *CurrentString;
    }

    return(Result);
}


internal void ParsePushCommand(vm_tokens *VMTokens, 
                               vm_string *ASMInstructions)
{
    Assert(VMTokens->VMTokenCount == 3);

    vm_string VMStringPushSegment = VMTokens->VMTokens[1];
    vm_string VMStringPushValue = VMTokens->VMTokens[2];

    if(VMStringsAreEqual(&VMStringPushSegment, "constant", 8))
    {
        /* NOTE(Marko): "push constant X" translates to
                            @X
                            D=A
                            @SP
                            A=M
                            M=D
                            @SP
                            M=M+1 

                        Excluding X, there are 28 characters. Then we simply 
                        need to include the length of VMStringPushValue

        */
        ASMInstructions->CurrentLength += 28+VMStringPushValue.CurrentLength;
        if(ASMInstructions->MemorySize <= ASMInstructions->CurrentLength)
        {
            GrowVMString(ASMInstructions);
        }

        ASMInstructions->Contents[0] = '@';
        char *PushValuePosition = &ASMInstructions->Contents[1];
        // NOTE(Marko): Passing (ASMInstructions->CurrentLength-1) to account 
        //              for the first char already being filled in by '@'
        CopyVMString(VMStringPushValue.Contents,
                     VMStringPushValue.CurrentLength,
                     PushValuePosition,
                     ASMInstructions->CurrentLength-1);
        char *OneAfterPushValue = PushValuePosition+VMStringPushValue.CurrentLength;
        char *RestOfPushConstantString = "\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
        uint32 RestOfPushConstantStringLength = 27;
        // TODO(Marko): Check if ASMInstructions->MemorySize is large enough. 
        CopyVMString(RestOfPushConstantString,
                     RestOfPushConstantStringLength,
                     OneAfterPushValue,
                     ASMInstructions->CurrentLength-1-VMStringPushValue.CurrentLength);
        ASMInstructions->Contents[ASMInstructions->CurrentLength] = '\0';
    }
}


internal void ParseReturnCommand(vm_string *ASMInstructions)
{

}


internal void ParseArithmeticCommand(vm_tokens *VMTokens, 
                                     vm_string *ASMInstructions)
{
    Assert(VMTokens->VMTokenCount == 1);
    vm_string VMStringArithmeticCommand = VMTokens->VMTokens[0];
    if(VMStringsAreEqual(&VMStringArithmeticCommand, "add", 3))
    {
        /* NOTE(Marko): "add" translates to:
                            @SP
                            M=M-1
                            A=M
                            D=M
                            @SP
                            M=M-1
                            A=M
                            M=M+D
                            @SP
                            M=M+1

                        Nothing depends on the input, so we can just hard code it in. 
        */
        vm_string ArithmeticAsm;
        ArithmeticAsm.Contents = "@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=M+D\n@SP\nM=M+1\n";
        ArithmeticAsm.CurrentLength = 48;
        ArithmeticAsm.MemorySize = 49;
        ASMInstructions->CurrentLength = ArithmeticAsm.CurrentLength;
        if(ASMInstructions->MemorySize <= ASMInstructions->CurrentLength)
        {
            GrowVMString(ASMInstructions);
        }
        CopyVMString(ArithmeticAsm.Contents,
                     ArithmeticAsm.CurrentLength,
                     ASMInstructions->Contents,
                     ASMInstructions->CurrentLength);
        ASMInstructions->Contents[ASMInstructions->CurrentLength] = '\0';
    }
}

void ParseTokensToASM(vm_tokens *VMTokens,
                      vm_string *ASMInstructions)
{
    switch(VMTokens->VMTokenCount)
    {
        case 1:
        {
            /* NOTE(Marko): Possible VM instructions of length 1:
                                "return"
                                "add"
                                "sub"
                                "neg"
                                "eq"
                                "get"
                                "lt"
                                "and"
                                "or"
                                "not"

                            Of these, only "return" is not an arithmetic 
                            command. 

            */
            if(VMStringsAreEqual(&VMTokens->VMTokens[0], "return", 6))
            {
                ParseReturnCommand(ASMInstructions);
            }
            else
            {
                ParseArithmeticCommand(VMTokens, ASMInstructions);
            }
        } break;

        case 2:
        {

        } break;

        case 3:
        {
            if(VMStringsAreEqual(&VMTokens->VMTokens[0], "push", 4))
            {
                ParsePushCommand(VMTokens, ASMInstructions);
            }

        } break;

        default:
        {
            InvalidCodePath; 
        }
    }
}

void TokenizeLine(vm_string *VMInputString, 
                  uint32 *InputIndex,
                  vm_tokens *VMTokens)
{
    // NOTE(Marko): Splits current line of VMInputString into tokens and puts 
    //              those tokens into VMTokens struct

    // TODO(Marko): Consider scoping the two passes separately.
    // TODO(Marko): Is there a way to do this in one pass? 
    //
    // NOTE(Marko): Counting how many tokens on the line. 
    //
    uint32 TokenCount = 0;
    char *CurrentChar = &VMInputString->Contents[*InputIndex];
    while(*CurrentChar != NEWLINE)
    {
        if(*CurrentChar == WHITESPACE)
        {
            TokenCount++;
        }
        CurrentChar++;
    }
    // NOTE(Marko): Final token is deliniated using \n so add one more to 
    //              TokenCount
    TokenCount++;
    //
    // NOTE(Marko): Allocate the vm_string structs
    //
    VMTokens->VMTokenCount = TokenCount;

    // NOTE(Marko): Putting tokens into vm_tokens struct. 
    CurrentChar = &VMInputString->Contents[*InputIndex];
    uint32 TokenIndex = 0;
    while(*CurrentChar != NEWLINE)
    {
        char *CurrentTokenBegin = CurrentChar;
        while((*CurrentChar != WHITESPACE) && (*CurrentChar != NEWLINE))
        {
            (*InputIndex)++;
            CurrentChar++;
        }
        char *CurrentTokenEnd = CurrentChar; 

        uint32 CurrentTokenLength = 
            (uint32)(CurrentTokenEnd - CurrentTokenBegin);

        vm_string *CurrentToken = AllocateVMString(CurrentTokenLength+1);
        CurrentToken->CurrentLength = CurrentTokenLength;
        CopyVMString(CurrentTokenBegin, 
                     CurrentTokenLength,
                     CurrentToken->Contents,
                     CurrentToken->CurrentLength);
        // NOTE(Marko): Null terminate
        CurrentToken->Contents[CurrentToken->CurrentLength] = '\0';

        VMTokens->VMTokens[TokenIndex] = *CurrentToken;
        TokenIndex++;

        if(*CurrentChar != NEWLINE)
        {
            CurrentChar++;
            (*InputIndex)++;
        }
    }
}