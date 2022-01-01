// Initialize stack pointer to start at 256 (i.e. RAM[0] contains 256)
@256
D=A
@SP
M=D

// Push a number onto the stack
@69
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 5 (5+5=10)
@SP
M=M-1
A=M
D=M
@10
M=D

