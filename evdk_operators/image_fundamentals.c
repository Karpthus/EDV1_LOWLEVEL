/*! ***************************************************************************
 *
 * \brief     Fundamental functions for image processing
 * \file      image_fundamentals.c
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

#include <string.h>
#include "image_fundamentals.h"

#define MAX_IMAGES (16)

/*!
 * \brief Local array of image pointers. Is used to free all allocated image
 *        data when calling the function deleteAllImages(). The value of the
 *        MAX_IMAGES define might need tweaking to your application. Increasing
 *        this number allows for more images to be stored, but requires more
 *        memory and more checks every time an image is created or deleted.
 */
static image_t * imgs[MAX_IMAGES] = {NULL};

// Function prototypes
uint8_t clip(int32_t val);

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

/*!
 * \brief Clips input values to the range 0 - 255
 *
 * This function is a helper function for UYVY to BGR888 conversion.
 *
 * \param[in]  val input value
 *
 * \return Clipped value
 */
inline uint8_t clip(int32_t val)
{
    return (val < 0) ? 0 : ((val > 255) ? 255 : val);
}

/// \name Getter functions for individual pixels
/// \{

/*!
 * \brief Getter functions for individual pixels
 *
 * \param[in] img A pointer to the image
 * \param[in] c   Column (x) coordinate of the pixel
 * \param[in] r   Row (y) coordinate of the pixel
 *
 * \return Pixel value at coordinate (c,r)
 */

inline uint8_pixel_t getUint8Pixel(const image_t *img, const int32_t c, const int32_t r)
{
    return (*((uint8_pixel_t *)(img->data) + (r * img->cols + c)));
}

inline int16_pixel_t getInt16Pixel(const image_t *img, const int32_t c, const int32_t r)
{
    return (*((int16_pixel_t *)(img->data) + (r * img->cols + c)));
}

inline int32_pixel_t getInt32Pixel(const image_t *img, const int32_t c, const int32_t r)
{
    return (*((int32_pixel_t *)(img->data) + (r * img->cols + c)));
}

inline float_pixel_t getFloatPixel(const image_t *img, const int32_t c, const int32_t r)
{
    return (*((float_pixel_t *)(img->data) + (r * img->cols + c)));
}

inline uyvy_pixel_t getUyvyPixel(const image_t *img, const int32_t c, const int32_t r)
{
    return (*((uyvy_pixel_t *)(img->data) + (r * img->cols + c)));
}

inline bgr888_pixel_t getBgr888Pixel(const image_t *img, const int32_t c, const int32_t r)
{
    return (*((bgr888_pixel_t *)(img->data) + (r * img->cols + c)));
}
/// \}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

/// \name Setter functions for individual pixels
/// \{

/*!
 * \brief Setter functions for individual pixels
 *
 * \param[in] img   A pointer to the image
 * \param[in] c     Column (x) coordinate of the pixel
 * \param[in] r     Row (y) coordinate of the pixel
 * \param[in] value New pixel value for coordinate (c,r)
 */

inline void setUint8Pixel(const image_t *img, const int32_t c, const int32_t r, const  uint8_pixel_t value)
{
    *((uint8_pixel_t *)(img->data) + (r * img->cols + c)) = value;
}

inline void setInt16Pixel(const image_t *img, const int32_t c, const int32_t r, const int16_pixel_t value)
{
    *((int16_pixel_t *)(img->data) + (r * img->cols + c)) = value;
}

inline void setInt32Pixel(const image_t *img, const int32_t c, const int32_t r, const int32_pixel_t value)
{
    *((int32_pixel_t *)(img->data) + (r * img->cols + c)) = value;
}

inline void setFloatPixel(const image_t *img, const int32_t c, const int32_t r, const float_pixel_t value)
{
    *((float_pixel_t *)(img->data) + (r * img->cols + c)) = value;
}

inline void setUyvyPixel(const image_t *img, const int32_t c, const int32_t r, const uyvy_pixel_t value)
{
    *((uyvy_pixel_t *)(img->data) + (r * img->cols + c)) = value;
}

inline void setBgr888Pixel(const image_t *img, const int32_t c, const int32_t r, const bgr888_pixel_t value)
{
    *((bgr888_pixel_t *)(img->data) + (r * img->cols + c)) = value;
}

/// \}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

/// \name Functions for creating new images
/// \{

/*!
 * \brief Functions for creating new images
 *
 * After the image has been used and is not needed any more, make sure that the
 * image is deleted by calling the function delete<type>Image() or
 * deleteAllImages().
 *
 * \param[in] cols The number of columns for the new image
 * \param[in] rows The number of rows for the new image
 *
 * \return A pointer to the new image. NULL if memory allocation failed.
 */
image_t *newUint8Image(const uint32_t cols, const uint32_t rows)
{
    image_t *img = (image_t *)malloc(sizeof(image_t));
    if(img == NULL)
    {
        // Unable to allocate memory for new image
        return NULL;
    }

    img->data = (uint8_t *)malloc((rows * cols) * sizeof(uint8_pixel_t));

    if(img->data == NULL)
    {
        // Unable to allocate memory for data
        free(img);
        return NULL;
    }

    img->cols = cols;
    img->rows = rows;
    img->type = IMGTYPE_UINT8;

    // Add image to the images array
    for(uint32_t i = 0; i < MAX_IMAGES; i++)
    {
        if(imgs[i] == NULL)
        {
            imgs[i] = img;
            return(img);
        }
    }

    // Should never get here, because it means the imgs array is too small
    free(img->data);
    free(img);

    return NULL;
}

image_t *newInt16Image(const uint32_t cols, const uint32_t rows)
{
    image_t *img = (image_t *)malloc(sizeof(image_t));
    if(img == NULL)
    {
        // Unable to allocate memory for new image
        return NULL;
    }

    img->data = (uint8_t *)malloc((rows * cols) * sizeof(int16_pixel_t));

    if(img->data == NULL)
    {
        // Unable to allocate memory for data
        free(img);
        return NULL;
    }

    img->cols = cols;
    img->rows = rows;
    img->type = IMGTYPE_INT16;

    // Add image to the images array
    for(uint32_t i = 0; i < MAX_IMAGES; i++)
    {
        if(imgs[i] == NULL)
        {
            imgs[i] = img;
            return(img);
        }
    }

    // Should never get here, because it means the imgs array is too small
    free(img->data);
    free(img);

    return NULL;
}

image_t *newInt32Image(const uint32_t cols, const uint32_t rows)
{
    image_t *img = (image_t *)malloc(sizeof(image_t));
    if(img == NULL)
    {
        // Unable to allocate memory for new image
        return NULL;
    }

    img->data = (uint8_t *)malloc((rows * cols) * sizeof(int32_pixel_t));

    if(img->data == NULL)
    {
        // Unable to allocate memory for data
        free(img);
        return NULL;
    }

    img->cols = cols;
    img->rows = rows;
    img->type = IMGTYPE_INT32;

    // Add image to the images array
    for(uint32_t i = 0; i < MAX_IMAGES; i++)
    {
        if(imgs[i] == NULL)
        {
            imgs[i] = img;
            return(img);
        }
    }

    // Should never get here, because it means the imgs array is too small
    free(img->data);
    free(img);

    return NULL;
}

image_t *newFloatImage(const uint32_t cols, const uint32_t rows)
{
    image_t *img = (image_t *)malloc(sizeof(image_t));
    if(img == NULL)
    {
        // Unable to allocate memory for new image
        return NULL;
    }

    img->data = (uint8_t *)malloc((rows * cols) * sizeof(float_pixel_t));

    if(img->data == NULL)
    {
        // Unable to allocate memory for data
        free(img);
        return NULL;
    }

    img->cols = cols;
    img->rows = rows;
    img->type = IMGTYPE_FLOAT;

    // Add image to the images array
    for(uint32_t i = 0; i < MAX_IMAGES; i++)
    {
        if(imgs[i] == NULL)
        {
            imgs[i] = img;
            return(img);
        }
    }

    // Should never get here, because it means the imgs array is too small
    free(img->data);
    free(img);

    return NULL;
}

