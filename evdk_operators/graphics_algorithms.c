/*! ***************************************************************************
 *
 * \brief     Graphics algorithms
 * \file      graphics_algorithms.c
 * \author    Hugo Arends - HAN Embedded Vision and Machine Learning
 * \author
 * \date      October 2024
 *
 * \see       Myler, H. R., & Weeks, A. R. (2009). The pocket handbook of
 *            image processing algorithms in C. Prentice Hall Press.
 * \see       Heckbert, P. S. (1989). Fundamentals of texture mapping and
 *            image warping. p. 9-29
 * \see       Affine transformation. (n.d.). In Wikipedia. Retrieved December
 *            12, 2019, from
 *            https://en.wikipedia.org/wiki/Affine_transformation
 * \see       Wikipedia contributors. (2024, October 1). Bresenham's line
 *            algorithm. In Wikipedia, The Free Encyclopedia. Retrieved 06:37,
 *            October 15, 2024, from
 *            https://en.wikipedia.org/w/index.php?title=
 *            Bresenham%27s_line_algorithm&oldid=1248820154
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
#include "graphics_algorithms.h"
#include "morphological_filters.h"

#include "math.h"
#include "fonts.h"

/*!
 * \brief Pointer to the selected font
 *
 * The available fonts are in fonts.h and fonts.c.
 *
 * Note that Monospaced_plain_10 is the default font and should not me removed
 * from fonts.c and/or fonts.h.
 */
static const char *font = Monospaced_plain_10;

/*!
 * \brief Local x value
 */
static int32_t local_x = 0;

/*!
 * \brief Local y value
 */
static int32_t local_y = 0;

/*!
 * \brief Local background color value
 */
static uint8_pixel_t local_uint8_background = 255;
static bgr888_pixel_t local_bgr888_background = (bgr888_pixel_t){0xFF,0xFF,0xFF};

/*!
 * \brief Local foreground color value
 */
static uint8_pixel_t local_uint8_foreground = 0;
static bgr888_pixel_t local_bgr888_foreground = (bgr888_pixel_t){0x00,0x00,0x00};

/*!
 * \brief Local variable indicating if characters should be drawn flipped
 */
static uint32_t local_flip_characters = 0;

/*!
 * \brief Sets the font
 *
 * Set the font
 *
 * \param[in]  f  A pointer to a font
 */
void textSetfont(const char *f)
{
    font = f;
}

/*!
 * \brief Sets the text color
 *
 * Setter function for the uint8_pixel_t text color
 *
 * \param[in]  c  Color of the text 0-255
 */
void textSetUint8Colors(const uint8_pixel_t background, const uint8_pixel_t foreground)
{
    local_uint8_background = background;
    local_uint8_foreground = foreground;
}

/*!
 * \brief Sets the text color
 *
 * Setter function for the bgr888_pixel_t text color
 *
 * \param[in]  c  Color of the text 0-255
 */
void textSetBgr888Colors(const bgr888_pixel_t background, const bgr888_pixel_t foreground)
{
    local_bgr888_background = background;
    local_bgr888_foreground = foreground;
}

/*!
 * \brief Sets x and y
 *
 * This function updates the current (x,y) position
 *
 * \param[in]  x  New value for x
 * \param[in]  y  New value for y
 */
void textSetxy(const int32_t x, const int32_t y)
{
    local_x = x;
    local_y = y;
}

/*!
 * \brief Flips the text in the y-axis
 *
 * The windows driver flips all images around the y-axis. This function provides
 * the option to flip each character when it is drawn, so when it is flipped by
 * windows, it is visualized properly.
 *
 * \param[in]  flipped  0 characters should NOT be flipped in the y-axis (default)
 *                      1 characters should be flipped in the y-axis
 */
void textSetFlipCharacters(const uint32_t flipped)
{
    local_flip_characters = flipped;
}

/*!
 * \brief Displays a character at the current (x,y) position
 *
 * Writes a character to the image
 *
 * \param[in]  img Image
 * \param[in]  c   Character to write
 */
void textPutchar(image_t *img, const char c)
{
    // Get the first four parameters from the font
    //uint8_t font_width = font[0];
    uint8_t font_height = font[1];
    uint8_t font_firstchar = font[2];
    uint8_t font_numchars = font[3];

    // Calculate the index in the jump table
    uint32_t index = 4 + ((c - font_firstchar) * 4);

    // Read the data from the jump table
    uint8_t offset1 = font[index];
    uint8_t offset2 = font[index + 1];
    uint8_t n_bytes = font[index + 2];
    uint8_t char_width = font[index + 3];

    // Calculate the index in the data table
    index = 4 + (font_numchars * 4) + (256UL * offset1) + offset2;

    // Calculate the number of bytes for each column
    uint8_t bytes_per_col = (font_height / 8);
    if((font_height % 8) > 0)
    {
        bytes_per_col++;
    }

    // Calculate the total number of bytes for this character
    uint32_t bytes_total = bytes_per_col * char_width;

    // Temporary variable for y position
    int32_t y_tmp = local_y;

    // Temporary variable counting the character height in pixels
    int32_t height = 0;

    // Loop all bytes in a character
    for(uint32_t n=0; n<bytes_total; n++)
    {
        // Initially a byte is cleared
        char data = 0;

        // Is there data available in the character table for this byte?
        if(n < n_bytes)
        {
            data = font[index + n];
        }

        // Next column?
        if((n % bytes_per_col) == 0)
        {
            y_tmp=local_y;
            local_x++;

            // Stop if the x value is outside screen boundaries
            if(local_x >= img->cols)
            {
                return;
            }

            height = 0;
        }

        // Loop all bits in the byte and update the frame buffer
        for(uint8_t mask = 1; mask != 0; mask <<= 1)
        {
            if(img->type == IMGTYPE_UINT8)
            {
                uint8_pixel_t val = (data & mask) ? local_uint8_foreground : 
                    local_uint8_background;

                setUint8Pixel(img, local_x, y_tmp, val);
            }
            else if(img->type == IMGTYPE_BGR888)
            {
                bgr888_pixel_t val = (data & mask) ? local_bgr888_foreground : 
                    local_bgr888_background;

                setBgr888Pixel(img, local_x, y_tmp, val);
            }
            else
            {
                // Image type not supported for text
                while(1)
                {}
            }

            if(local_flip_characters == 0)
            {
                y_tmp++;

                // Stop if the y value is outside screen boundaries
                if(y_tmp >= img->rows)
                {
                    break;
                }
            }
            else
            {
                y_tmp--;

                // Stop if the y value is outside screen boundaries
                if(y_tmp < 0)
                {
                    break;
                }
            }

            // Stop if the font height has been reached
            height++;
            if(height >= font_height)
            {
                break;
            }
        }
    }
}

