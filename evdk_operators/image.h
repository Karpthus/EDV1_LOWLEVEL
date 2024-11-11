/*! ***************************************************************************
 *
 * \brief     Image definitions
 * \file      image.h
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
#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <limits.h>
#include <float.h>
#include <stdint.h>
#include <stdlib.h>

#define EVDK5_WIDTH  (160)
#define EVDK5_HEIGHT (120)

// Set one of these defines to 1
#define USB_IMAGE_TYPE_BGR888 (1)
#define USB_IMAGE_TYPE_UYVY   (0)

#if (USB_IMAGE_TYPE_BGR888 == 1) && (USB_IMAGE_TYPE_UYVY == 1)
#error Only one USB image type can be supported
#endif

// ----------------------------------------------------------------------------
// Type definitions
// ----------------------------------------------------------------------------
/// Defines the image types
typedef enum
{
    IMGTYPE_UINT8  =  1, ///< An image with pixels of type ::uint8_pixel_t.
    IMGTYPE_INT16  =  2, ///< An image with pixels of type ::int16_pixel_t.
    IMGTYPE_INT32  =  4, ///< An image with pixels of type ::int32_pixel_t.
    IMGTYPE_FLOAT  =  8, ///< An image with pixels of type ::float_pixel_t.
    IMGTYPE_UYVY   = 16, ///< An image with pixels of type ::uyvy_pixel_t.
    IMGTYPE_BGR888 = 32, ///< An image with pixels of type ::bgr888_pixel_t.

}eImageType;

/// \brief Type definition of a uint8 pixel
///
/// 8 bits per pixel
typedef uint8_t uint8_pixel_t;

/// \brief Type definition of an int16 pixel
///
/// 16 bits per pixel
typedef int16_t int16_pixel_t;

/// \brief Type definition of an int32 pixel
///
/// 32 bits per pixel
typedef int32_t int32_pixel_t;

/// \brief Type definition of a float pixel
///
/// 32 bits per pixel
typedef float float_pixel_t;

/// \brief Type definition of an uyvy pixel
///
/// 32 bits per two pixels stored in the following format:
///
///  31                                0
/// |YYYYYYYY VVVVVVVV|YYYYYYYY UUUUUUUU|
/// |     pixel 2     |     pixel 1     |
typedef uint16_t uyvy_pixel_t;

/// \brief Type definition of an BGR888 pixel
///
/// 3*8=24 bits per pixel
typedef struct bgr888_pixel_t
{
    uint8_t b; ///< blue
    uint8_t g; ///< green
    uint8_t r; ///< red

}bgr888_pixel_t;

/// \name Definitions for min/max pixel values
/// \{

/*!
 * \brief Definitions for min/max pixel values
 */
#define UINT8_PIXEL_MIN (0x00)
#define UINT8_PIXEL_MAX (0xFF)

#define INT16_PIXEL_MIN (INT16_MIN)
#define INT16_PIXEL_MAX (INT16_MAX)

#define INT32_PIXEL_MIN (INT32_MIN)
#define INT32_PIXEL_MAX (INT32_MAX)

#define FLOAT_PIXEL_MIN (-(FLT_MAX))
#define FLOAT_PIXEL_MAX (FLT_MAX)

/// \}

/*!
 * \brief Platform dependent assertion macros
 */
#ifdef MCUXPRESSO_SDK
#ifdef DEBUG
#include "fsl_debug_console.h"
#define ASSERT(a,b) {                                                 \
                        if(a)                                         \
                        {                                             \
                            PRINTF("\r\n");                           \
                            PRINTF("ASSERTION ERROR: %s\r\n", b);     \
                            PRINTF("  Function: %s()\r\n", __func__); \
                            PRINTF("  File    : %s\r\n", __FILE__);   \
                            PRINTF("  Line    : %d\r\n", __LINE__);   \
                            while(1){}                                \
                        }                                             \
                    }
#else
#define ASSERT(a,b) ((void)0)
#endif
#else
#include <stdio.h>
#define ASSERT(a,b) {                                               \
                        if(a)                                       \
                        {                                           \
                            printf("\n");                           \
                            printf("ASSERTION ERROR: %s\n", b);     \
                            printf("  Function: %s()\n", __func__); \
                            printf("  File    : %s\n", __FILE__);   \
                            printf("  Line    : %d\n", __LINE__);   \
                            fflush(stdout);                         \
                            exit(0);                                \
                        }                                           \
                    }
#endif

/// Defines the attributes of an image
typedef struct
{
    int32_t     cols; ///< Number of columns in the image
    int32_t     rows; ///< Number of rows in the image
    eImageType  type; ///< The type of pixels in the image
    uint8_t    *data; ///< A pointer to the pixel data

}image_t;

/// Defines the relative brightness to look for in an image
typedef enum
{
    BRIGHTNESS_BRIGHT = 0, ///< Find relative bright objects
    BRIGHTNESS_DARK,       ///< Find relative dark objects

}eBrightness;

/// Defines the neighbourhood connectivity
typedef enum
{
    CONNECTED_FOUR  = 4, ///< 4-connected neighbourhood
    CONNECTED_EIGHT = 8, ///< 8-connected neighbourhood

}eConnected;

/// Defines a pixel coordinate
typedef struct
{
    int32_t x; ///< x-value
    int32_t y; ///< y-value

}point_t;

#endif // _IMAGE_H_

#ifdef __cplusplus
}
#endif
