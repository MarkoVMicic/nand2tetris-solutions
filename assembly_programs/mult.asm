// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

//Psuedocode:
//
//  R0 * R1 = R2
//  i.e.
//  if R0 | R1 == 0:
//	  R2 = 0
//  if R0,R1 > 0 | R0,R1 < 0:
//  	R0 + R0 + ... + R0 = R2
//	    {--------v-------}
//	  	      R1 times
//  if R0 > 0, R1 < 0:
//	    R0 + R0 + ... + R0 = temp
//	    {--------v-------}
//	  	    R1 times
//	R2 = -tmp
//
//Machine Code:

	@R2		// Initialize R2 to 0. 
	M=0

	@R0
	D=M
	@ZEROPRODUCT
	D; JEQ			// If RAM[R0] == 0, then R0*R1 == 0, so jump to setting RAM[R2] = 0.

	@R1
	D=M
	@ZEROPRODUCT
	D; JEQ			// If RAM[R1] == 0, then R0*R1 == 0, so jump to setting RAM[R2] = 0.

// We will compute R0*R1, so we set n = |RAM[R1]| (note the absolute value signs)

(SETN)				// Unused label for readability
	@R1
	D=M
	@NEGATIVEN
	D; JLT

(POSITIVEN)		// Unused label for readability
	@R1
	D=M
	@n
	M=D 		// Set n = RAM[R1] i.e. the number of times to add R0 to itself
	@PRODUCT
	0; JMP

(NEGATIVEN)
	@n
	M=0
	@R1
	D=M
	@n
	M=M-D 		// Set n = |RAM[R1]| i.e. the number of times to add R0 to itself
	@PRODUCT
	0; JMP 		// Redundant Jump added for readability


(PRODUCT)
	@i
	M=0				// Set i = 0
	@R2				// Initialize RAM[R2] = 0
	M=0	
	(PRODUCTLOOP)
		@i
		D=M
		@n
		D=D-M
		@R1SIGNCHECK	
		D; JGE				// If we have already added R0 n times, then jump out of the loop, else iterate one more time. 

		@R0
		D=M
		@R2
		M=M+D

		@i
		M=M+1
		@PRODUCTLOOP
		0; JMP
	
// We note here that at this point, SIGN(RAM[R0] == SIGN(RAM[R2])), so that we need only 
// check the sign of R1 to determine if RAM[R3] should be negative or positive.  
(R1SIGNCHECK)
	@R1
	D=M
	@END
	D;JGT
	@R2
	M=-M
	@END
	0;JMP

(ZEROPRODUCT)
	@R2
	M=0
	@END
	0; JMP

(END)
	@END
	0;JMP
