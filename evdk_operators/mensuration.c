/*! ***************************************************************************
 *
 * \brief     Evaluation of features associated with objects in an image
 * \file      mensuration.c
 * \author    Hugo Arends - HAN Embedded Vision and Machine Learning
 * \author
 * \date      October 2024
 *
 * \see       Myler, H. R., & Weeks, A. R. (2009). The pocket handbook of
 *            image processing algorithms in C. Prentice Hall Press.
 * \see       Benkrid, K., Crookes, D., & Benkrid, A. (2000, September).
 *            Design and FPGA implementation of a perimeter estimator.
 *            In Proceedings of the Irish Machine Vision and Image Processing
 *            Conference (pp. 51-57).
 * \see       Gonzalez, R. (). 11.3.4 Moment Invariants. In Digital Image
 *            Processing. pp. 839-842. New Jersey: Pearson Prentice Hall.
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
#include "mensuration.h"
#include "morphological_filters.h"
#include "operators.h"

#include <math.h>
#include <string.h>

// Local function prototypes
float ncm(const image_t *img, const uint8_t blobnr,
          const int32_t p, const int32_t q);

uint8_pixel_t lowestNeighbour(const image_t *img, const int32_t x,
                              const int32_t y, const eConnected c);

/*!
 * \brief Calculates the area of a BLOB in \p img by counting the number of
 *        pixels.
 *
 * The result is written into the BLOB info structure.
 *
 * \param[in]  img      A pointer to a binary or a labelled image
 * \param[out] blobinfo A pointer to a BLOB info structure
 * \param[in]  blobnr   \li In a binary image: must be 1
 *                      \li In a labelled image: the number of the BLOB of
 *                          interest
 */
void area(const image_t *img, blobinfo_t *blobinfo, const uint32_t blobnr)
{
    // Verify image validity
    ASSERT(img == NULL, "img image is invalid");
    ASSERT(img->data == NULL, "img data is invalid");

    uint32_t imsize = img->rows * img->cols;
    uint8_pixel_t *p = (uint8_pixel_t *)img->data;
    uint32_t cnt = 0;

    // Scan input image for blobnr
    for(uint32_t i=0; i<imsize; ++i)
    {
        if(*p++ == blobnr)
        {
            ++cnt;
        }
    }

    // Set the result
    blobinfo->area = cnt;
}

/*!
 * \brief Measures the geographic centre of an object
 *
 * Also known as the first central moment
 *
 * \param[in]  img      A pointer to a binary or a labelled image
 * \param[out] blobinfo A pointer to a BLOB info structure
 * \param[in]  blobnr   \li In a binary image: must be 1
 *                      \li In a labelled image: the number of the BLOB of 
 *                          interest
 */
void centroid(const image_t *img, blobinfo_t *blobinfo, const uint32_t blobnr)
{
    // Verify image validity
    ASSERT(img == NULL, "img image is invalid");
    ASSERT(img->data == NULL, "img data is invalid");

    uint32_t cnt = 0;
    uint32_t xc = 0.0f;
    uint32_t yc = 0.0f;

    // Scan input image for blobnr
    // Pass through from left top to right bottom
    for(int32_t y = 0; y < img->rows; y++)
    {
        for(int32_t x = 0; x < img->cols; x++)
        {
            uint8_pixel_t p = getUint8Pixel(img,x,y);

            if(p == blobnr)
            {
                ++cnt;
                xc += x;
                yc += y;
            }
        }
    }

    // Set the result
    blobinfo->area = cnt;
    if(cnt == 0)
    {
        blobinfo->centroid.x = -1;
        blobinfo->centroid.y = -1;
    }
    else
    {
        blobinfo->centroid.x = (int32_t)((xc / cnt) + 1);
        blobinfo->centroid.y = (int32_t)((yc / cnt) + 1);
    }
}

/*!
 * \brief Counts and labels all BLOBs
 *
 * A BLOB is a Binary Linked Object and it’s pixels are either 4-connected or
 * 8-connected. Labelling is performed in ascending order from left-top to
 * right-bottom using an iterative algorithm. The algorithm scans the entire
 * image several times. The number of iterations depends on the object shapes.
 *
 * \param[in]  src       A pointer to the source image
 * \param[out] dst       A pointer to the destination image
 * \param[in]  connected The connectivity to determine how labels are
 *                       connected. Must be of type ::eConnected.
 *
 * \return The number of unique labels in the image
 */
