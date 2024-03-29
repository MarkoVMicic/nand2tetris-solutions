// Initialize stack pointer to start at 256 (i.e. RAM[0] contains 256)
@256
D=A
@SP
M=D

// Perform things in StackTest
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
D=M-D
@NOT_EQUAL_0
D;JNE
(EQUAL_0)
@SP
A=M
M=-1
@EQUAL_END_0
0;JMP
(NOT_EQUAL_0)
@SP
A=M
M=0
@EQUAL_END_0
0;JMP
(EQUAL_END_0)
@SP
M=M+1
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
@16
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
D=M-D
@NOT_EQUAL_1
D;JNE
(EQUAL_1)
@SP
A=M
M=-1
@EQUAL_END_1
0;JMP
(NOT_EQUAL_1)
@SP
A=M
M=0
@EQUAL_END_1
0;JMP
(EQUAL_END_1)
@SP
M=M+1
@16
D=A
@SP
A=M
M=D
@SP
M=M+1
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
D=M-D
@NOT_EQUAL_2
D;JNE
(EQUAL_2)
@SP
A=M
M=-1
@EQUAL_END_2
0;JMP
(NOT_EQUAL_2)
@SP
A=M
M=0
@EQUAL_END_2
0;JMP
(EQUAL_END_2)
@SP
M=M+1
@892
D=A
@SP
A=M
M=D
@SP
M=M+1
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
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
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
@892
D=A
@SP
A=M
M=D
@SP
M=M+1
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
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
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
@32767
D=A
@SP
A=M
M=D
@SP
M=M+1
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
D=M-D
@NOT_GT_0
D;JLE
(GT_0)
@SP
A=M
M=-1
@GT_END_0
0;JMP
(NOT_GT_0)
@SP
A=M
M=0
@GT_END_0
0;JMP
(GT_END_0)
@SP
M=M+1
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
@32767
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
D=M-D
@NOT_GT_1
D;JLE
(GT_1)
@SP
A=M
M=-1
@GT_END_1
0;JMP
(NOT_GT_1)
@SP
A=M
M=0
@GT_END_1
0;JMP
(GT_END_1)
@SP
M=M+1
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
D=M-D
@NOT_GT_2
D;JLE
(GT_2)
@SP
A=M
M=-1
@GT_END_2
0;JMP
(NOT_GT_2)
@SP
A=M
M=0
@GT_END_2
0;JMP
(GT_END_2)
@SP
M=M+1
@57
D=A
@SP
A=M
M=D
@SP
M=M+1
@31
D=A
@SP
A=M
M=D
@SP
M=M+1
@53
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
M=M+D
@SP
M=M+1
@112
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
M=M-D
@SP
M=M+1
@SP
M=M-1
A=M
M=-M
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
M=M&D
@SP
M=M+1
@82
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
M=M|D
@SP
M=M+1
@SP
M=M-1
A=M
M=!M
@SP
M=M+1

