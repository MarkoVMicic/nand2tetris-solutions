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

inline uint32 
SafeTruncateUInt64(uint64 Value)
{           
    // TODO: Defines for maximum values
    Assert(Value <= 0xFFFFFFFF);
    uint32 Result = (uint32)Value;

    return(Result);
}

//
// NOTE(Marko): Read and Write file functions
//
#if WIN_ASSEMBLER_DEBUG
/*
    NOTE:
    IMPORTANT: 

    
    These are blocking, and the write doesn't protect against locked data. If 
    we were shipping this assembler, we'd need to make this a lot more robust. 

    To indicate this, we've prefixed the functions and data structures with 
    "debug" and use them only in debug builds (of course we haven't ever built 
    a "release" build because this is a toy project)
 */
    struct debug_read_file_result
    {
        uint32 ContentsSize;
        void *Contents;
    };

    #define DEBUG_FREE_FILE_MEMORY(name) void name(void *Memory)
    typedef DEBUG_FREE_FILE_MEMORY(debug_free_file_memory);

    #define DEBUG_READ_ENTIRE_FILE(name) debug_read_file_result name(char *Filename)
    typedef DEBUG_READ_ENTIRE_FILE(debug_read_entire_file);

    #define DEBUG_WRITE_ENTIRE_FILE(name) bool32 name(char *Filename, uint32 MemorySize, void *Memory)
    typedef DEBUG_WRITE_ENTIRE_FILE(debug_write_entire_file);
#endif

struct asm_string
{
    char *Contents;
    uint32 Length;
};


internal void CopyString(char *SourceString, 
                         uint32 SourceStringLength,
                         char *DestString, 
                         uint32 DestStringLength)
{
    Assert(DestStringLength >= SourceStringLength);
    
    for(uint32 i = 0; i < DestStringLength; i++)
    {
        DestString[i] = SourceString[i];
    }
}


// TODO(Marko): Make use of this function instead of using CopyString() 
//              directly?
internal void CopyAsmString(asm_string *SourceAsmString,
                            asm_string *DestAsmString)
{
    CopyString(SourceAsmString->Contents,
               SourceAsmString->Length,
               DestAsmString->Contents,
               DestAsmString->Length);
}



#define WIN_ASSEMBLER_H
#endif