image_t *newUyvyImage(const uint32_t cols, const uint32_t rows)
{
    image_t *img = (image_t *)malloc(sizeof(image_t));
    if(img == NULL)
    {
        // Unable to allocate memory for new image
        return NULL;
    }

    img->data = (uint8_t *)malloc((rows * cols) * sizeof(uyvy_pixel_t));

    if(img->data == NULL)
    {
        // Unable to allocate memory for data
        free(img);
        return NULL;
    }

    img->cols = cols;
    img->rows = rows;
    img->type = IMGTYPE_UYVY;

    // Add image to the images array
    for(uint32_t i = 0; i < MAX_IMAGES; i++)
    {
        if(imgs[i] == NULL)
        {
            imgs[i] = img;
            return(img);
        }
    }

    // Should never get here, because it means the imgs array is too small
    free(img->data);
    free(img);

    return NULL;
}

image_t *newBgr888Image(const uint32_t cols, const uint32_t rows)
{
    image_t *img = (image_t *)malloc(sizeof(image_t));
    if(img == NULL)
    {
        // Unable to allocate memory for new image
        return NULL;
    }

    img->data = (uint8_t *)malloc((rows * cols) * sizeof(bgr888_pixel_t));

    if(img->data == NULL)
    {
        // Unable to allocate memory for data
        free(img);
        return NULL;
    }

    img->cols = cols;
    img->rows = rows;
    img->type = IMGTYPE_BGR888;

    // Add image to the images array
    for(uint32_t i = 0; i < MAX_IMAGES; i++)
    {
        if(imgs[i] == NULL)
        {
            imgs[i] = img;
            return(img);
        }
    }

    // Should never get here, because it means the imgs array is too small
    free(img->data);
    free(img);

    return NULL;
}

/// \}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

/// \name Functions for creating new images without data allocation
/// \{

/*!
 * \brief Functions for creating new images without data allocation
 *
 * After the image has been used and is not needed any more, make sure that the
 * image is deleted by calling the function deleteImage().
 *
 * \param[in] cols The number of columns for the new image
 * \param[in] rows The number of rows for the new image
 *
 * \return A pointer to the new image. NULL if memory allocation failed.
 */
image_t *newEmptyUint8Image(const uint32_t cols, const uint32_t rows)
{
    image_t *img = (image_t *)malloc(sizeof(image_t));
    if(img == NULL)
    {
        // Unable to allocate memory for new image
        return NULL;
    }

    img->cols = cols;
    img->rows = rows;
    img->type = IMGTYPE_UINT8;
    img->data = NULL;

    // Add image to the images array
    for(uint32_t i = 0; i < MAX_IMAGES; i++)
    {
        if(imgs[i] == NULL)
        {
            imgs[i] = img;
            return(img);
        }
    }

    // Should never get here, because it means the imgs array is too small
    free(img->data);
    free(img);

    return NULL;
}

image_t *newEmptyInt16Image(const uint32_t cols, const uint32_t rows)
{
    image_t *img = (image_t *)malloc(sizeof(image_t));
    if(img == NULL)
    {
        // Unable to allocate memory for new image
        return NULL;
    }

    img->cols = cols;
    img->rows = rows;
    img->type = IMGTYPE_INT16;
    img->data = NULL;

    // Add image to the images array
    for(uint32_t i = 0; i < MAX_IMAGES; i++)
    {
        if(imgs[i] == NULL)
        {
            imgs[i] = img;
            return(img);
        }
    }

    // Should never get here, because it means the imgs array is too small
    free(img->data);
    free(img);

    return NULL;
}

image_t *newEmptyInt32Image(const uint32_t cols, const uint32_t rows)
{
    image_t *img = (image_t *)malloc(sizeof(image_t));
    if(img == NULL)
    {
        // Unable to allocate memory for new image
        return NULL;
    }

    img->cols = cols;
    img->rows = rows;
    img->type = IMGTYPE_INT32;
    img->data = NULL;

    // Add image to the images array
    for(uint32_t i = 0; i < MAX_IMAGES; i++)
    {
        if(imgs[i] == NULL)
        {
            imgs[i] = img;
            return(img);
        }
    }

    // Should never get here, because it means the imgs array is too small
    free(img->data);
    free(img);

    return NULL;
}

image_t *newEmptyFloatImage(const uint32_t cols, const uint32_t rows)
{
    image_t *img = (image_t *)malloc(sizeof(image_t));
    if(img == NULL)
    {
        // Unable to allocate memory for new image
        return NULL;
    }

    img->cols = cols;
    img->rows = rows;
    img->type = IMGTYPE_FLOAT;
    img->data = NULL;

    // Add image to the images array
    for(uint32_t i = 0; i < MAX_IMAGES; i++)
    {
        if(imgs[i] == NULL)
        {
            imgs[i] = img;
            return(img);
        }
    }

    // Should never get here, because it means the imgs array is too small
    free(img->data);
    free(img);

    return NULL;
}

image_t *newEmptyUyvyImage(const uint32_t cols, const uint32_t rows)
{
    image_t *img = (image_t *)malloc(sizeof(image_t));
    if(img == NULL)
    {
        // Unable to allocate memory for new image
        return NULL;
    }

    img->cols = cols;
    img->rows = rows;
    img->type = IMGTYPE_UYVY;
    img->data = NULL;

    // Add image to the images array
    for(uint32_t i = 0; i < MAX_IMAGES; i++)
    {
        if(imgs[i] == NULL)
        {
            imgs[i] = img;
            return(img);
        }
    }

    // Should never get here, because it means the imgs array is too small
    free(img->data);
    free(img);

    return NULL;
}

image_t *newEmptyBgr888Image(const uint32_t cols, const uint32_t rows)
{
    image_t *img = (image_t *)malloc(sizeof(image_t));
    if(img == NULL)
    {
        // Unable to allocate memory for new image
        return NULL;
    }

    img->cols = cols;
    img->rows = rows;
    img->type = IMGTYPE_BGR888;
    img->data = NULL;

    // Add image to the images array
    for(uint32_t i = 0; i < MAX_IMAGES; i++)
    {
        if(imgs[i] == NULL)
        {
            imgs[i] = img;
            return(img);
        }
    }

    // Should never get here, because it means the imgs array is too small
    free(img->data);
    free(img);

    return NULL;
}

/// \}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

/// \name Functions for deleting images
/// \{

void deleteUint8Image(image_t *img)
{
    // Remove image from the array
    for(uint32_t i = 0; i < MAX_IMAGES; i++)
    {
        if(imgs[i] == img)
        {
            imgs[i] = NULL;
        }
    }

    free(img->data);
    free(img);
}

void deleteInt16Image(image_t *img)
{
    // Remove image from the array
    for(uint32_t i = 0; i < MAX_IMAGES; i++)
    {
        if(imgs[i] == img)
        {
            imgs[i] = NULL;
        }
    }

    free(img->data);
    free(img);
}

void deleteInt32Image(image_t *img)
{
    // Remove image from the array
    for(uint32_t i = 0; i < MAX_IMAGES; i++)
    {
        if(imgs[i] == img)
        {
            imgs[i] = NULL;
        }
    }

    free(img->data);
    free(img);
}

void deleteFloatImage(image_t *img)
{
    // Remove image from the array
    for(uint32_t i = 0; i < MAX_IMAGES; i++)
    {
        if(imgs[i] == img)
        {
            imgs[i] = NULL;
        }
    }

    free(img->data);
    free(img);
}

void deleteUyvyImage(image_t *img)
{
    // Remove image from the array
    for(uint32_t i = 0; i < MAX_IMAGES; i++)
    {
        if(imgs[i] == img)
        {
            imgs[i] = NULL;
        }
    }

    free(img->data);
    free(img);
}

void deleteBgr888Image(image_t *img)
{
    // Remove image from the array
    for(uint32_t i = 0; i < MAX_IMAGES; i++)
    {
        if(imgs[i] == img)
        {
            imgs[i] = NULL;
        }
    }

    free(img->data);
    free(img);
}

