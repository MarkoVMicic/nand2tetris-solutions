// Return address stored in 20.
@69
D=A
@20
M=D

// R13 contains FRAME pointer
// 25 is stored in R13, goal is to store 20 in R14
@25
D=A
@R13
M=D

// Store initial value in R14
@R13
D=M
@R14
M=D
// loop
@5
D=A
@R15
M=D
(RET_ADD_RETRIEVE_LOOPX)
@R14
M=M-1
@R15
M=M-1
D=M
@RET_ADD_RETRIEVE_LOOPX
D;JGT

// Dereference R14
@R14
A=M
// Store return address
D=M
@R14
M=D
