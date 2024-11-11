/*! ***************************************************************************
 *
 * \brief     Compute a given nonlinear function over a local window and
 *            replacing a single pixel value
 * \file      nonlinear_filters.c
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
#include "nonlinear_filters.h"

/*!
 * \brief Removes positive outliers
 *
 * Is better at removing Gaussian type noise and preserves edges.
 *
 * \param[in]  src A pointer to the source image
 * \param[out] dst A pointer to the destination image
 * \param[in]  n   Size of the square window
 */
void harmonic(const image_t *src, image_t *dst, const uint8_t n)
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
    ASSERT(src == dst, "src and dst are the same images");

    // Verify parameters
    ASSERT((n%2) == 0, "window size is not an odd value");

    // Loop all pixels
    for(int32_t y=0; y<src->rows; y++)
    {
        for(int32_t x=0; x<src->cols; x++)
        {
            // Initialize filter specific variables
            float sum = 0;
            uint32_t cnt = 0;

            // Apply the kernel only for pixels within the image
            for(int32_t j=-n/2; j<=n/2; j++)
            {
                for(int32_t i=-n/2; i<=n/2; i++)
                {
                    if((x+i) >= 0 &&
                       (y+j) >= 0 &&
                       (x+i) <  src->cols &&
                       (y+j) <  src->rows)
                    {
                        // Count the number of valid cells
                        cnt++;

                        // Calculation for each window cell
                        uint8_pixel_t p = getUint8Pixel(src,x+i,y+j);

                        if(p == 0)
                        {
                            sum = 0.0f;

                            // Break from loops
                            i=n/2+1;
                            j=n/2+1;
                        }
                        else
                        {
                            sum += 1.0f / p;
                        }
                    }
                }
            }

            // Calculate and store the result
            if(sum == 0)
            {
                setUint8Pixel(dst,x,y,0);
            }
            else
            {
                setUint8Pixel(dst,x,y,(uint8_pixel_t)(cnt/sum + 0.5f));
            }
        }
    }
}

/*!
 * \brief Removes negative outlier noise
 *
 * \param[in]  src A pointer to the source image
 * \param[out] dst A pointer to the destination image
 * \param[in]  n   Size of the square window
 */
void maximum(const image_t *src, image_t *dst, const uint8_t n)
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
    ASSERT(src == dst, "src and dst are the same images");

    // Verify parameters
    ASSERT((n%2) == 0, "window size is not an odd value");

    // Loop all pixels
    for(int32_t y=0; y<src->rows; y++)
    {
        for(int32_t x=0; x<src->cols; x++)
        {
            // Initialize filter specific variables
            uint8_pixel_t max = UINT8_PIXEL_MIN;

            // Apply the kernel only for pixels within the image
            for(int32_t j=-n/2; j<=n/2; j++)
            {
                for(int32_t i=-n/2; i<=n/2; i++)
                {
                    if((x+i) >= 0 &&
                       (y+j) >= 0 &&
                       (x+i) <  src->cols &&
                       (y+j) <  src->rows)
                    {
                        // Calculation for each window cell
                        uint8_pixel_t p = getUint8Pixel(src,x+i,y+j);
                        max = p > max ? p : max;
                    }
                }
            }

            // Calculate and store the result
            setUint8Pixel(dst,x,y,max);
        }
    }
}

/*!
 * \brief Calculates the arithmetic mean of the pixels within the window
 *
 * The window is always a square.
 *
 * \param[in]  src A pointer to the source image
 * \param[out] dst A pointer to the destination image
 * \param[in]  n   Size of the square window
 */
