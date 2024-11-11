/**
  ******************************************************************************
  * File    rotate180_cm33.s
  * Authors Tim van der Staaij
  *         H. Arends
  * Version V4.0
  * Date    09 October 2024
  * Brief   ARM Cortex-M33 assembler routine to reverse the order of a
  *         byte sequence. Assumptions:
  *             - aligned data
  *             - uint8_t image type
  *             - number of data bytes in the image is divisible by 32
  *
  *         Original by Tim van der Staaij for Cortex-M4 and Keil ARM compiler
  *         Updated by Hugo Arends for Cortex-M33 and GCC compiler
  *
  *         ARM Cortex-M33 instruction set can be found here:
  *         https://developer.arm.com/documentation/100235/0100
  *
  *         Note: For using this function declare the following function
  *               prototype external in the project:
  *
  *               extern void rotate180_cm33(const image_t *img);
  *
  *         Example usage:
  *
  *               rotate180_cm33(img);
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

        .section .text.rotate180_cm33
        .weak rotate180_cm33
        .type rotate180_cm33, %function

rotate180_cm33:

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

        // Register allocation:
        // r0 Forward pointer
        // r1 Backward pointer
        // r2-r5 Input words read from forward pointer / Output words
        // r6-r9 Input words read from backward pointer / Output words
        // r10 Additional output word used for word reversal
        PUSH {r4-r10}

        // Load four words from image pointer
        // r0 = image_t.cols
        // r1 = image_t.rows
        // r2 = image_t.type
        // r3 = image_t.data
        LDMIA r0, {r0-r3}

        // Image size = cols x rows
        MUL r1, r0, r1

        // Backward pointer = size + data pointer
        ADD r1, r1, r3

        // Forward pointer = data pointer
        MOV r0, r3

rev_loop:
        LDMIA r0, {r2-r5}  // Load four words from forward pointer
                           //  - LDM   : Load Multiple registers
                           //  - IA    : Increment address After each access
                           //  - {...} : Register list

        LDMDB r1!, {r6-r9} // Load four words from backward pointer and update
                           //  - LDM   : Load Multiple registers
                           //  - DB    : Decrement address Before each access
                           //  - !     : The final address that is loaded from,
                           //            is written back into r1
                           //  - {...} : Register list

        // Both pointers (r0 and r1) now point to the lowest word that
        // they just loaded

        // Reverse the bytes in each word, as well as the words themselves
        REV r10, r2
        REV r2,  r9
        REV r9,  r3
        REV r3,  r8
        REV r8,  r4
        REV r4,  r7
        REV r7,  r5
        REV r5,  r6

        STMIA r0!, {r2-r5}  // Write transformed words back to forward pointer and update
                            //  - STM   : Store Multiple registers
                            //  - IA    : Increment address After each access
                            //  - !     : The final address + 1 that is stored to,
                            //            is written back into r0
                            //  - {...} : Register list

        STMIA r1, {r7-r10}  // Write transformed words back to backward pointer
                            //  - STM   : Store Multiple registers
                            //  - IA    : Increment address After each access
                            //  - {...} : Register list

        // Repeat until forward and backward pointer meet
        CMP r0, r1
        BNE rev_loop

        POP {r4-r10}
        BX lr
