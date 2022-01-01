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

// Put something in local space
@69
D=A
@303
M=D

// Push local 3
@LCL
D=M
@3
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
