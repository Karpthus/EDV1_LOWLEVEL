/*! ***************************************************************************
 *
 * \brief     Histogram operations
 * \file      histogram_operations.c
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
#include "image_fundamentals.h"
#include "histogram_operations.h"

/*!
 * \brief Creates an histogram of an image
 *
 * The function does not check memory boundaries. It simply assumes that the
 * \p hist pointer points to memory allocated by the caller of this function.
 *
 * \param[in]  img  A pointer to a source image
 * \param[out] hist A pointer to an array of 256 uint32_t
 */
void histogram(const image_t *img, uint32_t *hist)
{
    // Verify image validity
    ASSERT(img == NULL, "img image is invalid");
    ASSERT(img->data == NULL, "img data is invalid");
    ASSERT(img->type != IMGTYPE_UINT8, "img type is invalid");

    // Verify histogram validity
    ASSERT(hist == NULL, "hist is invalid");

    // Clear the histogram
    // Note: You might have learned that the memset() function is faster than
    //       this for-loop. However, the compiler optimizes such a loop to a
    //       call to memset() anyway (at least when optimization is set to
    //       -Ofast).
    for(uint32_t i=0; i<256; ++i)
    {
        hist[i]=0;
    }

    // Set the image size
    uint32_t imsize = img->rows * img->cols;

    // Set image pointer
    uint8_pixel_t *d = (uint8_pixel_t *)img->data;

    // Create the histogram
    for(uint32_t i=0; i<imsize; ++i)
    {
        hist[*d++]++;
    }
}

/*!
 * \brief Adjusts the image brightness
 *
 * The \p brightness is added to each pixel value in the source image and the
 * result is stored in the destination image.
 * \n
 * s_i = g_i + \p brightness
 *
 * \param[in]  src        A pointer to the source image
 * \param[out] dst        A pointer to the destination image
 * \param[in]  brightness Brightness value that is added to each pixel
 */
void brightness(const image_t *src, image_t *dst, const int32_t brightness)
{
    // Verify image validity
    ASSERT(src == NULL, "src image is invalid");
    ASSERT(dst == NULL, "dst image is invalid");
    ASSERT(src->data == NULL, "src data is invalid");
    ASSERT(dst->data == NULL, "dst data is invalid");
    ASSERT(src->type != IMGTYPE_UINT8, "src type is invalid");
    ASSERT(dst->type != IMGTYPE_UINT8, "dst type is invalid");

    // Verify image consistency
    ASSERT(src->cols != dst->cols, "src and dst have different number of columns");
    ASSERT(src->rows != dst->rows, "src and dst have different number of rows");

    int32_t i;

    // Loop all pixels
    for(int32_t y=0; y<src->rows; y++)
    {
        for(int32_t x=0; x<src->cols; x++)
        {
            // Get the pixel from the original image and modify brightness
            i = getUint8Pixel(src, x, y) + brightness;

            // Clip the result
            if(i>255)
            {
                i=255;
            }

            if(i<0)
            {
                i=0;
            }

            // Store the result
            setUint8Pixel(dst, x, y, i);
        }
    }
}

/*!
 * \brief Enhances the visual appearance of an image
 *
 * The function calculates the average pixel value of the source image. This
 * average value is then used to calculate the destination pixel value as
 * follows:
 * \n
 *     s_i = (\p contrast * (g_i - average)) + average
 * \n
 *
 * \param[in]  src      A pointer to the source image
 * \param[out] dst      A pointer to the destination image
 * \param[in]  contrast New distance to the average pixel value
 *
 * \todo Implement this function
 */
void contrast(const image_t *src, image_t *dst, const float contrast)
{
    // ********************************************
    // Remove this block when implementation starts
    #warning TODO: contrast

    // Added to prevent compiler warnings

    (void)src;
    (void)dst;
    (void)contrast;

    return;
    // ********************************************
}
