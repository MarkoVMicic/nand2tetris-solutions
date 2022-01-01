// Initialize stack pointer to start at 256 (i.e. RAM[0] contains 256)
@256
D=A
@SP
M=D

// Push two numbers on the stack. 8 < 9 -> -1
@8
D=A
@SP
A=M
M=D
@SP
M=M+1
@9 
D=A
@SP
A=M
M=D
@SP
M=M+1

// Less than check
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
D=M-D
@NOT_LT_0
D;JGE
(LT_0)
@SP
A=M
M=-1
@LT_END_0
0;JMP
(NOT_LT_0)
@SP
A=M
M=0
@LT_END_0
0;JMP
(LT_END_0)
@SP
M=M+1

// Push two numbers on the stack. 9 !< 8 -> 0
@9
D=A
@SP
A=M
M=D
@SP
M=M+1
@8 
D=A
@SP
A=M
M=D
@SP
M=M+1

// Less than check
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
D=M-D
@NOT_LT_1
D;JGE
(LT_1)
@SP
A=M
M=-1
@LT_END_1
0;JMP
(NOT_LT_1)
@SP
A=M
M=0
@LT_END_1
0;JMP
(LT_END_1)
@SP
M=M+1

// Push two numbers on the stack. 69 !< 69 -> 0
@69
D=A
@SP
A=M
M=D
@SP
M=M+1
@69 
D=A
@SP
A=M
M=D
@SP
M=M+1

// Less than check
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
D=M-D
@NOT_LT_2
D;JGE
(LT_2)
@SP
A=M
M=-1
@LT_END_2
0;JMP
(NOT_LT_2)
@SP
A=M
M=0
@LT_END_2
0;JMP
(LT_END_2)
@SP
M=M+1

