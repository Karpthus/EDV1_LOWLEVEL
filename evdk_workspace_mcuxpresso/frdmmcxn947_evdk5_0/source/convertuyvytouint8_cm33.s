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


		// \todo Implement this function


        BX lr
