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

// push constant 420  
@420          
D=A         
@SP         
A=M         
M=D         
@SP         
M=M+1 

// Or
// 69:  0000000001000101
// 420: 0000000110100100
//      -----------------
// 485: 0000000111100101
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
