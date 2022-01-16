// Place ARG at address 300
@300
D=A
@ARG
M=D

// Return value is 69
@69
D=A
@306
M=D

// Stack pointer is 1 above return value
@307
D=A
@SP
M=D

// Place return value in place pointed to by ARG
@SP
M=M-1
A=M
D=M
@ARG
A=M
M=D

// Place SP at ARG+1
@ARG
D=M
D=D+1
@SP
M=D