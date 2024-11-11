/*! ***************************************************************************
 *
 * \brief     For adding a controlled amount of noise to images
 * \file      noise.c
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
#include "noise.h"

#define _USE_MATH_DEFINES
#include "math.h"
#include "time.h"

/*!
 * \brief Generates image data containing salt and pepper noise
 *
 * The function removes the original image data.
 * Salt and pepper noise is named after the white and black appearance
 * it adds to images. The salt value is set to 0. The pepper value is set to
 * 255. Salt and pepper noise occur in images acquired by cameras containing
 * malfunctioning pixels.
 *
 * \param[out] img         A pointer to the image
 * \param[in]  probability The probability of the noise appearing in the image
 */
void saltPepper(image_t *img, const float probability)
{
    // Verify image validity
    ASSERT(img == NULL, "img image is invalid");
    ASSERT(img->data == NULL, "img data is invalid");
    ASSERT(img->type != IMGTYPE_UINT8, "img type is invalid");

    // Set window boundaries
    uint32_t window_size = (uint32_t)(probability * (RAND_MAX/2));
    uint32_t boundary_right = (RAND_MAX/2) + window_size;
    uint32_t boundary_left = (RAND_MAX/2) - window_size;

    // Seed the pseudo random-number generator
    srand(time(0));

    for(int32_t y=0; y<img->rows; y++)
    {
        for(int32_t x=0; x<img->cols; x++)
        {
            // Get random-number
            uint32_t val = rand();

            // Within 'pepper' window? Set pixel to 0
            if((val >= (RAND_MAX/2)) && (val < boundary_right))
                setUint8Pixel(img, x, y, 0);

            // Within 'salt' window? Set pixel to 255
            else if((val >= boundary_left) && (val < (RAND_MAX/2)))
                setUint8Pixel(img, x, y, 255);
        }
    }
}

/*!
 * \brief Generates image data containing Gaussian noise
 *
 * The function removes the original image data.
 * The Gaussian value for each pixel is then calculated with the Box-Muller
 * method.
 *
 * \param[out] img  A pointer to the image
 * \param[in]  var  Standard deviation of the noise
 * \param[in]  mean Mean value of the noise
 */
void gaussian(image_t *img, const float var, const float mean)
{
    // Verify image validity
    ASSERT(img == NULL, "img image is invalid");
    ASSERT(img->data == NULL, "img data is invalid");
    ASSERT(img->type != IMGTYPE_UINT8, "img type is invalid");

    float noise, theta;

    // Seed the pseudo random-number generator
    srand(time(0));

    for(int32_t y=0; y<img->rows; y++)
    {
        for(int32_t x=0; x<img->cols; x++)
        {
            // Calculate Gaussian noise using Box-Muller method
            theta = ((float)rand() / (float)RAND_MAX) * 2 * 3.14159265359f;
            noise = sqrt(-2 * var * log((float)rand() / (float)RAND_MAX));
            noise = noise * cos(theta);
            noise = noise + mean;

            // Clip the result
            if(noise>255)
                noise=255;

            if(noise<0)
                noise=0;

            // Store the result
            setUint8Pixel(img, x, y, (uint8_pixel_t)(noise + 0.5f));
        }
    }
}

/*!
 * \brief Generates image data containing uniform noise between \p min and \p
 *        max
 *
 * The function removes the original image data.
 * Uniform noise is used to degrade images in the evaluation of image
 * processing algorithms.
 *
 * \param[out] img A pointer to the image
 * \param[in]  min Minimum value of the uniform noise
 * \param[in]  max Maximum value of the uniform noise
 */
void uniform(image_t *img, const uint8_pixel_t min, const uint8_pixel_t max)
{
    // Verify image validity
    ASSERT(img == NULL, "img image is invalid");
    ASSERT(img->data == NULL, "img data is invalid");
    ASSERT(img->type != IMGTYPE_UINT8, "img type is invalid");

    // Verify parameters
    ASSERT(min > max, "min is greater than max");

    float noise;

    // Seed the pseudo random-number generator
    srand(time(0));

    for(int32_t y=0; y<img->rows; y++)
    {
        for(int32_t x=0; x<img->cols; x++)
        {
            // Calculate uniform noise
            noise = ((float)rand() / (float)RAND_MAX) * (max - min) + min;

            // Clip the result
            if(noise>255)
                noise=255;

            if(noise<0)
                noise=0;

            // Store the result
            setUint8Pixel(img, x, y, (uint8_pixel_t)(noise + 0.5f));
        }
    }
}