void deleteAllImages(void)
{
    // Remove image from the array
    for(uint32_t i = 0; i < MAX_IMAGES; i++)
    {
        if(imgs[i] != NULL)
        {
            free(imgs[i]->data);
            free(imgs[i]);
            imgs[i] = NULL;
        }
    }
}

/// \}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

/// \name Functions for copying images
/// \{

/*!
 * \brief Functions for copying images
 *
 * Memory must have already been allocated for both the source and the
 * destination image. Source and destination must be of the same
 * ::eImageType.
 *
 * \param[in] src A pointer to the source image
 * \param[in] dst A pointer to the destination image
 */

void copyUint8Image(const image_t *src, image_t *dst)
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

    // Copy data
    memcpy(dst->data, src->data, src->cols * src->rows * sizeof(uint8_pixel_t));
}

void copyInt16Image(const image_t *src, image_t *dst)
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

    // Copy data
    memcpy(dst->data, src->data, src->cols * src->rows * sizeof(int16_pixel_t));
}

void copyInt32Image(const image_t *src, image_t *dst)
{
    // Verify image validity
    ASSERT(src == NULL, "src image is invalid");
    ASSERT(dst == NULL, "dst image is invalid");
    ASSERT(src->data == NULL, "src data is invalid");
    ASSERT(dst->data == NULL, "dst data is invalid");
    ASSERT(src->type != IMGTYPE_INT32, "src type is invalid");
    ASSERT(dst->type != IMGTYPE_INT32, "dst type is invalid");

    // Verify image consistency
    ASSERT(src->cols != dst->cols, "src and dst have different number of columns");
    ASSERT(src->rows != dst->rows, "src and dst have different number of rows");
    ASSERT(src == dst, "src and dst are the same images");

    // Copy data
    memcpy(dst->data, src->data, src->cols * src->rows * sizeof(int32_pixel_t));
}

void copyFloatImage(const image_t *src, image_t *dst)
{
    // Verify image validity
    ASSERT(src == NULL, "src image is invalid");
    ASSERT(dst == NULL, "dst image is invalid");
    ASSERT(src->data == NULL, "src data is invalid");
    ASSERT(dst->data == NULL, "dst data is invalid");
    ASSERT(src->type != IMGTYPE_FLOAT, "src type is invalid");
    ASSERT(dst->type != IMGTYPE_FLOAT, "dst type is invalid");

    // Verify image consistency
    ASSERT(src->cols != dst->cols, "src and dst have different number of columns");
    ASSERT(src->rows != dst->rows, "src and dst have different number of rows");
    ASSERT(src == dst, "src and dst are the same images");

    // Copy data
    memcpy(dst->data, src->data, src->cols * src->rows * sizeof(float_pixel_t));
}

void copyUyvyImage(const image_t *src, image_t *dst)
{
    // Verify image validity
    ASSERT(src == NULL, "src image is invalid");
    ASSERT(dst == NULL, "dst image is invalid");
    ASSERT(src->data == NULL, "src data is invalid");
    ASSERT(dst->data == NULL, "dst data is invalid");
    ASSERT(src->type != IMGTYPE_UYVY, "src type is invalid");
    ASSERT(dst->type != IMGTYPE_UYVY, "dst type is invalid");

    // Verify image consistency
    ASSERT(src->cols != dst->cols, "src and dst have different number of columns");
    ASSERT(src->rows != dst->rows, "src and dst have different number of rows");
    ASSERT(src == dst, "src and dst are the same images");

    // Copy data
    memcpy(dst->data, src->data, src->cols * src->rows * sizeof(uyvy_pixel_t));
}

void copyBgr888Image(const image_t *src, image_t *dst)
{
    // Verify image validity
    ASSERT(src == NULL, "src image is invalid");
    ASSERT(dst == NULL, "dst image is invalid");
    ASSERT(src->data == NULL, "src data is invalid");
    ASSERT(dst->data == NULL, "dst data is invalid");
    ASSERT(src->type != IMGTYPE_BGR888, "src type is invalid");
    ASSERT(dst->type != IMGTYPE_BGR888, "dst type is invalid");

    // Verify image consistency
    ASSERT(src->cols != dst->cols, "src and dst have different number of columns");
    ASSERT(src->rows != dst->rows, "src and dst have different number of rows");
    ASSERT(src == dst, "src and dst are the same images");

    // Copy data
    memcpy(dst->data, src->data, src->cols * src->rows * sizeof(bgr888_pixel_t));
}

/// \}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

/// \name Functions for clearing images
/// \{

/*!
 * \brief Functions for clearing images
 *
 * All channels are set to the value 0.
 *
 * \param[in] img A pointer to the image that can be cleared
 */

void clearUint8Image(image_t *img)
{
    // Verify image validity
    ASSERT(img == NULL, "img image is invalid");
    ASSERT(img->data == NULL, "img data is invalid");
    ASSERT(img->type != IMGTYPE_UINT8, "img type is invalid");

    register long int i = img->rows * img->cols;
    register uint8_pixel_t *d = (uint8_pixel_t *)img->data;

    // Loop all pixels and clear
    while(i-- > 0)
    {
        *d++ = (uint8_pixel_t)0;
    }
}

void clearInt16Image(image_t *img)
{
    // Verify image validity
    ASSERT(img == NULL, "img image is invalid");
    ASSERT(img->data == NULL, "img data is invalid");
    ASSERT(img->type != IMGTYPE_INT16, "img type is invalid");

    register long int i = img->rows * img->cols;
    register int16_pixel_t *d = (int16_pixel_t *)img->data;

    // Loop all pixels and clear
    while(i-- > 0)
    {
        *d++ = (int16_pixel_t)0;
    }
}

void clearInt32Image(image_t *img)
{
    // Verify image validity
    ASSERT(img == NULL, "img image is invalid");
    ASSERT(img->data == NULL, "img data is invalid");
    ASSERT(img->type != IMGTYPE_INT32, "img type is invalid");

    register long int i = img->rows * img->cols;
    register int32_pixel_t *d = (int32_pixel_t *)img->data;

    // Loop all pixels and clear
    while(i-- > 0)
    {
        *d++ = (int32_pixel_t)0;
    }
}

void clearFloatImage(image_t *img)
{
    // Verify image validity
    ASSERT(img == NULL, "img image is invalid");
    ASSERT(img->data == NULL, "img data is invalid");
    ASSERT(img->type != IMGTYPE_FLOAT, "img type is invalid");

    register long int i = img->rows * img->cols;
    register float_pixel_t *d = (float_pixel_t *)img->data;

    // Loop all pixels and clear
    while(i-- > 0)
    {
        *d++ = (float_pixel_t)0.0f;
    }
}

/// \}

/*!
 * \brief Converts an uyvy_pixel_t image to an uint8_pixel_t image
 *
 * This function removes the chroma values U and V from the uyvy_pixel_t image
 * and stores the Y values in an uint8_pixel_t image.
 *
 * \param[in]  src A pointer to the uyvy_pixel_t image
 * \param[out] dst A pointer to the converted uint8_pixel_t image
 *
 * \todo Implement this function
 */
void convertUyvyToUint8(image_t *src, image_t *dst)
{
    uint32_t i = src->rows * src->cols;
    uint8_pixel_t *uint8_pixel = (uint8_pixel_t *)dst->data;
    uyvy_pixel_t *uyvy_pixel = (uyvy_pixel_t *)src->data;

    // Loop all pixels and convert
    while(i-- > 0)
    {
        *uint8_pixel = (uint8_pixel_t)(*uyvy_pixel >> 8);

        // Next pixel
        uint8_pixel++;
        uyvy_pixel++;
    }
}

/*!
 * \brief Converts an uint8_pixel_t image to an uyvy_pixel_t image
 *
 * This function copies the uint8_pixel_t to the Y values and adds adds the same
 * chroma values for U and V (0x80) to the new uyvy_pixel_t image. By adding
 * 0x80 U and V, the result is a greyscale image.
 *
 * \param[in]  src A pointer to the uint8_pixel_t image
 * \param[out] dst A pointer to the converted uyvy_pixel_t image
 */
