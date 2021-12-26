#if !defined(VM_MAIN_H)

#include <stddef.h>
#include <stdio.h>

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

#define VM_MAIN_H
#endif