void mean(const image_t *src, image_t *dst, const uint8_t n)
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
    ASSERT(src == dst, "src and dst are the same images");

    // Verify parameters
    ASSERT((n%2) == 0, "window size is not an odd value");

    // Loop all pixels
    for(int32_t y=0; y<src->rows; y++)
    {
        for(int32_t x=0; x<src->cols; x++)
        {
            // Initialize filter specific variables
            uint32_t sum = 0;
            uint32_t cnt = 0;

            // Apply the kernel only for pixels within the image
            for(int32_t j=-n/2; j<=n/2; j++)
            {
                for(int32_t i=-n/2; i<=n/2; i++)
                {
                    if((x+i) >= 0 &&
                       (y+j) >= 0 &&
                       (x+i) <  src->cols &&
                       (y+j) <  src->rows)
                    {
                        // Count the number of valid pixels
                        cnt++;

                        // Calculation for each window cell
                        sum += getUint8Pixel(src,x+i,y+j);
                    }
                }
            }

            // Calculate and store the result
            setUint8Pixel(dst,x,y,(uint8_pixel_t)((float)sum/(float)cnt + 0.5f));
        }
    }
}

/*!
 * \brief Calculates the arithmetic mean of the pixels within the window
 *
 * The window is always a 3x3 square. The border pixels are ignored.
 *
 * \param[in]  src A pointer to the source image
 * \param[out] dst A pointer to the destination image
 *
 * \todo Implement this function
 */
void meanFast(const image_t *src, image_t *dst)
{
    // ********************************************
    // Remove this block when implementation starts
    #warning TODO: meanFast

    // Added to prevent compiler warnings
    (void)src;
    (void)dst;

    return;
    // ********************************************
}

/*!
 * \brief The median is calculated by sorting the pixels in the \p n x \p n
 * window and selecting the centre value
 *
 * Removes long tailed noise and salt and pepper type noise.
 * Has minimum blurring effect and preserves spatial details.
 * Can remove outlier noise from images that contain less than 50% of its
 * pixels as outliers
 *
 * \param[in]  src A pointer to the source image
 * \param[out] dst A pointer to the destination image
 * \param[in]  n   Size of the square window
 */
void median(const image_t *src, image_t *dst, const uint8_t n)
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
    ASSERT(src == dst, "src and dst are the same images");

    // Verify parameters
    ASSERT((n%2) == 0, "window size is not an odd value");
    ASSERT(n > 11, "window size is too large");

    uint8_pixel_t median[121];

    // Loop all pixels
    for(int32_t y=0; y<src->rows; y++)
    {
        for(int32_t x=0; x<src->cols; x++)
        {
            // Initialize filter specific variables
            uint32_t cnt = 0;

            // Apply the kernel only for pixels within the image
            for(int32_t j=-n/2; j<=n/2; j++)
            {
                for(int32_t i=-n/2; i<=n/2; i++)
                {
                    if((x+i) >= 0 &&
                       (y+j) >= 0 &&
                       (x+i) <  src->cols &&
                       (y+j) <  src->rows)
                    {
                        // Store pixel value in array
                        median[cnt] = getUint8Pixel(src,x+i,y+j);

                        // Count the number of valid cells
                        cnt++;
                    }
                }
            }

            // Use the 'Insertion sort' algorithm to sort the median values
            // Loop through all elements, starting at second
            for(uint32_t i=1; i<cnt; i++)
            {
                // Copy the element
                uint8_pixel_t a = median[i];

                // Move 'backwards' and shift all values bigger then the current
                // value to the next position in the array
                uint32_t j;
                for(j=i; j>0 && median[j-1] > a; j--){ median[j] = median[j-1]; }

                // Insert current element at this position in the array
                median[j] = a;
            }

            // Check if there is an even number of pixels.
            // This might be the case for border pixels.
            if((cnt%2) == 0)
            {
                // Store the result
                setUint8Pixel(dst,x,y,(median[(cnt/2) - 1] + median[cnt/2]) / 2.0f + 0.5f);
            }
            else
            {
                // Store the result
                setUint8Pixel(dst,x,y,median[cnt/2]);
            }
        }
    }
}

/*!
 * \brief Used to remove short tailed noise, such as Gaussian and uniform type
 *        noise
 *
 * \param[in]  src A pointer to the source image
 * \param[out] dst A pointer to the destination image
 * \param[in]  n   Size of the square window
 */
