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
void drawHistogram(image_t *src, image_t *dst);

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
    const int HIST_HEIGHT = 400;

    // -------------------------------------------------------------------------
    // Local variables. For every image_t, also create a Mat. They will share
    // the image data buffer! :)
    // -------------------------------------------------------------------------
    image_t *cam = newBgr888Image(IMG_WIDTH, IMG_HEIGHT);
    image_t *src = newUint8Image(IMG_WIDTH, IMG_HEIGHT);
    image_t *dst = newUint8Image(IMG_WIDTH, IMG_HEIGHT);

    cv::Mat cv_cam(IMG_HEIGHT, IMG_WIDTH, CV_8UC3, cam->data);
    cv::Mat cv_src(IMG_HEIGHT, IMG_WIDTH, CV_8UC1, src->data);
    cv::Mat cv_dst(IMG_HEIGHT, IMG_WIDTH, CV_8UC1, dst->data);

    // Histogram images
    image_t *hst_src = newUint8Image(256, HIST_HEIGHT);
    image_t *hst_dst = newUint8Image(256, HIST_HEIGHT);
    cv::Mat cv_hst_src(HIST_HEIGHT, 256, CV_8UC1, hst_src->data);
    cv::Mat cv_hst_dst(HIST_HEIGHT, 256, CV_8UC1, hst_dst->data);

    cv::namedWindow("cam", cv::WINDOW_NORMAL | cv::WINDOW_FREERATIO);
    cv::namedWindow("src", cv::WINDOW_NORMAL | cv::WINDOW_FREERATIO);
    cv::namedWindow("dst", cv::WINDOW_NORMAL | cv::WINDOW_FREERATIO);
    cv::namedWindow("hst_src", cv::WINDOW_NORMAL | cv::WINDOW_FREERATIO);
    cv::namedWindow("hst_dst", cv::WINDOW_NORMAL | cv::WINDOW_FREERATIO);

    // -------------------------------------------------------------------------
    // Neatly layout the windows
    // -------------------------------------------------------------------------
    int y = 0;
    cv::moveWindow("cam", 0, 0);
    cv::moveWindow("src", cv::getWindowImageRect("cam").x +
                          cv::getWindowImageRect("cam").width, 0);
    cv::moveWindow("dst", cv::getWindowImageRect("src").x +
                          cv::getWindowImageRect("src").width, 0);

    y = cv::getWindowImageRect("src").height + 90;
    cv::moveWindow("hst_src", cv::getWindowImageRect("src").x, y);
    cv::moveWindow("hst_dst", cv::getWindowImageRect("dst").x, y);

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

        // Examples, select one!
        scale(src, dst);
//        brightness(src, dst, 100);
//        contrast(src, dst, 2.0f);

        // Draw histograms
        drawHistogram(src, hst_src);
        drawHistogram(dst, hst_dst);


        // ---------------------------------------------------------------------
        // Show images
        // ---------------------------------------------------------------------
        cv::imshow("cam", cv_cam);
        cv::imshow("src", cv_src);
        cv::imshow("dst", cv_dst);
        cv::imshow("hst_src", cv_hst_src);
        cv::imshow("hst_dst", cv_hst_dst);

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

void drawHistogram(image_t *src, image_t *dst)
{
    uint32_t hist[256] = {0};

    // Calculate the histogram
    histogram(src, hist);

    // Normalize

    // Find min and max in the current histogram
    float hist_min = hist[0];
    float hist_max = hist[0];
    for(uint32_t i=1; i<256; ++i)
    {
        hist_min = (hist[i] < hist_min) ? hist[i] : hist_min;
        hist_max = (hist[i] > hist_max) ? hist[i] : hist_max;
    }

    // Single value histogram?
    if(hist_min == hist_max)
    {
        // Set to center value and update histogram
        for(uint32_t i=0; i<256; ++i)
        {
            hist[i] = dst->rows / 2;
        }
    }
    else
    {
        // Calculate normalization factor
        float factor = (float)(dst->rows - 20) / (float)(hist_max - hist_min);

        // Calculate normalization value and update histogram
        for(uint32_t i=0; i<256; ++i)
        {
            hist[i] = ((hist[i] - hist_min) * factor) + 0.5f;
        }
    }

    // Draw the histogram lines (flipped)
    for(int32_t i=0; i<256; ++i)
    {
        // Draw white background
        point_t from = {.x=i, .y=0};
        point_t to = {.x=i, .y=dst->rows};

        drawLineUint8(dst, from, to, 255);

        // Draw histogram value (if any)
        if(hist[i] != 0)
        {
            from = {.x=i, .y=dst->rows - (int32_t)hist[i]};
            to = {.x=i, .y=dst->rows};

            drawLineUint8(dst, from, to, 0);
        }
    }

    // Show the maximum value in the histogram in the left top
    char str[16] = {'\0'};
    sprintf(str, "%d", (uint32_t)hist_max);

    textSetfont(Dialog_bold_16);
    textSetxy(0,0);
    textSetUint8Colors(255, 0);
    textPutstring(dst, str);


    // Find min and max gray values in the original histogram
    histogram(src, hist);
    uint8_pixel_t min_gray = 0;
    uint8_pixel_t max_gray = 255;

    int i=0;
    while(hist[i++] == 0)
    {}
    min_gray = i-1;

    i=255;
    while(hist[i--] == 0)
    {}
    max_gray = i+1;

    sprintf(str, "%d  %d", min_gray, max_gray);

    textSetfont(Dialog_bold_16);
    textSetxy(160,0);
    textSetUint8Colors(255, 0);
    textPutstring(dst, str);
}