void convertUint8ToUyvy(image_t *src, image_t *dst)
{
    uint32_t i = src->rows * src->cols;
    uint8_pixel_t *uint8_pixel = (uint8_pixel_t *)src->data;
    uyvy_pixel_t *uyvy_pixel = (uyvy_pixel_t *)dst->data;

    // Loop all pixels and convert
    while(i-- > 0)
    {
        *uyvy_pixel = ((uyvy_pixel_t)(*uint8_pixel)) << 8;
        *uyvy_pixel |= 0x0080;

        // Next pixel
        uyvy_pixel++;
        uint8_pixel++;
    }
}

/*!
 * \brief Converts an uyvy_pixel_t image to an int16_pixel_t image
 *
 * This function removes the chroma values U and V from the uyvy_pixel_t image
 * and stores th Y values in an int16_pixel_t image.
 *
 * \param[in]  src A pointer to the uyvy_pixel_t image
 * \param[out] dst A pointer to the converted uint8_pixel_t image
 */
void convertUyvyToInt16(image_t *src, image_t *dst)
{
    uint32_t i = src->rows * src->cols;
    uyvy_pixel_t *uyvy_pixel = (uyvy_pixel_t *)src->data;
    int16_pixel_t *int16_pixel = (int16_pixel_t *)dst->data;

    // Loop all pixels and convert
    while(i-- > 0)
    {
        *int16_pixel = (int16_pixel_t)((*uyvy_pixel) >> 8);

        // Next pixel
        uyvy_pixel++;
        int16_pixel++;
    }
}

/*!
 * \brief Converts an uyvy_pixel_t image to a bgr888_pixel_t image
 *
 * This function takes two uyvy_pixel_t pixels and calculates two
 * bgr888_pixel_t pixels according to the following equations:
 *
 * R = Y + 1.140 * (V - 128)
 * G = Y - 0.394 * (U - 128) - (0.581 * (V - 128))
 * B = Y + 2.032 * (U - 128)
 *
 * \see https://docs.nvidia.com/cuda/archive/10.1/npp/group__yuvtorgb.html
 *
 * \param[in]  src A pointer to the uyvy_pixel_t image
 * \param[out] dst A pointer to the converted bgr888_pixel_t image
 */
void convertUyvyToBgr888(image_t *src, image_t *dst)
{
    uint32_t i = src->rows * src->cols;
    uyvy_pixel_t *uyvy_pixel = (uyvy_pixel_t *)src->data;
    bgr888_pixel_t *bgr888_pixel = (bgr888_pixel_t *)dst->data;

    while(i > 0)
    {
        // Decrement by 2, because the chroma values are stored in two pixels
        i -= 2;

        uyvy_pixel_t uy = *uyvy_pixel++;
        uyvy_pixel_t vy = *uyvy_pixel++;

        int32_t u = (uy & 0xFFU) - 128;
        int32_t y1 = uy >> 8;
        int32_t v = (vy & 0xFFU) - 128;
        int32_t y2 = vy >> 8;

        bgr888_pixel->r = clip(y1 + (1.140f * v));
        bgr888_pixel->g = clip(y1 - (0.394f * v) - (0.581f * u));
        bgr888_pixel->b = clip(y1 + (2.032f * u));
        bgr888_pixel++;

        bgr888_pixel->r = clip(y2 + (1.140f * v));
        bgr888_pixel->g = clip(y2 - (0.394f * v) - (0.581f * u));
        bgr888_pixel->b = clip(y2 + (2.032f * u));
        bgr888_pixel++;

// Test with alternative coefficients

#if 0

        bgr888_pixel->r = clip(y1 + (1.370705f * (v)));
        bgr888_pixel->g = clip(y1 - (0.698001f * (v)) - (0.337633f * (u)));
        bgr888_pixel->b = clip(y1 + (1.732446f * (u)));
        bgr888_pixel++;

        bgr888_pixel->r = clip(y2 + (1.370705f * (v)));
        bgr888_pixel->g = clip(y2 - (0.698001f * (v)) - (0.337633f * (u)));
        bgr888_pixel->b = clip(y2 + (1.732446f * (u)));
        bgr888_pixel++;

#endif

// Several alternative calculation methods
// \see https://learn.microsoft.com/en-us/windows/win32/medfound/recommended-8-bit-yuv-formats-for-video-rendering

#if 0
        int32_t C = y1;
        int32_t D = u - 128;
        int32_t E = u - 128;

        // Calculations using doubles
        bgr888_pixel->r = clip( ( 1.164383 * C                   + 1.596027 * E  ) );
        bgr888_pixel->g = clip( ( 1.164383 * C - (0.391762 * D) - (0.812968 * E) ) );
        bgr888_pixel->b = clip( ( 1.164383 * C +  2.017232 * D                   ) );
        bgr888_pixel++;

        C = y2;

        bgr888_pixel->r = clip( ( 1.164383 * C                   + 1.596027 * E  ) );
        bgr888_pixel->g = clip( ( 1.164383 * C - (0.391762 * D) - (0.812968 * E) ) );
        bgr888_pixel->b = clip( ( 1.164383 * C +  2.017232 * D                   ) );
        bgr888_pixel++;
#endif

#if 0
        int32_t C = y1;
        int32_t D = u - 128;
        int32_t E = u - 128;

        // Calculations using floats
        bgr888_pixel->r = clip( ( 1.164383f * C                   +  1.596027f * E  ) );
        bgr888_pixel->g = clip( ( 1.164383f * C - (0.391762f * D) - (0.812968f * E) ) );
        bgr888_pixel->b = clip( ( 1.164383f * C +  2.017232f * D                    ) );
        bgr888_pixel++;

        C = y2;

        bgr888_pixel->r = clip( ( 1.164383f * C                   +  1.596027f * E  ) );
        bgr888_pixel->g = clip( ( 1.164383f * C - (0.391762f * D) - (0.812968f * E) ) );
        bgr888_pixel->b = clip( ( 1.164383f * C +  2.017232f * D                    ) );
        bgr888_pixel++;
#endif

#if 0
        int32_t C = y1;
        int32_t D = u - 128;
        int32_t E = u - 128;

        // Calculations using integers
        bgr888_pixel->r = clip(( 298 * C           + 409 * E + 128) >> 8);
        bgr888_pixel->g = clip(( 298 * C - 100 * D - 208 * E + 128) >> 8);
        bgr888_pixel->b = clip(( 298 * C + 516 * D           + 128) >> 8);
        bgr888_pixel++;

        C = y2;

        bgr888_pixel->r = clip(( 298 * C           + 409 * E + 128) >> 8);
        bgr888_pixel->g = clip(( 298 * C - 100 * D - 208 * E + 128) >> 8);
        bgr888_pixel->b = clip(( 298 * C + 516 * D           + 128) >> 8);
        bgr888_pixel++;
#endif
    }
}

/*!
 * \brief Converts an uint8_pixel_t image to a bgr888_pixel_t image
 *
 * This function assigns the uint8_pixel_t to all channels of the
 * bgr888_pixel_t image.
*
 * \param[in]  src A pointer to the uint8_pixel_t image
 * \param[out] dst A pointer to the converted bgr888_pixel_t image
 */
void convertUint8ToBgr888(image_t *src, image_t *dst)
{
    uint32_t i = src->rows * src->cols;
    uint8_pixel_t *uint8_pixel = (uint8_pixel_t *)src->data;
    bgr888_pixel_t *bgr888_pixel = (bgr888_pixel_t *)dst->data;

    while(i-- > 0)
    {
        uint8_t val = *uint8_pixel;

        bgr888_pixel->r = val;
        bgr888_pixel->g = val;
        bgr888_pixel->b = val;

        bgr888_pixel++;
        uint8_pixel++;
    }
}

/*!
 * \brief Converts a bgr888_pixel_t image to a uint8_pixel_t image
 *
 * This function takes all channels from a bgr888_pixel_t and computes a
 * uint8_pixel_t value using specific coefficients for each channel.
 *
 * \param[in]  src A pointer to the bgr888_pixel_t image
 * \param[out] dst A pointer to the converted uint8_pixel_t image
 */
