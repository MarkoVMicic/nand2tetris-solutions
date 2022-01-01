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

// push constant 20
@20          
D=A         
@SP         
A=M         
M=D         
@SP         
M=M+1    


// VM Code
// sub: 69-20 = 49
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

