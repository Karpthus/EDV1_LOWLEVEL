/*! ***************************************************************************
 *
 * \brief     Unit test functions
 * \file      main.h
 * \author    Hugo Arends - HAN Embedded Vision and Machine Learning
 * \author
 * \date      June 2020
 *
 * \copyright 2020 HAN University of Applied Sciences. All Rights Reserved.
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

#ifndef _MAIN_H_
#define _MAIN_H_

#include "stdio.h"
#include "string.h"
#include "unity.h"

#include "image.h"
#include "operators.h"

#include "test_coding_and_compression.h"
#include "test_graphics_algorithms.h"
#include "test_histogram_operations.h"
#include "test_image_fundamentals.h"
#include "test_mensuration.h"
#include "test_morphological_filters.h"
#include "test_nonlinear_filters.h"
#include "test_segmentation.h"
#include "test_spatial_filters.h"
#include "test_spatial_frequency_filters.h"
#include "test_transforms.h"

// ----------------------------------------------------------------------------
// Defines
// ----------------------------------------------------------------------------
/// Define for matching basic pixel data
#define TEST_ASSERT_EQUAL_uint8_pixel_t_ARRAY_MESSAGE TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE
#define TEST_ASSERT_EQUAL_int16_pixel_t_ARRAY_MESSAGE TEST_ASSERT_EQUAL_INT16_ARRAY_MESSAGE

// ----------------------------------------------------------------------------
// Function prototypes
// ----------------------------------------------------------------------------
/*!
 * \brief Pretty prints image data to stdout
 *
 * The data is pretty printed based on the image type.
 * The \p title is printed before the image data.
 *
 * \param[in] img
 * \param[in] title
 */
void prettyprint(const image_t *img, const char *title);

// ----------------------------------------------------------------------------
// Function implementation
// ----------------------------------------------------------------------------

#endif // _MAIN_H_
