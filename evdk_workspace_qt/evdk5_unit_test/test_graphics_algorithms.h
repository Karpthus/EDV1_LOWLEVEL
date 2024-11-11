/*! ***************************************************************************
 *
 * \brief     Unit test functions for graphics algorithms
 * \file      test_graphics_algorithms.h
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

#ifndef _TEST_GRAPHICS_ALGORITHMS_H_
#define _TEST_GRAPHICS_ALGORITHMS_H_

/// \brief Unit test function for affineTransformation()
void test_affineTransformation(void);

/// \brief Unit test function for warpPerspective()
void test_warpPerspective(void);

/// \brief Unit test function for warpPerspectiveFast()
void test_warpPerspectiveFast(void);

/// \brief Unit test function for zoom()
void test_zoom(void);

/// \brief Unit test function for zoomFactor()
void test_zoomFactor(void);

#endif // _TEST_GRAPHICS_ALGORITHMS_H_
