// Initialize stack pointer to start at 256 (i.e. RAM[0] contains 256)
@256
D=A
@SP
M=D

// Push two numbers on the stack. 
@8
D=A
@SP
A=M
M=D
@SP
M=M+1
@7 
D=A
@SP
A=M
M=D
@SP
M=M+1

// Equality check
@SP         // A:0 M:258 D:?  
M=M-1       // A:// Initialize stack pointer to start at 256 (i.e. RAM[0] contains 256)
@256
D=A
@SP
M=D

// Push two numbers on the stack. 
@8
D=A
@SP
A=M
M=D
@SP
M=M+1
@7 
D=A
@SP
A=M
M=D
@SP
M=M+1

// Less than check
@SP         // A:0 M:258 D:?  
M=M-1       // A:0 M:257 D:?
A=M         // A:257 M:A D:?
D=M         // A:257 M:A D:7
@SP         // A:0 M:257 D:7
M=M-1       // A:0 M:256 D:7
A=M         // A:256 M:8 D:7
D=D-M       // A:256 M:8 D:7-8=-1
@NOT_LT     // A:NOT_LT M:? D:A-B
D;JGE       // A:NOT_LT M:? D:A-B
(LT)
@SP
A=M
M=1
@END_LT
0;JMP     
(NOT_LT)
@SP
A=M
M=0
@END_LT
0;JMP
(END_LT)
@SP
M=M+10 M:257 D:?
A=M         // A:257 M:A D:?
D=M         // A:257 M:A D:A
@SP         // A:0 M:257 D:A
M=M-1       // A:0 M:256 D:A
A=M         // A:256 M:B D:A
D=M-D       // A:256 M:B D:A-B
@NOT_EQUAL  // A:NOT_EQUAL M:? D:A-B
D;JNE       // A:NOT_EQUAL M:? D:A-B
(EQUAL)
@SP
A=M
M=1
@END_EQ
0;JMP     
(NOT_EQUAL)
@SP
A=M
M=0
@END_EQ
0;JMP
(END_EQ)
@SP
M=M+1