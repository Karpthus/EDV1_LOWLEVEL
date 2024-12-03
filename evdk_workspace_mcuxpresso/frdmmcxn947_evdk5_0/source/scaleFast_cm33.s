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

    // Step 2: Precompute LUT
    uint8_pixel_t LUT[256];
    if (max == min) {
        // If all pixels are the same, map everything to a neutral value (e.g., 128)
        for (int i = 0; i < 256; ++i) {
            LUT[i] = 128;
        }
    } else {
        //Using the standard formula to compute the scale factor for the image
        float scaleFactor = 255.0f / (max - min);
        for (int i = 0; i < 256; ++i) {
            LUT[i] = (uint8_pixel_t)((i - min) * scaleFactor + 0.5f);
        }
    }

    // Step 3: Scale and write pixels using LUT
    uint32_t *dst_data = (uint32_t *)dst->data;
    s = (uint32_t *)src->data; // Reset source pointer

    for (uint32_t i = 0; i < imsize_8; ++i) {
        // First 4 pixels
        uint32_t four_pixels = *s++;
        uint8_pixel_t p0 = LUT[(four_pixels >> 0) & 0xFF];
        uint8_pixel_t p1 = LUT[(four_pixels >> 8) & 0xFF];
        uint8_pixel_t p2 = LUT[(four_pixels >> 16) & 0xFF];
        uint8_pixel_t p3 = LUT[(four_pixels >> 24) & 0xFF];
        *dst_data++ = (p0 << 0) | (p1 << 8) | (p2 << 16) | (p3 << 24);
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
        PUSH {r4-r11}		    //Sent registers to the stack
        LDMIA r0, {r2-r5}	    //Loading the struct of source
        MOV r0, r5			    //Move pointer data to r0
		LDMIA r1!, {r2-r5}      // r4 now contains the address of dst->data
		MUL r2, r2, r3		    //Number of pixels
		MOV r1, r5           // r1 becomes the working pointer for the destination