void convertBgr888ToUint8(image_t *src, image_t *dst)
{
    uint32_t i = src->rows * src->cols;
    bgr888_pixel_t *bgr888_pixel = (bgr888_pixel_t *)src->data;
    uint8_pixel_t *uint8_pixel = (uint8_pixel_t *)dst->data;

    while(i-- > 0)
    {
        *uint8_pixel = (bgr888_pixel->r * 0.299f) +
                       (bgr888_pixel->g * 0.587f) +
                       (bgr888_pixel->b * 0.114f);

        bgr888_pixel++;
        uint8_pixel++;
    }
}

/*!
 * \brief Converts a bgr888_pixel_t image to a int16_pixel_t image
 *
 * This function takes all channels from a bgr888_pixel_t and computes a
 * int16_pixel_t value using specific coefficients for each channel.
 *
 * \param[in]  src A pointer to the bgr888_pixel_t image
 * \param[out] dst A pointer to the converted int16_pixel_t image
 */
void convertBgr888ToInt16(image_t *src, image_t *dst)
{
    uint32_t i = src->rows * src->cols;
    bgr888_pixel_t *bgr888_pixel = (bgr888_pixel_t *)src->data;
    int16_pixel_t *int16_pixel = (int16_pixel_t *)dst->data;

    while(i-- > 0)
    {
        *int16_pixel = (bgr888_pixel->r * 0.299f) +
                       (bgr888_pixel->g * 0.587f) +
                       (bgr888_pixel->b * 0.114f);

        bgr888_pixel++;
        int16_pixel++;
    }
}

/*!
 * \brief Converts any source image to an uint8_pixel_t destination image
 *
 * This function checks the source image type and calls the appropriate function
 * for conversion to an image of type uint8_pixel_t.
 *
 * \param[in]  src A pointer to any source image
 * \param[out] dst A pointer to the destination uint8_pixel_t image
 */
void convertToUint8(image_t *src, image_t *dst)
{
    // Verify image validity
    ASSERT(src == NULL, "src image is invalid");
    ASSERT(dst == NULL, "dst image is invalid");
    ASSERT(src->data == NULL, "src data is invalid");
    ASSERT(dst->data == NULL, "dst data is invalid");

    // Verify image consistency
    ASSERT(src->cols != dst->cols, "src and dst have different number of columns");
    ASSERT(src->rows != dst->rows, "src and dst have different number of rows");
    ASSERT(src == dst, "src and dst are the same images");

    if(src->type == IMGTYPE_UYVY)
    {
        convertUyvyToUint8(src, dst);
    }
    else if(src->type == IMGTYPE_BGR888)
    {
        convertBgr888ToUint8(src, dst);
    }
    else
    {
        // Conversion not implemented
        while(1)
        {}
    }
}

/*!
 * \brief Converts any source image to an uyvy_pixel_t destination image
 *
 * This function checks the source image type and calls the appropriate function
 * for conversion to an image of type uyvy_pixel_t.
 *
 * \param[in]  src A pointer to any source image
 * \param[out] dst A pointer to the destination uyvy_pixel_t image
 */
void convertToUyvy(image_t *src, image_t *dst)
{
    // Verify image validity
    ASSERT(src == NULL, "src image is invalid");
    ASSERT(dst == NULL, "dst image is invalid");
    ASSERT(src->data == NULL, "src data is invalid");
    ASSERT(dst->data == NULL, "dst data is invalid");

    // Verify image consistency
    ASSERT(src->cols != dst->cols, "src and dst have different number of columns");
    ASSERT(src->rows != dst->rows, "src and dst have different number of rows");
    ASSERT(src == dst, "src and dst are the same images");

    if(src->type == IMGTYPE_UINT8)
    {
        convertUint8ToUyvy(src, dst);
    }
    else
    {
        // Conversion not implemented
        while(1)
        {}
    }
}

/*!
 * \brief Converts any source image to an bgr888_pixel_t destination image
 *
 * This function checks the source image type and calls the appropriate function
 * for conversion to an image of type bgr888_pixel_t.
 *
 * \param[in]  src A pointer to any source image
 * \param[out] dst A pointer to the destination bgr888_pixel_t image
 */
void convertToBgr888(image_t *src, image_t *dst)
{
    // Verify image validity
    ASSERT(src == NULL, "src image is invalid");
    ASSERT(dst == NULL, "dst image is invalid");
    ASSERT(src->data == NULL, "src data is invalid");
    ASSERT(dst->data == NULL, "dst data is invalid");

    // Verify image consistency
    ASSERT(src->cols != dst->cols, "src and dst have different number of columns");
    ASSERT(src->rows != dst->rows, "src and dst have different number of rows");
    ASSERT(src == dst, "src and dst are the same images");

    if(src->type == IMGTYPE_UINT8)
    {
        convertUint8ToBgr888(src, dst);
    }
    else if(src->type == IMGTYPE_UYVY)
    {
        convertUyvyToBgr888(src, dst);
    }
    else
    {
        // Conversion not implemented
        while(1)
        {}
    }
}

/*!
 * \brief This function sets all pixels in the source image with value \p
 *        selected to \p value in the destination image
 *
 * \param[in]  src      A pointer to the source image
 * \param[out] dst      A pointer to the destination image
 * \param[in]  selected Pixel value that will be updated
 * \param[in]  value    New pixel value
 */
void setSelectedToValue(const image_t *src, image_t *dst,
                        const uint8_pixel_t selected, const uint8_pixel_t value)
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
    
    uint32_t i = src->rows * src->cols;
    uint8_pixel_t *s = (uint8_pixel_t *)src->data;
    uint8_pixel_t *d = (uint8_pixel_t *)dst->data;

    dst->rows = src->rows;
    dst->cols = src->cols;
    dst->type = src->type;

    // Loop all pixels and set selected pixel to value
    // Copy all others
    while(i-- > 0)
    {
        if(*s == selected)
        {
            *d = value;
        }
        else
        {
            *d = *s;
        }

        // Next pixel
        s++;
        d++;
    }
}

/*!
 * \brief Counts the number of pixels in the \p c connected neighbourhood that
 *        have value \p p
 *
 * \param[in] img A pointer to the image
 * \param[in] x   x-value coordinate
 * \param[in] y   y-value coordinate
 * \param[in] p   Value to look for in the neighbourhood
 * \param[in] c   Neighbourhood connectivity defined by ::eConnected
 *
 * \return The number of neighbour pixels with value \p p
 */
