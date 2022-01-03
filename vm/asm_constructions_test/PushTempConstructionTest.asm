// Initialize stack pointer to start at 256 (i.e. RAM[0] contains 256)
@256
D=A
@SP
M=D

// Put something in temp space
@69
D=A
@8
M=D

// Push temp 3
@8
D=M
@SP
A=M
M=D
@SP
M=M+1
