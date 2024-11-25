/**
  ******************************************************************************
  * File    convertuyvytouint8_cm33.s
  * Authors ...
  * Version V1.0
  * Date    09 October 2024
  * Brief   ARM Cortex-M33 assembler routine to copy and convert a UYVY image
  *         to a uint8 image.
  *         Assumptions:
  *             - aligned data
  *             - number of bytes is divisible by 8
  *
  *         ARM Cortex-M33 instruction set can be found here:
  *         https://developer.arm.com/documentation/100235/0100
  *
  *         Note: For using this function declare the following function
  *               prototype external in the project:
  *
  *               extern void convertuyvytouint8_cm33(image_t *src, image_t *dst);
  *
  *         Example usage:
  *
  *               convertuyvytouint8_cm33(src, dst);
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

        .section .text.convertUyvyToUint8_cm33
        .weak convertUyvyToUint8_cm33
        .type convertUyvyToUint8_cm33, %function
/*void convertUyvyToUint8(image_t *src, image_t *dst)
{
    uint32_t i = src->rows * src->cols;
    uint8_pixel_t *uint8_pixel = (uint8_pixel_t *)dst->data;
    uyvy_pixel_t *uyvy_pixel = (uyvy_pixel_t *)src->data;

    // Loop all pixels and convert
    while(i-- > 0)
    {
        *uint8_pixel = (uint8_pixel_t)(*uyvy_pixel >> 8);

        // Next pixel
        uint8_pixel++;
        uyvy_pixel++;
    }
}*/

//Intial setup for the instructions
convertUyvyToUint8_cm33:

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

        //r2 is reserved for number of pixels
        //r0 is reserved for data src
        //r1 is reserved for data dst

		PUSH {r4-r10}		    //Sent registers to the stack
        LDMIA r0, {r2-r5}	    //Loading the struct of source
        MOV r0, r5			    //Move pointer data to r0
		LDMIA r1!, {r2-r5}      // r4 now contains the address of dst->data
		MUL r2, r2, r3		    //Number of pixels
		MOV r1, r5           // r1 becomes the working pointer for the destination		    //Moce data dst to r1
//main converting loop
//Loop is made to load 8 pixels into the register to reduce
//
//
convert_loop:
    LDMIA r0!, {r3, r4, r5, r6}  // Load 8 UYVY pixels into r3, r4, r5 and r6

    //First register
    LSR r7, r3, #8		// Extract 1st Y (from r3, bits [15:8])
    BFI r8, r7, #0, #8  // Insert into r8 (bits [7:0])
    LSR r7, r3, #24		// Extract 2nd Y (from r3, bits [31:24])
    BFI r8, r7, #8, #8  // Insert into r8 (bits [15:8])
    //Second register
    LSR r7, r4, #8		// Extract 3rd Y (from r4, bits [15:8])
    BFI r8, r7, #16, #8 // Insert into r8 (bits [23:16])
    LSR r7, r4, #24		// Extract 4nd Y (from r4, bits [31:24])
    BFI r8, r7, #24, #8 // Insert into r8 (bits [31:24])
	//third register
    LSR r7, r5, #8		// Extract 5st Y (from r5, bits [15:8])
    BFI r9, r7, #0, #8  // Insert into r9 (bits [7:0])
    LSR r7, r5, #24		// Extract 6nd Y (from r5, bits [31:24])
    BFI r9, r7, #8, #8  // Insert into r9 (bits [15:8])
    //fourth register
    LSR r7, r6, #8		// Extract 7rd Y (from r6, bits [15:8])
    BFI r9, r7, #16, #8 // Insert into r9 (bits [23:16])
    LSR r7, r6, #24		// Extract 7nd Y (from r6, bits [31:24])
    BFI r9, r7, #24, #8 // Insert into r9 (bits [31:24])

    STMIA r1!, {r8 -r9}   // Store 8 Y values (2 bytes each) to dst, increment r1 by 8 bytes

    SUBS r2, r2, #8     // Decrement pixel counter by 4
    BNE convert_loop     // Repeat if more pixels remain

done:
    POP {r4-r10}         // Restore callee-saved registers
    BX lr                // Return to caller                  // Return to caller
