/*! ***************************************************************************
 *
 * \brief     Evaluation of features associated with objects in an image
 * \file      mensuration.h
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
#ifndef _MENSURATION_H_
#define _MENSURATION_H_

#include "image.h"

/// Defines features that can be measured of BLOBs
typedef struct
{
    point_t centroid;    ///< The centroid coordinate of the BLOB
    uint32_t area;       ///< The BLOB area in number of pixels
    float perimeter;     ///< The perimeter of the BLOB
    float circularity;   ///< The circularity of the BLOB
    float hu_moments[4]; ///< The first four Hu invariant moments of the BLOB

}blobinfo_t;

// Functions are documented in the source file

void area(const image_t *img, blobinfo_t *blobinfo, const uint32_t blobnr);
void centroid(const image_t *img, blobinfo_t *blobinfo, const uint32_t blobnr);
uint32_t labelIterative(const image_t *src, image_t *dst, const eConnected connected);
uint32_t labelTwoPass(const image_t *src, image_t *dst, const eConnected connected,
                      const uint32_t lutSize);
void circularity(const image_t *img, blobinfo_t *blobinfo, const uint32_t blobnr);
void huInvariantMoments(const image_t *img, blobinfo_t *blobinfo,const uint32_t blobnr);
void perimeter(const image_t *img, blobinfo_t *blobinfo, const uint32_t blobnr);

#endif // _MENSURATION_H_

#ifdef __cplusplus
}
#endif