uint32_t neighbourCount(const image_t *img, const int32_t x, const int32_t y,
                        const uint8_pixel_t p, const eConnected c)
{
    // Verify image validity
    ASSERT(img == NULL, "img image is invalid");
    ASSERT(img->data == NULL, "img data is invalid");
    ASSERT(img->type != IMGTYPE_UINT8, "img type is invalid");

    // Verify image consistency
    ASSERT(x < 0, "x-value is out of range");
    ASSERT(x >= img->cols, "x-value is out of range");
    ASSERT(y < 0, "y-value is out of range");
    ASSERT(y >= img->rows, "y-value is out of range");

    register uint8_pixel_t *s = (uint8_pixel_t *)(img->data + (y * img->cols + x));

    unsigned int cnt = 0;

    // Left-top pixel
    if(x == 0 && y == 0)
    {
        if(*(s + sizeof(uint8_pixel_t)              ) == p){ cnt++; } // right
        if(*(s + (img->cols * sizeof(uint8_pixel_t))) == p){ cnt++; } // down

        if(c == CONNECTED_EIGHT)
        {
            if(*(s + (img->cols * sizeof(uint8_pixel_t)) + 1) == p){ cnt++; } // down-right
        }
    }
    // Right-top pixel
    else if(x == img->cols-1 && y == 0)
    {
        if(*(s - sizeof(uint8_pixel_t)              ) == p){ cnt++; } // left
        if(*(s + (img->cols * sizeof(uint8_pixel_t))) == p){ cnt++; } // down

        if(c == CONNECTED_EIGHT)
        {
            if(*(s + (img->cols * sizeof(uint8_pixel_t)) - 1) == p){ cnt++; } // down-left
        }
    }
    // Left-bottom pixel
    else if(x == 0 && y == img->rows-1)
    {
        if(*(s - (img->cols * sizeof(uint8_pixel_t))) == p){ cnt++; } // up
        if(*(s + sizeof(uint8_pixel_t)              ) == p){ cnt++; } // right

        if(c == CONNECTED_EIGHT)
        {
            if(*(s - (img->cols * sizeof(uint8_pixel_t)) + 1) == p){ cnt++; } // up-right
        }

    }
    // Right-bottom pixel
    else if(x == img->cols-1 && y == img->rows-1)
    {
        if(*(s - (img->cols * sizeof(uint8_pixel_t))) == p){ cnt++; } // up
        if(*(s - sizeof(uint8_pixel_t)              ) == p){ cnt++; } // left

        if(c == CONNECTED_EIGHT)
        {
            if(*(s - (img->cols * sizeof(uint8_pixel_t)) - 1) == p){ cnt++; } // up-left
        }
    }
    // Top border pixels
    else if(y == 0)
    {
        if(*(s - sizeof(uint8_pixel_t)              ) == p){ cnt++; } // left
        if(*(s + sizeof(uint8_pixel_t)              ) == p){ cnt++; } // right
        if(*(s + (img->cols * sizeof(uint8_pixel_t))) == p){ cnt++; } // down

        if(c == CONNECTED_EIGHT)
        {
            if(*(s + (img->cols * sizeof(uint8_pixel_t)) - 1) == p){ cnt++; } // down-left
            if(*(s + (img->cols * sizeof(uint8_pixel_t)) + 1) == p){ cnt++; } // down-right
        }
    }
    // Bottom border pixels
    else if(y == img->rows-1)
    {
        if(*(s - (img->cols * sizeof(uint8_pixel_t))) == p){ cnt++; } // up
        if(*(s - sizeof(uint8_pixel_t)              ) == p){ cnt++; } // left
        if(*(s + sizeof(uint8_pixel_t)              ) == p){ cnt++; } // right

        if(c == CONNECTED_EIGHT)
        {
            if(*(s - (img->cols * sizeof(uint8_pixel_t)) - 1) == p){ cnt++; } // up-left
            if(*(s - (img->cols * sizeof(uint8_pixel_t)) + 1) == p){ cnt++; } // up-right
        }
    }
    // Left border pixels
    else if(x == 0)
    {
        if(*(s - (img->cols * sizeof(uint8_pixel_t))) == p){ cnt++; } // up
        if(*(s + sizeof(uint8_pixel_t)              ) == p){ cnt++; } // right
        if(*(s + (img->cols * sizeof(uint8_pixel_t))) == p){ cnt++; } // down

        if(c == CONNECTED_EIGHT)
        {
            if(*(s - (img->cols * sizeof(uint8_pixel_t)) + 1) == p){ cnt++; } // up-right
            if(*(s + (img->cols * sizeof(uint8_pixel_t)) + 1) == p){ cnt++; } // down-right
        }
    }
    // Right border pixels
    else if(x == img->cols-1)
    {
        if(*(s - (img->cols * sizeof(uint8_pixel_t))) == p){ cnt++; } // up
        if(*(s - sizeof(uint8_pixel_t)              ) == p){ cnt++; } // left
        if(*(s + (img->cols * sizeof(uint8_pixel_t))) == p){ cnt++; } // down

        if(c == CONNECTED_EIGHT)
        {
            if(*(s - (img->cols * sizeof(uint8_pixel_t)) - 1) == p){ cnt++; } // up-left
            if(*(s + (img->cols * sizeof(uint8_pixel_t)) - 1) == p){ cnt++; } // down-left
        }
    }
    else
    {
        // Inner pixels
        if(*(s - (img->cols * sizeof(uint8_pixel_t))) == p){ cnt++; } // up
        if(*(s - sizeof(uint8_pixel_t)              ) == p){ cnt++; } // left
        if(*(s + sizeof(uint8_pixel_t)              ) == p){ cnt++; } // right
        if(*(s + (img->cols * sizeof(uint8_pixel_t))) == p){ cnt++; } // down

        if(c == CONNECTED_EIGHT)
        {
            if(*(s - (img->cols * sizeof(uint8_pixel_t)) - 1) == p){ cnt++; } // up-left
            if(*(s - (img->cols * sizeof(uint8_pixel_t)) + 1) == p){ cnt++; } // up-right
            if(*(s + (img->cols * sizeof(uint8_pixel_t)) - 1) == p){ cnt++; } // down-left
            if(*(s + (img->cols * sizeof(uint8_pixel_t)) + 1) == p){ cnt++; } // down-right
        }
    }

    return cnt;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

/*!
 * \brief Applies a filter mask to an image by convolving the filter mask with
 *        the original image
 *
 * \param[in]  src A pointer to the source image
 * \param[out] dst A pointer to the destination image
 * \param[in]  msk A pointer to the mask image
 */
void convolve(const image_t *src, image_t *dst, const image_t *msk)
{
    // Verify image validity
    ASSERT(src == NULL, "src image is invalid");
    ASSERT(dst == NULL, "dst image is invalid");
    ASSERT(msk == NULL, "msk image is invalid");
    ASSERT(src->data == NULL, "src data is invalid");
    ASSERT(dst->data == NULL, "dst data is invalid");
    ASSERT(msk->data == NULL, "msk data is invalid");
    ASSERT(msk->rows % 2 == 0, "mask rows must be odd");
    ASSERT(msk->cols % 2 == 0, "mask cols must be odd");

    // Verify image consistency
    ASSERT(src->cols != dst->cols, "src and dst have different number of columns");
    ASSERT(src->rows != dst->rows, "src and dst have different number of rows");
    ASSERT(src == dst, "src and dst are the same images");

    if(src->type == IMGTYPE_INT16)
    {
        ASSERT(dst->type != IMGTYPE_INT16, "dst type is invalid");
        ASSERT(msk->type != IMGTYPE_INT16, "msk type is invalid");

        // Loop all pixels
        for(int32_t y=0; y<src->rows; y++)
        {
            for(int32_t x=0; x<src->cols; x++)
            {
                int32_t val = 0;
                int32_t dr = (msk->rows/2);
                int32_t dc = (msk->cols/2);

                // Apply the kernel only for pixels within the image
                for(int32_t j=-dr; j<=dr; j++)
                {
                    for(int32_t i=-dc; i<=dc; i++)
                    {
                        if(((x-i) >= 0) &&
                            ((y-j) >= 0) &&
                            ((x-i) <  src->cols) &&
                            ((y-j) <  src->rows))
                        {
                            val += getInt16Pixel(src,x-i,y-j) *
                                   getInt16Pixel(msk,i+dc,j+dr);
                        }
                    }
                }

                // Clip the result
                if(val>INT16_PIXEL_MAX)
                {
                    val=INT16_PIXEL_MAX;
                }

                if(val<INT16_PIXEL_MIN)
                {
                    val=INT16_PIXEL_MIN;
                }

                // Store the result
                setInt16Pixel(dst, x, y, val);
            }
        }
    }
    else if(src->type == IMGTYPE_UINT8)
    {
        ASSERT(dst->type != IMGTYPE_UINT8, "dst type is invalid");
        ASSERT(msk->type != IMGTYPE_UINT8, "msk type is invalid");

        // Loop all pixels
        for(int32_t y=0; y<src->rows; y++)
        {
            for(int32_t x=0; x<src->cols; x++)
            {
                int32_t val = 0;
                int32_t dr = (msk->rows/2);
                int32_t dc = (msk->cols/2);

                // Apply the kernel only for pixels within the image
                for(int32_t j=-dr; j<=dr; j++)
                {
                    for(int32_t i=-dc; i<=dc; i++)
                    {
                        if(((x-i) >= 0) &&
                            ((y-j) >= 0) &&
                            ((x-i) <  src->cols) &&
                            ((y-j) <  src->rows))
                        {
                            val += getUint8Pixel(src,x-i,y-j) * getUint8Pixel(msk,i+dc,j+dr);
                        }
                    }
                }

                // Clip the result
                if(val>UINT8_PIXEL_MAX)
                {
                    val=UINT8_PIXEL_MAX;
                }

                if(val<UINT8_PIXEL_MIN)
                {
                    val=UINT8_PIXEL_MIN;
                }

                // Store the result
                setUint8Pixel(dst, x, y, val);
            }
        }
    }
}

/*!
 * \brief Applies a filter mask to an image by convolving the filter mask with
 *        the original image
 *
 * Assumes a 3x3 mask and ignores the border pixels. Also assumes that all
 * images are of type int16_pixel_t.
 *
 * \param[in]  src A pointer to the source image
 * \param[out] dst A pointer to the destination image
 * \param[in]  msk A pointer to the mask image
 *
 * \todo Implement this function
 */
void convolveFast(const image_t *src, image_t *dst, const image_t *msk)
{
    const int32_t srcCols = src->cols;
    const int32_t srcRows = src->rows;

    int16_t *srcData = (int16_t*)src->data;
    int16_t *dstData = (int16_t*)dst->data;
    int16_t *mskData = (int16_t*)msk->data;


    // Iterate over the entire image
    // By beginning at dr and dc its starts at 1 and not 0 and
    //and -1 on the maximum will skip the border pixels
    for (int32_t y = 1; y < srcRows - 1; ++y) {
        for (int32_t x = 1; x < srcCols - 1; ++x) {
            int32_t val = 0;

            // Unroll the 3x3 kernel
            val += *(srcData + (y - 1) * srcCols + (x - 1)) * (mskData[0]);
            val += *(srcData + (y - 1) * srcCols + (x)) * (mskData[3]);
            val += *(srcData + (y - 1) * srcCols + (x + 1)) * (mskData[6]);

            val += *(srcData + (y) * srcCols + (x - 1)) * (mskData[1]);
            val += *(srcData + (y) * srcCols + (x)) * (mskData[4]);
            val += *(srcData + (y) * srcCols + (x + 1)) * (mskData[7]);

            val += *(srcData + (y + 1) * srcCols + (x - 1)) * (mskData[2]);
            val += *(srcData + (y + 1) * srcCols + (x)) * (mskData[5]);
            val += *(srcData + (y + 1) * srcCols + (x + 1)) * (mskData[8]);

            // Store the result in the destination image
            *(dstData + (y * dst->cols) + x) = (int16_t)val;
        }
    }

}

/*!
 * \brief Compares two images mathematically
 *
 * The result is a two-dimensional expression of equivalence. The mask image
 * is often referred to as the template. The correlation is then called
 * template matching.
 *
 * \param[in]  src A pointer to the source image
 * \param[out] dst A pointer to the destination image
 * \param[in]  msk A pointer to the mask image
 */
void correlate(const image_t *src, image_t *dst, const image_t *msk)
{
    // Verify image validity
    ASSERT(src == NULL, "src image is invalid");
    ASSERT(dst == NULL, "dst image is invalid");
    ASSERT(msk == NULL, "msk image is invalid");
    ASSERT(src->data == NULL, "src data is invalid");
    ASSERT(dst->data == NULL, "dst data is invalid");
    ASSERT(msk->data == NULL, "msk data is invalid");
    ASSERT(msk->rows % 2 == 0, "mask rows must be odd");
    ASSERT(msk->cols % 2 == 0, "mask cols must be odd");

    // Verify image consistency
    ASSERT(src->cols != dst->cols, "src and dst have different number of columns");
    ASSERT(src->rows != dst->rows, "src and dst have different number of rows");
    ASSERT(src == dst, "src and dst are the same images");

    if(src->type == IMGTYPE_INT16)
    {
        ASSERT(dst->type != IMGTYPE_INT16, "dst type is invalid");
        ASSERT(msk->type != IMGTYPE_INT16, "msk type is invalid");

        // Loop all pixels
        for(int32_t y=0; y<src->rows; y++)
        {
            for(int32_t x=0; x<src->cols; x++)
            {
                int32_t val = 0;
                int32_t dr = (msk->rows/2);
                int32_t dc = (msk->cols/2);

                // Apply the kernel only for pixels within the image
                for(int32_t j=-dr; j<=dr; j++)
                {
                    for(int32_t i=-dc; i<=dc; i++)
                    {
                        if(((x+i) >= 0) &&
                            ((y+j) >= 0) &&
                            ((x+i) <  src->cols) &&
                            ((y+j) <  src->rows))
                        {
                            val += getInt16Pixel(src,x+i,y+j) *
                                   getInt16Pixel(msk,i+dc,j+dr);
                        }
                    }
                }

                // Clip the result
                if(val>INT16_PIXEL_MAX)
                {
                    val=INT16_PIXEL_MAX;
                }

                if(val<INT16_PIXEL_MIN)
                {
                    val=INT16_PIXEL_MIN;
                }

                // Store the result
                setInt16Pixel(dst, x, y, val);
            }
        }
    }
    else if(src->type == IMGTYPE_UINT8)
    {
        ASSERT(dst->type != IMGTYPE_UINT8, "dst type is invalid");
        ASSERT(msk->type != IMGTYPE_UINT8, "msk type is invalid");

        // Loop all pixels
        for(int32_t y=0; y<src->rows; y++)
        {
            for(int32_t x=0; x<src->cols; x++)
            {
                int32_t val = 0;
                int32_t dr = (msk->rows/2);
                int32_t dc = (msk->cols/2);

                // Apply the kernel only for pixels within the image
                for(int32_t j=-dr; j<=dr; j++)
                {
                    for(int32_t i=-dc; i<=dc; i++)
                    {
                        if(((x+i) >= 0) &&
                            ((y+j) >= 0) &&
                            ((x+i) <  src->cols) &&
                            ((y+j) <  src->rows))
                        {
                            val += getUint8Pixel(src,x+i,y+j) * getUint8Pixel(msk,i+dc,j+dr);
                        }
                    }
                }

                // Clip the result
                if(val>UINT8_PIXEL_MAX)
                {
                    val=UINT8_PIXEL_MAX;
                }

                if(val<UINT8_PIXEL_MIN)
                {
                    val=UINT8_PIXEL_MIN;
                }

                // Store the result
                setUint8Pixel(dst, x, y, val);
            }
        }
    }
}

/*!
 * \brief Scales an image to it's full pixel depth.
 *
 * This function can be used to enhance the image contrast. It is also used to
 * scale larger pixel data types to smaller pixel data types, e.g. float to
 * basic.
 *
 * \param[in]  src A pointer to the source image
 * \param[out] dst A pointer to the destination image
 */
void scale(const image_t *src ,image_t *dst)
{
    // Verify image validity
    ASSERT(src == NULL, "src image is invalid");
    ASSERT(dst == NULL, "dst image is invalid");
    ASSERT(src->type != IMGTYPE_UINT8, "src type is invalid");
    ASSERT(dst->type != IMGTYPE_UINT8, "dst type is invalid");
    ASSERT(src->data == NULL, "src data is invalid");
    ASSERT(dst->data == NULL, "dst data is invalid");

    // Verify image consistency
    ASSERT(src->cols != dst->cols, "src and dst have different number of columns");
    ASSERT(src->rows != dst->rows, "src and dst have different number of rows");

    uint8_pixel_t min=UINT8_PIXEL_MAX, max=UINT8_PIXEL_MIN;
    uint32_t imsize = src->rows * src->cols;
    uint8_pixel_t *s = (uint8_pixel_t *)src->data;

    // Scan input image for min/max values
    for(uint32_t i=0; i<imsize; ++i)
    {
        if(*s < min)
        {
            min = *s;
        }

        if(*s > max)
        {
            max = *s;
        }

        ++s;
    }

    // Restore pointers
    s = (uint8_pixel_t *)src->data;
    uint8_pixel_t *d = (uint8_pixel_t *)dst->data;

    // Prevent division by zero
    if(max == min)
    {
        // Scale the output to basic image type
        for(uint32_t i=0; i<imsize; ++i)
        {
            *d++ = (uint8_pixel_t)128;
        }
    }
    else
    {
        // Scale the output to basic image type
        for(uint32_t i=0; i<imsize; ++i)
        {
            *d++ = (uint8_pixel_t)((255.0f/(max-min)) * (*s++ - min) + 0.5f);
        }
    }
}

/*!
 * \brief Scales an int16_pixel_t image to uint8_pixel_t image
 *
 * This function can be used to convert a int16_pixel_t image to a uint8_pixel_t
 * image. The uint8_pixel_t image will be scaled to its full contrast. This
 * function is mainly used for displaying int16_pixel_t images.
 *
 * \param[in]  src A pointer to the source image
 * \param[out] dst A pointer to the destination image
 */
void scaleInt16ToUint8(const image_t *src ,image_t *dst)
{
    // Verify image validity
    ASSERT(src == NULL, "src image is invalid");
    ASSERT(dst == NULL, "dst image is invalid");
    ASSERT(src->type != IMGTYPE_INT16, "src type is invalid");
    ASSERT(dst->type != IMGTYPE_UINT8, "dst type is invalid");
    ASSERT(src->data == NULL, "src data is invalid");
    ASSERT(dst->data == NULL, "dst data is invalid");

    // Verify image consistency
    ASSERT(src->cols != dst->cols, "src and dst have different number of columns");
    ASSERT(src->rows != dst->rows, "src and dst have different number of rows");

    int16_pixel_t min=INT16_PIXEL_MAX, max=INT16_PIXEL_MIN;
    uint32_t imsize = src->rows * src->cols;

    int16_pixel_t *s = (int16_pixel_t *)src->data;

    // Scan input image for min/max values
    for(uint32_t i=0; i<imsize; ++i)
    {
        if(*s < min)
        {
            min = *s;
        }

        if(*s > max)
        {
            max = *s;
        }

        ++s;
    }

    // Restore pointers
    s = (int16_pixel_t *)src->data;
    uint8_pixel_t *d = (uint8_pixel_t *)dst->data;

    // Prevent division by zero
    if(max == min)
    {
        // Scale the output to basic image type
        for(uint32_t i=0; i<imsize; ++i)
        {
            *d++ = (uint8_pixel_t)128;
        }
    }
    else
    {
        // Scale the output to basic image type
        for(uint32_t i=0; i<imsize; ++i)
        {
            *d++ = (uint8_pixel_t)((255.0f/(max-min)) * (*s++ - min) + 0.5f);
        }
    }
}

/*!
 * \brief Scales an float_pixel_t image to uint8_pixel_t image
 *
 * This function can be used to convert a float_pixel_t image to a uint8_pixel_t
 * image. The uint8_pixel_t image will be scaled to its full contrast. This
 * function is mainly used for displaying int16_pixel_t images.
 *
 * \param[in]  src A pointer to the source image
 * \param[out] dst A pointer to the destination image
 */
void scaleFloatToUint8(const image_t *src ,image_t *dst)
{
    // Verify image validity
    ASSERT(src == NULL, "src image is invalid");
    ASSERT(dst == NULL, "dst image is invalid");
    ASSERT(src->type != IMGTYPE_FLOAT, "src type is invalid");
    ASSERT(dst->type != IMGTYPE_UINT8, "dst type is invalid");
    ASSERT(src->data == NULL, "src data is invalid");
    ASSERT(dst->data == NULL, "dst data is invalid");

    // Verify image consistency
    ASSERT(src->cols != dst->cols, "src and dst have different number of columns");
    ASSERT(src->rows != dst->rows, "src and dst have different number of rows");

    float_pixel_t min=FLOAT_PIXEL_MAX, max=FLOAT_PIXEL_MIN;
    uint32_t imsize = src->rows * src->cols;

    float_pixel_t *s = (float_pixel_t *)src->data;

    // Scan input image for min/max values
    for(uint32_t i=0; i<imsize; ++i)
    {
        if(*s < min)
        {
            min = *s;
        }

        if(*s > max)
        {
            max = *s;
        }

        ++s;
    }

    // Restore pointers
    s = (float_pixel_t *)src->data;
    uint8_pixel_t *d = (uint8_pixel_t *)dst->data;

    // Prevent division by zero
    if(max == min)
    {
        // Scale the output to basic image type
        for(uint32_t i=0; i<imsize; ++i)
        {
            *d++ = (uint8_pixel_t)128;
        }
    }
    else
    {
        // Scale the output to basic image type
        for(uint32_t i=0; i<imsize; ++i)
        {
            *d++ = (uint8_pixel_t)((255.0f/(max-min)) * (*s++ - min) + 0.5f);
        }
    }
}

/*!
 * \brief Scales an uint8_pixel_t image to its full range (0-255) with an ultra
 *        fast implementation
 *
 * \param[in]  src A pointer to the source image
 * \param[out] dst A pointer to the destination image
 *
 * \todo Implement this function
 */
void scaleFast(const image_t *src, image_t *dst)
{
    uint8_pixel_t min = UINT8_MAX, max = 0;
    uint32_t imsize_8 = (src->rows * src->cols) / 4;
    uint32_t *s = (uint32_t *)src->data;

    // Step 1: Find min and max
    //Sped up by handeling 4 pixels at a time.
    for (uint32_t i = 0; i < imsize_8; ++i) {
        uint32_t four_pixels = *s++;
        if((uint8_pixel_t)(four_pixels >> 0) > max){max = (uint8_pixel_t)(four_pixels >> 0);}
        if((uint8_pixel_t)(four_pixels >> 0) < min){min = (uint8_pixel_t)(four_pixels >> 0);}
        //Reading the second pixel
        if((uint8_pixel_t)(four_pixels >> 8) > max){max = (uint8_pixel_t)(four_pixels >> 8);}
        if((uint8_pixel_t)(four_pixels >> 8) < min){min = (uint8_pixel_t)(four_pixels >> 8);}
        //Reading the third pixel
        if((uint8_pixel_t)(four_pixels >> 16) > max){max = (uint8_pixel_t)(four_pixels >> 16);}
        if((uint8_pixel_t)(four_pixels >> 16) < min){min = (uint8_pixel_t)(four_pixels >> 16);}
        //Reading the four pixel
        if((uint8_pixel_t)(four_pixels >> 24) > max){max = (uint8_pixel_t)(four_pixels >> 24);}
        if((uint8_pixel_t)(four_pixels >> 24) < min){min = (uint8_pixel_t)(four_pixels >> 24);}
    }

    // Step 2: Precompute LUT
    uint8_pixel_t LUT[256];
    if (max == min) {
        // If all pixels are the same, map everything to a neutral value (e.g., 128)
        for (int i = 0; i < 256; ++i) {
            LUT[i] = 128;
        }
    } else {
        //Using the standard formula to compute the scale factor for the image
        float scaleFactor = 255.0f / (max - min);
        for (int i = 0; i < 256; ++i) {
            LUT[i] = (uint8_pixel_t)((i - min) * scaleFactor + 0.5f);
        }
    }

    // Step 3: Scale and write pixels using LUT
    uint32_t *dst_data = (uint32_t *)dst->data;
    s = (uint32_t *)src->data; // Reset source pointer

    for (uint32_t i = 0; i < imsize_8; ++i) {
        // First 4 pixels
        uint32_t four_pixels = *s++;
        uint8_pixel_t p0 = LUT[(four_pixels >> 0) & 0xFF];
        uint8_pixel_t p1 = LUT[(four_pixels >> 8) & 0xFF];
        uint8_pixel_t p2 = LUT[(four_pixels >> 16) & 0xFF];
        uint8_pixel_t p3 = LUT[(four_pixels >> 24) & 0xFF];
        *dst_data++ = (p0 << 0) | (p1 << 8) | (p2 << 16) | (p3 << 24);
    }
}
