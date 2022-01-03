// Initialize stack pointer to start at 256 (i.e. RAM[0] contains 256)
@256
D=A
@SP
M=D

// Put a number in THIS
@69
D=A
@THIS
M=D

// Put a number in THAT
@420
D=A
@THAT
M=D

// Push pointer 0
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1

// Push pointer 1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
