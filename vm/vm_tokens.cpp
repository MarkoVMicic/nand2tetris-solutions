#include "vm_tokens.h"
#include "vm_string.h"

// TODO(Marko): Fix some of the Pop commands so that they do not require any 
//              assembly storage! 
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


internal void ParsePopCommand(vm_tokens *VMTokens,
                              vm_string *ASMInstructions,
                              instruction_counts *InstructionCounts)
{
    Assert(VMTokens->VMTokenCount == 3);

    vm_string VMStringPopSegment = VMTokens->VMTokens[1];
    vm_string VMStringPopValue = VMTokens->VMTokens[2];

    vm_string ArgumentString = {"argument",8,9};
    vm_string LocalString = {"local",5,6};
    vm_string StaticString = {"static",6,7};
    vm_string ConstantString = {"constant",8,9};
    vm_string ThisString = {"this",4,5};
    vm_string ThatString = {"that",4,5};
    vm_string PointerString = {"pointer",7,8};
    vm_string TempString = {"temp",4,5};

    // TODO(Marko): Bounds checking to make sure that VMStringPopValue corresponds to a valid address (i.e. does not go out of bounds)
    if(VMStringsAreEqual(&VMStringPopSegment, &ArgumentString))
    {
        /* NOTE(Marko): "pop argument X" translates to
                            @ARG
                            D=M
                            @X
                            D=D+A
                            @ARG_POP
                            M=D
                            @SP
                            M=M-1
                            A=M
                            D=M
                            @ARG_POP
                            A=M
                            M=D

                        Excluding X (which is the specified local section) there are 65 characters.  
        */
        vm_string FirstPart = {"@ARG\nD=M\n@",10,11};
        vm_string SecondPart = {"\nD=D+A\n@ARG_POP\nM=D\n@SP\nM=M-1\nA=M\nD=M\n@ARG_POP\nA=M\nM=D\n",55,56};

        ASMInstructions->CurrentLength = 
            FirstPart.CurrentLength +
            SecondPart.CurrentLength +
            1*VMStringPopValue.CurrentLength;
        if(ASMInstructions->MemorySize <= ASMInstructions->CurrentLength)
        {
            GrowVMString(ASMInstructions);
        }

        {
            char *PasteCharLocation = ASMInstructions->Contents;
            uint32 LengthRemaining = ASMInstructions->CurrentLength;

            CopyVMString(FirstPart.Contents,
                         FirstPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += FirstPart.CurrentLength;
            LengthRemaining -= FirstPart.CurrentLength;

            CopyVMString(VMStringPopValue.Contents,
                         VMStringPopValue.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += VMStringPopValue.CurrentLength;
            LengthRemaining -= VMStringPopValue.CurrentLength;

            CopyVMString(SecondPart.Contents,
                         SecondPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += SecondPart.CurrentLength;
            LengthRemaining -= SecondPart.CurrentLength;
        }
        ASMInstructions->Contents[ASMInstructions->CurrentLength] = '\0';
        InstructionCounts->PopArgumentCount++;
    }
    else if(VMStringsAreEqual(&VMStringPopSegment, &LocalString))
    {
        /* NOTE(Marko): "pop local X" translates to
                            @LCL
                            D=M
                            @X
                            D=D+A
                            @LCL_POP
                            M=D
                            @SP
                            M=M-1
                            A=M
                            D=M
                            @LCL_POP
                            A=M
                            M=D

                        Excluding X (which is the specified local section) there are 65 characters.  
        */
        vm_string FirstPart = {"@LCL\nD=M\n@",10,11};
        vm_string SecondPart = {"\nD=D+A\n@LCL_POP\nM=D\n@SP\nM=M-1\nA=M\nD=M\n@LCL_POP\nA=M\nM=D\n",55,56};

        ASMInstructions->CurrentLength = 
            FirstPart.CurrentLength + 
            SecondPart.CurrentLength + 
            1*VMStringPopValue.CurrentLength;
        if(ASMInstructions->MemorySize <= ASMInstructions->CurrentLength)
        {
            GrowVMString(ASMInstructions);
        }

        {
            char *PasteCharLocation = ASMInstructions->Contents;
            uint32 LengthRemaining = ASMInstructions->CurrentLength;

            CopyVMString(FirstPart.Contents,
                         FirstPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += FirstPart.CurrentLength;
            LengthRemaining -= FirstPart.CurrentLength;

            CopyVMString(VMStringPopValue.Contents,
                         VMStringPopValue.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += VMStringPopValue.CurrentLength;
            LengthRemaining -= VMStringPopValue.CurrentLength;

            CopyVMString(SecondPart.Contents,
                         SecondPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += SecondPart.CurrentLength;
            LengthRemaining -= SecondPart.CurrentLength;
        }
        ASMInstructions->Contents[ASMInstructions->CurrentLength] = '\0';
        InstructionCounts->PopLocalCount++;
    }
    else if(VMStringsAreEqual(&VMStringPopSegment, &StaticString))
    {
        
    }
    else if(VMStringsAreEqual(&VMStringPopSegment, &ConstantString))
    {
        // NOTE(Marko): "pop constant i" is not valid. 
        InvalidCodePath;
    }
    else if(VMStringsAreEqual(&VMStringPopSegment, &ThisString))
    {
        /* NOTE(Marko): "pop this X" translates to
                            @THIS
                            D=M
                            @X
                            D=D+A
                            @THIS_POP
                            M=D
                            @SP
                            M=M-1
                            A=M
                            D=M
                            @THIS_POP
                            A=M
                            M=D

                        Excluding X (which is the specified local section) there are 68 characters.  
        */
        
        vm_string FirstPart = {"@THIS\nD=M\n@",11,12};
        vm_string SecondPart = {"\nD=D+A\n@THIS_POP\nM=D\n@SP\nM=M-1\nA=M\nD=M\n@THIS_POP\nA=M\nM=D\n",57,58};
        ASMInstructions->CurrentLength = 
            FirstPart.CurrentLength +
            SecondPart.CurrentLength +
            1*VMStringPopValue.CurrentLength;
        if(ASMInstructions->MemorySize <= ASMInstructions->CurrentLength)
        {
            GrowVMString(ASMInstructions);
        }

        {
            char *PasteCharLocation = ASMInstructions->Contents;
            uint32 LengthRemaining = ASMInstructions->CurrentLength;

            CopyVMString(FirstPart.Contents,
                         FirstPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += FirstPart.CurrentLength;
            LengthRemaining -= FirstPart.CurrentLength;

            CopyVMString(VMStringPopValue.Contents,
                         VMStringPopValue.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += VMStringPopValue.CurrentLength;
            LengthRemaining -= VMStringPopValue.CurrentLength;

            CopyVMString(SecondPart.Contents,
                         SecondPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += SecondPart.CurrentLength;
            LengthRemaining -= SecondPart.CurrentLength;
        }
        ASMInstructions->Contents[ASMInstructions->CurrentLength] = '\0';
        InstructionCounts->PopThisCount++;
        
    }
    else if(VMStringsAreEqual(&VMStringPopSegment, &ThatString))
    {
        /* NOTE(Marko): "pop that X" translates to
                            @THAT
                            D=M
                            @X
                            D=D+A
                            @THAT_POP
                            M=D
                            @SP
                            M=M-1
                            A=M
                            D=M
                            @THAT_POP
                            A=M
                            M=D

                        Excluding X (which is the specified local section) there are 68 characters.  
        */
        vm_string FirstPart = {"@THAT\nD=M\n@",11,12};
        vm_string SecondPart = {"\nD=D+A\n@THAT_POP\nM=D\n@SP\nM=M-1\nA=M\nD=M\n@THAT_POP\nA=M\nM=D\n",57,58};

        ASMInstructions->CurrentLength = 
            FirstPart.CurrentLength +
            SecondPart.CurrentLength +
            1*VMStringPopValue.CurrentLength;
        if(ASMInstructions->MemorySize <= ASMInstructions->CurrentLength)
        {
            GrowVMString(ASMInstructions);
        }

        {
            char *PasteCharLocation = ASMInstructions->Contents;
            uint32 LengthRemaining = ASMInstructions->CurrentLength;

            CopyVMString(FirstPart.Contents,
                         FirstPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += FirstPart.CurrentLength;
            LengthRemaining -= FirstPart.CurrentLength;

            CopyVMString(VMStringPopValue.Contents,
                         VMStringPopValue.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += VMStringPopValue.CurrentLength;
            LengthRemaining -= VMStringPopValue.CurrentLength;

            CopyVMString(SecondPart.Contents,
                         SecondPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += SecondPart.CurrentLength;
            LengthRemaining -= SecondPart.CurrentLength;
        }
        ASMInstructions->Contents[ASMInstructions->CurrentLength] = '\0';
        InstructionCounts->PopThatCount++;        
    }
    else if(VMStringsAreEqual(&VMStringPopSegment, &PointerString))
    {
        /* NOTE(Marko): "pop pointer 0" corresponds to popping into base 
                        address of THIS.
                        This translates to:
                            @SP
                            M=M-1
                            A=M
                            D=M
                            @THIS
                            M=D

                        "pop pointer 1" corresponds to popping into base 
                        address of THAT.
                        This translates to:
                            @SP
                            M=M-1
                            A=M
                            D=M
                            @THIS
                            M=D

                        Both cases have 28 characters.                   
        */

        // NOTE(Marko): Make sure that the pop value is either 0 or 1. 
        vm_string ZeroString = {"0",1,2};
        vm_string OneString = {"1",1,2};
        vm_string BaseAddress;
        if(VMStringsAreEqual(&VMStringPopValue, &ZeroString))
        {
            BaseAddress.Contents = "THIS";
            BaseAddress.CurrentLength = 4;
            BaseAddress.MemorySize = 5;
        }
        else if(VMStringsAreEqual(&VMStringPopValue, &OneString))
        {
            BaseAddress.Contents = "THAT";
            BaseAddress.CurrentLength = 4;
            BaseAddress.MemorySize = 5;            
        }
        else
        {
            InvalidCodePath;
        }

        vm_string FirstPart = {"@SP\nM=M-1\nA=M\nD=M\n@", 19, 20};
        vm_string SecondPart = {"\nM=D\n",5,6};

        ASMInstructions->CurrentLength = 
            FirstPart.CurrentLength + 
            SecondPart.CurrentLength + 
            BaseAddress.CurrentLength;
        if(ASMInstructions->MemorySize <= ASMInstructions->CurrentLength)
        {
            GrowVMString(ASMInstructions);
        }

        {
            char *PasteCharLocation = ASMInstructions->Contents;
            uint32 LengthRemaining = ASMInstructions->CurrentLength;

            CopyVMString(FirstPart.Contents,
                         FirstPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += FirstPart.CurrentLength;
            LengthRemaining -= FirstPart.CurrentLength;

            CopyVMString(BaseAddress.Contents,
                         BaseAddress.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += BaseAddress.CurrentLength;
            LengthRemaining -= BaseAddress.CurrentLength;

            CopyVMString(SecondPart.Contents,
                         SecondPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += SecondPart.CurrentLength;
            LengthRemaining -= SecondPart.CurrentLength;
        }
        ASMInstructions->Contents[ASMInstructions->CurrentLength] = '\0';
        InstructionCounts->PopPointerCount++;   
    }
    else if(VMStringsAreEqual(&VMStringPopSegment, &TempString))
    {
        /* NOTE(Marko): "pop temp X" translates to
                            @SP
                            M=M-1
                            A=M
                            D=M
                            @X
                            M=D

                        Where X is TEMP_ADDRESS_START + VMStringPopValue, and 
                        it must not exceed TEMP_ADDRESS_END
                        Excluding X, it has 24 characters. 
        */
        uint32 PopAddressValue = 
            TEMP_ADDRESS_START + VMStringToUInt32(&VMStringPopValue);
        if(PopAddressValue > TEMP_ADDRESS_END)
        {
            // NOTE(Marko): PopAddressValue is not valid.
            InvalidCodePath;
        }
        vm_string VMStringPopAddress = UInt32ToVMString(PopAddressValue);

        vm_string FirstPart = {"@SP\nM=M-1\nA=M\nD=M\n@",19,20};
        vm_string SecondPart = {"\nM=D\n",5,6};

        ASMInstructions->CurrentLength = 
            FirstPart.CurrentLength +
            SecondPart.CurrentLength +
            VMStringPopAddress.CurrentLength;
        if(ASMInstructions->MemorySize <= ASMInstructions->CurrentLength)
        {
            GrowVMString(ASMInstructions);
        }

        {
            char *PasteCharLocation = ASMInstructions->Contents;
            uint32 LengthRemaining = ASMInstructions->CurrentLength;

            CopyVMString(FirstPart.Contents,
                         FirstPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += FirstPart.CurrentLength;
            LengthRemaining -= FirstPart.CurrentLength;

            CopyVMString(VMStringPopAddress.Contents,
                         VMStringPopAddress.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += VMStringPopAddress.CurrentLength;
            LengthRemaining -= VMStringPopAddress.CurrentLength;

            CopyVMString(SecondPart.Contents,
                         SecondPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += SecondPart.CurrentLength;
            LengthRemaining -= SecondPart.CurrentLength;
        }
        ASMInstructions->Contents[ASMInstructions->CurrentLength] = '\0';
        InstructionCounts->PopTempCount++;   
    }
    else
    {
        InvalidCodePath;
    }
}                              


internal void ParsePushCommand(vm_tokens *VMTokens, 
                               vm_string *ASMInstructions,
                               instruction_counts *InstructionCounts)
{
    Assert(VMTokens->VMTokenCount == 3);

    vm_string VMStringPushSegment = VMTokens->VMTokens[1];
    vm_string VMStringPushValue = VMTokens->VMTokens[2];

    vm_string ArgumentString = {"argument",8,9};
    vm_string LocalString = {"local",5,6};
    vm_string StaticString = {"static",6,7};
    vm_string ConstantString = {"constant",8,9};
    vm_string ThisString = {"this",4,5};
    vm_string ThatString = {"that",4,5};
    vm_string PointerString = {"pointer",7,8};
    vm_string TempString = {"temp",4,5};
    if(VMStringsAreEqual(&VMStringPushSegment, &ArgumentString))
    {
        /* NOTE(Marko): "push argument X" translates to
                    @ARG
                    D=M
                    @X
                    A=D+A
                    D=M
                    @SP
                    A=M
                    M=D
                    @SP
                    M=M+1

                43 characters excluding X (where X is the address number)
        */
        vm_string FirstPart = {"@ARG\nD=M\n@",10,11};
        vm_string SecondPart = {"\nA=D+A\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",33,34};
        ASMInstructions->CurrentLength = 
            FirstPart.CurrentLength + 
            SecondPart.CurrentLength + 
            VMStringPushValue.CurrentLength;
        if(ASMInstructions->MemorySize <= ASMInstructions->CurrentLength)
        {
            GrowVMString(ASMInstructions);
        }
        {
            char *PasteCharLocation = ASMInstructions->Contents;
            uint32 LengthRemaining = ASMInstructions->CurrentLength;

            CopyVMString(FirstPart.Contents,
                         FirstPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += FirstPart.CurrentLength;
            LengthRemaining -= FirstPart.CurrentLength;

            CopyVMString(VMStringPushValue.Contents,
                         VMStringPushValue.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += VMStringPushValue.CurrentLength;
            LengthRemaining -= VMStringPushValue.CurrentLength;

            CopyVMString(SecondPart.Contents,
                         SecondPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += SecondPart.CurrentLength;
            LengthRemaining -= SecondPart.CurrentLength;
        }
        ASMInstructions->Contents[ASMInstructions->CurrentLength] = '\0';
        InstructionCounts->PushArgumentCount++;       
    }
    else if(VMStringsAreEqual(&VMStringPushSegment, &LocalString))
    {
        /* NOTE(Marko): "push local X" translates to
                            @LCL
                            D=M
                            @X
                            A=D+A
                            D=M
                            @SP
                            A=M
                            M=D
                            @SP
                            M=M+1

                        43 characters excluding X (where X is the address number)
        */
        vm_string FirstPart = {"@LCL\nD=M\n@",10,11};
        vm_string SecondPart = {"\nA=D+A\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",33,34};
        ASMInstructions->CurrentLength = 
            FirstPart.CurrentLength + 
            SecondPart.CurrentLength + 
            VMStringPushValue.CurrentLength;
        if(ASMInstructions->MemorySize <= ASMInstructions->CurrentLength)
        {
            GrowVMString(ASMInstructions);
        }
        {
            char *PasteCharLocation = ASMInstructions->Contents;
            uint32 LengthRemaining = ASMInstructions->CurrentLength;

            CopyVMString(FirstPart.Contents,
                         FirstPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += FirstPart.CurrentLength;
            LengthRemaining -= FirstPart.CurrentLength;

            CopyVMString(VMStringPushValue.Contents,
                         VMStringPushValue.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += VMStringPushValue.CurrentLength;
            LengthRemaining -= VMStringPushValue.CurrentLength;

            CopyVMString(SecondPart.Contents,
                         SecondPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += SecondPart.CurrentLength;
            LengthRemaining -= SecondPart.CurrentLength;
        }
        ASMInstructions->Contents[ASMInstructions->CurrentLength] = '\0';
        InstructionCounts->PushLocalCount++;        
    }
    else if(VMStringsAreEqual(&VMStringPushSegment, &StaticString))
    {
        
    }
    else if(VMStringsAreEqual(&VMStringPushSegment, &ConstantString))
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
        vm_string FirstPart = {"@",1,2};
        vm_string SecondPart = {"\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",27,28};
        ASMInstructions->CurrentLength = 
            FirstPart.CurrentLength + 
            SecondPart.CurrentLength + 
            VMStringPushValue.CurrentLength;
        if(ASMInstructions->MemorySize <= ASMInstructions->CurrentLength)
        {
            GrowVMString(ASMInstructions);
        }
        {
            char *PasteCharLocation = ASMInstructions->Contents;
            uint32 LengthRemaining = ASMInstructions->CurrentLength;

            CopyVMString(FirstPart.Contents,
                         FirstPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += FirstPart.CurrentLength;
            LengthRemaining -= FirstPart.CurrentLength;

            CopyVMString(VMStringPushValue.Contents,
                         VMStringPushValue.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += VMStringPushValue.CurrentLength;
            LengthRemaining -= VMStringPushValue.CurrentLength;

            CopyVMString(SecondPart.Contents,
                         SecondPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += SecondPart.CurrentLength;
            LengthRemaining -= SecondPart.CurrentLength;
        }
        ASMInstructions->Contents[ASMInstructions->CurrentLength] = '\0';
        InstructionCounts->PushConstantCount++;
    }
    else if(VMStringsAreEqual(&VMStringPushSegment, &ThisString))
    {
        /* NOTE(Marko): "push this X" translates to
                            @THIS
                            D=M
                            @X
                            A=D+A
                            D=M
                            @SP
                            A=M
                            M=D
                            @SP
                            M=M+1

                        43 characters excluding X (where X is the address number)
        */
        vm_string FirstPart = {"@THIS\nD=M\n@",11,12};
        vm_string SecondPart = {"\nA=D+A\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",33,34};
        ASMInstructions->CurrentLength = 
            FirstPart.CurrentLength + 
            SecondPart.CurrentLength + 
            VMStringPushValue.CurrentLength;
        if(ASMInstructions->MemorySize <= ASMInstructions->CurrentLength)
        {
            GrowVMString(ASMInstructions);
        }
        {
            char *PasteCharLocation = ASMInstructions->Contents;
            uint32 LengthRemaining = ASMInstructions->CurrentLength;

            CopyVMString(FirstPart.Contents,
                         FirstPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += FirstPart.CurrentLength;
            LengthRemaining -= FirstPart.CurrentLength;

            CopyVMString(VMStringPushValue.Contents,
                         VMStringPushValue.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += VMStringPushValue.CurrentLength;
            LengthRemaining -= VMStringPushValue.CurrentLength;

            CopyVMString(SecondPart.Contents,
                         SecondPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += SecondPart.CurrentLength;
            LengthRemaining -= SecondPart.CurrentLength;
        }
        ASMInstructions->Contents[ASMInstructions->CurrentLength] = '\0';
        InstructionCounts->PushThisCount++;       
    }
    else if(VMStringsAreEqual(&VMStringPushSegment, &ThatString))
    {
        /* NOTE(Marko): "push that X" translates to
                            @THAT
                            D=M
                            @X
                            A=D+A
                            D=M
                            @SP
                            A=M
                            M=D
                            @SP
                            M=M+1

                        43 characters excluding X (where X is the address number)
        */
        vm_string FirstPart = {"@THAT\nD=M\n@",11,12};
        vm_string SecondPart = {"\nA=D+A\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",33,34};
        ASMInstructions->CurrentLength = 
            FirstPart.CurrentLength + 
            SecondPart.CurrentLength + 
            VMStringPushValue.CurrentLength;
        if(ASMInstructions->MemorySize <= ASMInstructions->CurrentLength)
        {
            GrowVMString(ASMInstructions);
        }
        {
            char *PasteCharLocation = ASMInstructions->Contents;
            uint32 LengthRemaining = ASMInstructions->CurrentLength;

            CopyVMString(FirstPart.Contents,
                         FirstPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += FirstPart.CurrentLength;
            LengthRemaining -= FirstPart.CurrentLength;

            CopyVMString(VMStringPushValue.Contents,
                         VMStringPushValue.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += VMStringPushValue.CurrentLength;
            LengthRemaining -= VMStringPushValue.CurrentLength;

            CopyVMString(SecondPart.Contents,
                         SecondPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += SecondPart.CurrentLength;
            LengthRemaining -= SecondPart.CurrentLength;
        }
        ASMInstructions->Contents[ASMInstructions->CurrentLength] = '\0';
        InstructionCounts->PushThatCount++;              
    }
    else if(VMStringsAreEqual(&VMStringPushSegment, &PointerString))
    {
        
    }
    else if(VMStringsAreEqual(&VMStringPushSegment, &TempString))
    {
        /* NOTE(Marko): push temp X translates to:
                            @X
                            D=M
                            @SP
                            A=M
                            M=D
                            @SP
                            M=M+1

                        Where X is TEMP_ADDRESS_START + VMStringPopValue, and 
                        it must not exceed TEMP_ADDRESS_END.
                        Excluding X, there are 28 characters. 
        */
        uint32 PushAddressValue = 
            TEMP_ADDRESS_START + VMStringToUInt32(&VMStringPushValue);
        if(PushAddressValue > TEMP_ADDRESS_END)
        {
            // NOTE(Marko): PushAddressValue is not valid.
            InvalidCodePath;
        }
        vm_string VMStringPushAddress = UInt32ToVMString(PushAddressValue);

        vm_string FirstPart = {"@",1,2};
        vm_string SecondPart = {"\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",27,28};
        ASMInstructions->CurrentLength = 
            FirstPart.CurrentLength + 
            SecondPart.CurrentLength + 
            VMStringPushAddress.CurrentLength;
        if(ASMInstructions->MemorySize <= ASMInstructions->CurrentLength)
        {
            GrowVMString(ASMInstructions);
        }

        {
            char *PasteCharLocation = ASMInstructions->Contents;
            uint32 LengthRemaining = ASMInstructions->CurrentLength;

            CopyVMString(FirstPart.Contents,
                         FirstPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += FirstPart.CurrentLength;
            LengthRemaining -= FirstPart.CurrentLength;

            CopyVMString(VMStringPushAddress.Contents,
                         VMStringPushAddress.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += VMStringPushAddress.CurrentLength;
            LengthRemaining -= VMStringPushAddress.CurrentLength;

            CopyVMString(SecondPart.Contents,
                         SecondPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += SecondPart.CurrentLength;
            LengthRemaining -= SecondPart.CurrentLength;
        }
        ASMInstructions->Contents[ASMInstructions->CurrentLength] = '\0';
        InstructionCounts->PushTempCount++;   
    }
    else
    {
        InvalidCodePath;
    }

}


internal void ParseReturnCommand(vm_string *ASMInstructions)
{

}


internal void ParseArithmeticCommand(vm_tokens *VMTokens, 
                                     vm_string *ASMInstructions,
                                     instruction_counts *InstructionCounts)
{
    Assert(VMTokens->VMTokenCount == 1);
    vm_string VMStringArithmeticCommand = VMTokens->VMTokens[0];
    /* NOTE(Marko): Possible arithmetic commands:
                                "add"
                                "sub"
                                "neg"
                                "eq"
                                "gt"
                                "lt"
                                "and"
                                "or"
                                "not"
            */
    vm_string AddString = {"add", 3, 4};
    vm_string SubString = {"sub", 3, 4};
    vm_string NegString = {"neg", 3, 4};
    vm_string EqString = {"eq", 2, 3};
    vm_string GtString = {"gt", 2, 3};
    vm_string LtString = {"lt", 2, 3};
    vm_string AndString = {"and", 3, 4};
    vm_string OrString = {"or", 2, 3};
    vm_string NotString = {"not", 3, 4};

    vm_string ArithmeticAsm;
    if(VMStringsAreEqual(&VMStringArithmeticCommand, &AddString))
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

                        Nothing depends on the input, so we can just hard code 
                        it in. 
        */
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

        InstructionCounts->AddCount++;
    }
    else if(VMStringsAreEqual(&VMStringArithmeticCommand, &SubString))
    {
        // NOTE(Marko): Executes X-Y, where the stack looks like:
        //                |  Address   |    Memory    |
        //                |---------------------------|
        //                |    256     |      X       |
        //                |---------------------------|
        //                |    257     |      Y       |
        //                |---------------------------|
        //     SP ----->  |    258     |     ???      |
        //                |---------------------------|

        /* NOTE(Marko): "sub" translates to
                            @SP
                            M=M-1
                            A=M
                            D=M
                            @SP
                            M=M-1
                            A=M
                            M=M-D
                            @SP
                            M=M+1

                        Nothing depends on the input, so we can just hard code 
                        it in. 
        */
        ArithmeticAsm.Contents = "@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=M-D\n@SP\nM=M+1\n";
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

        InstructionCounts->SubCount++;

    }
    else if(VMStringsAreEqual(&VMStringArithmeticCommand, &NegString))
    {
        // NOTE(Marko): Negates value at top of stack.

        /* NOTE(Marko): "neg" translates to:
                            @SP
                            M=M-1
                            A=M
                            M=-M
                            @SP
                            M=M+1

                        Nothing depends on the input, so we can just hard code 
                        it in. 

        */
        ArithmeticAsm.Contents = "@SP\nM=M-1\nA=M\nM=-M\n@SP\nM=M+1\n";
        ArithmeticAsm.CurrentLength = 29;
        ArithmeticAsm.MemorySize = 30;

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

        InstructionCounts->NegCount++;
    }
    else if(VMStringsAreEqual(&VMStringArithmeticCommand, &EqString))
    {

        // NOTE(Marko): Outputs: 0000000000000000 for false (aka 0)
        //                       1111111111111111 for true (aka -1)

        /* NOTE(Marko): "eq" translates to
                            @SP
                            M=M-1
                            A=M
                            D=M
                            @SP
                            M=M-1
                            A=M
                            D=M-D
                            @NOT_EQUAL_X
                            D;JNE
                            (EQUAL_X)
                            @SP
                            A=M
                            M=-1
                            @EQUAL_END_X
                            0;JMP
                            (NOT_EQUAL_X)
                            @SP
                            A=M
                            M=0
                            @EQUAL_END_X
                            0;JMP
                            (EQUAL_END_X)
                            @SP
                            M=M+1

                        Excluding the X's (which is the EqCount), there are 
                        162 characters.
                        There are 6 slots in which to place the EqCount. 
        */
        vm_string EqCountString = UInt32ToVMString(InstructionCounts->EqCount);
        vm_string FirstPart = {"@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=M-D\n@NOT_EQUAL_",49,50};
        vm_string SecondPart = {"\nD;JNE\n(EQUAL_",14,15};
        vm_string ThirdPart = {")\n@SP\nA=M\nM=-1\n@EQUAL_END_",26,27};
        vm_string FourthPart = {"\n0;JMP\n(NOT_EQUAL_",18,19};
        vm_string FifthPart = {")\n@SP\nA=M\nM=0\n@EQUAL_END_",25,26};
        vm_string SixthPart = {"\n0;JMP\n(EQUAL_END_",18,19};
        vm_string SeventhPart = {")\n@SP\nM=M+1\n",12,13};

        ASMInstructions->CurrentLength = 
            FirstPart.CurrentLength +
            SecondPart.CurrentLength +
            ThirdPart.CurrentLength +
            FourthPart.CurrentLength +
            FifthPart.CurrentLength +
            SixthPart.CurrentLength +
            SeventhPart.CurrentLength +
            6*EqCountString.CurrentLength;

        if(ASMInstructions->MemorySize <= ASMInstructions->CurrentLength)
        {
            GrowVMString(ASMInstructions);
        }
        // NOTE(Marko): Copying strings. Scoped out for text-editing purposes. 
        {
            char *PasteCharLocation = ASMInstructions->Contents;
            uint32 LengthRemaining = ASMInstructions->CurrentLength;

            CopyVMString(FirstPart.Contents,
                         FirstPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += FirstPart.CurrentLength;
            LengthRemaining -= FirstPart.CurrentLength;
            CopyVMString(EqCountString.Contents,
                         EqCountString.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += EqCountString.CurrentLength;
            LengthRemaining -= EqCountString.CurrentLength;

            CopyVMString(SecondPart.Contents,
                         SecondPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += SecondPart.CurrentLength;
            LengthRemaining -= SecondPart.CurrentLength;
            CopyVMString(EqCountString.Contents,
                         EqCountString.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += EqCountString.CurrentLength;
            LengthRemaining -= EqCountString.CurrentLength;

            CopyVMString(ThirdPart.Contents,
                         ThirdPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += ThirdPart.CurrentLength;
            LengthRemaining -= ThirdPart.CurrentLength;
            CopyVMString(EqCountString.Contents,
                         EqCountString.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += EqCountString.CurrentLength;
            LengthRemaining -= EqCountString.CurrentLength;

            CopyVMString(FourthPart.Contents,
                         FourthPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += FourthPart.CurrentLength;
            LengthRemaining -= FourthPart.CurrentLength;
            CopyVMString(EqCountString.Contents,
                         EqCountString.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += EqCountString.CurrentLength;
            LengthRemaining -= EqCountString.CurrentLength;

            CopyVMString(FifthPart.Contents,
                         FifthPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += FifthPart.CurrentLength;
            LengthRemaining -= FifthPart.CurrentLength;
            CopyVMString(EqCountString.Contents,
                         EqCountString.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += EqCountString.CurrentLength;
            LengthRemaining -= EqCountString.CurrentLength;

            CopyVMString(SixthPart.Contents,
                         SixthPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += SixthPart.CurrentLength;
            LengthRemaining -= SixthPart.CurrentLength;
            CopyVMString(EqCountString.Contents,
                         EqCountString.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += EqCountString.CurrentLength;
            LengthRemaining -= EqCountString.CurrentLength;

            CopyVMString(SeventhPart.Contents,
                         SeventhPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);

        }
        ASMInstructions->Contents[ASMInstructions->CurrentLength] = '\0';
        InstructionCounts->EqCount++;
    }
    else if(VMStringsAreEqual(&VMStringArithmeticCommand, &GtString))
    {
        // NOTE(Marko): Checks if X > Y, where the stack looks like:
        //                |  Address   |    Memory    |
        //                |---------------------------|
        //                |    256     |      X       |
        //                |---------------------------|
        //                |    257     |      Y       |
        //                |---------------------------|
        //     SP ----->  |    258     |     ???      |
        //                |---------------------------|

        // NOTE(Marko): Outputs: 0000000000000000 for false (aka 0)
        //                       1111111111111111 for true (aka -1)

        /*
            NOTE(Marko): "gt" translates to
                            @SP
                            M=M-1
                            A=M
                            D=M
                            @SP
                            M=M-1
                            A=M
                            D=M-D
                            @NOT_GT_X
                            D;JLE
                            (GT_X)
                            @SP
                            A=M
                            M=-1
                            @GT_END_X
                            0;JMP
                            (NOT_GT_X)
                            @SP
                            A=M
                            M=0
                            @GT_END_X
                            0;JMP
                            (GT_END_X)
                            @SP
                            M=M+1

                        Excluding the X's (which is the GtCount), there are 
                        144 characters.
                        There are 6 slots in which to place the 
                        GtCount.        
        */
        vm_string GtCountString = UInt32ToVMString(InstructionCounts->GtCount);
        vm_string FirstPart = {"@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=M-D\n@NOT_GT_",46,47};
        vm_string SecondPart = {"\nD;JLE\n(GT_",11,12};
        vm_string ThirdPart = {")\n@SP\nA=M\nM=-1\n@GT_END_",23,24};
        vm_string FourthPart = {"\n0;JMP\n(NOT_GT_",15,16};
        vm_string FifthPart = {")\n@SP\nA=M\nM=0\n@GT_END_",22,23};
        vm_string SixthPart = {"\n0;JMP\n(GT_END_",15,16};
        vm_string SeventhPart = {")\n@SP\nM=M+1\n",12,13};

        ASMInstructions->CurrentLength = 
            FirstPart.CurrentLength +
            SecondPart.CurrentLength +
            ThirdPart.CurrentLength +
            FourthPart.CurrentLength +
            FifthPart.CurrentLength +
            SixthPart.CurrentLength +
            SeventhPart.CurrentLength +
            6*GtCountString.CurrentLength;

        if(ASMInstructions->MemorySize <= ASMInstructions->CurrentLength)
        {
            GrowVMString(ASMInstructions);
        }

        // NOTE(Marko): Copying strings. Scoped out for text-editing purposes. 
        {    
            char *PasteCharLocation = ASMInstructions->Contents;
            uint32 LengthRemaining = ASMInstructions->CurrentLength;

            CopyVMString(FirstPart.Contents,
                         FirstPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += FirstPart.CurrentLength;
            LengthRemaining -= FirstPart.CurrentLength;
            CopyVMString(GtCountString.Contents,
                         GtCountString.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += GtCountString.CurrentLength;
            LengthRemaining -= GtCountString.CurrentLength;

            CopyVMString(SecondPart.Contents,
                         SecondPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += SecondPart.CurrentLength;
            LengthRemaining -= SecondPart.CurrentLength;
            CopyVMString(GtCountString.Contents,
                         GtCountString.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += GtCountString.CurrentLength;
            LengthRemaining -= GtCountString.CurrentLength;

            CopyVMString(ThirdPart.Contents,
                         ThirdPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += ThirdPart.CurrentLength;
            LengthRemaining -= ThirdPart.CurrentLength;
            CopyVMString(GtCountString.Contents,
                         GtCountString.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += GtCountString.CurrentLength;
            LengthRemaining -= GtCountString.CurrentLength;

            CopyVMString(FourthPart.Contents,
                         FourthPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += FourthPart.CurrentLength;
            LengthRemaining -= FourthPart.CurrentLength;
            CopyVMString(GtCountString.Contents,
                         GtCountString.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += GtCountString.CurrentLength;
            LengthRemaining -= GtCountString.CurrentLength;

            CopyVMString(FifthPart.Contents,
                         FifthPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += FifthPart.CurrentLength;
            LengthRemaining -= FifthPart.CurrentLength;
            CopyVMString(GtCountString.Contents,
                         GtCountString.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += GtCountString.CurrentLength;
            LengthRemaining -= GtCountString.CurrentLength;

            CopyVMString(SixthPart.Contents,
                         SixthPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += SixthPart.CurrentLength;
            LengthRemaining -= SixthPart.CurrentLength;
            CopyVMString(GtCountString.Contents,
                         GtCountString.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += GtCountString.CurrentLength;
            LengthRemaining -= GtCountString.CurrentLength;

            CopyVMString(SeventhPart.Contents,
                         SeventhPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
        }

        ASMInstructions->Contents[ASMInstructions->CurrentLength] = '\0';

        InstructionCounts->GtCount++;
    }
    else if(VMStringsAreEqual(&VMStringArithmeticCommand, &LtString))
    {
        // NOTE(Marko): Checks if X < Y, where the stack looks like:
        //                |  Address   |    Memory    |
        //                |---------------------------|
        //                |    256     |      X       |
        //                |---------------------------|
        //                |    257     |      Y       |
        //                |---------------------------|
        //     SP ----->  |    258     |     ???      |
        //                |---------------------------|

        // NOTE(Marko): Outputs: 0000000000000000 for false (aka 0)
        //                       1111111111111111 for true (aka -1)

        /*
            NOTE(Marko): "lt" translates to
                            @SP
                            M=M-1
                            A=M
                            D=M
                            @SP
                            M=M-1
                            A=M
                            D=M-D
                            @NOT_LT_X
                            D;JGE
                            (LT_X)
                            @SP
                            A=M
                            M=-1
                            @LT_END_X
                            0;JMP
                            (NOT_LT_X)
                            @SP
                            A=M
                            M=0
                            @LT_END_X
                            0;JMP
                            (LT_END_X)
                            @SP
                            M=M+1

                        Excluding the X's (which is the LtCount), there are 
                        144 characters.
                        There are 6 slots in which to place the 
                        LtCount.        
        */
        vm_string LtCountString = UInt32ToVMString(InstructionCounts->LtCount);
        vm_string FirstPart = {"@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=M-D\n@NOT_LT_",46,47};
        vm_string SecondPart = {"\nD;JGE\n(LT_",11,12};
        vm_string ThirdPart = {")\n@SP\nA=M\nM=-1\n@LT_END_",23,24};
        vm_string FourthPart = {"\n0;JMP\n(NOT_LT_",15,16};
        vm_string FifthPart = {")\n@SP\nA=M\nM=0\n@LT_END_",22,23};
        vm_string SixthPart = {"\n0;JMP\n(LT_END_",15,16};
        vm_string SeventhPart = {")\n@SP\nM=M+1\n",12,13};

        ASMInstructions->CurrentLength = 
            FirstPart.CurrentLength +
            SecondPart.CurrentLength +
            ThirdPart.CurrentLength +
            FourthPart.CurrentLength +
            FifthPart.CurrentLength +
            SixthPart.CurrentLength +
            SeventhPart.CurrentLength +
            6*LtCountString.CurrentLength;

        if(ASMInstructions->MemorySize <= ASMInstructions->CurrentLength)
        {
            GrowVMString(ASMInstructions);
        }

        // NOTE(Marko): Copying strings. Scoped out for text-editing purposes. 
        {    
            char *PasteCharLocation = ASMInstructions->Contents;
            uint32 LengthRemaining = ASMInstructions->CurrentLength;

            CopyVMString(FirstPart.Contents,
                         FirstPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += FirstPart.CurrentLength;
            LengthRemaining -= FirstPart.CurrentLength;
            CopyVMString(LtCountString.Contents,
                         LtCountString.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += LtCountString.CurrentLength;
            LengthRemaining -= LtCountString.CurrentLength;

            CopyVMString(SecondPart.Contents,
                         SecondPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += SecondPart.CurrentLength;
            LengthRemaining -= SecondPart.CurrentLength;
            CopyVMString(LtCountString.Contents,
                         LtCountString.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += LtCountString.CurrentLength;
            LengthRemaining -= LtCountString.CurrentLength;

            CopyVMString(ThirdPart.Contents,
                         ThirdPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += ThirdPart.CurrentLength;
            LengthRemaining -= ThirdPart.CurrentLength;
            CopyVMString(LtCountString.Contents,
                         LtCountString.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += LtCountString.CurrentLength;
            LengthRemaining -= LtCountString.CurrentLength;

            CopyVMString(FourthPart.Contents,
                         FourthPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += FourthPart.CurrentLength;
            LengthRemaining -= FourthPart.CurrentLength;
            CopyVMString(LtCountString.Contents,
                         LtCountString.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += LtCountString.CurrentLength;
            LengthRemaining -= LtCountString.CurrentLength;

            CopyVMString(FifthPart.Contents,
                         FifthPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += FifthPart.CurrentLength;
            LengthRemaining -= FifthPart.CurrentLength;
            CopyVMString(LtCountString.Contents,
                         LtCountString.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += LtCountString.CurrentLength;
            LengthRemaining -= LtCountString.CurrentLength;

            CopyVMString(SixthPart.Contents,
                         SixthPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += SixthPart.CurrentLength;
            LengthRemaining -= SixthPart.CurrentLength;
            CopyVMString(LtCountString.Contents,
                         LtCountString.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += LtCountString.CurrentLength;
            LengthRemaining -= LtCountString.CurrentLength;

            CopyVMString(SeventhPart.Contents,
                         SeventhPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
        }

        ASMInstructions->Contents[ASMInstructions->CurrentLength] = '\0';

        InstructionCounts->LtCount++;
    }
    else if(VMStringsAreEqual(&VMStringArithmeticCommand, &AndString))
    {
        /* NOTE(Marko): "and" translates to:
                            @SP
                            M=M-1
                            A=M
                            D=M
                            @SP
                            M=M-1
                            A=M
                            M=M&D
                            @SP
                            M=M+1

                        Nothing depends on the input, so we can just hard code 
                        it in. 
        */
        ArithmeticAsm.Contents = "@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=M&D\n@SP\nM=M+1\n";
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

        InstructionCounts->AndCount++;
    }
    else if(VMStringsAreEqual(&VMStringArithmeticCommand, &OrString))
    {        
        /* NOTE(Marko): "or" translates to:
                            @SP
                            M=M-1
                            A=M
                            D=M
                            @SP
                            M=M-1
                            A=M
                            M=M|D
                            @SP
                            M=M+1

                        Nothing depends on the input, so we can just hard code 
                        it in. 
        */
        ArithmeticAsm.Contents = "@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=M|D\n@SP\nM=M+1\n";
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

        InstructionCounts->OrCount++;
    }
    else if(VMStringsAreEqual(&VMStringArithmeticCommand, &NotString))
    {
        // NOTE(Marko): Bitwise NOT of value at top of stack.

        /* NOTE(Marko): "neg" translates to:
                            @SP
                            M=M-1
                            A=M
                            M=!M
                            @SP
                            M=M+1
        */
        ArithmeticAsm.Contents = "@SP\nM=M-1\nA=M\nM=!M\n@SP\nM=M+1\n";
        ArithmeticAsm.CurrentLength = 29;
        ArithmeticAsm.MemorySize = 30;

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

        InstructionCounts->NotCount++;
    }
    else
    {
        InvalidCodePath;
    }
}

void ParseTokensToASM(vm_tokens *VMTokens,
                      vm_string *ASMInstructions,
                      instruction_counts *InstructionCounts)
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
                ParseArithmeticCommand(VMTokens, 
                                       ASMInstructions, 
                                       InstructionCounts);
            }
        } break;

        case 2:
        {

        } break;

        case 3:
        {
            if(VMStringsAreEqual(&VMTokens->VMTokens[0], "push", 4))
            {
                ParsePushCommand(VMTokens, ASMInstructions, InstructionCounts);
            }
            else if(VMStringsAreEqual(&VMTokens->VMTokens[0], "pop", 3))
            {
                ParsePopCommand(VMTokens, ASMInstructions, InstructionCounts);
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