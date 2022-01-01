#if !defined(VM_STRING_H)
#include "vm_main.h"



#define DEFAULT_INITIAL_VM_STRING_SIZE 32
#define INITIAL_ASM_OUTPUT_STRING_SIZE 256

struct vm_string
{
    char *Contents;
    uint32 CurrentLength;
    // NOTE(Marko): We keep track of the memory size, so that we can call 
    //              realloc() with a larger size if CurrentLength ever exceeds 
    //              MemorySize. 
    uint32 MemorySize;  
};

uint32 VMStringToUInt32(vm_string *VMString);

vm_string UInt32ToVMString(uint32 UInt32);

void GrowVMString(vm_string *VMString);

bool32 VMStringsAreEqual(vm_string *VMStringA, 
                         vm_string *VMStringB);


bool32 VMStringsAreEqual(vm_string *VMStringA, 
                         char *StringB, 
                         uint32 StringBLength);

void FreeVMString(vm_string *VMString);

vm_string *AllocateVMString(uint32 MemoryBlockSize);

void CopyVMString(char *SourceString, 
                  uint32 SourceStringLength, 
                  char *DestString, 
                  uint32 DestStringLength);

vm_string RetrieveProgramNameFromInputFileName(const char *InputFileName);

void FreeProgramName(vm_string GlobalProgramName);

#define VM_STRING_H
#endif
