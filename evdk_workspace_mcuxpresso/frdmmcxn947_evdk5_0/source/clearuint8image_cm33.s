/**
  ******************************************************************************
  * File    clearuint8image_cm33.s
  * Authors ...
  * Version V4.0
  * Date    09 October 2024
  * Brief   ARM Cortex-M33 assembler routine to set a byte sequence to 0.
  *         Assumptions:
  *             - aligned data
  *             - uint8_t image type
  *             - number of data bytes in the image is divisible by 32
  *
  *         ARM Cortex-M33 instruction set can be found here:
  *         https://developer.arm.com/documentation/100235/0100
  *
  *         Note: For using this function declare the following function
  *               prototype external in the project:
  *
  *               extern void clearuint8image_cm33(const image_t *img);
  *
  *         Example usage:
  *
  *               clearuint8image_cm33(img);
  *
  *
  * Copyright (C) 2024 HAN University of Applied Sciences. All Rights Reserved.
  *
  * Permission is hereby granted, free of charge, to any person obtaining a
  * copy of this software and associated documentation files (the "Software"),
  * to deal in the Software without restriction, including without limitation
  * the rights to use, copy, modify, merge, publish, distribute, sublicense,
  * and/or sell copies of the Software, and to permit persons to whom the
  * Software is furnished to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in
  * all copies or substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  * IN THE SOFTWARE.
  ******************************************************************************
  */
        .syntax unified
        .cpu cortex-m33
        .thumb

        .section .text.clearUint8Image_cm33
        .weak clearUint8Image_cm33
        .type clearUint8Image_cm33, %function

clearUint8Image_cm33:

        // r0-r3 : function arguments and scratch registers
        //         caller-saved: must be saved before this procedure is called
        //         r0-r1 are also the result registers
        // r4-r11: local variables
        //         callee-saved: must be saved by this procedure
        // r12   : scratch register
        //         caller-saved: must be saved before this procedure is called
        // r13   : Stack Pointer
        // r14   : Link Register
        // r15   : Program Counter


		PUSH {r4-r11} // Save regsiters to the stack

		//Loading image data, takes the data in r0 and splits it over r0,r1,r2,r3
        // r0 = image_t.cols
        // r1 = image_t.rows
        // r2 = image_t.type
        // r3 = image_t.data
		LDMIA r0, {r0-r3}

		//Save amout of pixels in r1, rows * cols
		MUL r1, r0, r1
		MOV r0, r3 // moce picture data from r3 to r0

		//Setting all the registers to 0 so that all the registers are used to clear the image
		MOV r4, #0x00000000
		MOV r5 , r4
		MOV r6 , r4
		MOV r7 , r4
		MOV r8 , r4
		MOV r9 , r4
		MOV r10 , r4
		MOV r11 , r4

//Keep in mind that B is checking for branching, what is specified after is what kind of check is made
//EQ = equals to 0
//HI = > than 0
//NE = not equal
//These are the ones used in the code, check the documentation for more information

// Since using 7 registers we need to keep in mind that 4 x 7 is 28 bytes each loop
clear_loop:
		STMIA r0!, {r4-r11}  //Store all multiple values to r0, and update r0
		SUBS r1, r1, #32	 //Substract 28 of the number of pixels
		BHI clear_loop		 //Checks if there are more pixels to be cleared

		//Handeling remaining pixels if there are any
		ADD r1, r1, #28 	 //Restore r1 to positive if it went (negative)
		CMP r1, #0 			 //Check if rhere are any pixels left
		BEQ done			 //If no pixels are left skip clear_remainder and clean up the stack

//Clearing the remaining
clear_remainder:
		STRB r4, [r0], #1	 //Store a the lowerst byte of r4 that equal 0 in the memory from r0
							 //And the #1 automaticly updates the r0 register
		SUBS r1 ,r1, #1		 //Decrement the amount of pixels left to clear
		BNE clear_remainder  //Keep clearing pixels until r1 == 0

done:
		//Clean up the stack after finishing the operation
		POP {r4-r11}
        BX lr
