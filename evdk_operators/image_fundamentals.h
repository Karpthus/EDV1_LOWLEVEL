/*! ***************************************************************************
 *
 * \brief     Fundamental functions for image processing
 * \file      image_fundamentals.h
 * \author    Hugo Arends - HAN Embedded Vision and Machine Learning
 * \author
 * \date      October 2024
 *
 * \see       Myler, H. R., & Weeks, A. R. (2009). The pocket handbook of
 *            image processing algorithms in C. Prentice Hall Press.
 *
 * \copyright 2024 HAN University of Applied Sciences. All Rights Reserved.
 *            \n\n
 *            Permission is hereby granted, free of charge, to any person
 *            obtaining a copy of this software and associated documentation
 *            files (the "Software"), to deal in the Software without
 *            restriction, including without limitation the rights to use,
 *            copy, modify, merge, publish, distribute, sublicense, and/or sell
 *            copies of the Software, and to permit persons to whom the
 *            Software is furnished to do so, subject to the following
 *            conditions:
 *            \n\n
 *            The above copyright notice and this permission notice shall be
 *            included in all copies or substantial portions of the Software.
 *            \n\n
 *            THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *            EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *            OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *            NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *            HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *            WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *            FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *            OTHER DEALINGS IN THE SOFTWARE.
 *
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/// Include guard to prevent recursive inclusion
#ifndef _IMAGE_FUNDAMENTALS_H_
#define _IMAGE_FUNDAMENTALS_H_

#include "image.h"

// Functions are documented in the source file

/// \name Functions for creating new images
/// \{
image_t *newUint8Image(const uint32_t cols, const uint32_t rows);
image_t *newInt16Image(const uint32_t cols, const uint32_t rows);
image_t *newInt32Image(const uint32_t cols, const uint32_t rows);
image_t *newFloatImage(const uint32_t cols, const uint32_t rows);
image_t *newUyvyImage(const uint32_t cols, const uint32_t rows);
image_t *newBgr888Image(const uint32_t cols, const uint32_t rows);
/// \}

/// \name Functions for creating new images without data allocation
/// \{
image_t *newEmptyUint8Image(const uint32_t cols, const uint32_t rows);
image_t *newEmptyInt16Image(const uint32_t cols, const uint32_t rows);
image_t *newEmptyInt32Image(const uint32_t cols, const uint32_t rows);
image_t *newEmptyFloatImage(const uint32_t cols, const uint32_t rows);
image_t *newEmptyUyvyImage(const uint32_t cols, const uint32_t rows);
image_t *newEmptyBgr888Image(const uint32_t cols, const uint32_t rows);
/// \}

/// \name Functions for deleting images
/// \{
void deleteUint8Image(image_t *img);
void deleteInt16Image(image_t *img);
void deleteInt32Image(image_t *img);
void deleteFloatImage(image_t *img);
void deleteUyvyImage(image_t *img);
void deleteBgr888Image(image_t *img);
void deleteAllImages(void);
/// \}

/// \name Functions for copying images
/// \{
void copyUint8Image(const image_t *src, image_t *dst);
void copyInt16Image(const image_t *src, image_t *dst);
void copyInt32Image(const image_t *src, image_t *dst);
void copyFloatImage(const image_t *src, image_t *dst);
void copyUyvyImage(const image_t *src, image_t *dst);
void copyBgr888Image(const image_t *src, image_t *dst);
/// \}

/// \name Functions for clearing images
/// \{
void clearUint8Image(image_t *img);
void clearInt16Image(image_t *img);
void clearInt32Image(image_t *img);
void clearFloatImage(image_t *img);

extern void clearUint8Image_cm33(const image_t *img);
/// \}

/// \name Getter functions for individual pixels
/// \{
extern uint8_pixel_t getUint8Pixel(const image_t *img, const int32_t c, const int32_t r);
extern int16_pixel_t getInt16Pixel(const image_t *img, const int32_t c, const int32_t r);
extern int32_pixel_t getInt32Pixel(const image_t *img, const int32_t c, const int32_t r);
extern float_pixel_t getFloatPixel(const image_t *img, const int32_t c, const int32_t r);
extern uyvy_pixel_t getUyvyPixel(const image_t *img, const int32_t c, const int32_t r);
extern bgr888_pixel_t getBgr888Pixel(const image_t *img, const int32_t c, const int32_t r);

/// \}

/// \name Setter functions for individual pixels
/// \{
extern void setUint8Pixel(const image_t *img, const int32_t c, const int32_t r, uint8_pixel_t const value);
extern void setInt16Pixel(const image_t *img, const int32_t c, const int32_t r, int16_pixel_t const value);
extern void setInt32Pixel(const image_t *img, const int32_t c, const int32_t r, int32_pixel_t const value);
extern void setFloatPixel(const image_t *img, const int32_t c, const int32_t r, float_pixel_t const value);
extern void setUyvyPixel(const image_t *img, const int32_t c, const int32_t r, const uyvy_pixel_t value);
extern void setBgr888Pixel(const image_t *img, const int32_t c, const int32_t r, const bgr888_pixel_t value);
/// \}

/// \name Conversion functions between image types
/// \{
void convertUyvyToUint8(image_t *src, image_t *dst);
void convertUyvyToInt16(image_t *src, image_t *dst);
void convertUint8ToUyvy(image_t *src, image_t *dst);
void convertUyvyToBgr888(image_t *src, image_t *dst);
void convertUint8ToBgr888(image_t *src, image_t *dst);
void convertBgr888ToUint8(image_t *src, image_t *dst);
void convertBgr888ToInt16(image_t *src, image_t *dst);

extern void convertUyvyToUint8_cm33(image_t *src, image_t *dst);
/// \}

/// \name Convenience functions for conversions between image types
/// \{
void convertToUint8(image_t *src, image_t *dst);
void convertToUyvy(image_t *src, image_t *dst);
void convertToBgr888(image_t *src, image_t *dst);
/// \}

// Functions are documented in the source file

void setSelectedToValue(const image_t *src, image_t *dst, const uint8_pixel_t selected, const uint8_pixel_t value);
uint32_t neighbourCount(const image_t *img, const int32_t x, const int32_t y, const uint8_pixel_t p, const eConnected c);
void scale(const image_t *src, image_t *dst);
void scaleInt16ToUint8(const image_t *src, image_t *dst);
void scaleFloatToUint8(const image_t *src ,image_t *dst);
void scaleFast(const image_t *src, image_t *dst);
void convolve(const image_t *src, image_t *dst, const image_t *msk);
void convolveFast(const image_t *src, image_t *dst, const image_t *msk);
void correlate(const image_t *src, image_t *dst, const image_t *msk);

extern void scaleFast_cm33(image_t *src, image_t *dst);

#endif // _IMAGE_FUNDAMENTALS_H_

#ifdef __cplusplus
}
#endif
