// Initialize stack pointer to start at 256 (i.e. RAM[0] contains 256)
@256
D=A
@SP
M=D

// Initialize local pointer to start at 300
@300
D=A
@LCL
M=D

// Push a number onto the stack
@69
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop local 3
@LCL
D=M
@3
D=D+A
@LCL_POP
M=D
@SP
M=M-1
A=M
D=M
@LCL_POP
A=M
M=D
