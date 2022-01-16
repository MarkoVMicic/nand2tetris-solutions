#include "vm_tokens.h"
#include "vm_string.h"

// NOTE(Marko): global variable declared in vm_main.exe
global_variable vm_string GlobalProgramName;

//
//  NOTE(Marko): VMTokens Memory Management
//
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

vm_tokens *AllocateVMTokens(uint32 TokenCount, 
                            uint32 TokenVMStringSize, 
                            vm_error_list *ErrorList)
{
    vm_tokens *Result = (vm_tokens *)malloc(sizeof(vm_tokens));
    if(Result != 0)
    {
        Result->VMTokens = (vm_string *)malloc(TokenCount*sizeof(vm_string));
        if(Result->VMTokens != 0)
        {
             Result->VMTokenCount = TokenCount;
            for(uint32 TokenIndex = 0; 
                TokenIndex < Result->VMTokenCount; 
                TokenIndex++)
            {
                vm_string *CurrentString = 
                    AllocateVMString(TokenVMStringSize);
                Result->VMTokens[TokenIndex] = *CurrentString;
            }
        }
        else
        {
            vm_string Error = ConstructVMStringFromCString("malloc failed while allocating array of vm_string pointers in VMTokens.");
            AddErrorToErrorList(ErrorList, &Error);
        }
    }
    else
    {
        vm_string Error = ConstructVMStringFromCString("malloc failed while allocating vm_token struct.");
        AddErrorToErrorList(ErrorList, &Error);
    }
    

    return(Result);
}

//
//  NOTE(Marko): Pop Commands
//

