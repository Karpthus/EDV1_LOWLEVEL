/*! ***************************************************************************
 *
 * \brief     Image from file example for the EVDK5
 * \file      main.cpp
 * \author    Hugo Arends - HAN Embedded Vision and Machine Learning
 * \author
 * \date      October 2024
 *
 * \note      Successfully tested with the following software versions:
 *            OpenCV 4.8.0
 *            Qt 6.7.1 MinGW 64 bit
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
#include "opencv2/opencv.hpp"

#include "operators.h"

// -----------------------------------------------------------------------------
// Local function prototypes
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Local variables
// -----------------------------------------------------------------------------
std::string img_folder = "./../../evdk_images/";

// -----------------------------------------------------------------------------
// Function implementation
// -----------------------------------------------------------------------------
int main(void)
{
    printf("EVDK5 Image from file example - %s %s\n", __DATE__, __TIME__);

    // -------------------------------------------------------------------------
    // Open an image from a file
    // -------------------------------------------------------------------------
    cv::Mat cv_img;

//    cv_img = cv::imread(img_folder + "frdm-mcxn947-box-01.bmp");
    cv_img = cv::imread(img_folder + "frdm-mcxn947-box-02.bmp");
//    cv_img = cv::imread(img_folder + "frdm-mcxn947-box-03.bmp");
//    cv_img = cv::imread(img_folder + "affine-dst1.png");
//    cv_img = cv::imread(img_folder + "affine-dst2.png");
//    cv_img = cv::imread(img_folder + "affine-src.png");
//    cv_img = cv::imread(img_folder + "filters.png");
//    cv_img = cv::imread(img_folder + "rbb_test_01.bmp");
//    cv_img = cv::imread(img_folder + "rbb_test_02.bmp");
//    cv_img = cv::imread(img_folder + "rbb_test_03.bmp");
//    cv_img = cv::imread(img_folder + "fillholes_test_01.bmp");
//    cv_img = cv::imread(img_folder + "fillholes_test_02.bmp");
//    cv_img = cv::imread(img_folder + "label_test_01.bmp");
//    cv_img = cv::imread(img_folder + "label_test_02.bmp");
//    cv_img = cv::imread(img_folder + "shapes_test_circle_01.jpg");
//    cv_img = cv::imread(img_folder + "shapes_test_square_01.jpg");
//    cv_img = cv::imread(img_folder + "shapes_test_triangle_01.jpg");

    if(cv_img.empty())
    {
        printf("Image not opened\n");
        return 0;
    }

    printf("Image opened\n");

    // -------------------------------------------------------------------------
    // Set image dimensions
    // -------------------------------------------------------------------------
    const int IMG_WIDTH = cv_img.cols;
    const int IMG_HEIGHT = cv_img.rows;

    // -------------------------------------------------------------------------
    // Create an image without data and set its data pointer to the Mat's data
    // -------------------------------------------------------------------------
    image_t *img = newEmptyBgr888Image(IMG_WIDTH, IMG_HEIGHT);

    img->data = cv_img.data;

    // -------------------------------------------------------------------------
    // Local variables. For every image_t, also create a Mat. They will share
    // the image data buffer! :)
    // -------------------------------------------------------------------------
    image_t *src = newUint8Image(IMG_WIDTH, IMG_HEIGHT);
    image_t *dst = newUint8Image(IMG_WIDTH, IMG_HEIGHT);
    image_t *tmp= newUint8Image(IMG_WIDTH, IMG_HEIGHT);

    cv::Mat cv_src(IMG_HEIGHT, IMG_WIDTH, CV_8UC1, src->data);
    cv::Mat cv_dst(IMG_HEIGHT, IMG_WIDTH, CV_8UC1, dst->data);
    cv::Mat cv_tmp(IMG_HEIGHT, IMG_WIDTH, CV_8UC1, tmp->data);

    cv::namedWindow("img", cv::WINDOW_NORMAL | cv::WINDOW_FREERATIO);
    cv::namedWindow("src", cv::WINDOW_NORMAL | cv::WINDOW_FREERATIO);
    cv::namedWindow("dst", cv::WINDOW_NORMAL | cv::WINDOW_FREERATIO);
    cv::namedWindow("tmp", cv::WINDOW_NORMAL | cv::WINDOW_FREERATIO);

    // -------------------------------------------------------------------------
    // Neatly layout the windows
    // -------------------------------------------------------------------------
    cv::moveWindow("img", 0, 0);
    cv::moveWindow("src", cv::getWindowImageRect("img").x +
                          cv::getWindowImageRect("img").width, 0);

    int y = cv::getWindowImageRect("src").height + 90;
    cv::moveWindow("tmp", 0, y);
    cv::moveWindow("dst", cv::getWindowImageRect("tmp").x +
                          cv::getWindowImageRect("tmp").width, y);

    // -------------------------------------------------------------------------
    // Image processing pipeline
    // -------------------------------------------------------------------------


    convertBgr888ToUint8(img, src);
    threshold(src, tmp, 0, 64);
    scale(tmp, dst);


    // -------------------------------------------------------------------------
    // Show images
    // -------------------------------------------------------------------------
    cv::imshow("img", cv_img);
    cv::imshow("src", cv_src);
    cv::imshow("dst", cv_dst);
    cv::imshow("tmp", cv_tmp);

    // -------------------------------------------------------------------------
    // Wait for a key is pressed
    // -------------------------------------------------------------------------
    cv::waitKey(0);

    printf("Key press detected\n");

    // -------------------------------------------------------------------------
    // Cleanup
    // -------------------------------------------------------------------------
    // Need to restore img data pointer to NULL, because the cv_img Mat will
    // also free the allocated data
    img->data = NULL;
    deleteAllImages();

    cv::destroyAllWindows();

    printf("EVDK5 application ended\n");

    return 0;
}