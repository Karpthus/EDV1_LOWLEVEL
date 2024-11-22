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

        //r6 is reserved for number of pixels
        //r0 is reserved for data src
        //r1 is reserved for data dst

		PUSH {r4-r10}		    //Sent registers to the stack
        LDMIA r0, {r2-r5}	    //Loading the struct of source
        MUL r6, r0, r1		    //Number of pixels
        MOV r0, r5			    //Move pointer data to r0
        LDMIA r1, {r2-r5}	    //Loading struct data dst
        MOV r1, r5			    //Moce data dst to r1
//main converting loop
//Loop is made to load 4 pixels into the register to reduce 
//Used for looping is BLO, this means branch if lower than CMP value
//
convert_loop:
        CMP r6, #4              //Checking the amount of pixels is >= 4
        BLO handle_remainder    //If under 4 pixels are remaining jump to single pixel handle_remainder

		LDMIA r0!, {r7, r8}     //Load 2 pixels in r7 and 2 pixels into r8

        LSR r9, r7, #8          // Extract 1st Y (from r7, bits [15:8])
        STRB r9, [r1], #1       // Store 1st Y to dst, increment r1

        LSR r9, r7, #24         // Extract 2nd Y (from r7, bits [31:24])
        STRB r9, [r1], #1       // Store 2nd Y to dst, increment r1

        LSR r9, r8, #8          // Extract 3rd Y (from r8, bits [15:8])
        STRB r9, [r1], #1       // Store 3rd Y to dst, increment r1

        LSR r9, r8, #24         // Extract 4th Y (from r8, bits [31:24])
        STRB r9, [r1], #1       // Store 4th Y to dst, increment r1

        SUBS r6, r6, #4         // Decrement pixel counter by 4

		BNE convert_loop  	//Jump back to the beginning until all pixels are processed

handle_remainder:
        CMP r6, #0              // Check if any pixels remain
        BEQ done                // If none remain, exit

        LDRH r7, [r0], #2       // Load remaining UYVY pixel (2 bytes)
        LSR r9, r7, #8          // Extract Y (from bits [15:8])
        STRB r9, [r1], #1       // Store Y to dst

        SUBS r6, r6, #1         // Decrement remaining pixel count
        BEQ handle_remainder      // Repeat for any remaining pixels

done:
		POP {r4-R10}  //return registers to their place
        BX lr