void midpoint(const image_t *src, image_t *dst, const uint8_t n)
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
    ASSERT(src == dst, "src and dst are the same images");

    // Verify parameters
    ASSERT((n%2) == 0, "window size is not an odd value");

    // Loop all pixels
    for(int32_t y=0; y<src->rows; y++)
    {
        for(int32_t x=0; x<src->cols; x++)
        {
            // Initialize filter specific variables
            uint8_pixel_t min = UINT8_PIXEL_MAX;
            uint8_pixel_t max = UINT8_PIXEL_MIN;

            // Apply the kernel only for pixels within the image
            for(int32_t j=-n/2; j<=n/2; j++)
            {
                for(int32_t i=-n/2; i<=n/2; i++)
                {
                    if((x+i) >= 0 &&
                       (y+j) >= 0 &&
                       (x+i) <  src->cols &&
                       (y+j) <  src->rows)
                    {
                        // Calculation for each window cell
                        uint8_pixel_t p = getUint8Pixel(src,x+i,y+j);
                        min = p < min ? p : min;
                        max = p > max ? p : max;
                    }
                }
            }

            // Calculate and store the result
            setUint8Pixel(dst,x,y,((min + max)/2.0f) + 0.5f);
        }
    }
}

/*!
 * \brief Removes positive outlier noise
 *
 * \param[in]  src A pointer to the source image
 * \param[out] dst A pointer to the destination image
 * \param[in]  n   Size of the square window
 */
void minimum(const image_t *src, image_t *dst, const uint8_t n)
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
    ASSERT(src == dst, "src and dst are the same images");

    // Verify parameters
    ASSERT((n%2) == 0, "window size is not an odd value");

    // Loop all pixels
    for(int32_t y=0; y<src->rows; y++)
    {
        for(int32_t x=0; x<src->cols; x++)
        {
            // Initialize filter specific variables
            uint8_pixel_t min = UINT8_PIXEL_MAX;

            // Apply the kernel only for pixels within the image
            for(int32_t j=-n/2; j<=n/2; j++)
            {
                for(int32_t i=-n/2; i<=n/2; i++)
                {
                    if((x+i) >= 0 &&
                       (y+j) >= 0 &&
                       (x+i) <  src->cols &&
                       (y+j) <  src->rows)
                    {
                        // Calculation for each window cell
                        uint8_pixel_t p = getUint8Pixel(src,x+i,y+j);
                        min = p < min ? p : min;
                    }
                }
            }

            // Calculate and store the result
            setUint8Pixel(dst,x,y,min);
        }
    }
}

/*!
 * \brief Outputs the difference between the local maximum and minimum
 *
 * \param[in]  src A pointer to the source image
 * \param[out] dst A pointer to the destination image
 * \param[in]  n   Size of the square window
 */
void range(const image_t *src, image_t *dst, const uint8_t n)
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
    ASSERT(src == dst, "src and dst are the same images");

    // Verify parameters
    ASSERT((n%2) == 0, "window size is not an odd value");

    // Loop all pixels
    for(int32_t y=0; y<src->rows; y++)
    {
        for(int32_t x=0; x<src->cols; x++)
        {
            // Initialize filter specific variables
            uint8_pixel_t min = UINT8_PIXEL_MAX;
            uint8_pixel_t max = UINT8_PIXEL_MIN;

            // Apply the kernel only for pixels within the image
            for(int32_t j=-n/2; j<=n/2; j++)
            {
                for(int32_t i=-n/2; i<=n/2; i++)
                {
                    if((x+i) >= 0 &&
                       (y+j) >= 0 &&
                       (x+i) <  src->cols &&
                       (y+j) <  src->rows)
                    {
                        // Calculation for each window cell
                        uint8_pixel_t p = getUint8Pixel(src,x+i,y+j);
                        min = p < min ? p : min;
                        max = p > max ? p : max;
                    }
                }
            }

            // Calculate and store the result
            setUint8Pixel(dst,x,y,max - min);
        }
    }
}
