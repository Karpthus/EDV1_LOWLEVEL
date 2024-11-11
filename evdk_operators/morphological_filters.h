/*! ***************************************************************************
 *
 * \brief     Smooth the contours of objects and decompose an image into its
 *            fundamental geometrical shape
 * \file      morphological_filters.h
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
#ifndef _MORPHOLOGICAL_FILTERS_H_
#define _MORPHOLOGICAL_FILTERS_H_

#include "image.h"

// Functions are documented in the source file

void dilation(const image_t *src, image_t *dst, const uint8_t *mask, const uint8_t n);
void dilationGray(const image_t *src, image_t *dst, const uint8_t *mask, const uint8_t n);
void erosion(const image_t *src, image_t *dst, const uint8_t *mask, const uint8_t n);
void erosionGray(const image_t *src, image_t *dst, const uint8_t *mask, const uint8_t n);
void fillHolesIterative(const image_t *src, image_t *dst, const eConnected c);
uint32_t fillHolesTwoPass(const image_t *src, image_t *dst,
                          const eConnected connected, const uint32_t lutSize);
void hitmiss(const image_t *src, image_t *dst, const uint8_t *m1, const uint8_t *m2);
void outline(const image_t *src, image_t *dst, const uint8_t *mask, const uint8_t n);
void removeBorderBlobsIterative(const image_t *src, image_t *dst, const eConnected c);
uint32_t removeBorderBlobsTwoPass(const image_t *src, image_t *dst,
                                  const eConnected connected, const uint32_t lutSize);
void skeleton(const image_t *src, image_t *dst, const uint8_t *mask, const uint8_t n);

#endif // _MORPHOLOGICAL_FILTERS_H_

#ifdef __cplusplus
}
#endif