uint32_t labelIterative(const image_t *src, image_t *dst, const eConnected connected)
{
    // Verify image validity
    ASSERT(src == NULL, "src image is invalid");
    ASSERT(dst == NULL, "dst image is invalid");
    ASSERT(src->data == NULL, "src data is invalid");
    ASSERT(dst->data == NULL, "dst data is invalid");
    ASSERT(src->type != IMGTYPE_UINT8, "src type is invalid");
    ASSERT(dst->type != IMGTYPE_UINT8, "dst type is invalid");

    register int32_t blobcnt = 1;
    register uint32_t changed = 0;

    image_t * cnt = newInt32Image(src->cols, src->rows);

    if(cnt == NULL)
    {
        return 0;
    }

    // Assign numbers in ascending order from left-top to right-bottom
    for(int32_t r = 0; r < src->rows; r++)
    {
        for(int32_t c = 0; c < src->cols; c++)
        {
            uint8_pixel_t pixel = getUint8Pixel(src, c, r);
            setInt32Pixel(cnt, c, r, (pixel == 1) ? blobcnt++ : 0);
        }
    }

    do
    {
        changed = 0;

        // Pass through from left top to right bottom
        for(int32_t r = 0; r < cnt->rows; r++)
        {
            for(int32_t c = 0; c < cnt->cols; c++)
            {
                int32_pixel_t pixel = getInt32Pixel(cnt,c,r);

                // Is this an object pixel?
                if(pixel != 0)
                {
                    // +-+-+-+
                    // | |N| |
                    // +-+-+-+
                    // | |P| |
                    // +-+-+-+
                    // | | | |
                    // +-+-+-+
                    if(r > 0)
                    {
                        int32_pixel_t neighbour = getInt32Pixel(cnt,(c),(r-1));

                        if((neighbour != 0) && (neighbour < pixel))
                        {
                            setInt32Pixel(cnt,c,r,neighbour);
                            changed = 1;
                        }
                    }

                    // +-+-+-+
                    // | | | |
                    // +-+-+-+
                    // |N|P| |
                    // +-+-+-+
                    // | | | |
                    // +-+-+-+
                    if(c > 0)
                    {
                        int32_pixel_t neighbour = getInt32Pixel(cnt,(c-1),(r));

                        if((neighbour != 0) && (neighbour < pixel))
                        {
                            setInt32Pixel(cnt,c,r,neighbour);
                            changed = 1;
                        }
                    }


                    // +-+-+-+
                    // | | | |
                    // +-+-+-+
                    // | |P|N|
                    // +-+-+-+
                    // | | | |
                    // +-+-+-+
                    if(c < (cnt->cols-1))
                    {
                        int32_pixel_t neighbour = getInt32Pixel(cnt,(c+1),(r));

                        if((neighbour != 0) && (neighbour < pixel))
                        {
                            setInt32Pixel(cnt,c,r,neighbour);
                            changed = 1;
                        }
                    }

                    // +-+-+-+
                    // | | | |
                    // +-+-+-+
                    // | |P| |
                    // +-+-+-+
                    // | |N| |
                    // +-+-+-+
                    if(r < (cnt->rows-1))
                    {
                        int32_pixel_t neighbour = getInt32Pixel(cnt,(c),(r+1));

                        if((neighbour != 0) && (neighbour < pixel))
                        {
                            setInt32Pixel(cnt,c,r,neighbour);
                            changed = 1;
                        }
                    }

                    if(connected == CONNECTED_EIGHT)
                    {
                        // +-+-+-+
                        // |N| | |
                        // +-+-+-+
                        // | |P| |
                        // +-+-+-+
                        // | | | |
                        // +-+-+-+
                        if((c > 0) && (r > 0))
                        {
                            int32_pixel_t neighbour = getInt32Pixel(cnt,(c-1),(r-1));

                            if((neighbour != 0) && (neighbour < pixel))
                            {
                                setInt32Pixel(cnt,c,r,neighbour);
                                changed = 1;
                            }
                        }

                        // +-+-+-+
                        // | | |N|
                        // +-+-+-+
                        // | |P| |
                        // +-+-+-+
                        // | | | |
                        // +-+-+-+
                        if((c < (cnt->cols - 1)) && (r > 0))
                        {
                            int32_pixel_t neighbour = getInt32Pixel(cnt,(c+1),(r-1));

                            if((neighbour != 0) && (neighbour < pixel))
                            {
                                setInt32Pixel(cnt,c,r,neighbour);
                                changed = 1;
                            }
                        }

                        // +-+-+-+
                        // | | | |
                        // +-+-+-+
                        // | |P| |
                        // +-+-+-+
                        // |N| | |
                        // +-+-+-+
                        if((c > 0) && (r < (cnt->rows - 1)))
                        {
                            int32_pixel_t neighbour = getInt32Pixel(cnt,(c-1),(r+1));

                            if((neighbour != 0) && (neighbour < pixel))
                            {
                                setInt32Pixel(cnt,c,r,neighbour);
                                changed = 1;
                            }
                        }

                        // +-+-+-+
                        // | | | |
                        // +-+-+-+
                        // | |P| |
                        // +-+-+-+
                        // | | |N|
                        // +-+-+-+
                        if((c < (cnt->cols - 1)) && (r < (cnt->rows - 1)))
                        {
                            int32_pixel_t neighbour = getInt32Pixel(cnt,(c+1),(r+1));

                            if((neighbour != 0) && (neighbour < pixel))
                            {
                                setInt32Pixel(cnt,c,r,neighbour);
                                changed = 1;
                            }
                        }
                    }
                }
            }
        }
    }while(changed == 1);

    // Assign labels in the correct order
    blobcnt = 1;

    // Pass through from left top to right bottom
    for(int32_t r = 0; r < cnt->rows; r++)
    {
        for(int32_t c = 0; c < cnt->cols; c++)
        {
            int32_pixel_t pixel = getInt32Pixel(cnt,c,r);

            if((pixel != 0) && (pixel >= blobcnt))
            {
                // Set selected to value
                for(int32_t rr = 0; rr < cnt->rows; rr++)
                {
                    for(int32_t cc = 0; cc < cnt->cols; cc++)
                    {
                        if(getInt32Pixel(cnt,cc,rr) == pixel)
                        {
                            setInt32Pixel(cnt,cc,rr, blobcnt);
                        }
                    }
                }

                blobcnt++;
            }
        }
    }

    // Copy result
    for(int32_t r = 0; r < src->rows; r++)
    {
        for(int32_t c = 0; c < src->cols; c++)
        {
            int32_pixel_t pixel = getInt32Pixel(cnt, c, r);
            setUint8Pixel(dst, c, r, (uint8_pixel_t)(pixel % 255));
        }
    }

    // Cleanup
    deleteInt32Image(cnt);

    return(blobcnt-1);
}

