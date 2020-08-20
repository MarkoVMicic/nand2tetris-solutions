// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.
//
//Psuedocode:
//
//LOOPBEGIN:
//	keyboard_input = RAM[KEYBOARD]		// If keyboard is pressed, keyboard_input == 1, otherwise keyboard_input == 0. 
//	if keyboard_input == 0:				// Check if keyboard is not pressed
//		if screen[0] == 0:				// Nothing needs to be done if the screen is empty. 
//			JMP LOOPBEGIN
//		else:
//			empty_screen()				// Empty the screen if it is not empty. 
//			JMP LOOPBEGIN
//
//	else:								// Keyboard is pressed
//		if screen[0] == 0:				
//			fill_screen()				// Fill the screen if it is empty
//			JMP LOOPBEGIN
//		else:
//			JMP LOOPBEGIN				// Nothing needs to be done if the screen is already filled 
//
//******************************************************************
//Function psuedocode:
//
//def empty_screen():
//	addr = SCREEN
//	sizeofscreen = sizeof(SCREEN)	// Look up this size. Should be number of pixels on the screen. 
//	i = 0
//	EMPTYSCREENLOOP:
//		if i > sizeofscreen, goto EMPTYSCREENEND	// don't write outside of the screen memory
//		RAM[addr] = 0		// 0 = 0000000000000000 in two's complement
//		addr = addr + 1		// Next set of 16 pixels
//		i = i+1
//		GOTO: EMPTYSCREENLOOP
//
//
//
//	EMPTYSCREENEND:
//		goto LOOPBEGIN
//******************************************************************
//def fill_screen():
//	addr = SCREEN
//	sizeofscreen = sizeof(SCREEN)	// Look up this size. Should be number of pixels on the screen. 
//	i = 0
//	FILLSCREENLOOP:
//		if i > sizeofscreen, goto FILLSCREENEND
//		RAM[addr] = -1		// -1 = 1111111111111111 in two's complement
//		addr = addr + 1		// Next set of 16 pixels
//		i = i+1
//		GOTO: FILLSCREENLOOP
//
//
//
//	FILLSCREENEND:
//		goto LOOPBEGIN
//
//
//******************************************************************
// Machine Code 

	@8192
	D=A
	@sizeofscreen 					// Store the size of the screen here.
	M=D	
(BEGINPROGRAM)
	@KBD 		// Base address of keyboard memory, 24576
	D=M 		// D-register contains currently pressed key. 

	

	@EMPTYSCREENCOMMAND
	D; JEQ 					// Jump to emptying the screen if the keyboard input is 0 (no key pressed), otherwise continue to 
							// fill screen. 

(FILLSCREENCOMMAND)			// Unused label added for readability of code. 
	@SCREEN
	D=M 					// D register now contains the color of the first pixel on the screen.
	@BEGINPROGRAM
	D;JNE 					// Jump to the beginning of the program if the first pixel is already filled, otherwise continue to 
							// fill screen.
	@SCREEN
	D=A 					// D register now contains the base address of the screen (i.e. memory address of first pixel)
	@address
	M=D 					// The memory found at variable address now contains the address of the first pixel

	@i
	M=0						// Initialize i=0

	(FILLSCREENLOOP)
		@i
		D=M
		@sizeofscreen
		D=D-M
		@BEGINPROGRAM
		D; JGT				// if i > sizeofscreen, we have emptied all pixels, so go back to beginning of program. 

		@address
		A=M 				// Grab the value of current pixel address from the variable address
		M=-1					// Set RAM[address] = -1 (which is bitwise 1111111111111111)

		@i
		M=M+1				// i++
		@address
		M=M+1				// address++
		@FILLSCREENLOOP
		0;JMP 				// jump to beginning of loop.


(EMPTYSCREENCOMMAND)
	@SCREEN
	D=M 					// D register now contains the color of the first pixel on the screen. 
	@BEGINPROGRAM
	D; JEQ					// Jump to the beginning of the program if the first pixel is already empty, otherwise continue to 
							// empty screen. 

	@SCREEN
	D=A 					// D register now contains the base address of the screen (i.e. memory address of first pixel)
	@address
	M=D 					// The memory found at variable address now contains the address of the first pixel

	@i
	M=0						// Initialize i=0

	(EMPTYSCREENLOOP)
		@i
		D=M
		@sizeofscreen
		D=D-M
		@BEGINPROGRAM
		D; JGT				// if i > sizeofscreen, we have emptied all pixels, so go back to beginning of program. 

		@address
		A=M 				// Grab the value of current pixel address from the variable address
		M=0					// Set RAM[address] = 0

		@i
		M=M+1				// i++
		@address
		M=M+1				// address++
		@EMPTYSCREENLOOP
		0;JMP 				// jump to beginning of loop. 









