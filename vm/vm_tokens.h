#if !defined(VM_TOKENS_H)

#include "vm_string.h"

#define COMMENT_SLASH '/'
#define NEWLINE '\n'
#define WHITESPACE ' '
#define TAB '\t'

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
                      vm_string *ASMInstructions);

void TokenizeLine(vm_string *VMInputString,
                  uint32 *InputIndex,
                  vm_tokens *VMTokens);

#define VM_TOKENS_H
#endif