/*!
 * \brief Counts and labels all BLOBs
 *
 * A BLOB is a Binary Linked Object and it’s pixels are either 4-connected or
 * 8-connected. Labelling is performed in ascending order from left-top to
 * right-bottom using an iterative algorithm. The algorithm scans the entire
 * image two times and keeps track of label equivalence in a lookup table.
 *
 * \param[in]  src       A pointer to the source image
 * \param[out] dst       A pointer to the destination image
 * \param[in]  connected The connectivity to determine how labels are
 *                       connected. Must be of type ::eConnected.
 * \param[in]  lutSize   The maximum number of labels to find. Is used to
 *                       dynamically allocate memory for the lookup table. A
 *                       lower value is faster, but is also able to find fewer
 *                       labels. The value is application dependent.
 *
 * \return The number of unique labels in the image
 *         Returns 0 if
 *         \li No unique labels in the image
 *         \li Memory allocation failed
 *         \li Lookup table is too small
 *
 * \todo Implement this function
 */
uint32_t labelTwoPass(const image_t *src, image_t *dst,
                      const eConnected connected, const uint32_t lutSize) {
    uint32_t width = src->cols, height = src->rows;
    uint32_t pixels = width * height;
    // Allocate and initialize LUT
    uint32_t *lut = (uint32_t *)malloc(lutSize * sizeof(uint32_t));
    if (!lut) {
        return 0; // Memory allocation failed
    }
    memset(lut, 0, lutSize * sizeof(uint32_t));
    uint32_t nextLabel = 1; // Start labeling from 1

    // First Pass: Assign initial labels and record equivalences
    for (uint32_t y = 1; y < height - 1; y++) { // Skip border pixels
        for (uint32_t x = 1; x < width - 1; x++) {
            if (src->data[y * width + x] == 1) { // Foreground pixel
                uint32_t neighbors[4] = {0};
                if(connected == CONNECTED_FOUR){
                    neighbors[0] = dst->data[(y - 1) * width + x];     // Top
                    neighbors[1] = dst->data[y * width + (x - 1)];     // Left
                }
                else {
                    neighbors[0] = dst->data[(y - 1) * width + x];     // Top
                    neighbors[1] = dst->data[y * width + (x - 1)];     // Left
                    neighbors[2] = dst->data[(y - 1) * width + (x - 1)]; // Top-left
                    neighbors[3] = dst->data[(y - 1) * width + (x + 1)]; // Top-right
                }

                uint32_t minLabel = UINT32_MAX;
                for (uint32_t i = 0; i < (connected == CONNECTED_FOUR ? 2 : 4); i++) {
                    if (neighbors[i] > 0) {
                        // Find root label for this neighbor
                        uint32_t root = neighbors[i];
                        while (lut[root] != 0 && lut[root] != root) {
                            root = lut[root];
                        }
                        if (root < minLabel) {
                            minLabel = root;
                        }
                    }
                }

                if (minLabel == UINT32_MAX) {
                    dst->data[y * width + x] = nextLabel;
                    lut[nextLabel] = nextLabel; // New label points to itself
                    nextLabel++;
                } else {
                    dst->data[y * width + x] = minLabel;
                    // Update equivalences for all neighbors
                    for (uint32_t i = 0; i < (connected == CONNECTED_FOUR ? 2 : 4); i++) {
                        if (neighbors[i] > 0) {
                            uint32_t root = neighbors[i];
                            while (lut[root] != 0 && lut[root] != root) {
                                root = lut[root];
                            }
                            if (root != minLabel) {
                                lut[root] = minLabel;
                            }
                        }
                    }
                }
            }
            else {
                dst->data[y * width + x] = 0;
            }
        }
    }

    // Flatten the LUT and remap labels to be consecutive
    uint32_t *newLabels = (uint32_t *)calloc(nextLabel, sizeof(uint32_t));
    if (!newLabels) {
        free(lut);
        return 0; // Memory allocation failed
    }
    uint32_t currentLabel = 1;

    // First flatten
    for (uint32_t i = 1; i < nextLabel; i++) {
        uint32_t root = i;
        while (lut[root] != 0 && lut[root] != root) {
            root = lut[root];
        }
        lut[i] = root;
    }

    // Create consecutive mapping and count unique labels
    uint32_t uniqueLabels = 0;
    for (uint32_t i = 1; i < nextLabel; i++) {
        if (lut[i] != 0) {
            if (newLabels[lut[i]] == 0) {
                newLabels[lut[i]] = currentLabel++;
                uniqueLabels++;
            }
            lut[i] = newLabels[lut[i]];
        }
    }

    // Second Pass: Relabel image using flattened LUT
    for (uint32_t i = 0; i < pixels; i++) {
        if (dst->data[i] > 0) {
            dst->data[i] = lut[dst->data[i]];
        }
    }

    // Cleanup
    free(lut);
    free(newLabels);

    // Return number of unique labels (or 0 if none found)
    return uniqueLabels > 0 ? uniqueLabels : 0;
}