/*!
 * \brief Displays a string of characters starting at position (x,y).
 *
 * Writes a string of character to the image.
 *
 * A '\n' character moves the (x,y) position to the next line, taking the
 * current selected font height into account.
 *
 * A '\r' character is ignored.
 *
 * \param[in]  img  Image
 * \param[in]  str  '\0' terminated string
 */
void textPutstring(image_t *img, const char *str)
{
    int32_t delta = 0;
    int32_t org_x = local_x;

    uint32_t i=0;
    while(str[i] != '\0')
    {
        if(str[i] == '\n')
        {
            // Go to a new line
            // Set the original x value and increment the y value by the font
            // height
            delta += font[1];
            textSetxy(org_x, local_y + delta);
        }
        else if(str[i] == '\r')
        {
            // Ignore
        }
        else
        {
            textPutchar(img, str[i]);
        }

        i++;
    }
}

/*!
 * \brief Draws a line
 *
 * The line is draw from point \p p1 to \p p2. All drawn pixels will have the
 * value \p val.
 *
 * \param[inout]  img A pointer to the image
 * \param[in]     p1  Point 1
 * \param[in]     p2  Point 2
 * \param[in]     val Pixel value of the line
 */
void drawLineUint8(image_t *img, point_t p1, point_t p2, uint8_pixel_t val)
{
    int dx = abs(p2.x - p1.x);
    int dy = abs(p2.y - p1.y);
    int sgnX = (p1.x < p2.x) ? 1 : -1;
    int sgnY = (p1.y < p2.y) ? 1 : -1;
    int e = 0;

    for(int i=0; i < dx+dy; i++)
    {
        // Check boundaries
        if((p1.x >= 0) &&
           (p1.x < img->cols) &&
           (p1.y >= 0) &&
           (p1.y < img->rows))
        {
            setUint8Pixel(img, p1.x, p1.y, val);
        }

        int e1 = e + dy;
        int e2 = e - dx;
        if (abs(e1) < abs(e2))
        {
            p1.x += sgnX;
            e = e1;
        }
        else
        {
            p1.y += sgnY;
            e = e2;
        }
    }
}

void drawLineBgr888(image_t *src, point_t p1, point_t p2, bgr888_pixel_t val)
{
    int dx = abs(p2.x - p1.x);
    int dy = abs(p2.y - p1.y);
    int sgnX = p1.x < p2.x ? 1 : -1;
    int sgnY = p1.y < p2.y ? 1 : -1;
    int e = 0;

    for(int i=0; i < dx+dy; i++)
    {
        // Check boundaries
        if((p1.x >= 0) &&
           (p1.x < src->cols) &&
           (p1.y >= 0) &&
           (p1.y < src->rows))
        {
            setBgr888Pixel(src,p1.x,p1.y,val);
        }

        int e1 = e + dy;
        int e2 = e - dx;
        if (abs(e1) < abs(e2))
        {
            p1.x += sgnX;
            e = e1;
        }
        else
        {
            p1.y += sgnY;
            e = e2;
        }
    }
}

void drawLineUyvy(image_t *src, point_t p1, point_t p2, uyvy_pixel_t val)
{
    int dx = abs(p2.x - p1.x);
    int dy = abs(p2.y - p1.y);
    int sgnX = p1.x < p2.x ? 1 : -1;
    int sgnY = p1.y < p2.y ? 1 : -1;
    int e = 0;

    for(int i=0; i < dx+dy; i++)
    {
        // Check boundaries
        if((p1.x >= 0) &&
           (p1.x < src->cols) &&
           (p1.y >= 0) &&
           (p1.y < src->rows))
        {
            setUyvyPixel(src,p1.x,p1.y,val);
        }

        int e1 = e + dy;
        int e2 = e - dx;
        if (abs(e1) < abs(e2))
        {
            p1.x += sgnX;
            e = e1;
        }
        else
        {
            p1.y += sgnY;
            e = e2;
        }
    }
}

/*!
 * \brief Applies an affine transformation to the source image
 *
 * The transformation is defined by mask \p m. The result is placed in the
 * destination image.
 * With \p d set to forward transformation, all pixels in the source image are
 * mapped to a new location in the destination image.
 * With \p d set to backward transformation, all pixels in the destination image
 * are mapped to a pixel in the source image.
 *
 * \param[in]  src A pointer to the source image
 * \param[out] dst A pointer to the destination image
 * \param[in]  d   Transformation direction of type ::eTransformDirection
 * \param[in]  m   A pointer to a 2x3 transformation matrix
 */
