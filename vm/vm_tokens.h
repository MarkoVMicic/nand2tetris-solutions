#if !defined(VM_TOKENS_H)

#include "vm_string.h"

#define COMMENT_SLASH '/'
#define NEWLINE '\n'
#define WHITESPACE ' '
#define TAB '\t'

struct vm_tokens
{
    vm_string *VMTokens;
    uint32 VMTokenCount;
};


vm_string ParseTokensToASM(vm_tokens *VMTokens);

vm_tokens TokenizeLine(vm_string *VMInputString,
                       uint32 *InputIndex);

#define VM_TOKENS_H
#endif