/*!
 * \brief Calculates the circularity of the blob
 *
 * The circularity of a blob is defined as 4 * pi * (area / (perimeter^2))
 * The function calculates the area and/or perimeter first if either of these
 * fields in the blobinfo is zero.
 *
 * \param[in]  img      A pointer to a binary or a labelled image
 * \param[out] blobinfo A pointer to a BLOB info structure
 * \param[in]  blobnr   \li In a binary image: must be 1
 *                      \li In a labelled image: the number of the BLOB of
 *                          interest
 */
void circularity(const image_t *img, blobinfo_t *blobinfo, const uint32_t blobnr)
{
    if(blobinfo->area == 0)
    {
        area(img, blobinfo, blobnr);
    }

    if(blobinfo->perimeter == 0)
    {
        perimeter(img, blobinfo, blobnr);
    }

    blobinfo->circularity = 4 * 3.14159f *
                            (blobinfo->area / (blobinfo->perimeter * blobinfo->perimeter));
}

/*!
 * \brief Calculates Hu invariant moments 1 to 4.
 *
 * \see Gonzalez, R. (). 11.3.4 Moment Invariants. In Digital Image
 *      Processing. pp. 839-842. New Jersey: Pearson Prentice Hall.
 *
 * The function calls the function ncm() for calculating the normalized central
 * moments.
 *
 * \param[in]  img      A pointer to a binary or a labelled image
 * \param[out] blobinfo A pointer to a BLOB info structure
 * \param[in]  blobnr   \li In a binary image: must be 1
 *                      \li In a labelled image: the number of the BLOB of 
 *                          interest
 */
