#if !defined(WIN_ASSEMBLER_H)


// NOTE: We are putting these #define in the code to make the code more 
//       searchable. Now we can more easily search for global variables, 
//       locally-persistent variables, and internal functions much more easily 
//       than if these three vastly different types were all indicated with 
//       the same static keyword. 
#define internal static    
#define local_persist static
#define global_variable static 




#define WIN_ASSEMBLER_H
#endif
