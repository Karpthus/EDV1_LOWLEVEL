/*! ***************************************************************************
 *
 * \brief     Graphics algorithms
 * \file      graphics_algorithms.h
 * \author    Hugo Arends - HAN Embedded Vision and Machine Learning
 * \author
 * \date      October 2024
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
#ifndef _GRAPHICS_ALGORITHMS_H_
#define _GRAPHICS_ALGORITHMS_H_

#include "image.h"

/// Defines the direction of transformation functions
typedef enum
{
    TRANSFORM_FORWARD,  ///< Forward transformation
    TRANSFORM_BACKWARD, ///< Backward (reverse) transformation

}eTransformDirection;

/// Defines the zoom direction for zooming functions
typedef enum
{
    ZOOM_IN, ///< Zoom in
    ZOOM_OUT ///< Zoom out

}eZoom;

// Functions are documented in the source file

void textSetfont(const char *f);
void textSetUint8Colors(const uint8_pixel_t background, const uint8_pixel_t foreground);
void textSetBgr888Colors(const bgr888_pixel_t background, const bgr888_pixel_t foreground);
void textSetxy(const int32_t x, const int32_t y);
void textSetFlipCharacters(const uint32_t flip);
void textPutchar(image_t *img, const char c);
void textPutstring(image_t *img, const char *str);
void drawLineUint8(image_t *img, point_t p1, point_t p2, uint8_pixel_t val);
void drawLineBgr888(image_t *src, point_t p1, point_t p2, bgr888_pixel_t val);
void drawLineUyvy(image_t *src, point_t p1, point_t p2, uyvy_pixel_t val);
void affineTransformation(const image_t *src, image_t *dst,
                          eTransformDirection d, float m[][3]);
void rotate(const image_t *src, image_t *dst, const float radians,
            const point_t center);
void rotate180_c(const image_t *img);
void warpPerspective(const image_t *src, image_t *dst,
                     const point_t *from, const point_t *to,
                     eTransformDirection d);
void warpPerspectiveFast(const image_t *src, image_t *dst,
                         const point_t *from, eTransformDirection d);
void zoom(const image_t *src, image_t *dst,
          const int32_t x, const int32_t y,
          const int32_t hor, const int32_t ver,
          const eZoom zd);
void zoomFactor(const image_t *src, image_t *dst,
          const int32_t x, const int32_t y,
          const int32_t hor, const int32_t ver,
          const eZoom zd, const int16_t factor);

// Hardware specific functions are documented in this header file

/*!
 * \brief Rotate the image 180 degrees (ARM Cortex-M implementation)
 *
 * This operation is also known as flipping in both horizontal and vertical
 * direction. This function uses inline assembly for improved performance on
 * ARM Cortex-M devices (MCUXpresso-IDE only).
 *
 * \param[in] img
 */
void rotate180_arm(const image_t *img);

/*!
 * \brief Rotate the image 180 degrees (ARM Cortex-M33 assembly implementation)
 *
 * This operation is also known as flipping in both horizontal and vertical
 * direction. The implementation and more documentation is available in the
 * file rotate180_cm33.s (MCUXpresso-IDE only).
 *
 * \param[in] img
 */
extern void rotate180_cm33(const image_t *img);

#endif // _GRAPHICS_ALGORITHMS_H_

#ifdef __cplusplus
}
#endif
