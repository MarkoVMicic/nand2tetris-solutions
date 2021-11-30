#if !defined(WIN_ASSEMBLER_H)

#include <stdint.h>
#include <stddef.h>

// NOTE: We are putting these #define in the code to make the code more 
//       searchable. Now we can more easily search for global variables, 
//       locally-persistent variables, and internal functions much more easily 
//       than if these three vastly different types were all indicated with 
//       the same static keyword. 
#define internal static    
#define local_persist static
#define global_variable static 

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

#if WIN_ASSSEMBLER_DEBUG
    // NOTE: Assert macro -> Write to the NULL pointer if Expression is false. 
    //       This is platform independent. 
    // TODO: Complete Assertion Macro
    #define Assert(Expression) if(!(Expression)){* (int *)0 = 0;}
#else
    #define Assert(Expression)
#endif

#define InvalidCodePath Assert(!"InvalidCodePath");

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

struct asm_string
{
    char *Contents;
    uint32 Length;
};



#define WIN_ASSEMBLER_H
#endif
