/*! ***************************************************************************
 *
 * \brief     Segmentation operations
 * \file      segmentation.h
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
#ifndef _SEGMENTATION_H_
#define _SEGMENTATION_H_

#include "image.h"

// Functions are documented in the source file

void threshold(const image_t *src, image_t *dst,
               const uint8_pixel_t min, const uint8_pixel_t max);
void thresholdOptimum(const image_t *src, image_t *dst, const eBrightness b);
void threshold2Means(const image_t *src, image_t *dst, const eBrightness b);
void thresholdOtsu(const image_t *src, image_t *dst, const eBrightness b);
void lineDetector(const image_t *src, image_t *dst, int16_t mask[][3]);

#endif // _SEGMENTATION_H_

#ifdef __cplusplus
}
#endif
