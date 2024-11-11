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


		// \todo Implement this function


        BX lr
