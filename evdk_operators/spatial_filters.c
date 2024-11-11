/*! ***************************************************************************
 *
 * \brief     Discrete convolutional filters or filters that convolve on image
 *            with the other
 * \file      spatial_filters.c
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
#include "spatial_filters.h"

#include <math.h>

/*!
 * \brief Applies a 3x3 Gaussian filter
 *
 * Gaussian filters remove high frequency noise, but causes blurring.
 *
 * \param[in]  src A pointer to the source image
 * \param[out] dst A pointer to the destination image
 */
void gaussianFilter_3x3(const image_t *src, image_t *dst)
{
    // Verify image validity
    ASSERT(src == NULL, "src image is invalid");
    ASSERT(dst == NULL, "dst image is invalid");
    ASSERT(src->data == NULL, "src data is invalid");
    ASSERT(dst->data == NULL, "dst data is invalid");
    ASSERT(src->type != IMGTYPE_INT16, "src type is invalid");
    ASSERT(dst->type != IMGTYPE_INT16, "dst type is invalid");

    // Verify image consistency
    ASSERT(src->cols != dst->cols, "src and dst have different number of columns");
    ASSERT(src->rows != dst->rows, "src and dst have different number of rows");
    ASSERT(src == dst, "src and dst are the same images");

    // Define mask
    int16_pixel_t msk_data[3 * 3] =
    {
        1,2,1,
        2,4,2,
        1,2,1,
    };

    // Create the mask image
    image_t msk =
    {
        .cols=3,
        .rows=3,
        .type=IMGTYPE_INT16,
        .data=(uint8_t *)msk_data,
    };

    convolve(src, dst, &msk);
}

/*!
 * \brief Applies a 5x5 Gaussian filter
 *
 * Gaussian filters remove high frequency noise, but causes blurring.
 *
 * \param[in]  src A pointer to the source image
 * \param[out] dst A pointer to the destination image
 */
void gaussianFilter_5x5(const image_t *src, image_t *dst)
{
    // Verify image validity
    ASSERT(src == NULL, "src image is invalid");
    ASSERT(dst == NULL, "dst image is invalid");
    ASSERT(src->data == NULL, "src data is invalid");
    ASSERT(dst->data == NULL, "dst data is invalid");
    ASSERT(src->type != IMGTYPE_INT16, "src type is invalid");
    ASSERT(dst->type != IMGTYPE_INT16, "dst type is invalid");

    // Verify image consistency
    ASSERT(src->cols != dst->cols, "src and dst have different number of columns");
    ASSERT(src->rows != dst->rows, "src and dst have different number of rows");
    ASSERT(src == dst, "src and dst are the same images");

    // Define mask
    int16_pixel_t msk_data[5 * 5] =
    {
        1, 4, 7, 4, 1,
        4,16,26,16, 4,
        7,26,41,26, 7,
        4,16,26,16, 4,
        1, 4, 7, 4, 1,
    };

    // Create the mask image
    image_t msk =
    {
        .cols=5,
        .rows=5,
        .type=IMGTYPE_INT16,
        .data=(uint8_t *)msk_data,
    };

    convolve(src, dst, &msk);
}

/*!
 * \brief Applies a 3x3 Laplacian filter
 *
 * Laplacian filters give the second derivative in two directions.
 * This filter enhances changes.
 *
 * \param[in]  src A pointer to the source image
 * \param[out] dst A pointer to the destination image
 */
void laplacianFilter_3x3(const image_t *src, image_t *dst)
{
    // Verify image validity
    ASSERT(src == NULL, "src image is invalid");
    ASSERT(dst == NULL, "dst image is invalid");
    ASSERT(src->data == NULL, "src data is invalid");
    ASSERT(dst->data == NULL, "dst data is invalid");
    ASSERT(src->type != IMGTYPE_INT16, "src type is invalid");
    ASSERT(dst->type != IMGTYPE_INT16, "dst type is invalid");

    // Verify image consistency
    ASSERT(src->cols != dst->cols, "src and dst have different number of columns");
    ASSERT(src->rows != dst->rows, "src and dst have different number of rows");
    ASSERT(src == dst, "src and dst are the same images");

    // Define mask
    int16_pixel_t msk_data[3 * 3] =
    {
        0,-1, 0,
       -1, 4,-1,
        0,-1, 0,
    };

    // Create the mask image
    image_t msk =
    {
        .cols=3,
        .rows=3,
        .type=IMGTYPE_INT16,
        .data=(uint8_t *)msk_data,
    };

    convolve(src, dst, &msk);
}

/*!
 * \brief Applies a 5x5 Laplacian filter
 *
 * Laplacian filters give the second derivative in two directions.
 * This filter enhances changes.
 *
 * \param[in]  src A pointer to the source image
 * \param[out] dst A pointer to the destination image
 */
void laplacianFilter_5x5(const image_t *src, image_t *dst)
{
    // Verify image validity
    ASSERT(src == NULL, "src image is invalid");
    ASSERT(dst == NULL, "dst image is invalid");
    ASSERT(src->data == NULL, "src data is invalid");
    ASSERT(dst->data == NULL, "dst data is invalid");
    ASSERT(src->type != IMGTYPE_INT16, "src type is invalid");
    ASSERT(dst->type != IMGTYPE_INT16, "dst type is invalid");

    // Verify image consistency
    ASSERT(src->cols != dst->cols, "src and dst have different number of columns");
    ASSERT(src->rows != dst->rows, "src and dst have different number of rows");
    ASSERT(src == dst, "src and dst are the same images");

    // Define mask
    int16_pixel_t msk_data[5 * 5] =
    {
        -1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,
        -1,-1,24,-1,-1,
        -1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,
    };

    // Create the mask image
    image_t msk =
    {
        .cols=5,
        .rows=5,
        .type=IMGTYPE_INT16,
        .data=(uint8_t *)msk_data,
    };

    convolve(src, dst, &msk);
}