void affineTransformation(const image_t *src, image_t *dst,
                          eTransformDirection d, float m[][3])
{
    // Verify image validity
    ASSERT(src == NULL, "src image is invalid");
    ASSERT(dst == NULL, "dst image is invalid");
    ASSERT(src->data == NULL, "src data is invalid");
    ASSERT(dst->data == NULL, "dst data is invalid");
    ASSERT(src->type != IMGTYPE_UINT8, "src type is invalid");
    ASSERT(dst->type != IMGTYPE_UINT8, "dst type is invalid");

    // Verify image consistency
    ASSERT(src == dst, "src and dst are the same images");

    // Verify parameters
    ASSERT(d != TRANSFORM_FORWARD && d != TRANSFORM_BACKWARD, "d is invalid");
    ASSERT(m == NULL, "matrix is invalid");

    if(d == TRANSFORM_BACKWARD)
    {
        // Loop all pixels
        for(int32_t y=0; y<dst->rows; y++)
        {
            for(int32_t x=0; x<dst->cols; x++)
            {
                // Given the transformation matrix, calculate the new
                // coordinates
                int32_t xs = (x * m[0][0]) + (y * m[0][1]) + (m[0][2]);
                int32_t ys = (x * m[1][0]) + (y * m[1][1]) + (m[1][2]);

                if((xs >= 0) &&
                   (ys >= 0) &&
                   (xs <  src->cols) &&
                   (ys <  src->rows))
                {
                    // Backward transformation: for all coordinates in dst, find
                    // the closest matching coordinate in src
                    setUint8Pixel(dst, x, y, getUint8Pixel(src, xs, ys));
                }
            }
        }
    }
    else // FORWARD
    {
        // Loop all pixels
        for(int32_t y=0; y<src->rows; y++)
        {
            for(int32_t x=0; x<src->cols; x++)
            {
                // Given the transformation matrix, calculate the new
                // coordinates
                int32_t xd = (x * m[0][0]) + (y * m[0][1]) + (m[0][2]);
                int32_t yd = (x * m[1][0]) + (y * m[1][1]) + (m[1][2]);

                if((xd >= 0) &&
                   (yd >= 0) &&
                   (xd <  dst->cols) &&
                   (yd <  dst->rows))
                {
                    // Forward transformation: for all coordinates in src, find
                    // the closest matching coordinate in dst
                    setUint8Pixel(dst, xd, yd, getUint8Pixel(src, x, y));
                }
            }
        }
    }
}

/*!
 * \brief Rotates an image by a given angle
 *
 * A positive angle will cause a CW rotation, because
 * the y-axis points downward.
 *
 * \param[in]  src     A pointer to the source image
 * \param[out] dst     A pointer to the destination image
 * \param[in]  radians Angle in radians
 * \param[in]  center  Pixel location that will be the origin of rotation
 */
void rotate(const image_t *src, image_t *dst, const float radians,
            const point_t center)
{
    // Verify image validity
    ASSERT(src == NULL, "src image is invalid");
    ASSERT(dst == NULL, "dst image is invalid");
    ASSERT(src->data == NULL, "src data is invalid");
    ASSERT(dst->data == NULL, "dst data is invalid");
    ASSERT(src->type != IMGTYPE_UINT8, "src type is invalid");
    ASSERT(dst->type != IMGTYPE_UINT8, "dst type is invalid");

    // Verify image consistency
    ASSERT(src == dst, "src and dst are the same images");

    // Verify parameters
    ASSERT(center.x < 0 || center.x >= src->cols, "invalid origin.x value");
    ASSERT(center.y < 0 || center.y >= src->rows, "invalid origin.y value");

    // Local variables
    float dx, dy;
    float sinr = sin(radians);
    float cosr = cos(radians);
    float xn;
    float yn;

    // Rotate
    for(int y=0; y < src->rows; y++)
    {
        for(int x=0; x < src->cols; x++)
        {
            // Translate to center
            dx = (float)x - (float)center.x;
            dy = (float)y - (float)center.y;

            // Rotate.
            // Using inverse functiondue to backward transformation
            xn = (dx * cosr) + (dy * sinr);
            yn = (dx * -sinr) + (dy * cosr);

            // Translate back
            xn = xn + (float)center.x;
            yn = yn + (float)center.y;

            if((xn >= 0) &&
               (yn >= 0) &&
               (xn <  src->cols) &&
               (yn <  src->rows))
            {
                // Backward transformation: for all coordinates in dst, find
                // the closest matching coordinate in src
                setUint8Pixel(dst, x, y, getUint8Pixel(src,xn,yn));
            }
        }
    }
}

/*!
 * \brief Rotate the image 180 degrees (C implementation)
 *
 * This operation is also known as flipping in both horizontal and vertical
 * direction.
 *
 * \param[in,out] img
 */
void rotate180_c(const image_t *img)
{
    // Verify image validity
    ASSERT(img == NULL, "img image is invalid");
    ASSERT(img->data == NULL, "img data is invalid");
    ASSERT(img->type != IMGTYPE_UINT8, "img type is invalid");

    register uint32_t i;
    register uint8_pixel_t *s = (uint8_pixel_t *)img->data;
    register uint8_pixel_t *d = (uint8_pixel_t *)(img->data +
                              (img->rows * img->cols * sizeof(uint8_pixel_t)) -
                              (1 * sizeof(uint8_pixel_t)));
    register uint8_pixel_t t;

    for(i = (img->rows * img->cols) / 2; i > 0; i--)
    {
        t    = *s;
        *s++ = *d;
        *d-- = t;
    }
}

#ifdef CPU_MCXN947VDF_cm33
/*!
 * \brief Rotate the image 180 degrees (ARM Cortex implementation)
 *
 * This operation is also known as flipping in both horizontal and vertical
 * direction.
 *
 * \param[in,out] img
 */
