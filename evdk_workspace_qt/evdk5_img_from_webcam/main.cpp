/*! ***************************************************************************
 *
 * \brief     Histogram example from a webcam for the EVDK5
 * \file      main.cpp
 * \author    Hugo Arends - HAN Embedded Vision and Machine Learning
 * \author
 * \date      October 2024
 *
 * \see       Ana Huamán (n.d.). Histogram Calculation. In OpenCV 4.10.0.
 *            Retrieved September 24, 2024, from
 *            https://docs.opencv.org/4.10.0/d8/dbc/tutorial_histogram_calculation.html
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

// -----------------------------------------------------------------------------
// Function implementation
// -----------------------------------------------------------------------------
int main(void)
{
    printf("EVDK5 Histogram example - %s %s\n", __DATE__, __TIME__);

    // -------------------------------------------------------------------------
    // Try to open the EVDK5 uvc video capture device
    // -------------------------------------------------------------------------
    cv::VideoCapture cap(1);

    if (!cap.isOpened())
    {
        printf("No EVDK5 video stream\n");
        return 0;
    }

    printf("Video stream opened\n");

    // -------------------------------------------------------------------------
    // Set image dimensions
    // -------------------------------------------------------------------------
    const int IMG_WIDTH = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    const int IMG_HEIGHT = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    // -------------------------------------------------------------------------
    // Local variables. For every image_t, also create a Mat. They will share
    // the image data buffer! :)
    // -------------------------------------------------------------------------
    image_t *cam = newBgr888Image(IMG_WIDTH, IMG_HEIGHT);
    image_t *src = newUint8Image(IMG_WIDTH, IMG_HEIGHT);
    image_t *dst = newUint8Image(IMG_WIDTH, IMG_HEIGHT);
    image_t *tmp= newUint8Image(IMG_WIDTH, IMG_HEIGHT);

    cv::Mat cv_cam(IMG_HEIGHT, IMG_WIDTH, CV_8UC3, cam->data);
    cv::Mat cv_src(IMG_HEIGHT, IMG_WIDTH, CV_8UC1, src->data);
    cv::Mat cv_dst(IMG_HEIGHT, IMG_WIDTH, CV_8UC1, dst->data);
    cv::Mat cv_tmp(IMG_HEIGHT, IMG_WIDTH, CV_8UC1, tmp->data);

    cv::namedWindow("cam", cv::WINDOW_NORMAL | cv::WINDOW_FREERATIO);
    cv::namedWindow("src", cv::WINDOW_NORMAL | cv::WINDOW_FREERATIO);
    cv::namedWindow("dst", cv::WINDOW_NORMAL | cv::WINDOW_FREERATIO);
    cv::namedWindow("tmp", cv::WINDOW_NORMAL | cv::WINDOW_FREERATIO);

    // -------------------------------------------------------------------------
    // Neatly layout the windows
    // -------------------------------------------------------------------------
    cv::moveWindow("cam", 0, 0);
    cv::moveWindow("src", cv::getWindowImageRect("cam").x +
                          cv::getWindowImageRect("cam").width, 0);

    int y = cv::getWindowImageRect("src").height + 90;
    cv::moveWindow("tmp", 0, y);
    cv::moveWindow("dst", cv::getWindowImageRect("tmp").x +
                          cv::getWindowImageRect("tmp").width, y);

    // -------------------------------------------------------------------------
    // Start main loop
    // -------------------------------------------------------------------------
    printf("Press any key to quit\n");

    while(true)
    {
        // ---------------------------------------------------------------------
        // Capture camera image in BGR format
        // ---------------------------------------------------------------------
        cap.read(cv_cam);

        if(cv_cam.empty())
        {
            printf("Video stream probably stopped\n");
            break;
        }

        // ---------------------------------------------------------------------
        // Image processing pipeline
        // ---------------------------------------------------------------------


        convertBgr888ToUint8(cam, src);
        threshold(src, tmp, 0, 64);
        scale(tmp, dst);


        // ---------------------------------------------------------------------
        // Show images
        // ---------------------------------------------------------------------
        cv::imshow("cam", cv_cam);
        cv::imshow("src", cv_src);
        cv::imshow("dst", cv_dst);
        cv::imshow("tmp", cv_tmp);

        // ---------------------------------------------------------------------
        // Break from the main loop if a key is pressed
        // ---------------------------------------------------------------------
        char c = (char)cv::waitKey(1);

        if(c != -1)
        {
            printf("Key press detected\n");
            break;
        }
    }

    // -------------------------------------------------------------------------
    // Cleanup
    // -------------------------------------------------------------------------
    deleteAllImages();

    cv::destroyAllWindows();
    cap.release();

    printf("EVDK5 application ended\n");

    return 0;
}
