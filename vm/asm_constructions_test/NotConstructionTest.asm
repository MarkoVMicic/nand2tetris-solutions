// Initialize stack pointer to start at 256 (i.e. RAM[0] contains 256)
@256
D=A
@SP
M=D


// push constant 69
@69          
D=A         
@SP         
A=M         
M=D         
@SP         
M=M+1 

// not:
// 69:  0000000001000101
// !69: 1111111110111010 (-70)

@SP
M=M-1
A=M
M=!M
@SP
M=M+1