void rotate180_arm(const image_t *img)
{
    // Hardware specific implementation

    // This target implementation uses inline assembly.
    // 32 bits are addressed at once, but the byte order needs to
    // be reversed (REV instruction) before writing.

    // Pointer to the first four pixels
    register uint32_t *first_ptr = (uint32_t *)img->data;

    // Pointer to the end of the data
    register uint32_t *last_ptr = (uint32_t *)(img->data + (img->rows * img->cols * sizeof(uint8_pixel_t)));

    // Temporary variables
    register uint32_t  first_pixels, last_pixels;

    while(first_ptr != last_ptr)
    {
        // Read pixels
        first_pixels = *first_ptr;
        last_pixels = *(--last_ptr);

        // Reverse 32-bit byte order : b3 b2 b1 b0 -> b0 b1 b2 b3
        __asm__ ("REV %[result], %[value]" : [result] "=r" (first_pixels) : [value] "r" (first_pixels));
        __asm__ ("REV %[result], %[value]" : [result] "=r" (last_pixels)  : [value] "r" (last_pixels));

        // When the Release target is selected, this idiom is recognized by the
        // compiler and the REV instruction will be implemented.
//        first_pixels = ((first_pixels >> 24) & 0x000000FF) |
//                       ((first_pixels >>  8) & 0x0000FF00) |
//                       ((first_pixels <<  8) & 0x00FF0000) |
//                       ((first_pixels << 24) & 0xFF000000);
//
//        last_pixels = ((last_pixels >> 24) & 0x000000FF) |
//                      ((last_pixels >>  8) & 0x0000FF00) |
//                      ((last_pixels <<  8) & 0x00FF0000) |
//                      ((last_pixels << 24) & 0xFF000000);

        *(first_ptr++) = last_pixels;
        *last_ptr      = first_pixels;
    }
}
#endif


/*!
 * \brief Warps an image
 *
 * The function takes 4 coordinates in the source image and takes 4 coordinates
 * in the destination image. These 8 coordinates are used to setup an 8x8
 * matrix system. The matrix coefficients are solved by using Gaussian
 * elimination. The calculated matrix coefficients are then used to setup a
 * transformation matrix and calculate the destination coordinates for each
 * pixel in the source image.
 * A temporary mask image is drawn to write only those pixels to the destination
 * that are inside the polygon.
 *
 * \param[in]  src       A pointer to the source image
 * \param[out] dst       A pointer to the destination image
 * \param[in]  from      A pointer to an array of 4 coordinates in the source
 *                       image
 * \param[in]  to        A pointer to an array of 4 coordinates in the
 *                       destination image
 */