void huInvariantMoments(const image_t *img, blobinfo_t *blobinfo, const uint32_t blobnr)
{
    // Verify image validity
    ASSERT(img == NULL, "img image is invalid");
    ASSERT(img->data == NULL, "img data is invalid");
    ASSERT(img->type != IMGTYPE_UINT8, "img type is invalid");

    // Verify BLOB info validity
    ASSERT(blobinfo == NULL, "blobinfo is invalid");

    // Calculate normalized central moments
    float ncm_20 = ncm(img,blobnr,2,0);
    float ncm_02 = ncm(img,blobnr,0,2);
    float ncm_11 = ncm(img,blobnr,1,1);
    float ncm_30 = ncm(img,blobnr,3,0);
    float ncm_12 = ncm(img,blobnr,1,2);
    float ncm_21 = ncm(img,blobnr,2,1);
    float ncm_03 = ncm(img,blobnr,0,3);

    // Calculate the Hu invariant moments
    blobinfo->hu_moments[0] = ncm_20 + ncm_02;
    blobinfo->hu_moments[1] = powf(ncm_20 - ncm_02, 2) + (4 * powf(ncm_11, 2));
    blobinfo->hu_moments[2] = powf(ncm_30 - (3*ncm_12), 2) + powf((3*ncm_21) - ncm_03, 2);
    blobinfo->hu_moments[3] = powf(ncm_30 + ncm_12, 2) + powf(ncm_21 + ncm_03, 2);

    // Add more Hu invariant moments here if required by the application
}

/*!
 * \brief Calculates the two dimensional normalized central moments specified
 *        by \p p and \p q
 *
 * Two dimensional moments are used to describe the distributions of pixels.
 * The function takes advantage of several symmetry properties of the central
 * moments.
 *
 * \param[in] img    A pointer to the image
 * \param[in] blobnr \li In a binary image: must be 1
 *                   \li In a labelled image: the number of the BLOB of interest
 * \param[in] p      The moment in the horizontal direction
 * \param[in] q      The moment in the vertical direction
 *
 * \return The calculated normalized central moment
 */
float ncm(const image_t *img, const uint8_t blobnr, const int32_t p, const int32_t q)
{
    // Verify image validity
    ASSERT(img == NULL, "img image is invalid");
    ASSERT(img->data == NULL, "img data is invalid");
    ASSERT(img->type != IMGTYPE_UINT8, "img type is invalid");

    if((p==0) && (q==0))
    {
        return(1.0f);
    }

    if(((p==1) && (q==0)) || ((p==0) && (q==1)))
    {
        return(0.0f);
    }

    register int32_t c, r;
    float cc, rc;

    float m00 = 0.0f;
    float m10 = 0.0f;
    float m01 = 0.0f;
    float upq = 0.0f;

    // Calculate moments m00, m10 and m01
    for(r = (img->rows-1); r >= 0; r--)
    {
        for(c = (img->cols-1); c >= 0; c--)
            if(getUint8Pixel(img,c,r) == blobnr)
            {
                m00 += 1.0f;
                m10 += c;
                m01 += r;
            }
    }

    // Calculate centroids
    cc = m10/m00;
    rc = m01/m00;

    // Calculate central moments ...
    // ... when p=0
    if(p==0)
    {
        for(r = (img->rows-1); r >= 0; r--)
        {
            for(c = (img->cols-1); c >= 0; c--)
            {
                if(getUint8Pixel(img,c,r) == blobnr)
                {
                    upq += powf((float)(r-rc),(float)q);
                }
            }
        }
    }

    // ... when q=0
    else if(q==0)
    {
        for(r = (img->rows-1); r >= 0; r--)
        {
            for(c = (img->cols-1); c >= 0; c--)
            {
                if(getUint8Pixel(img,c,r) == blobnr)
                {
                    upq += powf((float)(c-cc),(float)p);
                }
            }
        }
    }
    else
    {
        // ... or when p!=0 and q!=0
        for(r = (img->rows-1); r >= 0; r--)
        {
            for(c = (img->cols-1); c >= 0; c--)
            {
                if(getUint8Pixel(img,c,r) == blobnr)
                {
                    upq += powf((float)(c-cc),(float)p) * 
                           powf((float)(r-rc),(float)q);
                }
            }
        }
    }

    // Return normalized central moment
    return(upq / powf(m00,((((float)(p+q))/2.0f)+1.0f)));
}

