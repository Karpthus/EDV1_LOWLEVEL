/*       void scaleFast(const image_t *src, image_t *dst)
{
    uint8_pixel_t min = UINT8_MAX, max = 0;
    uint32_t imsize_8 = (src->rows * src->cols) / 4;
    uint32_t *s = (uint32_t *)src->data;

    // Step 1: Find min and max
    //Sped up by handeling 4 pixels at a time.
    for (uint32_t i = 0; i < imsize_8; ++i) {
        uint32_t four_pixels = *s++;
        if((uint8_pixel_t)(four_pixels >> 0) > max){max = (uint8_pixel_t)(four_pixels >> 0);}
        if((uint8_pixel_t)(four_pixels >> 0) < min){min = (uint8_pixel_t)(four_pixels >> 0);}
        //Reading the second pixel
        if((uint8_pixel_t)(four_pixels >> 8) > max){max = (uint8_pixel_t)(four_pixels >> 8);}
        if((uint8_pixel_t)(four_pixels >> 8) < min){min = (uint8_pixel_t)(four_pixels >> 8);}
        //Reading the third pixel
        if((uint8_pixel_t)(four_pixels >> 16) > max){max = (uint8_pixel_t)(four_pixels >> 16);}
        if((uint8_pixel_t)(four_pixels >> 16) < min){min = (uint8_pixel_t)(four_pixels >> 16);}
        //Reading the four pixel
        if((uint8_pixel_t)(four_pixels >> 24) > max){max = (uint8_pixel_t)(four_pixels >> 24);}
        if((uint8_pixel_t)(four_pixels >> 24) < min){min = (uint8_pixel_t)(four_pixels >> 24);}
    }


}*/

        .syntax unified
        .cpu cortex-m33
        .thumb

        .section .text.scaleFast_cm33
        .weak scaleFast_cm33
        .type scaleFast_cm33, %function

scaleFast_cm33:

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
        // r0 Src pointer
        // r1 Dst pointer
        // r2 number of pixels
        // r3 Scale factor

        // r4 reserved for max
        // r5 reserved for min

        PUSH {r4-r11}		    //Sent registers to the stack
        LDMIA r0, {r2-r5}	    //Loading the struct of source
        MOV r0, r5			    //Move pointer data to r0
		LDMIA r1!, {r2-r5}      // r4 now contains the address of dst->data
		MUL r2, r2, r3		    //Number of pixels
		MOV r11, r2
		PUSH {r11}
		MOV r1, r5              // r1 becomes the working pointer for the destination

		MOV r4, #0
		MOV r5, #255

loop_min_max:
		CMP r2, #0
		BEQ scaleFactor

		LDMIA r0!, {r6}

		//0xFF = 00000000 00000000 00000000 11111111
		AND r10, r6, #0xFF  // Loading the first pixel with 0xFF
		CMP r10, r4
		IT HI
		MOVHI r4, r10 		// Update max when higher
		CMP r10, r5
		IT LS
		MOVLS r5, r10		// Update min when lower

		LSR r6, r6, #8      // Shift r6 right by 8 bits to find the second
		AND r10, r6, #0xFF
		CMP r10, r4
		IT HI
		MOVHI r4, r10 		// Update max when higher
		CMP r10, r5
		IT LS
		MOVLS r5, r10		// Update min when lower

		LSR r6, r6, #8      // Shift r6 right by 8 bits to find the second
		AND r10, r6, #0xFF
		CMP r10, r4
		IT HI
		MOVHI r4, r10 		// Update max when higher
		CMP r10, r5
		IT LS
		MOVLS r5, r10		// Update min when lower

		LSR r6, r6, #8      // Shift r6 right by 8 bits to find the second
		AND r10, r6, #0xFF
		CMP r10, r4
		IT HI
		MOVHI r4, r10 		// Update max when higher
		CMP r10, r5
		IT LS
		MOVLS r5, r10		// Update min when lower


		SUBS r2, r2, #4    // Decrement pixel counter by 4
    	B loop_min_max    // Repeat if more pixels remain

scaleFactor:
		MOV r6, #255
		SUBS r3, r4, r5
		LSLS r3, r3, #16
		UDIV r3, r6, r3

		POP {r11}
		SUBS r0, r0, r11
		MOV r2 , r11

		B applyScale

applyScale:
		LDMIA r0!, {r6}

		AND r10, r6, #0xFF
		SUBS r10, r10, r5
		MUL r8, r10 , r3
		LSRS r7, r7, #16
		BFI r7, r10, #0, #8

		LSR r6, r6, #8
		AND r10, r6, #0xFF
		SUBS r10, r10, r5
		MUL r10, r10 , r3
		BFI r7, r10, #8, #8

		LSR r6, r6, #8
		AND r10, r6, #0xFF
		SUBS r10, r10, r5
		MUL r10, r10 , r3
		BFI r7, r10, #16, #8

		LSR r6, r6, #8
		AND r10, r6, #0xFF
		SUBS r10, r10, r5
		MUL r10, r10 , r3
		BFI r7, r10, #24, #8

		STMIA r1!, {r7}

		SUBS r2, r2, #4
		CMP r2, #0
		BNE applyScale

done:
    	POP {r4-r11}         // Restore callee-saved registers
    	BX lr