void warpPerspective(const image_t *src, image_t *dst,
                     const point_t *from,  const point_t *to,
                     eTransformDirection d)
{
    // Verify image validity
    ASSERT(src == NULL, "src image is invalid");
    ASSERT(dst == NULL, "dst image is invalid");
    ASSERT(src->data == NULL, "src data is invalid");
    ASSERT(dst->data == NULL, "dst data is invalid");
    ASSERT(src->type != IMGTYPE_UINT8, "src type is invalid");
    ASSERT(dst->type != IMGTYPE_UINT8, "dst type is invalid");

    // Verify image consistency
    ASSERT(src == dst, "src and dst are the same images");

    // Verify parameters
    ASSERT(from == NULL, "invalid from values");
    ASSERT(to == NULL, "invalid to values");

    const int N = 9;

    // From "from" to "unit square"
    float A[N];

    float sx = from[0].x - from[1].x + from[2].x - from[3].x;
    float sy = from[0].y - from[1].y + from[2].y - from[3].y;

    // Is the from polygon a parallelogram?
    if((sx == 0) && (sy == 0))
    {
        // Yes, mapping is affine
        A[0] = from[1].x - from[0].x;
        A[1] = from[2].x - from[1].x;
        A[2] = from[0].x;
        A[3] = from[1].y - from[0].y;
        A[4] = from[2].y - from[1].y;
        A[5] = from[0].y;
        A[6] = 0;
        A[7] = 0;
        A[8] = 1;
    }
    else
    {
        // No, mapping is projective
        float dx1 = from[1].x - from[2].x;
        float dx2 = from[3].x - from[2].x;
        float dy1 = from[1].y - from[2].y;
        float dy2 = from[3].y - from[2].y;

        float g = ((sy * dx2) + (sx * (from[2].y - from[3].y))) /
                  ((dy1 * dx2) - (dy2 * dx1));
        float h = ((sy * (from[2].x - from[1].x)) + (sx * dy1)) /
                  ((dy1 * dx2) - (dy2 * dx1));

        A[0] = from[1].x - from[0].x + (g * from[1].x);
        A[1] = from[3].x - from[0].x + (h * from[3].x);
        A[2] = from[0].x;
        A[3] = from[1].y - from[0].y + (g * from[1].y);
        A[4] = from[3].y - from[0].y + (h * from[3].y);
        A[5] = from[0].y;
        A[6] = g;
        A[7] = h;
        A[8] = 1;
    }

    // From "unit square" to "to"
    float B[N];

    sx = to[0].x - to[1].x + to[2].x - to[3].x;
    sy = to[0].y - to[1].y + to[2].y - to[3].y;

    // Is the from polygon a parallelogram?
    if((sx == 0) && (sy == 0))
    {
        // Yes, mapping is affine
        B[0] = to[1].x - to[0].x;
        B[1] = to[2].x - to[1].x;
        B[2] = to[0].x;
        B[3] = to[1].y - to[0].y;
        B[4] = to[2].y - to[1].y;
        B[5] = to[0].y;
        B[6] = 0;
        B[7] = 0;
        B[8] = 1;
    }
    else
    {
        // No, mapping is projective
        float dx1 = to[1].x - to[2].x;
        float dx2 = to[3].x - to[2].x;
        float dy1 = to[1].y - to[2].y;
        float dy2 = to[3].y - to[2].y;

        float g = ((sy * dx2) + (sx * (to[2].y - to[3].y))) /
                  ((dy1 * dx2) - (dy2 * dx1));
        float h = ((sy * (to[2].x - to[1].x)) + (sx * dy1)) /
                  ((dy1 * dx2) - (dy2 * dx1));

        B[0] = to[1].x - to[0].x + (g * to[1].x);
        B[1] = to[3].x - to[0].x + (h * to[3].x);
        B[2] = to[0].x;
        B[3] = to[1].y - to[0].y + (g * to[1].y);
        B[4] = to[3].y - to[0].y + (h * to[3].y);
        B[5] = to[0].y;
        B[6] = g;
        B[7] = h;
        B[8] = 1;
    }

    // Draw mask image
    image_t *msk = newUint8Image(dst->cols, dst->rows);

    if(msk == NULL)
    {
        return;
    }

    clearUint8Image(msk);
    drawLineUint8(msk, to[0], to[1], 1);
    drawLineUint8(msk, to[1], to[2], 1);
    drawLineUint8(msk, to[2], to[3], 1);
    drawLineUint8(msk, to[3], to[0], 1);
    fillHolesIterative(msk, msk, CONNECTED_FOUR);

    if(d == TRANSFORM_FORWARD)
    {
        // Inverse AI = A^-1
        float factor = 1.0f/((A[0] * A[4]       ) -
                             (A[0] * A[5] * A[7]) -
                             (A[1] * A[3]       ) +
                             (A[1] * A[5] * A[6]) +
                             (A[2] * A[3] * A[7]) -
                             (A[2] * A[4] * A[6]));
        float AI[N];
        AI[0] = factor * ((A[4]       ) - (A[5] * A[7]));
        AI[1] = factor * ((A[2] * A[7]) - (A[1]       ));
        AI[2] = factor * ((A[1] * A[5]) - (A[2] * A[4]));
        AI[3] = factor * ((A[5] * A[6]) - (A[3]       ));
        AI[4] = factor * ((A[0]       ) - (A[2] * A[6]));
        AI[5] = factor * ((A[2] * A[3]) - (A[0] * A[5]));
        AI[6] = factor * ((A[3] * A[7]) - (A[4] * A[6]));
        AI[7] = factor * ((A[1] * A[6]) - (A[0] * A[7]));
        AI[8] = factor * ((A[0] * A[4]) - (A[1] * A[3]));

        // Multiply matrices: T = B * A^-1
        // {{B0,B1,B2},{B3,B4,B5},{B6,B7,B8}}*{{A0,A1,A2},{A3,A4,A5},{A6,A7,A8}}
        float T[9];
        T[0] = (B[0] * AI[0]) + (B[1] * AI[3]) + (B[2] * AI[6]);
        T[1] = (B[0] * AI[1]) + (B[1] * AI[4]) + (B[2] * AI[7]);
        T[2] = (B[0] * AI[2]) + (B[1] * AI[5]) + (B[2] * AI[8]);
        T[3] = (B[3] * AI[0]) + (B[4] * AI[3]) + (B[5] * AI[6]);
        T[4] = (B[3] * AI[1]) + (B[4] * AI[4]) + (B[5] * AI[7]);
        T[5] = (B[3] * AI[2]) + (B[4] * AI[5]) + (B[5] * AI[8]);
        T[6] = (B[6] * AI[0]) + (B[7] * AI[3]) + (B[8] * AI[6]);
        T[7] = (B[6] * AI[1]) + (B[7] * AI[4]) + (B[8] * AI[7]);
        T[8] = (B[6] * AI[2]) + (B[7] * AI[5]) + (B[8] * AI[8]);

        // Loop all pixels
        // If the output produces too many pixels that are not set, the step size
        // should be decreased
        float step_size = 0.5f;
        for(float y=0; y<src->rows; y+=step_size)
        {
            for(float x=0; x<src->cols; x+=step_size)
            {
                // Given the transformation matrix, calculate the new
                // coordinates
                int32_t xn = (((x * T[0]) + (y * T[1]) + T[2]) /
                              ((x * T[6]) + (y * T[7]) + T[8])) + 0.5f;
                int32_t yn = (((x * T[3]) + (y * T[4]) + T[5]) /
                              ((x * T[6]) + (y * T[7]) + T[8])) + 0.5f;

                if((xn >= 0) &&
                   (yn >= 0) &&
                   (xn <  dst->cols) &&
                   (yn <  dst->rows))
                {
                    if(1 == getUint8Pixel(msk, xn, yn))
                    {
                        // Forward transformation: for all coordinates in src, find the
                        // closest matching coordinate in dst
                        setUint8Pixel(dst, xn, yn, getUint8Pixel(src,x,y));
                    }
                }
            }
        }
    }
    else // TRANSFORM_BACKWARD
    {
        // Inverse BI = B^-1
        float factor = 1.0f/((B[0] * B[4]       ) -
                             (B[0] * B[5] * B[7]) -
                             (B[1] * B[3]       ) +
                             (B[1] * B[5] * B[6]) +
                             (B[2] * B[3] * B[7]) -
                             (B[2] * B[4] * B[6]));
        float BI[N];
        BI[0] = factor * ((B[4]       ) - (B[5] * B[7]));
        BI[1] = factor * ((B[2] * B[7]) - (B[1]       ));
        BI[2] = factor * ((B[1] * B[5]) - (B[2] * B[4]));
        BI[3] = factor * ((B[5] * B[6]) - (B[3]       ));
        BI[4] = factor * ((B[0]       ) - (B[2] * B[6]));
        BI[5] = factor * ((B[2] * B[3]) - (B[0] * B[5]));
        BI[6] = factor * ((B[3] * B[7]) - (B[4] * B[6]));
        BI[7] = factor * ((B[1] * B[6]) - (B[0] * B[7]));
        BI[8] = factor * ((B[0] * B[4]) - (B[1] * B[3]));

        // Multiply matrices: T = A * B^-1
        // {{A0,A1,A2},{A3,A4,A5},{A6,A7,A8}}*{{B0,B1,B2},{B3,B4,B5},{B6,B7,B8}}
        float TI[9];
        TI[0] = (A[0] * BI[0]) + (A[1] * BI[3]) + (A[2] * BI[6]);
        TI[1] = (A[0] * BI[1]) + (A[1] * BI[4]) + (A[2] * BI[7]);
        TI[2] = (A[0] * BI[2]) + (A[1] * BI[5]) + (A[2] * BI[8]);
        TI[3] = (A[3] * BI[0]) + (A[4] * BI[3]) + (A[5] * BI[6]);
        TI[4] = (A[3] * BI[1]) + (A[4] * BI[4]) + (A[5] * BI[7]);
        TI[5] = (A[3] * BI[2]) + (A[4] * BI[5]) + (A[5] * BI[8]);
        TI[6] = (A[6] * BI[0]) + (A[7] * BI[3]) + (A[8] * BI[6]);
        TI[7] = (A[6] * BI[1]) + (A[7] * BI[4]) + (A[8] * BI[7]);
        TI[8] = (A[6] * BI[2]) + (A[7] * BI[5]) + (A[8] * BI[8]);

        // Loop all pixels
        for(int32_t y=0; y<dst->rows; y++)
        {
            for(int32_t x=0; x<dst->cols; x++)
            {
                if(1 == getUint8Pixel(msk, x, y))
                {
                    // Given the transformation matrix, calculate the new
                    // coordinates
                    int32_t xs = (((x * TI[0]) + (y * TI[1]) + TI[2]) /
                                  ((x * TI[6]) + (y * TI[7]) + TI[8])) + 0.5f;
                    int32_t ys = (((x * TI[3]) + (y * TI[4]) + TI[5]) /
                                  ((x * TI[6]) + (y * TI[7]) + TI[8])) + 0.5f;

                    if((xs >= 0) &&
                       (ys >= 0) &&
                       (xs <  src->cols) &&
                       (ys <  src->rows))
                    {
                        // Backward transformation: for all coordinates in dst, find
                        // the closest matching coordinate in src
                        setUint8Pixel(dst, x, y, getUint8Pixel(src, xs, ys));
                    }
                }
            }
        }
    }

    deleteUint8Image(msk);
}

