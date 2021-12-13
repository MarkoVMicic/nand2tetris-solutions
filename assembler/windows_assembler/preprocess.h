#if !defined(PREPROCESS_H)

#define COMMENT_SLASH '/'
#define A_INSTRUCTION_SYMBOL '@'
#define WHITESPACE ' '
#define TAB '\t'
#define CARRIAGE_RETURN '\r'
#define NEWLINE '\n'
#define OPEN_BRACKET '('
#define CLOSED_BRACKET ')'

// NOTE(Marko): Specified by nand2tetris to start here. 
#define USER_DEFINED_VAR_ADDRESS_START 16
#define USER_DEFINED_VAR_ADDRESS_END 15000

// NOTE(Marko): 65535 is unreachable using 15-bit addressing in the 
//              nand2tetris computer, so we can use it as a flag for 
//              uninitialized
#define UNINITIALIZED_ADDRESS 65535


#define PREPROCESS_H
#endif