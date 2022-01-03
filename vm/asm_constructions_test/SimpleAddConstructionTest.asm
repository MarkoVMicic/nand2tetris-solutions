// Initialize stack pointer to start at 256 (i.e. RAM[0] contains 256)
@256
D=A
@SP
M=D


// Current Stack:
//              address |   memory
//      SP  ->  256     |   ???
//              257     |   ???
//              258     |   ???
//              259     |   ???
//              260     |   ???


// VM Code:
// push constant 7
@7          // A:7 M:? D:? Constant fetch, A contains 7
D=A         // A:7 M:? D:7 Puts 7 from A into D
@SP         // A:0 M:256 D:7 A contains address of stack pointer (i.e. 0)
A=M         // A:256 M:256 D:7 A contains address of stack pointer
M=D         // A:256 M:7 D:7 Place that stack pointer points to now contains 7
@SP         // A:0 M:256 D:7 M contains address of stack pointer
M=M+1       // A:0 M:257 D:7 Stack pointer is now incremented to 257

// Current Stack:
//              address |   memory
//              256     |   7
//      SP  ->  257     |   ???
//              258     |   ???
//              259     |   ???
//              260     |   ???
//              261     |   ???


// VM Code:
// push constant 8
@8          // A:8 M:? D:7 Constant fetch, A contains 8
D=A         // A:8 M:? D:8 Puts 8 from A into D
@SP         // A:0 M:257 D:8 A contains address of stack pointer (i.e. 0)
A=M         // A:257 M:257 D:8 A contains address of stack pointer
M=D         // A:257 M:8 D:8 Place that stack pointer points to now contains 8
@SP         // A:0 M:257 D:8 M contains address of stack pointer
M=M+1       // A:0 M:258 D:8 Stack pointer is now incremented to 258 

// Current Stack:
//              address |   memory
//              256     |   7
//              257     |   8
//      SP  ->  258     |   ???
//              259     |   ???
//              260     |   ???
//              261     |   ???

// VM Code
// add
@SP         // A:0 M:258 D:8 M contains address of stack pointer (258)
M=M-1       // A:0 M:257 D:8 Decrement address of stack pointer. (257)
A=M         // A:257 M:8 D:8 M now contains 8 (stored in second push)
D=M         // A:257 M:8 D:8 D now stores 8.
@SP         // A:0 M:257 D:8 M contains address of stack pointer (257)
M=M-1       // A:0 M:256 D:8 Decrement address of stack pointer. (256)
A=M         // A:256 M:7 D:8 M now contains 7 (stored in first push)
M=M+D       // A:256 M:15 D:8 M now contains 15 
@SP         // A:0 M:256 D:7 M contains address of stack pointer
M=M+1       // A:0 M:257 D:7 Stack pointer is now incremented to 257

// Current Stack:
//              address |   memory
//              256     |   15
//      SP  ->  257     |   8 (???)
//              258     |   ???
//              259     |   ???
//              260     |   ???
//              261     |   ???