/*!
 * \brief Estimates the perimeter of a BLOB
 *
 * The value 1 is equal to the length of a single pixel.
 * The perimeter calculation is based on the following work:
 * \see Benkrid, K., Crookes, D., & Benkrid, A. (2000, September).
 *      Design and FPGA implementation of a perimeter estimator.
 *      In Proceedings of the Irish Machine Vision and Image Processing
 *      Conference (pp. 51-57).
 * Changes have however been made to the contribution of horizontal or vertical
 * links combined with a diagonal link. Instead of adding the value
 * 1/2+1/2*sqrt(2), the value 1/2*sqrt(5) is added.
 *
 * \param[in]  img      A pointer to a binary or a labelled image
 * \param[out] blobinfo A pointer to a BLOB info structure
 * \param[in]  blobnr   \li In a binary image: must be 1
 *                      \li In a labelled image: the number of the BLOB of
 *                          interest
 *
 * \todo Implement this function
 */
void perimeter(const image_t *img, blobinfo_t *blobinfo, const uint32_t blobnr)
{
    image_t *eroded = newUint8Image(img->cols, img->rows);
    image_t *thr = newUint8Image(img->cols, img->rows);

    uint8_pixel_t msk[3 * 3] =
        {
            0, 1, 0,
            1, 1, 1,
            0, 1, 0,
        };

    threshold(img, thr, blobnr, blobnr);
    erosion(thr, eroded, msk, 3);

    //Outline to make
    float perimeter = 0.0f; /**< Variable to accumulate the perimeter value. */

    // Iterate over all pixels of the image, excluding the borders
    for (int32_t row = 1; row < img->rows - 1; row++) {
        for (int32_t col = 1; col < img->cols - 1; col++) {
            // Check if this pixel belongs to the specified blob number
            if ((img->data[row * img->cols + col] == blobnr)&&(eroded->data[row * eroded->cols + col] == 0)) {
                int32_t pixel_value = 1; // Start with 1 for the center pixel

                // Check the 4 orthogonal neighbors (top, bottom, left, right)
                pixel_value += (img->data[(row - 1) * img->cols + col] == blobnr) && (eroded->data[(row - 1) * eroded->cols + col] == 0) ? 2 : 0; // top
                pixel_value += (img->data[(row + 1) * img->cols + col] == blobnr) && (eroded->data[(row + 1) * eroded->cols + col] == 0) ? 2 : 0; // bottom
                pixel_value += (img->data[row * img->cols + (col - 1)] == blobnr) && (eroded->data[row * eroded->cols + (col - 1)] == 0) ? 2 : 0; // left
                pixel_value += (img->data[row * img->cols + (col + 1)] == blobnr) && (eroded->data[row * eroded->cols + (col + 1)] == 0) ? 2 : 0; // right

                // Check the 4 diagonal neighbors (top-left, top-right, bottom-left, bottom-right)
                pixel_value += (img->data[(row - 1) * img->cols + (col - 1)] == blobnr) && (eroded->data[(row - 1) * eroded->cols + (col - 1)] == 0) ? 10 : 0; // top left
                pixel_value += (img->data[(row - 1) * img->cols + (col + 1)] == blobnr) && (eroded->data[(row - 1) * eroded->cols + (col + 1)] == 0) ? 10 : 0; // top right
                pixel_value += (img->data[(row + 1) * img->cols + (col - 1)] == blobnr) && (eroded->data[(row + 1) * eroded->cols + (col - 1)] == 0) ? 10 : 0; // bottom left
                pixel_value += (img->data[(row + 1) * img->cols + (col + 1)] == blobnr) && (eroded->data[(row + 1) * eroded->cols + (col + 1)] == 0) ? 10 : 0; // bottom right

                // Determine the perimeter contribution based on the sum of the neighboring pixels
                if (pixel_value == 5 || pixel_value == 15 || pixel_value == 7 || pixel_value == 25|| pixel_value == 27|| pixel_value == 17) {
                    perimeter += 1.0f; // Horizontal/vertical link
                } else if (pixel_value == 21 || pixel_value == 33) {
                    perimeter += 1.4142f; // Diagonal link
                } else if (pixel_value == 13 || pixel_value == 23) {
                    perimeter += 1.1180f;
                }
            }
        }
    }

    // Store the result in blobinfo
    blobinfo->perimeter = perimeter;
    deleteUint8Image(eroded);
    deleteUint8Image(thr);
}