/*!
 * \brief Warps an image
 *
 * The function takes 4 coordinates in the source image and uses the entire
 * destination image. These 8 coordinates are used to setup an 8x8
 * matrix system. The matrix coefficients are solved by using Gaussian
 * elimination. The calculated matrix coefficients are then used to setup a
 * transformation matrix and calculate the destination coordinates for each
 * pixel in the source image.
 * As the entire destination image is used, there is no need to check if a point
 * lies within the polygon.
 *
 * \param[in]  src       A pointer to the source image
 * \param[out] dst       A pointer to the destination image
 * \param[in]  from      A pointer to an array of 4 coordinates in the source
 *                       image
 */
void warpPerspectiveFast(const image_t *src, image_t *dst,
                         const point_t *from, eTransformDirection d)
{
    // Verify image validity
    ASSERT(src == NULL, "src image is invalid");
    ASSERT(dst == NULL, "dst image is invalid");
    ASSERT(src->data == NULL, "src data is invalid");
    ASSERT(dst->data == NULL, "dst data is invalid");
    ASSERT(src->type != IMGTYPE_UINT8, "src type is invalid");
    ASSERT(dst->type != IMGTYPE_UINT8, "dst type is invalid");

    // Verify image consistency
    ASSERT(src == dst, "src and dst are the same images");

    // Verify parameters
    ASSERT(from == NULL, "invalid from values");

    const point_t to[4] =
    {
        {0, 0},
        {dst->cols - 1, 0},
        {dst->cols - 1, dst->rows - 1},
        {0, dst->rows - 1},
    };

    const int N = 9;

    // "From" to "unit square"
    float A[N];

    float sx = from[0].x - from[1].x + from[2].x - from[3].x;
    float sy = from[0].y - from[1].y + from[2].y - from[3].y;

    if((sx == 0) && (sy == 0))
    {
        A[0] = from[1].x - from[0].x;
        A[1] = from[2].x - from[1].x;
        A[2] = from[0].x;
        A[3] = from[1].y - from[0].y;
        A[4] = from[2].y - from[1].y;
        A[5] = from[0].y;
        A[6] = 0;
        A[7] = 0;
        A[8] = 1;
    }
    else
    {
        float dx1 = from[1].x - from[2].x;
        float dx2 = from[3].x - from[2].x;
        float dy1 = from[1].y - from[2].y;
        float dy2 = from[3].y - from[2].y;

        float g = ((sy * dx2) + (sx * (from[2].y - from[3].y))) /
                  ((dy1 * dx2) - (dy2 * dx1));
        float h = ((sy * (from[2].x - from[1].x)) + (sx * dy1)) /
                  ((dy1 * dx2) - (dy2 * dx1));

        A[0] = from[1].x - from[0].x + (g * from[1].x);
        A[1] = from[3].x - from[0].x + (h * from[3].x);
        A[2] = from[0].x;
        A[3] = from[1].y - from[0].y + (g * from[1].y);
        A[4] = from[3].y - from[0].y + (h * from[3].y);
        A[5] = from[0].y;
        A[6] = g;
        A[7] = h;
        A[8] = 1;
    }

    // "Unit square" to "to"
    float B[N];

    sx = to[0].x - to[1].x + to[2].x - to[3].x;
    sy = to[0].y - to[1].y + to[2].y - to[3].y;

    if((sx == 0) && (sy == 0))
    {
        B[0] = to[1].x - to[0].x;
        B[1] = to[2].x - to[1].x;
        B[2] = to[0].x;
        B[3] = to[1].y - to[0].y;
        B[4] = to[2].y - to[1].y;
        B[5] = to[0].y;
        B[6] = 0;
        B[7] = 0;
        B[8] = 1;
    }
    else
    {
        float dx1 = to[1].x - to[2].x;
        float dx2 = to[3].x - to[2].x;
        float dy1 = to[1].y - to[2].y;
        float dy2 = to[3].y - to[2].y;

        float g = ((sy * dx2) + (sx * (to[2].y - to[3].y))) /
                  ((dy1 * dx2) - (dy2 * dx1));
        float h = ((sy * (to[2].x - to[1].x)) + (sx * dy1)) /
                  ((dy1 * dx2) - (dy2 * dx1));

        B[0] = to[1].x - to[0].x + (g * to[1].x);
        B[1] = to[3].x - to[0].x + (h * to[3].x);
        B[2] = to[0].x;
        B[3] = to[1].y - to[0].y + (g * to[1].y);
        B[4] = to[3].y - to[0].y + (h * to[3].y);
        B[5] = to[0].y;
        B[6] = g;
        B[7] = h;
        B[8] = 1;
    }

    if(d == TRANSFORM_FORWARD)
    {
        // Inverse A^-1
        float factor = 1.0f/((A[0] * A[4]       ) -
                               (A[0] * A[5] * A[7]) -
                               (A[1] * A[3]       ) +
                               (A[1] * A[5] * A[6]) +
                               (A[2] * A[3] * A[7]) -
                               (A[2] * A[4] * A[6]));
        float AI[N];
        AI[0] = factor * ((A[4]       ) - (A[5] * A[7]));
        AI[1] = factor * ((A[2] * A[7]) - (A[1]       ));
        AI[2] = factor * ((A[1] * A[5]) - (A[2] * A[4]));
        AI[3] = factor * ((A[5] * A[6]) - (A[3]       ));
        AI[4] = factor * ((A[0]       ) - (A[2] * A[6]));
        AI[5] = factor * ((A[2] * A[3]) - (A[0] * A[5]));
        AI[6] = factor * ((A[3] * A[7]) - (A[4] * A[6]));
        AI[7] = factor * ((A[1] * A[6]) - (A[0] * A[7]));
        AI[8] = factor * ((A[0] * A[4]) - (A[1] * A[3]));

        // Combine both matrices
        float T[9];
        T[0] = (B[0] * AI[0]) + (B[1] * AI[3]) + (B[2] * AI[6]);
        T[1] = (B[0] * AI[1]) + (B[1] * AI[4]) + (B[2] * AI[7]);
        T[2] = (B[0] * AI[2]) + (B[1] * AI[5]) + (B[2] * AI[8]);
        T[3] = (B[3] * AI[0]) + (B[4] * AI[3]) + (B[5] * AI[6]);
        T[4] = (B[3] * AI[1]) + (B[4] * AI[4]) + (B[5] * AI[7]);
        T[5] = (B[3] * AI[2]) + (B[4] * AI[5]) + (B[5] * AI[8]);
        T[6] = (B[6] * AI[0]) + (B[7] * AI[3]) + (B[8] * AI[6]);
        T[7] = (B[6] * AI[1]) + (B[7] * AI[4]) + (B[8] * AI[7]);
        T[8] = (B[6] * AI[2]) + (B[7] * AI[5]) + (B[8] * AI[8]);

        // Loop all pixels
        // If the output produces too many pixels that are not set, the step size
        // should be decreased
        float step_size = 0.5f;
        for(float y=0; y<src->rows; y+=step_size)
        {
            for(float x=0; x<src->cols; x+=step_size)
            {
                // Given the transformation matrix, calculate the new
                // coordinates
                int32_t xn = (((x * T[0]) + (y * T[1]) + T[2]) /
                              ((x * T[6]) + (y * T[7]) + T[8])) + 0.5f;
                int32_t yn = (((x * T[3]) + (y * T[4]) + T[5]) /
                              ((x * T[6]) + (y * T[7]) + T[8])) + 0.5f;

                if((xn >= 0) &&
                   (yn >= 0) &&
                   (xn <  dst->cols) &&
                   (yn <  dst->rows))
                {
                    // Forward transformation: for all coordinates in src, find the
                    // closest matching coordinate in dst
                    setUint8Pixel(dst, xn, yn, getUint8Pixel(src,x,y));
                }
            }
        }
    }
    else
    {
        // Inverse B^-1
        float factor = 1.0f/((B[0] * B[4]       ) -
                               (B[0] * B[5] * B[7]) -
                               (B[1] * B[3]       ) +
                               (B[1] * B[5] * B[6]) +
                               (B[2] * B[3] * B[7]) -
                               (B[2] * B[4] * B[6]));
        float BI[N];
        BI[0] = factor * ((B[4]       ) - (B[5] * B[7]));
        BI[1] = factor * ((B[2] * B[7]) - (B[1]       ));
        BI[2] = factor * ((B[1] * B[5]) - (B[2] * B[4]));
        BI[3] = factor * ((B[5] * B[6]) - (B[3]       ));
        BI[4] = factor * ((B[0]       ) - (B[2] * B[6]));
        BI[5] = factor * ((B[2] * B[3]) - (B[0] * B[5]));
        BI[6] = factor * ((B[3] * B[7]) - (B[4] * B[6]));
        BI[7] = factor * ((B[1] * B[6]) - (B[0] * B[7]));
        BI[8] = factor * ((B[0] * B[4]) - (B[1] * B[3]));

        // Combine both matrices
        float TI[9];
        TI[0] = (A[0] * BI[0]) + (A[1] * BI[3]) + (A[2] * BI[6]);
        TI[1] = (A[0] * BI[1]) + (A[1] * BI[4]) + (A[2] * BI[7]);
        TI[2] = (A[0] * BI[2]) + (A[1] * BI[5]) + (A[2] * BI[8]);
        TI[3] = (A[3] * BI[0]) + (A[4] * BI[3]) + (A[5] * BI[6]);
        TI[4] = (A[3] * BI[1]) + (A[4] * BI[4]) + (A[5] * BI[7]);
        TI[5] = (A[3] * BI[2]) + (A[4] * BI[5]) + (A[5] * BI[8]);
        TI[6] = (A[6] * BI[0]) + (A[7] * BI[3]) + (A[8] * BI[6]);
        TI[7] = (A[6] * BI[1]) + (A[7] * BI[4]) + (A[8] * BI[7]);
        TI[8] = (A[6] * BI[2]) + (A[7] * BI[5]) + (A[8] * BI[8]);

        // Loop all pixels
        for(int32_t y=0; y<dst->rows; y++)
        {
            for(int32_t x=0; x<dst->cols; x++)
            {
                // Given the transformation matrix, calculate the new
                // coordinates
                int32_t xs = (((x * TI[0]) + (y * TI[1]) + TI[2]) /
                              ((x * TI[6]) + (y * TI[7]) + TI[8])) + 0.5f;
                int32_t ys = (((x * TI[3]) + (y * TI[4]) + TI[5]) /
                              ((x * TI[6]) + (y * TI[7]) + TI[8])) + 0.5f;

                if((xs >= 0) &&
                   (ys >= 0) &&
                   (xs <  src->cols) &&
                   (ys <  src->rows))
                {
                    // Backward transformation: for all coordinates in dst, find
                    // the closest matching coordinate in src
                    setUint8Pixel(dst, x, y, getUint8Pixel(src, xs, ys));
                }
            }
        }
    }
}