/*!
 * \brief Sobel edge detection algorithm that produces the magnitude and
 *        direction of an edge in grayscale images
 *
 * The function first creates an horizontal and vertical enhanced image.
 * These enhanced images are used to calculate the Sobel magnitude and
 * direction for each pixel.
 *
 * \param[in]  src A pointer to the source image
 * \param[out] mag A pointer to the Sobel magnitude destination image
 * \param[out] dir A pointer to the Sobel direction destination image.
 *                 If this is a NULL pointer, the Sobel direction is
 *                 not calculated.
 */
void sobel(const image_t *src, image_t *mag, image_t *dir)
{
    // Verify image validity
    ASSERT(src == NULL, "src image is invalid");
    ASSERT(mag == NULL, "mag image is invalid");
    ASSERT(src->data == NULL, "src data is invalid");
    ASSERT(mag->data == NULL, "mag data is invalid");
    ASSERT(src->type != IMGTYPE_INT16, "src type is invalid");
    ASSERT(mag->type != IMGTYPE_INT16, "mag type is invalid");

    // Verify image consistency
    ASSERT(src->cols != mag->cols, "src and mag have different number of columns");
    ASSERT(src->rows != mag->rows, "src and mag have different number of rows");
    ASSERT(src == mag, "src and dst are the same images");

    // Define Sobel masks
    int16_pixel_t gh_msk_data[3 * 3] =
    {
        -1,-2,-1,
         0, 0, 0,
         1, 2, 1,
    };

    int16_pixel_t gv_msk_data[3 * 3] =
    {
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1,
    };

    // Create the mask image
    image_t msk =
    {
        .cols=3,
        .rows=3,
        .type=IMGTYPE_INT16,
        .data=NULL,
    };

    // Create temporary images and verify memory allocation
    image_t *gh = newInt16Image(src->cols, src->rows);
    if(gh == NULL)
    {
        // No memory allocated
        return;
    }

    image_t *gv = newInt16Image(src->cols, src->rows);

    if(gv == NULL)
    {
        // No memory allocated
        deleteInt16Image(gh);
        return;
    }

    clearInt16Image(gh);
    clearInt16Image(gv);

    // Calculate Gh by correlating the mask
    // Use correlation instead of convolution, because the
    // mask is not symmetrical in both horizontal and
    // vertical direction
    msk.data=(uint8_t *)gh_msk_data;
    correlate(src, gh, &msk);

    // Calculate Gv by correlating the mask
    // Use correlation instead of convolution, because the
    // mask is not symmetrical in both horizontal and
    // vertical direction
    msk.data=(uint8_t *)gv_msk_data;
    correlate(src, gv, &msk);

    // Loop all pixels
    if(dir == NULL)
    {
        for(int32_t y=0; y<src->rows; y++)
        {
            for(int32_t x=0; x<src->cols; x++)
            {
                // Msobel = |Gh| + |Gv|
                int16_pixel_t m = abs(getInt16Pixel(gh,x,y)) +
                                  abs(getInt16Pixel(gv,x,y));
                setInt16Pixel(mag,x,y,m);
            }
        }
    }
    else
    {
        ASSERT(dir == NULL, "dir image is invalid");
        ASSERT(dir->data == NULL, "dir data is invalid");
        ASSERT(dir->type != IMGTYPE_FLOAT, "dir type is invalid");

        // Verify image consistency
        ASSERT(dir->cols != mag->cols, "dir and mag have different number of columns");

        for(int32_t y=0; y<src->rows; y++)
        {
            for(int32_t x=0; x<src->cols; x++)
            {
                // Msobel = |Gh| + |Gv|
                int16_pixel_t m = abs(getInt16Pixel(gh,x,y)) +
                                  abs(getInt16Pixel(gv,x,y));
                setInt16Pixel(mag,x,y,m);

                // PHIsobel = tan-1(Gv/Gh)
                float_pixel_t phi = atanf(((float)getInt16Pixel(gv,x,y)) /
                                           (float)getInt16Pixel(gh,x,y));
                setFloatPixel(dir,x,y,phi);
            }
        }
    }

    // Cleanup temporary images
    deleteInt16Image(gh);
    deleteInt16Image(gv);
}

/*!
 * \brief Sobel edge detection algorithm that produces the magnitude of an edge
 *        in grayscale images
 *
 * The function first creates an horizontal and vertical enhanced image.
 * These enhanced images are used to calculate the Sobel magnitude for each
 * pixel. The function skips the borders to enhance performance.
 *
 * \param[in]  src A pointer to the source image
 * \param[out] mag A pointer to the Sobel magnitude destination image
 *
 * \todo Implement this function
 */
void sobelFast(const image_t *src, image_t *mag)
{
    // ********************************************
    // Remove this block when implementation starts
    #warning TODO: sobelFast

    // Added to prevent compiler warnings
    (void)src;
    (void)mag;

    return;
    // ********************************************
}