/*!
 * \brief Counts the number of pixels in the \p c connected neighbourhood that
 *        have value \p p
 *
 * \param[in] img A pointer to the image
 * \param[in] x   x-value coordinate
 * \param[in] y   y-value coordinate
 * \param[in] c   Neighbourhood connectivity defined by ::eConnected
 *
 * \return The number of neighbour pixels with value \p p
 */
uint8_pixel_t lowestNeighbour(const image_t *img, const int32_t x,
                              const int32_t y, const eConnected c)
{
    // Verify image validity
    ASSERT(img == NULL, "src image is invalid");
    ASSERT(img->data == NULL, "src data is invalid");
    ASSERT(img->type != IMGTYPE_UINT8, "src type is invalid");

    // Verify image consistency
    ASSERT(x < 0, "x-value is out of range");
    ASSERT(x >= img->cols, "x-value is out of range");
    ASSERT(y < 0, "y-value is out of range");
    ASSERT(y >= img->rows, "y-value is out of range");

    register uint8_pixel_t *s = (uint8_pixel_t *)(img->data + (y * img->cols + x));

    uint8_pixel_t val = UINT8_PIXEL_MAX;
    uint8_pixel_t pixel;

    // Left-top pixel
    if(x == 0 && y == 0)
    {
        pixel = *(s + sizeof(uint8_pixel_t)); // right
        if((pixel < val) && (pixel > 1)){ val = pixel; }
        pixel = *(s + (img->cols * sizeof(uint8_pixel_t))); // down
        if((pixel < val) && (pixel > 1)){ val = pixel; }

        if(c == CONNECTED_EIGHT)
        {
            pixel = *(s + (img->cols * sizeof(uint8_pixel_t)) + 1); // down-right
            if((pixel < val) && (pixel > 1)){ val = pixel; }
        }
    }
    // Right-top pixel
    else if(x == img->cols-1 && y == 0)
    {
        pixel = *(s - sizeof(uint8_pixel_t)); // left
        if((pixel < val) && (pixel > 1)){ val = pixel; }
        pixel = *(s + (img->cols * sizeof(uint8_pixel_t))); // down
        if((pixel < val) && (pixel > 1)){ val = pixel; }

        if(c == CONNECTED_EIGHT)
        {
            pixel = *(s + (img->cols * sizeof(uint8_pixel_t)) - 1); // down-left
            if((pixel < val) && (pixel > 1)){ val = pixel; }
        }
    }
    // Left-bottom pixel
    else if(x == 0 && y == img->rows-1)
    {
        pixel = *(s - (img->cols * sizeof(uint8_pixel_t))); // up
        if((pixel < val) && (pixel > 1)){ val = pixel; }
        pixel = *(s + sizeof(uint8_pixel_t)); // right
        if((pixel < val) && (pixel > 1)){ val = pixel; }

        if(c == CONNECTED_EIGHT)
        {
            pixel = *(s - (img->cols * sizeof(uint8_pixel_t)) + 1); // up-right
            if((pixel < val) && (pixel > 1)){ val = pixel; }
        }

    }
    // Right-bottom pixel
    else if(x == img->cols-1 && y == img->rows-1)
    {
        pixel = *(s - (img->cols * sizeof(uint8_pixel_t))); // up
        if((pixel < val) && (pixel > 1)){ val = pixel; }
        pixel = *(s - sizeof(uint8_pixel_t)); // left
        if((pixel < val) && (pixel > 1)){ val = pixel; }

        if(c == CONNECTED_EIGHT)
        {
            pixel = *(s - (img->cols * sizeof(uint8_pixel_t)) - 1); // up-left
            if((pixel < val) && (pixel > 1)){ val = pixel; }
        }
    }
    // Top border pixels
    else if(y == 0)
    {
        pixel = *(s - sizeof(uint8_pixel_t)); // left
        if((pixel < val) && (pixel > 1)){ val = pixel; }
        pixel = *(s + sizeof(uint8_pixel_t)); // right
        if((pixel < val) && (pixel > 1)){ val = pixel; }
        pixel = *(s + (img->cols * sizeof(uint8_pixel_t))); // down
        if((pixel < val) && (pixel > 1)){ val = pixel; }

        if(c == CONNECTED_EIGHT)
        {
            pixel = *(s + (img->cols * sizeof(uint8_pixel_t)) - 1); // down-left
            if((pixel < val) && (pixel > 1)){ val = pixel; }
            pixel = *(s + (img->cols * sizeof(uint8_pixel_t)) + 1); // down-right
            if((pixel < val) && (pixel > 1)){ val = pixel; }
        }
    }
    // Bottom border pixels
    else if(y == img->rows-1)
    {
        pixel = *(s - (img->cols * sizeof(uint8_pixel_t))); // up
        if((pixel < val) && (pixel > 1)){ val = pixel; }
        pixel = *(s - sizeof(uint8_pixel_t)); // left
        if((pixel < val) && (pixel > 1)){ val = pixel; }
        pixel = *(s + sizeof(uint8_pixel_t)); // right
        if((pixel < val) && (pixel > 1)){ val = pixel; }

        if(c == CONNECTED_EIGHT)
        {
            pixel = *(s - (img->cols * sizeof(uint8_pixel_t)) - 1);  // up-left
            if((pixel < val) && (pixel > 1)){ val = pixel; }
            pixel = *(s - (img->cols * sizeof(uint8_pixel_t)) + 1);  // up-right
            if((pixel < val) && (pixel > 1)){ val = pixel; }
        }
    }
    // Left border pixels
    else if(x == 0)
    {
        pixel = *(s - (img->cols * sizeof(uint8_pixel_t))); // up
        if((pixel < val) && (pixel > 1)){ val = pixel; }
        pixel = *(s + sizeof(uint8_pixel_t)); // right
        if((pixel < val) && (pixel > 1)){ val = pixel; }
        pixel = *(s + (img->cols * sizeof(uint8_pixel_t))); // down
        if((pixel < val) && (pixel > 1)){ val = pixel; }

        if(c == CONNECTED_EIGHT)
        {
            pixel = *(s - (img->cols * sizeof(uint8_pixel_t)) + 1); // up-right
            if((pixel < val) && (pixel > 1)){ val = pixel; }
            pixel = *(s + (img->cols * sizeof(uint8_pixel_t)) + 1); // down-right
            if((pixel < val) && (pixel > 1)){ val = pixel; }
        }
    }
    // Right border pixels
    else if(x == img->cols-1)
    {
        pixel = *(s - (img->cols * sizeof(uint8_pixel_t)));// up
        if((pixel < val) && (pixel > 1)){ val = pixel; }
        pixel = *(s - sizeof(uint8_pixel_t)); // left
        if((pixel < val) && (pixel > 1)){ val = pixel; }
        pixel = *(s + (img->cols * sizeof(uint8_pixel_t))); // down
        if((pixel < val) && (pixel > 1)){ val = pixel; }

        if(c == CONNECTED_EIGHT)
        {
            pixel = *(s - (img->cols * sizeof(uint8_pixel_t)) - 1); // up-left
            if((pixel < val) && (pixel > 1)){ val = pixel; }
            pixel = *(s + (img->cols * sizeof(uint8_pixel_t)) - 1); // down-left
            if((pixel < val) && (pixel > 1)){ val = pixel; }
        }
    }
    else
    {
        // Inner pixels
        pixel = *(s - (img->cols * sizeof(uint8_pixel_t))); // up
        if((pixel < val) && (pixel > 1)){ val = pixel; }
        pixel = *(s - sizeof(uint8_pixel_t)); // left
        if((pixel < val) && (pixel > 1)){ val = pixel; }
        pixel = *(s + sizeof(uint8_pixel_t)); // right
        if((pixel < val) && (pixel > 1)){ val = pixel; }
        pixel = *(s + (img->cols * sizeof(uint8_pixel_t))); // down
        if((pixel < val) && (pixel > 1)){ val = pixel; }

        if(c == CONNECTED_EIGHT)
        {
            pixel = *(s - (img->cols * sizeof(uint8_pixel_t)) - 1); // up-left
            if((pixel < val) && (pixel > 1)){ val = pixel; }
            pixel = *(s - (img->cols * sizeof(uint8_pixel_t)) + 1); // up-right
            if((pixel < val) && (pixel > 1)){ val = pixel; }
            pixel = *(s + (img->cols * sizeof(uint8_pixel_t)) - 1); // down-left
            if((pixel < val) && (pixel > 1)){ val = pixel; }
            pixel = *(s + (img->cols * sizeof(uint8_pixel_t)) + 1); // down-right
            if((pixel < val) && (pixel > 1)){ val = pixel; }
        }
    }

    return val;
}