/*!
 * \brief Zooms an image with a factor 2
 *
 * The function zooms in or out a specific region of the source image.
 * The result is written into the destination image starting at coordinate
 * (0,0).
 *
 * \param[in]  src A pointer to the source image
 * \param[out] dst A pointer to the destination image
 * \param[in]  x   Top left x-value of the selected region
 * \param[in]  y   Top left y-value of the selected region
 * \param[in]  hor Number of horizontal pixels of the region
 * \param[in]  ver Number of vertical pixels of the region
 * \param[in]  zd  Zooming direction of type ::eZoom
 */
void zoom(const image_t *src, image_t *dst,
          const int32_t x, const int32_t y,
          const int32_t hor, const int32_t ver,
          const eZoom zd)
{
    // Verify image validity
    ASSERT(src == NULL, "src image is invalid");
    ASSERT(dst == NULL, "dst image is invalid");
    ASSERT(src->data == NULL, "src data is invalid");
    ASSERT(dst->data == NULL, "dst data is invalid");
    ASSERT(src->type != IMGTYPE_UINT8, "src type is invalid");
    ASSERT(dst->type != IMGTYPE_UINT8, "dst type is invalid");

    // Verify image consistency
    ASSERT(src == dst, "src and dst are the same images");

    // Verify parameters
    ASSERT(x + hor > src->cols, "zooming out of src cols range");
    ASSERT(y + ver > src->rows, "zooming out of src rows range");

    int32_t m=0, n=0;

    // Zoom
    if(zd == ZOOM_IN)
    {
        // Verify parameters
        ASSERT(dst->cols < hor*2, "the zoomed image will not fit in dst");
        ASSERT(dst->rows < ver*2, "the zoomed image will not fit in dst");

        // Loop all pixels
        for(int32_t i=y; i<ver; i++)
        {
            for(int32_t j=x; j<hor; j++)
            {
                uint8_pixel_t p = getUint8Pixel(src, j, i);

                setUint8Pixel(dst,   m,   n, p);
                setUint8Pixel(dst, m+1,   n, p);
                setUint8Pixel(dst,   m, n+1, p);
                setUint8Pixel(dst, m+1, n+1, p);

                m += 2;
            }

            n += 2;
            m = 0;
        }
    }
    else // Dezoom
    {
        // Verify parameters
        ASSERT(dst->cols < hor/2, "the zoomed image will not fit in dst");
        ASSERT(dst->rows < ver/2, "the zoomed image will not fit in dst");

        // Loop all pixels
        for(int32_t i=y; i<ver; i+=2)
        {
            for(int32_t j=x; j<hor; j+=2)
            {
                setUint8Pixel(dst, m, n, getUint8Pixel(src, j, i));
                ++m;
            }

            ++n;
            m = 0;
        }
    }
}

