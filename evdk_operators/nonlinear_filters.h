/*! ***************************************************************************
 *
 * \brief     Compute a given nonlinear function over a local window and
 *            replacing a single pixel value
 * \file      nonlinear_filters.h
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
#ifndef _NONLINEAR_FILTERS_H_
#define _NONLINEAR_FILTERS_H_

#include "image.h"

// Functions are documented in the source file

void harmonic(const image_t *src, image_t *dst, const uint8_t n);
void maximum(const image_t *src, image_t *dst, const uint8_t n);
void mean(const image_t *src, image_t *dst, const uint8_t n);
void meanFast(const image_t *src, image_t *dst);
void median(const image_t *src, image_t *dst, const uint8_t n);
void midpoint(const image_t *src, image_t *dst, const uint8_t n);
void minimum(const image_t *src, image_t *dst, const uint8_t n);
void range(const image_t *src, image_t *dst, const uint8_t n);

#endif // _NONLINEAR_FILTERS_H_

#ifdef __cplusplus
}
#endif
