#if !defined(VM_TOKENS_H)

#include "vm_string.h"
#include "vm_error.h"

#define COMMENT_SLASH '/'
#define NEWLINE '\n'
#define WHITESPACE ' '
#define TAB '\t'

#define TEMP_ADDRESS_START 5
#define TEMP_ADDRESS_END 12

// TODO(Marko): Actually check that 3 is indeed the maximum token amount. 
#define MAX_VM_TOKEN_COUNT 3

struct vm_tokens
{
    vm_string *VMTokens;
    uint32 VMTokenCount;
};


void FreeVMTokens(vm_tokens *VMTokens);

vm_tokens *AllocateVMTokens(uint32 TokenCount, 
                            uint32 TokenVMStringSize);

void ParseTokensToASM(vm_tokens *VMTokens,
                      vm_string *ASMInstructions,
                      instruction_counts *InstructionCounts);

void TokenizeLine(vm_string *VMInputString,
                  uint32 *InputIndex,
                  vm_tokens *VMTokens,
                  vm_error_list *ErrorList);

#define VM_TOKENS_H
#endif