/*!
 * \brief Zooms an image with an adjustable factor
 *
 * The function zooms in or out a specific region in the source image.
 * The result is written into the destination image starting at coordinate
 * (0,0). The zooming factor is set by \p factor.
 *
 * \param[in]  src    A pointer to the source image
 * \param[out] dst    A pointer to the destination image
 * \param[in]  x      Top left x-value of the selected region
 * \param[in]  y      Top left y-value of the selected region
 * \param[in]  hor    Number of horizontal pixels of the region
 * \param[in]  ver    Number of vertical pixels of the region
 * \param[in]  zd     Zooming direction of type ::eZoom
 * \param[in]  factor Zooming factor
 */
void zoomFactor(const image_t *src, image_t *dst,
          const int32_t x, const int32_t y,
          const int32_t hor, const int32_t ver,
          const eZoom zd, const int16_t factor)
{
    int32_t m=0, n=0;

    // Zoom
    if(zd == ZOOM_IN)
    {
      //ASSERT(dst->cols < hor*2, "the zoomed image does not fit in dst");
      //ASSERT(dst->rows < ver*2, "the zoomed image does not fit in dst");

        // Loop all pixels
        for(int32_t i=y; i<ver; i++)
        {
            for(int32_t j=x; j<hor; j++)
            {
                uint8_pixel_t p = getUint8Pixel(src, j, i);

                for(int32_t k=0; k<factor; k++)
                {
                    for(int32_t l=0; l<factor; l++)
                    {
                        if((m+k >= 0) &&
                           (n+l >= 0) &&
                           (m+k <  dst->cols) &&
                           (n+l <  dst->rows))
                        {
                            setUint8Pixel(dst, m+k, n+l, p);
                        }
                    }
                }

                m += factor;
            }

            n += factor;
            m = 0;
        }
    }
    else // Dezoom
    {
      //ASSERT(dst->cols < hor/factor, "the zoomed image does not fit in dst");
      //ASSERT(dst->rows < ver/factor, "the zoomed image does not fit in dst");

        // Loop all pixels
        for(int32_t i=y; i<ver; i+=factor)
        {
            for(int32_t j=x; j<hor; j+=factor)
            {
                if((m >= 0) &&
                   (n >= 0) &&
                   (m <  dst->cols) &&
                   (n <  dst->rows))
                {
                    setUint8Pixel(dst, m, n, getUint8Pixel(src, j, i));
                }

                ++m;
            }

            ++n;
            m = 0;
        }
    }
}
