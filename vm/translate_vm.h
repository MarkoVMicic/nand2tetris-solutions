#if !defined(TRANSLATE_VM_H)

#define COMMENT_SLASH '/'
#define NEWLINE '\n'

void TranslateInstructionsToAsm(vm_string *VMInputString, 
                                vm_string *ASMOutputString);

#define TRANSLATE_VM_H
#endif
