#if !defined(VM_STRING_H)
#include "vm_main.h"



#define DEFAULT_INITIAL_VM_STRING_SIZE 256

struct vm_string
{
    char *Contents;
    uint32 CurrentLength;
    // NOTE(Marko): We keep track of the memory size, so that we can call 
    //              realloc() with a larger size if CurrentLength ever exceeds 
    //              MemorySize. 
    uint32 MemorySize;  
};

vm_string AllocateVMString(uint32 MemoryBlockSize);

#define VM_STRING_H
#endif
