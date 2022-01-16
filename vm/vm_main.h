#if !defined(VM_MAIN_H)

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// NOTE: We are putting these #define in the code to make the code more 
//       searchable. Now we can more easily search for global variables, 
//       locally-persistent variables, and internal functions much more easily 
//       than if these three vastly different types were all indicated with 
//       the same static keyword. 
#define internal static    
#define local_persist static
#define global_variable extern

typedef int8_t int8; 
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

// NOTE: unsigned char is the same as an 8-bit integer. 
typedef uint8_t uint8;  
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

#if VM_DEBUG
    // NOTE: Assert macro -> Write to the NULL pointer if Expression is false. 
    //       This is platform independent. 
    // TODO: Complete Assertion Macro
    #define Assert(Expression) if(!(Expression)){* (int *)0 = 0;}
#else
    #define Assert(Expression)
#endif

#define InvalidCodePath Assert(!"InvalidCodePath");

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

//
// NOTE(Marko): Simple File I/O
//

struct read_file_result
{
    char *Contents;
    uint32 Length;
};

//
// NOTE(Marko): InstructionCounts
//
struct instruction_counts
{
    // NOTE(Marko): Push counts
    uint32 PushArgumentCount;
    uint32 PushLocalCount;
    uint32 PushStaticCount;
    uint32 PushConstantCount;
    uint32 PushThisCount;
    uint32 PushThatCount;
    uint32 PushPointerCount;
    uint32 PushTempCount;

    // NOTE(Marko): Pop Counts
    uint32 PopArgumentCount;
    uint32 PopLocalCount;
    uint32 PopStaticCount;
    uint32 PopThisCount;
    uint32 PopThatCount;
    uint32 PopPointerCount;
    uint32 PopTempCount;
    
    // NOTE(Marko): Arithmetic Command counts
    uint32 AddCount;
    uint32 SubCount;
    uint32 NegCount;
    uint32 EqCount;
    uint32 GtCount;
    uint32 LtCount;
    uint32 AndCount;
    uint32 OrCount;
    uint32 NotCount;

    // Branching command counts
    uint32 LabelCount;
    uint32 GotoCount;
    uint32 IfGotoCount;

    // Function command counts
    uint32 FunctionCount;
    uint32 CallCount;
};



#define VM_MAIN_H
#endif