internal void ParsePopCommand(vm_tokens *VMTokens,
                              vm_string *ASMInstructions,
                              instruction_counts *InstructionCounts,
                              vm_error_list *ErrorList)
{
    Assert(VMTokens->VMTokenCount == 3);

    vm_string VMStringPopSegment = VMTokens->VMTokens[1];
    vm_string VMStringPopValue = VMTokens->VMTokens[2];

    vm_string ArgumentString = ConstructVMStringFromCString("argument");
    vm_string LocalString = ConstructVMStringFromCString("local");
    vm_string StaticString = ConstructVMStringFromCString("static");
    vm_string ConstantString = ConstructVMStringFromCString("constant");
    vm_string ThisString = ConstructVMStringFromCString("this");
    vm_string ThatString = ConstructVMStringFromCString("that");
    vm_string PointerString = ConstructVMStringFromCString("pointer");
    vm_string TempString = ConstructVMStringFromCString("temp");

    // TODO(Marko): Bounds checking to make sure that VMStringPopValue corresponds to a valid address (i.e. does not go out of bounds)
    if(VMStringsAreEqual(&VMStringPopSegment, &ArgumentString))
    {
        /* NOTE(Marko): "pop argument X" translates to
                            @ARG
                            D=M
                            @X
                            D=D+A
                            @R13
                            M=D
                            @SP
                            M=M-1
                            A=M
                            D=M
                            @R13
                            A=M
                            M=D

                        Excluding X (which is the specified local section) there are 57 characters.  
        */
        vm_string FirstPart = ConstructVMStringFromCString("@ARG\nD=M\n@");
        vm_string SecondPart = ConstructVMStringFromCString("\nD=D+A\n@R13\nM=D\n@SP\nM=M-1\nA=M\nD=M\n@R13\nA=M\nM=D\n");

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
                            @R13
                            M=D
                            @SP
                            M=M-1
                            A=M
                            D=M
                            @R13
                            A=M
                            M=D

                        Excluding X (which is the specified local section) there are 57 characters.  
        */
        vm_string FirstPart = ConstructVMStringFromCString("@LCL\nD=M\n@");
        vm_string SecondPart = ConstructVMStringFromCString("\nD=D+A\n@R13\nM=D\n@SP\nM=M-1\nA=M\nD=M\n@R13\nA=M\nM=D\n");

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
        /* NOTE(Marko): "pop static i" translates to
                            @SP
                            M=M-1
                            A=M
                            D=M
                            @<program_name>.i
                            M=D

                        where <program_name> is the contents of 
                        GlobalProgramName.
                        Excluding <program_name>.i there are 24 characters. 
       */

        vm_string FirstPart = ConstructVMStringFromCString("@SP\nM=M-1\nA=M\nD=M\n@");
        vm_string SecondPart = ConstructVMStringFromCString("\nM=D\n");
        vm_string Dot = ConstructVMStringFromCString(".");

        ASMInstructions->CurrentLength =
            FirstPart.CurrentLength +
            GlobalProgramName.CurrentLength +
            Dot.CurrentLength +
            VMStringPopValue.CurrentLength +
            SecondPart.CurrentLength;
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

            CopyVMString(GlobalProgramName.Contents,
                         GlobalProgramName.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += GlobalProgramName.CurrentLength;
            LengthRemaining -= GlobalProgramName.CurrentLength;

            CopyVMString(Dot.Contents,
                         Dot.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += Dot.CurrentLength;
            LengthRemaining -= Dot.CurrentLength;

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
        InstructionCounts->PopStaticCount++; 


    }
    else if(VMStringsAreEqual(&VMStringPopSegment, &ConstantString))
    {
        vm_string Error = ConstructVMStringFromCString("pop constant i is an invalid command.");
        AddErrorToErrorList(ErrorList, &Error);
        ASMInstructions->CurrentLength = 0;
    }
    else if(VMStringsAreEqual(&VMStringPopSegment, &ThisString))
    {
        /* NOTE(Marko): "pop this X" translates to
                            @THIS
                            D=M
                            @X
                            D=D+A
                            @R13
                            M=D
                            @SP
                            M=M-1
                            A=M
                            D=M
                            @R13
                            A=M
                            M=D

                        Excluding X (which is the specified local section) there are 58 characters.  
        */
        
        vm_string FirstPart = ConstructVMStringFromCString("@THIS\nD=M\n@");
        vm_string SecondPart = ConstructVMStringFromCString("\nD=D+A\n@R13\nM=D\n@SP\nM=M-1\nA=M\nD=M\n@R13\nA=M\nM=D\n");
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
                            @R13
                            M=D
                            @SP
                            M=M-1
                            A=M
                            D=M
                            @R13
                            A=M
                            M=D

                        Excluding X (which is the specified local section) there are 58 characters.  
        */
        vm_string FirstPart = ConstructVMStringFromCString("@THAT\nD=M\n@");
        vm_string SecondPart = ConstructVMStringFromCString("\nD=D+A\n@R13\nM=D\n@SP\nM=M-1\nA=M\nD=M\n@R13\nA=M\nM=D\n");

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
        vm_string ZeroString = ConstructVMStringFromCString("0");
        vm_string OneString = ConstructVMStringFromCString("1");
        vm_string BaseAddress;
        if(VMStringsAreEqual(&VMStringPopValue, &ZeroString))
        {
            BaseAddress = ConstructVMStringFromCString("THIS");
        }
        else if(VMStringsAreEqual(&VMStringPopValue, &OneString))
        {
            BaseAddress = ConstructVMStringFromCString("THAT");
        }
        else
        {
            // TODO(Marko): Restructure to avoid early return?
            vm_string Error = ConstructVMStringFromCString("pop pointer number incorrect: expected 0 or 1.");
            AddErrorToErrorList(ErrorList, &Error);
            ASMInstructions->CurrentLength = 0;
            // NOTE(Marko): Early return here!
            return;
        }

        vm_string FirstPart = ConstructVMStringFromCString("@SP\nM=M-1\nA=M\nD=M\n@");
        vm_string SecondPart = ConstructVMStringFromCString("\nM=D\n");

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
            // TODO(Marko): Restructure to avoid early return? 
            vm_string Error = ConstructVMStringFromCString("pop temp number was invalid. Expected 0,1,2,3,4,5,6, or 7.");
            AddErrorToErrorList(ErrorList, &Error);
            ASMInstructions->CurrentLength = 0;
            // NOTE(Marko): Early Return here! 
            return;
        }
        vm_string VMStringPopAddress = UInt32ToVMString(PopAddressValue);

        vm_string FirstPart = ConstructVMStringFromCString("@SP\nM=M-1\nA=M\nD=M\n@");
        vm_string SecondPart = ConstructVMStringFromCString("\nM=D\n");

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
        vm_string Error = ConstructVMStringFromCString("Unknown segment found while parsing pop command.");
        AddErrorToErrorList(ErrorList, &Error);
        ASMInstructions->CurrentLength = 0;
    }
}                              

//
//  NOTE(Marko): Push Commands
//

internal void ParsePushCommand(vm_tokens *VMTokens, 
                               vm_string *ASMInstructions,
                               instruction_counts *InstructionCounts,
                               vm_error_list *ErrorList)
{
    Assert(VMTokens->VMTokenCount == 3);

    vm_string VMStringPushSegment = VMTokens->VMTokens[1];
    vm_string VMStringPushValue = VMTokens->VMTokens[2];

    vm_string ArgumentString = ConstructVMStringFromCString("argument");
    vm_string LocalString = ConstructVMStringFromCString("local");
    vm_string StaticString = ConstructVMStringFromCString("static");
    vm_string ConstantString = ConstructVMStringFromCString("constant");
    vm_string ThisString = ConstructVMStringFromCString("this");
    vm_string ThatString = ConstructVMStringFromCString("that");
    vm_string PointerString = ConstructVMStringFromCString("pointer");
    vm_string TempString = ConstructVMStringFromCString("temp");

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
        vm_string FirstPart = ConstructVMStringFromCString("@ARG\nD=M\n@");
        vm_string SecondPart = ConstructVMStringFromCString("\nA=D+A\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
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
        vm_string FirstPart = ConstructVMStringFromCString("@LCL\nD=M\n@");
        vm_string SecondPart = ConstructVMStringFromCString("\nA=D+A\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
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
        /* NOTE(Marko): "push static i" translates to
                            @<program_name>.i
                            D=M
                            @SP
                            A=M
                            M=D
                            @SP
                            M=M+1

                        where <program_name> is the contents of 
                        GlobalProgramName.
                        Excluding <program_name>.i there are 28 characters. 
        */           

        vm_string FirstPart = ConstructVMStringFromCString("@");
        vm_string SecondPart = ConstructVMStringFromCString("\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
        vm_string Dot = ConstructVMStringFromCString(".");

        ASMInstructions->CurrentLength = 
            FirstPart.CurrentLength + 
            SecondPart.CurrentLength + 
            GlobalProgramName.CurrentLength +
            Dot.CurrentLength +
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

            CopyVMString(GlobalProgramName.Contents,
                         GlobalProgramName.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += GlobalProgramName.CurrentLength;
            LengthRemaining -= GlobalProgramName.CurrentLength;

            CopyVMString(Dot.Contents,
                         Dot.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += Dot.CurrentLength;
            LengthRemaining -= Dot.CurrentLength;

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
        InstructionCounts->PushStaticCount++; 
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
        vm_string FirstPart = ConstructVMStringFromCString("@");
        vm_string SecondPart = ConstructVMStringFromCString("\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
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
        vm_string FirstPart = ConstructVMStringFromCString("@THIS\nD=M\n@");
        vm_string SecondPart = ConstructVMStringFromCString("\nA=D+A\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
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
        vm_string FirstPart = ConstructVMStringFromCString("@THAT\nD=M\n@");
        vm_string SecondPart = ConstructVMStringFromCString("\nA=D+A\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
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
        /* NOTE(Marko): "push pointer 0" corresponds to pushing from base 
                        address of THIS.
                        This translates to
                            @THIS
                            D=M
                            @SP
                            A=M
                            M=D
                            @SP
                            M=M+1

                        "push pointer 1" corresponds to pushing from base 
                        address of THAT.
                        This translates to
                            @THAT
                            D=M
                            @SP
                            A=M
                            M=D
                            @SP
                            M=M+1

                        In both cases, there are a total of 32 characters
        */
        vm_string ZeroString = ConstructVMStringFromCString("0");
        vm_string OneString = ConstructVMStringFromCString("1");
        vm_string BaseAddress = {0};
        if(VMStringsAreEqual(&VMStringPushValue, &ZeroString))
        {
            BaseAddress = ConstructVMStringFromCString("THIS");
        }
        else if(VMStringsAreEqual(&VMStringPushValue, &OneString))
        {
            BaseAddress = ConstructVMStringFromCString("THAT");
        }
        else
        {
            // TODO(Marko): Perhaps restructure to avoid this early return. 
            vm_string Error = ConstructVMStringFromCString("push pointer number incorrect (wasn't 0 or 1).");
            AddErrorToErrorList(ErrorList, &Error);
            ASMInstructions->CurrentLength = 0;
            // NOTE(Marko): Early return here! 
            return;
        }

        vm_string FirstPart = ConstructVMStringFromCString("@");
        vm_string SecondPart = ConstructVMStringFromCString("\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");

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
        InstructionCounts->PushPointerCount++; 
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
            // TODO(Marko): Perhaps restructure to avoid this early return. 
            vm_string Error = ConstructVMStringFromCString("push temp had an invalid address. Expected 0,1,2,3,4,5,6 or 7.");
            AddErrorToErrorList(ErrorList, &Error);
            ASMInstructions->CurrentLength = 0;
            // NOTE(Marko): Early return here! 
            return;
        }
        vm_string VMStringPushAddress = UInt32ToVMString(PushAddressValue);

        vm_string FirstPart = ConstructVMStringFromCString("@");
        vm_string SecondPart = ConstructVMStringFromCString("\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
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
        vm_string Error = ConstructVMStringFromCString("Unknown segment found while parsing push command.");
        AddErrorToErrorList(ErrorList, &Error);
        ASMInstructions->CurrentLength = 0;
    }

}


//
//  NOTE(Marko): Arithmetic Commands
//


internal void ParseArithmeticCommand(vm_tokens *VMTokens, 
                                     vm_string *ASMInstructions,
                                     instruction_counts *InstructionCounts,
                                     vm_error_list *ErrorList)
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
    vm_string AddString = ConstructVMStringFromCString("add");
    vm_string SubString = ConstructVMStringFromCString("sub");
    vm_string NegString = ConstructVMStringFromCString("neg");
    vm_string EqString = ConstructVMStringFromCString("eq");
    vm_string GtString = ConstructVMStringFromCString("gt");
    vm_string LtString = ConstructVMStringFromCString("lt");
    vm_string AndString = ConstructVMStringFromCString("and");
    vm_string OrString = ConstructVMStringFromCString("or");
    vm_string NotString = ConstructVMStringFromCString("not");

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

        ArithmeticAsm = ConstructVMStringFromCString("@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=M+D\n@SP\nM=M+1\n");
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
        ArithmeticAsm = ConstructVMStringFromCString("@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=M-D\n@SP\nM=M+1\n");
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
        ArithmeticAsm = ConstructVMStringFromCString("@SP\nM=M-1\nA=M\nM=-M\n@SP\nM=M+1\n");

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
        // NOTE(Marko): EqCountString.Contents is heap allocated. Must call 
        //              FreeVMStringContents().
        vm_string EqCountString = UInt32ToVMString(InstructionCounts->EqCount);
        vm_string FirstPart = ConstructVMStringFromCString("@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=M-D\n@NOT_EQUAL_");
        vm_string SecondPart = ConstructVMStringFromCString("\nD;JNE\n(EQUAL_");
        vm_string ThirdPart = ConstructVMStringFromCString(")\n@SP\nA=M\nM=-1\n@EQUAL_END_");
        vm_string FourthPart = ConstructVMStringFromCString("\n0;JMP\n(NOT_EQUAL_");
        vm_string FifthPart = ConstructVMStringFromCString(")\n@SP\nA=M\nM=0\n@EQUAL_END_");
        vm_string SixthPart = ConstructVMStringFromCString("\n0;JMP\n(EQUAL_END_");
        vm_string SeventhPart = ConstructVMStringFromCString(")\n@SP\nM=M+1\n");

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
        FreeVMStringContents(&EqCountString);
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
        
        // NOTE(Marko): GtCountString.Contents is heap allocated. Must call 
        //              FreeVMStringContents().
        vm_string GtCountString = UInt32ToVMString(InstructionCounts->GtCount);
        vm_string FirstPart = ConstructVMStringFromCString("@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=M-D\n@NOT_GT_");
        vm_string SecondPart = ConstructVMStringFromCString("\nD;JLE\n(GT_");
        vm_string ThirdPart = ConstructVMStringFromCString(")\n@SP\nA=M\nM=-1\n@GT_END_");
        vm_string FourthPart = ConstructVMStringFromCString("\n0;JMP\n(NOT_GT_");
        vm_string FifthPart = ConstructVMStringFromCString(")\n@SP\nA=M\nM=0\n@GT_END_");
        vm_string SixthPart = ConstructVMStringFromCString("\n0;JMP\n(GT_END_");
        vm_string SeventhPart = ConstructVMStringFromCString(")\n@SP\nM=M+1\n");

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
        FreeVMStringContents(&GtCountString);
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
        
        // NOTE(Marko): LtCountstring.Contents is heap allocated. Must call 
        //              FreeVMStringContents()
        vm_string LtCountString = UInt32ToVMString(InstructionCounts->LtCount);
        vm_string FirstPart = ConstructVMStringFromCString("@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=M-D\n@NOT_LT_");
        vm_string SecondPart = ConstructVMStringFromCString("\nD;JGE\n(LT_");
        vm_string ThirdPart = ConstructVMStringFromCString(")\n@SP\nA=M\nM=-1\n@LT_END_");
        vm_string FourthPart = ConstructVMStringFromCString("\n0;JMP\n(NOT_LT_");
        vm_string FifthPart = ConstructVMStringFromCString(")\n@SP\nA=M\nM=0\n@LT_END_");
        vm_string SixthPart = ConstructVMStringFromCString("\n0;JMP\n(LT_END_");
        vm_string SeventhPart = ConstructVMStringFromCString(")\n@SP\nM=M+1\n");

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
        FreeVMStringContents(&LtCountString);
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
        ArithmeticAsm = ConstructVMStringFromCString("@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=M&D\n@SP\nM=M+1\n");
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
        ArithmeticAsm = ConstructVMStringFromCString("@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=M|D\n@SP\nM=M+1\n");
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
        ArithmeticAsm = ConstructVMStringFromCString("@SP\nM=M-1\nA=M\nM=!M\n@SP\nM=M+1\n");

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
       
        vm_string Error = ConstructVMStringFromCString("Unknown token found while trying to parse arithmetic commands.");
        AddErrorToErrorList(ErrorList, &Error);
        ASMInstructions->CurrentLength = 0;
    }
}

//
// NOTE(Marko): Branching commands
//

internal void ParseLabelCommand(vm_tokens *VMTokens,
                                vm_string *ASMInstructions,
                                instruction_counts *InstructionCounts,
                                vm_error_list *ErrorList)
{
    /*
        NOTE(Marko): "label <label>" corresponds to 

                        (<label>)    
    */

    vm_string LabelString = VMTokens->VMTokens[1];
    vm_string FirstPart = ConstructVMStringFromCString("(");
    vm_string SecondPart = ConstructVMStringFromCString(")\n");

    ASMInstructions->CurrentLength = 
        FirstPart.CurrentLength + 
        SecondPart.CurrentLength +
        LabelString.CurrentLength;
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

        CopyVMString(LabelString.Contents,
                     LabelString.CurrentLength,
                     PasteCharLocation,
                     LengthRemaining);
        PasteCharLocation += LabelString.CurrentLength;
        LengthRemaining -= LabelString.CurrentLength;

        CopyVMString(SecondPart.Contents,
                     SecondPart.CurrentLength,
                     PasteCharLocation,
                     LengthRemaining);
        PasteCharLocation += SecondPart.CurrentLength;
        LengthRemaining -= SecondPart.CurrentLength;
    }

    ASMInstructions->Contents[ASMInstructions->CurrentLength] = '\0';
    InstructionCounts->LabelCount++;
} 


internal void ParseGotoCommand(vm_tokens *VMTokens,
                               vm_string *ASMInstructions,
                               instruction_counts *InstructionCounts,
                               vm_error_list *ErrorList)
{
    /* NOTE(Marko): "goto <label>" corresponds to

                        @<label>
                        0;JMP
    */

    vm_string LabelString = VMTokens->VMTokens[1];
    vm_string FirstPart = ConstructVMStringFromCString("@");
    vm_string SecondPart = ConstructVMStringFromCString("\n0;JMP\n");

    ASMInstructions->CurrentLength = 
        FirstPart.CurrentLength + 
        SecondPart.CurrentLength +
        LabelString.CurrentLength;
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

        CopyVMString(LabelString.Contents,
                     LabelString.CurrentLength,
                     PasteCharLocation,
                     LengthRemaining);
        PasteCharLocation += LabelString.CurrentLength;
        LengthRemaining -= LabelString.CurrentLength;

        CopyVMString(SecondPart.Contents,
                     SecondPart.CurrentLength,
                     PasteCharLocation,
                     LengthRemaining);
        PasteCharLocation += SecondPart.CurrentLength;
        LengthRemaining -= SecondPart.CurrentLength;
    }

    ASMInstructions->Contents[ASMInstructions->CurrentLength] = '\0';
    InstructionCounts->GotoCount++;
}



internal void ParseIfGotoCommand(vm_tokens *VMTokens,
                                 vm_string *ASMInstructions,
                                 instruction_counts *InstructionCounts,
                                 vm_error_list *ErrorList)
{
    /* NOTE(Marko): "if-goto <label>" translates to

                        @SP
                        M=M-1
                        A=M
                        D=M
                        @<label>
                        D;JNE
    */

    vm_string LabelString = VMTokens->VMTokens[1];
    vm_string FirstPart = ConstructVMStringFromCString("@SP\nM=M-1\nA=M\nD=M\n@");
    vm_string SecondPart = ConstructVMStringFromCString("\nD;JNE\n");

    ASMInstructions->CurrentLength = 
        FirstPart.CurrentLength + 
        SecondPart.CurrentLength +
        LabelString.CurrentLength;
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

        CopyVMString(LabelString.Contents,
                     LabelString.CurrentLength,
                     PasteCharLocation,
                     LengthRemaining);
        PasteCharLocation += LabelString.CurrentLength;
        LengthRemaining -= LabelString.CurrentLength;

        CopyVMString(SecondPart.Contents,
                     SecondPart.CurrentLength,
                     PasteCharLocation,
                     LengthRemaining);
        PasteCharLocation += SecondPart.CurrentLength;
        LengthRemaining -= SecondPart.CurrentLength;
    }

    ASMInstructions->Contents[ASMInstructions->CurrentLength] = '\0';
    InstructionCounts->IfGotoCount++;
}

//
//  NOTE(Marko): Function Commands
//

internal void ParseFunctionCommand(vm_tokens *VMTokens,
                                   vm_string *ASMInstructions,
                                   instruction_counts *InstructionCounts,
                                   vm_error_list *ErrorList)
{
    /* NOTE(Marko): "function <name> <v>" indicates the start of a 
                    function with <v> local variables. 

                    It emits a label, and initializes all local variables to 0.
                    Note that since the caller of the function repositions LCL 
                    to the top of the stack, we can simply push 0 to the stack 
                    <v> times.
                    
                    This can be realized with

                        (<name>)
                        @<v>
                        D=A
                        (LOOP_<name>)
                        @SP
                        A=M
                        M=0
                        @SP
                        M=M+1
                        D=D-1
                        @LOOP_<name>
                        D;JGT

                    If there are no local variables, then we can shortcut to simply emit

                        (<name>)
    */

    vm_string FunctionNameString = VMTokens->VMTokens[1];
    vm_string LocalVariableCountString = VMTokens->VMTokens[2];
    uint32 LocalVariableCount = VMStringToUInt32(&LocalVariableCountString);

    if(LocalVariableCount == 0)
    {
        vm_string FirstPart = ConstructVMStringFromCString("(");
        vm_string SecondPart = ConstructVMStringFromCString(")\n");

        ASMInstructions->CurrentLength = 
            FirstPart.CurrentLength +
            SecondPart.CurrentLength +
            FunctionNameString.CurrentLength;
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

            CopyVMString(FunctionNameString.Contents,
                         FunctionNameString.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += FunctionNameString.CurrentLength;
            LengthRemaining -= FunctionNameString.CurrentLength;

            CopyVMString(SecondPart.Contents,
                         SecondPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += SecondPart.CurrentLength;
            LengthRemaining -= SecondPart.CurrentLength;
        }
    } // if(LocalVariableCount == 0)
    else // if(LocalVariableCount != 0)
    {
        vm_string FirstPart = ConstructVMStringFromCString("(");
        vm_string SecondPart = ConstructVMStringFromCString(")\n@");
        vm_string ThirdPart = ConstructVMStringFromCString("\nD=A\n(LOOP_");
        vm_string FourthPart = ConstructVMStringFromCString(")\n@SP\nA=M\nM=0\n@SP\nM=M+1\nD=D-1\n@LOOP_");
        vm_string FifthPart = ConstructVMStringFromCString("\nD;JGT\n");

        // TODO(Marko): Maybe it's better to add these lengths in the order 
        //              they appear in the assembly string. 
        ASMInstructions->CurrentLength = 
            FirstPart.CurrentLength +
            SecondPart.CurrentLength +
            ThirdPart.CurrentLength +
            FourthPart.CurrentLength +
            FifthPart.CurrentLength +
            3*FunctionNameString.CurrentLength +
            LocalVariableCountString.CurrentLength;
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

            CopyVMString(FunctionNameString.Contents,
                         FunctionNameString.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += FunctionNameString.CurrentLength;
            LengthRemaining -= FunctionNameString.CurrentLength;

            CopyVMString(SecondPart.Contents,
                         SecondPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += SecondPart.CurrentLength;
            LengthRemaining -= SecondPart.CurrentLength;

            CopyVMString(LocalVariableCountString.Contents,
                         LocalVariableCountString.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += LocalVariableCountString.CurrentLength;
            LengthRemaining -= LocalVariableCountString.CurrentLength;

            CopyVMString(ThirdPart.Contents,
                         ThirdPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += ThirdPart.CurrentLength;
            LengthRemaining -= ThirdPart.CurrentLength;

            CopyVMString(FunctionNameString.Contents,
                         FunctionNameString.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += FunctionNameString.CurrentLength;
            LengthRemaining -= FunctionNameString.CurrentLength;

            CopyVMString(FourthPart.Contents,
                         FourthPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += FourthPart.CurrentLength;
            LengthRemaining -= FourthPart.CurrentLength;

            CopyVMString(FunctionNameString.Contents,
                         FunctionNameString.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += FunctionNameString.CurrentLength;
            LengthRemaining -= FunctionNameString.CurrentLength;

            CopyVMString(FifthPart.Contents,
                         FifthPart.CurrentLength,
                         PasteCharLocation,
                         LengthRemaining);
            PasteCharLocation += FifthPart.CurrentLength;
            LengthRemaining -= FifthPart.CurrentLength;
        }
    } // else // if(LocalVariableCount != 0)
    ASMInstructions->Contents[ASMInstructions->CurrentLength] = '\0';
    InstructionCounts->FunctionCount++;
}


internal void ParseCallCommand(vm_tokens *VMTokens,
                               vm_string *ASMInstructions,
                               instruction_counts *InstructionCounts,
                               vm_error_list *ErrorList)
{
    /*
        NOTE(Marko): "call <name> <a>" calls the function with name <name>, 
                      passing <a> variables as arguments. 

                      Before "call <name> <a>", the vm code would have already 
                      pushed some arguments onto the stack. 

                      We then need to push the return address onto the stack, 
                      save the LCL, ARG, THIS, and THAT state onto the stack, 
                      reposition ARG and LCL for the called function to use, 
                      go to the function (via a label) and finally emit a 
                      label for the return address. 

                      To do this we have
                      
                      // Push Return Address
                                @RET_ADDRESS_CALLX
                                D=A
                                @SP
                                A=M
                                M=D
                                @SP
                                M=M+1
                      // Save state of current function:
                          // Push local address
                                @LCL
                                D=A
                                @SP
                                A=M
                                M=D
                                @SP
                                M=M+1
                          // Push argument address
                                @ARG
                                D=A
                                @SP
                                A=M
                                M=D
                                @SP
                                M=M+1
                          // Push this address
                                @THIS
                                D=A
                                @SP
                                A=M
                                M=D
                                @SP
                                M=M+1
                          // Push that address
                                @THAT
                                D=A
                                @SP
                                A=M
                                M=D
                                @SP
                                M=M+1
                      // Reposition ARG pointer to point to the function args
                         // Establish loop counter: <a> + 5 (since just pushed 
                         // 5 things onto the stack)
                                @<a+5>
                                D=A
                                (ARG_LOOP_X)
                                @SP
                                M=M-1
                                D=D-1
                                @ARG_LOOP_X
                                D;JGT
                        // Point ARG to the first arg
                                @SP
                                D=M
                                @ARG
                                M=D
                     // Reposition LCL pointer to point to top of stack. 
                         // Loop back up 
                                @<a+5>
                                D=A
                                (LCL_LOOP_X)
                                @SP
                                M=M+1
                                D=D-1
                                @LCL_LOOP_X
                                D;JGT
                         // Place LCL pointer
                                @SP
                                D=M
                                @LCL
                                M=D
                    // Go to function <name>
                                @<name>
                                0;JMP
                    // Emit return address label
                                (RET_ADDRESS_CALLX)
    */
    vm_string FunctionNameString = VMTokens->VMTokens[1];

    // NOTE(Marko): Compute ArgCount as 5+<a> where <a> is VMTokens[2]
    uint32 ArgCount = VMStringToUInt32(&VMTokens->VMTokens[2]);
    ArgCount += 5;
    vm_string ArgCountString = UInt32ToVMString(ArgCount);

    vm_string CallCountString = UInt32ToVMString(InstructionCounts->CallCount);

    vm_string FirstPart = ConstructVMStringFromCString("@RET_ADDRESS_CALL");
    vm_string SecondPart = ConstructVMStringFromCString("\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n@LCL\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n@ARG\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n@THIS\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n@THAT\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n@");
    vm_string ThirdPart = ConstructVMStringFromCString("\nD=A\n(ARG_LOOP_");
    vm_string FourthPart = ConstructVMStringFromCString(")\n@SP\nM=M-1\nD=D-1\n@ARG_LOOP_");
    vm_string FifthPart = ConstructVMStringFromCString("\nD;JGT\n@SP\nD=M\n@ARG\nM=D\n@");
    vm_string SixthPart = ConstructVMStringFromCString("\nD=A\n(LCL_LOOP_");
    vm_string SeventhPart = ConstructVMStringFromCString(")\n@SP\nM=M+1\nD=D-1\n@LCL_LOOP_");
    vm_string EighthPart = ConstructVMStringFromCString("\nD;JGT\n@SP\nD=M\n@LCL\nM=D\n@");
    vm_string NinthPart = ConstructVMStringFromCString("\n0;JMP\n(RET_ADDRESS_CALL");
    vm_string TenthPart = ConstructVMStringFromCString(")\n");


    ASMInstructions->CurrentLength = 
        FirstPart.CurrentLength + 
        CallCountString.CurrentLength + 
        SecondPart.CurrentLength + 
        ArgCountString.CurrentLength + 
        ThirdPart.CurrentLength + 
        CallCountString.CurrentLength + 
        FourthPart.CurrentLength + 
        CallCountString.CurrentLength + 
        FifthPart.CurrentLength + 
        ArgCountString.CurrentLength + 
        SixthPart.CurrentLength + 
        CallCountString.CurrentLength + 
        SeventhPart.CurrentLength + 
        CallCountString.CurrentLength + 
        EighthPart.CurrentLength + 
        FunctionNameString.CurrentLength + 
        NinthPart.CurrentLength + 
        CallCountString.CurrentLength + 
        TenthPart.CurrentLength;

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

        CopyVMString(CallCountString.Contents,
                     CallCountString.CurrentLength,
                     PasteCharLocation,
                     LengthRemaining);
        PasteCharLocation += CallCountString.CurrentLength;
        LengthRemaining -= CallCountString.CurrentLength;

        CopyVMString(SecondPart.Contents,
                     SecondPart.CurrentLength,
                     PasteCharLocation,
                     LengthRemaining);
        PasteCharLocation += SecondPart.CurrentLength;
        LengthRemaining -= SecondPart.CurrentLength;

        CopyVMString(ArgCountString.Contents,
                     ArgCountString.CurrentLength,
                     PasteCharLocation,
                     LengthRemaining);
        PasteCharLocation += ArgCountString.CurrentLength;
        LengthRemaining -= ArgCountString.CurrentLength;

        CopyVMString(ThirdPart.Contents,
                     ThirdPart.CurrentLength,
                     PasteCharLocation,
                     LengthRemaining);
        PasteCharLocation += ThirdPart.CurrentLength;
        LengthRemaining -= ThirdPart.CurrentLength;

        CopyVMString(CallCountString.Contents,
                     CallCountString.CurrentLength,
                     PasteCharLocation,
                     LengthRemaining);
        PasteCharLocation += CallCountString.CurrentLength;
        LengthRemaining -= CallCountString.CurrentLength;

        CopyVMString(FourthPart.Contents,
                     FourthPart.CurrentLength,
                     PasteCharLocation,
                     LengthRemaining);
        PasteCharLocation += FourthPart.CurrentLength;
        LengthRemaining -= FourthPart.CurrentLength;

        CopyVMString(CallCountString.Contents,
                     CallCountString.CurrentLength,
                     PasteCharLocation,
                     LengthRemaining);
        PasteCharLocation += CallCountString.CurrentLength;
        LengthRemaining -= CallCountString.CurrentLength;

        CopyVMString(FifthPart.Contents,
                     FifthPart.CurrentLength,
                     PasteCharLocation,
                     LengthRemaining);
        PasteCharLocation += FifthPart.CurrentLength;
        LengthRemaining -= FifthPart.CurrentLength;

        CopyVMString(ArgCountString.Contents,
                     ArgCountString.CurrentLength,
                     PasteCharLocation,
                     LengthRemaining);
        PasteCharLocation += ArgCountString.CurrentLength;
        LengthRemaining -= ArgCountString.CurrentLength;

        CopyVMString(SixthPart.Contents,
                     SixthPart.CurrentLength,
                     PasteCharLocation,
                     LengthRemaining);
        PasteCharLocation += SixthPart.CurrentLength;
        LengthRemaining -= SixthPart.CurrentLength;

        CopyVMString(CallCountString.Contents,
                     CallCountString.CurrentLength,
                     PasteCharLocation,
                     LengthRemaining);
        PasteCharLocation += CallCountString.CurrentLength;
        LengthRemaining -= CallCountString.CurrentLength;

        CopyVMString(SeventhPart.Contents,
                     SeventhPart.CurrentLength,
                     PasteCharLocation,
                     LengthRemaining);
        PasteCharLocation += SeventhPart.CurrentLength;
        LengthRemaining -= SeventhPart.CurrentLength;

        CopyVMString(CallCountString.Contents,
                     CallCountString.CurrentLength,
                     PasteCharLocation,
                     LengthRemaining);
        PasteCharLocation += CallCountString.CurrentLength;
        LengthRemaining -= CallCountString.CurrentLength;

        CopyVMString(EighthPart.Contents,
                     EighthPart.CurrentLength,
                     PasteCharLocation,
                     LengthRemaining);
        PasteCharLocation += EighthPart.CurrentLength;
        LengthRemaining -= EighthPart.CurrentLength;

        CopyVMString(FunctionNameString.Contents,
                     FunctionNameString.CurrentLength,
                     PasteCharLocation,
                     LengthRemaining);
        PasteCharLocation += FunctionNameString.CurrentLength;
        LengthRemaining -= FunctionNameString.CurrentLength;

        CopyVMString(NinthPart.Contents,
                     NinthPart.CurrentLength,
                     PasteCharLocation,
                     LengthRemaining);
        PasteCharLocation += NinthPart.CurrentLength;
        LengthRemaining -= NinthPart.CurrentLength;

        CopyVMString(CallCountString.Contents,
                     CallCountString.CurrentLength,
                     PasteCharLocation,
                     LengthRemaining);
        PasteCharLocation += CallCountString.CurrentLength;
        LengthRemaining -= CallCountString.CurrentLength;

        CopyVMString(TenthPart.Contents,
                     TenthPart.CurrentLength,
                     PasteCharLocation,
                     LengthRemaining);
        PasteCharLocation += TenthPart.CurrentLength;
        LengthRemaining -= TenthPart.CurrentLength;
    }
    ASMInstructions->Contents[ASMInstructions->CurrentLength] = '\0';
    InstructionCounts->CallCount++;

    FreeVMStringContents(&CallCountString);
    FreeVMStringContents(&ArgCountString);
}


internal void ParseReturnCommand(vm_string *ASMInstructions,
                                 instruction_counts *InstructionCounts,
                                 vm_error_list *ErrorList)
{
    // TODO(Marko): Jump to return address can be done in the bootstrap code! 
    //              Since we store return address in R14, we can simply emit 
    //              that jump code once in a known location, and just emit a 
    //              jump to that location, which then handles the jump to the 
    //              return location. Is this better than what I have written 
    //              down here? 
    /*  NOTE(Marko): The return command will store LCL in a temp variable, 
                     then use that temp variable to seek to the return 
                     variable and put that in another temp variable. Once 
                     that's done, it can use the first temp variable to 
                     restore the THAT, THIS, ARG, and LCL of the function that 
                     called it. Then it can use the return address (stored in 
                     the second temp variable) to go to the call site and 
                     continue executing. 

                     This is realized as follows:
                        // Store LCL in local variable
                                @LCL
                                D=A
                                @R13
                                M=D

                        // Store return address in another temp variable
                            // Store initial value in R14
                                @R13
                                D=M
                                @R14
                                M=D
                            // loop
                                @5
                                D=A
                                @R15
                                M=D
                                (RET_ADD_RETRIEVE_LOOPX)
                                @R14
                                M=M-1
                                @R15
                                M=M-1
                                D=M
                                @RET_ADD_RETRIEVE_LOOPX
                                D;JGT
                        // At this point, R13 contains pointer to FRAME
                        //                R14 contains FRAME-5
                        // If we dereference R14, we will retrieve the return 
                        // address, so we can store it, again in R14
                            // Dereference R14
                                @R14
                                A=M
                            // Store return address
                                D=M
                                @R14
                                M=D
                        // The last value on the stack is the return value. 
                        // Put that into the place pointed to by ARG
                                @SP
                                M=M-1
                                A=M
                                D=M
                                @ARG
                                A=M
                                M=D
                        // Place SP at ARG+1
                                @ARG
                                D=M
                                D=D+1
                                @SP
                                M=D
                        // Restore the memory segments of the caller
                            // THAT is at *(R13-1)
                                @R13
                                M=M-1
                                A=M
                                D=M
                                @THAT
                                M=D
                            // THIS is at *(R13-2)
                                @R13
                                M=M-1
                                A=M
                                D=M
                                @THIS
                                M=D
                            // ARG is at *(R13-3)
                                @R13
                                M=M-1
                                A=M
                                D=M
                                @ARG
                                M=D
                            // LCL is at *(R13-4)
                                @R13
                                M=M-1
                                A=M
                                D=M
                                @LCL
                                M=D
                        // Go to the return address
                                @R14
                                A=M
                                0;JMP
    */
    vm_string ReturnCountString = UInt32ToVMString(InstructionCounts->ReturnCount);
    vm_string FirstPart = ConstructVMStringFromCString("@LCL\nD=A\n@R13\nM=D\n@R13\nD=M\n@R14\nM=D\n@5\nD=A\n@R15\nM=D\n(RET_ADD_RETRIEVE_LOOP");
    vm_string SecondPart = ConstructVMStringFromCString(")\n@R14\nM=M-1\n@R15\nM=M-1\nD=M\n@RET_ADD_RETRIEVE_LOOP");
    vm_string ThirdPart = ConstructVMStringFromCString("\nD;JGT\n@R14\nA=M\nD=M\n@R14\nM=D\n@SP\nM=M-1\nA=M\nD=M\n@ARG\nA=M\nM=D\n@ARG\nD=M\nD=D+1\n@SP\nM=D\n@R13\nM=M-1\nA=M\nD=M\n@THAT\nM=D\n@R13\nM=M-1\nA=M\nD=M\n@THIS\nM=D\n@R13\nM=M-1\nA=M\nD=M\n@ARG\nM=D\n@R13\nM=M-1\nA=M\nD=M\n@LCL\nM=D\n@R14\nA=M\n0;JMP\n");

    ASMInstructions->CurrentLength = 
        FirstPart.CurrentLength + 
        ReturnCountString.CurrentLength + 
        SecondPart.CurrentLength + 
        ReturnCountString. CurrentLength + 
        ThirdPart.CurrentLength;

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

        CopyVMString(ReturnCountString.Contents,
                     ReturnCountString.CurrentLength,
                     PasteCharLocation,
                     LengthRemaining);
        PasteCharLocation += ReturnCountString.CurrentLength;
        LengthRemaining -= ReturnCountString.CurrentLength;

        CopyVMString(SecondPart.Contents,
                     SecondPart.CurrentLength,
                     PasteCharLocation,
                     LengthRemaining);
        PasteCharLocation += SecondPart.CurrentLength;
        LengthRemaining -= SecondPart.CurrentLength;

        CopyVMString(ReturnCountString.Contents,
                     ReturnCountString.CurrentLength,
                     PasteCharLocation,
                     LengthRemaining);
        PasteCharLocation += ReturnCountString.CurrentLength;
        LengthRemaining -= ReturnCountString.CurrentLength;

        CopyVMString(ThirdPart.Contents,
                     ThirdPart.CurrentLength,
                     PasteCharLocation,
                     LengthRemaining);
        PasteCharLocation += ThirdPart.CurrentLength;
        LengthRemaining -= ThirdPart.CurrentLength;
    }
    ASMInstructions->Contents[ASMInstructions->CurrentLength] = '\0';
    
    InstructionCounts->ReturnCount++;
    FreeVMStringContents(&ReturnCountString);
}


void ParseTokensToASM(vm_tokens *VMTokens,
                      vm_string *ASMInstructions,
                      instruction_counts *InstructionCounts,
                      vm_error_list *ErrorList)
{
    vm_string PushString = ConstructVMStringFromCString("push");
    vm_string PopString = ConstructVMStringFromCString("pop");

    vm_string GotoString = ConstructVMStringFromCString("goto");
    vm_string IfGotoString = ConstructVMStringFromCString("if-goto");
    vm_string LabelString = ConstructVMStringFromCString("label");

    vm_string FunctionString = ConstructVMStringFromCString("function");
    vm_string CallString = ConstructVMStringFromCString("call");
    vm_string ReturnString = ConstructVMStringFromCString("return");

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
            
            if(VMStringsAreEqual(&VMTokens->VMTokens[0], &ReturnString))
            {
                ParseReturnCommand(ASMInstructions, 
                                   InstructionCounts,
                                   ErrorList);
            }
            else
            {
                ParseArithmeticCommand(VMTokens, 
                                       ASMInstructions, 
                                       InstructionCounts,
                                       ErrorList);
            }
        } break;

        case 2:
        {
            /* NOTE(Marko): Possible VM instructions of length 2:
                                "label <label>"
                                "goto <label>"
                                "if-goto <label>"

            */
            if(VMStringsAreEqual(&VMTokens->VMTokens[0], &LabelString))
            {
                ParseLabelCommand(VMTokens, 
                                  ASMInstructions, 
                                  InstructionCounts, 
                                  ErrorList);

            }
            else if(VMStringsAreEqual(&VMTokens->VMTokens[0], &GotoString))
            {
                ParseGotoCommand(VMTokens, 
                                 ASMInstructions, 
                                 InstructionCounts, 
                                 ErrorList);
            }
            else if(VMStringsAreEqual(&VMTokens->VMTokens[0], &IfGotoString))
            {
                ParseIfGotoCommand(VMTokens,
                                   ASMInstructions,
                                   InstructionCounts,
                                   ErrorList);
            }
            else
            {
                vm_string Error = ConstructVMStringFromCString("Unrecognized command while trying to parse 2 tokens");
                AddErrorToErrorList(ErrorList, &Error);
                ASMInstructions->CurrentLength = 0;
            }

        } break;

        case 3:
        {
            /*  NOTE(Marko): Possible VM instructions of length 3:
                                "push segment i"
                                "pop segment i"
                                "function name nArgs"
                                "call name nVars"
            */
            if(IsNumericVMString(&VMTokens->VMTokens[2]))
            {
                if(VMStringsAreEqual(&VMTokens->VMTokens[0], &PushString))
                {
                    ParsePushCommand(VMTokens, 
                                     ASMInstructions, 
                                     InstructionCounts, 
                                     ErrorList);
                }
                else if(VMStringsAreEqual(&VMTokens->VMTokens[0], &PopString))
                {
                    ParsePopCommand(VMTokens, 
                                    ASMInstructions, 
                                    InstructionCounts, 
                                    ErrorList);
                }
                else if(VMStringsAreEqual(&VMTokens->VMTokens[0], 
                                          &FunctionString))
                {
                    ParseFunctionCommand(VMTokens,
                                         ASMInstructions,
                                         InstructionCounts,
                                         ErrorList);
                }
                else if(VMStringsAreEqual(&VMTokens->VMTokens[0], &CallString))
                {
                    ParseCallCommand(VMTokens,
                                     ASMInstructions,
                                     InstructionCounts,
                                     ErrorList);
                }
                else
                {
                    vm_string Error = ConstructVMStringFromCString("Unrecognized command while trying to parse 3 tokens. ");
                    AddErrorToErrorList(ErrorList, &Error);
                    ASMInstructions->CurrentLength = 0;
                }                
            }
            else
            {
                vm_string Error = ConstructVMStringFromCString("While parsing 3 tokens, encountered non-numerical third token, which is invalid");
                    AddErrorToErrorList(ErrorList, &Error);                
                    ASMInstructions->CurrentLength = 0;
            }

        } break;

        default:
        {
            // NOTE(Marko): Should never reach this part of the code. 
            InvalidCodePath; 
        }
    }
}

void TokenizeLine(vm_string *VMInputString, 
                  uint32 *InputIndex,
                  vm_tokens *VMTokens,
                  vm_error_list *ErrorList)
{
    // NOTE(Marko): Splits current line of VMInputString into tokens and puts 
    //              those tokens into VMTokens struct

    // TODO(Marko): Consider scoping the two passes separately.
    // TODO(Marko): Is there a way to do this in one pass? 
    //
    // NOTE(Marko): Counting how many tokens on the line. 
    //
    uint32 TokenCount = 0;
    bool32 HadInlineComment = 0;
    bool32 HadError = 0;
    char *CurrentChar = &VMInputString->Contents[*InputIndex];
    while(*CurrentChar != NEWLINE)
    {
        if((*CurrentChar == COMMENT_SLASH) && 
           (*(CurrentChar + 1) == COMMENT_SLASH))
        {
            // NOTE(Marko): inline comment found: ignore until newline.
            HadInlineComment = 1;
            while(*CurrentChar != NEWLINE)
            {
                CurrentChar++;
            }
            break;
        }
        else if(*CurrentChar == COMMENT_SLASH)
        {
            // NOTE(Marko): Floating slash is an error
            HadError = 1;
            vm_string Error = ConstructVMStringFromCString("Floating slash found while tokenizing line.");
            AddErrorToErrorList(ErrorList, &Error);
            VMTokens->VMTokenCount = 0;
            // NOTE(Marko): Advance InputIndex to end of line
            CurrentChar = &VMInputString->Contents[*InputIndex];
            while(*CurrentChar != NEWLINE)
            {
                CurrentChar++;
                (*InputIndex)++;
            }
            break;
        }
        else if(((*CurrentChar == WHITESPACE) || (*CurrentChar == TAB)) && 
                *(CurrentChar + 1) != WHITESPACE)
        {
            TokenCount++;
        }
        CurrentChar++;
    }
    // NOTE(Marko): If the final token is right next to \n, we need to add one 
    //              more to TokenCount. If we ended with an inline comment 
    //              then we don't want to increment TokenCount
    if(*(CurrentChar-1) != WHITESPACE && !HadInlineComment && !HadError)
    {
        TokenCount++;
    }

    if(TokenCount > MAX_VM_TOKEN_COUNT)
    {
        // NOTE(Marko): At this point there shouldn't be *that* many tokens 
        //              since we know how much there could possibly be ahead 
        //              of time 
        vm_string Error = ConstructVMStringFromCString("Too many tokens found.");
        AddErrorToErrorList(ErrorList, &Error);
        VMTokens->VMTokenCount = 0;

        // NOTE(Marko): Advance InputIndex to end of line
        CurrentChar = &VMInputString->Contents[*InputIndex];
        while(*CurrentChar != NEWLINE)
        {
            CurrentChar++;
            (*InputIndex)++;
        }
    }
    else
    {
         VMTokens->VMTokenCount = TokenCount;

        // NOTE(Marko): Putting tokens into vm_tokens struct. 
        CurrentChar = &VMInputString->Contents[*InputIndex];
        uint32 TokenIndex = 0;
        while((*CurrentChar != NEWLINE) && (TokenIndex < VMTokens->VMTokenCount))
        {
            char *CurrentTokenBegin = CurrentChar;
            while((*CurrentChar != WHITESPACE) && 
                  (*CurrentChar != TAB) &&
                  (*CurrentChar != NEWLINE))
            {
                (*InputIndex)++;
                CurrentChar++;
            }
            char *CurrentTokenEnd = CurrentChar; 

            uint32 CurrentTokenLength = 
                (uint32)(CurrentTokenEnd - CurrentTokenBegin);

            VMTokens->VMTokens[TokenIndex].CurrentLength = CurrentTokenLength;
            if(VMTokens->VMTokens[TokenIndex].MemorySize <= 
               VMTokens->VMTokens[TokenIndex].CurrentLength)
            {
                GrowVMString(&VMTokens->VMTokens[TokenIndex]);
            } 

            CopyVMString(CurrentTokenBegin,
                         CurrentTokenLength,
                         VMTokens->VMTokens[TokenIndex].Contents,
                         VMTokens->VMTokens[TokenIndex].CurrentLength);
            // NOTE(Marko): Null terminate
            VMTokens->VMTokens[TokenIndex].Contents[VMTokens->VMTokens[TokenIndex].CurrentLength]= '\0';
            TokenIndex++;

            if(*CurrentChar != NEWLINE)
            {
                CurrentChar++;
                (*InputIndex)++;
            }
        }       
    }
}