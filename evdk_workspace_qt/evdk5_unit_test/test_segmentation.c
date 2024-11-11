/*! ***************************************************************************
 *
 * \brief     Unit test functions for segmentation functions
 * \file      test_segmentation.c
 * \author    Hugo Arends - HAN Embedded Vision and Machine Learning
 * \author
 * \date      November 2024
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

#include "main.h"

void test_threshold(void)
{
    // Prepare images for testing
    uint8_pixel_t src_data[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        1,   1,   1,   1,   1,   1,   1,   1,
        2,   2,   2,   2,   2,   2,   2,   2,
        3,   3,   3,   3,   3,   3,   3,   3,
        4,   4,   4,   4,   4,   4,   4,   4,
        5,   5,   5,   5,   5,   5,   5,   5,
        6,   6,   6,   6,   6,   6,   6,   6,
        7,   7,   7,   7,   7,   7,   7,   7,
    };

    uint8_pixel_t exp_data_test_case_01[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_pixel_t exp_data_test_case_02[8 * 8] =
    {
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_pixel_t exp_data_test_case_03[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        1,   1,   1,   1,   1,   1,   1,   1,
    };

    uint8_pixel_t dst_data[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

     typedef struct testcase_t
     {
         uint8_pixel_t *src_data;
         uint8_pixel_t *exp_data;
         uint8_pixel_t min;
         uint8_pixel_t max;
     }testcase_t;

     // Compose array of test cases
     testcase_t testcases[] = {
         {src_data, exp_data_test_case_01, 2, 5},
         {src_data, exp_data_test_case_02, 0, 1},
         {src_data, exp_data_test_case_03, 7, 7},
     };

     // Prepare images
     image_t src = {8, 8, IMGTYPE_UINT8, NULL};
     image_t exp = {8, 8, IMGTYPE_UINT8, NULL};
     image_t dst = {8, 8, IMGTYPE_UINT8, dst_data};

     // Loop all test cases
     for(uint32_t i=0; i < (sizeof(testcases) / sizeof(testcase_t)); ++i)
     {
         // Set the data
         src.data = testcases[i].src_data;
         exp.data = testcases[i].exp_data;

         // Execute the operator
         threshold(&src, &dst, testcases[i].min, testcases[i].max);

         // Set test case name
         char name[80] = "";
         sprintf(name, "Test case %d of %d", i+1, (uint32_t)(sizeof(testcases) / sizeof(testcase_t)));

 #if 0
         // Print testcase info
         printf("\n---------------------------------------\n");
         printf("%s\n", name);

         // Print image data
         prettyprint(&src, "src");
         prettyprint(&exp, "exp");
         prettyprint(&dst, "dst");

 #endif

         // Verify the result
         TEST_ASSERT_EQUAL_uint8_pixel_t_ARRAY_MESSAGE(exp.data, dst.data, (exp.cols * exp.rows), name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.type, dst.type, name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.cols, dst.cols, name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.rows, dst.rows, name);
     }
}

void test_thresholdOptimum(void)
{
    // Prepare images for testing
    uint8_pixel_t src_data[8 * 8] =
    {
        5,   5,   5,   5,   6,   6,   6,   6,
        5,   5,   5,   5,   6,   6,   6,   6,
        5,   5,   5,   5,   6,   6,   6,   6,
        5,   5,   5,   5,   6,   6,   6,   6,
      250, 250, 250, 250, 251, 251, 251, 251,
      250, 250, 250, 250, 251, 251, 251, 251,
      250, 250, 250, 250, 251, 251, 251, 251,
      250, 250, 250, 250, 251, 251, 251, 251,
    };

    uint8_pixel_t exp_data_test_case_01[8 * 8] =
    {
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_pixel_t exp_data_test_case_02[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
    };

    uint8_pixel_t dst_data[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

     typedef struct testcase_t
     {
         uint8_pixel_t *src_data;
         uint8_pixel_t *exp_data;
         eBrightness brightness;
     }testcase_t;

     // Compose array of test cases
     testcase_t testcases[] = {
         {src_data, exp_data_test_case_01, BRIGHTNESS_DARK},
         {src_data, exp_data_test_case_02, BRIGHTNESS_BRIGHT},
     };

     // Prepare images
     image_t src = {8, 8, IMGTYPE_UINT8, NULL};
     image_t exp = {8, 8, IMGTYPE_UINT8, NULL};
     image_t dst = {8, 8, IMGTYPE_UINT8, dst_data};

     // Loop all test cases
     for(uint32_t i=0; i < (sizeof(testcases) / sizeof(testcase_t)); ++i)
     {
         // Set the data
         src.data = testcases[i].src_data;
         exp.data = testcases[i].exp_data;

         // Execute the operator
         thresholdOptimum(&src, &dst, testcases[i].brightness);

         // Set test case name
         char name[80] = "";
         sprintf(name, "Test case %d of %d", i+1, (uint32_t)(sizeof(testcases) / sizeof(testcase_t)));

 #if 0
         // Print testcase info
         printf("\n---------------------------------------\n");
         printf("%s\n", name);

         // Print image data
         prettyprint(&src, "src");
         prettyprint(&exp, "exp");
         prettyprint(&dst, "dst");

 #endif

         // Verify the result
         TEST_ASSERT_EQUAL_uint8_pixel_t_ARRAY_MESSAGE(exp.data, dst.data, (exp.cols * exp.rows), name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.type, dst.type, name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.cols, dst.cols, name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.rows, dst.rows, name);
     }
}

void test_threshold2Means(void)
{
    // Prepare images for testing
    uint8_pixel_t src_data_test_case_01[8 * 8] =
    {
        5,   5,   5,   5,   6,   6,   6,   6,
        6,   6,   7,   7,   7,   7,   7,   7,
        7,   7,   7,   7,   7,   7,   8,   8,
        8,   8,   8,   8,   9,   9,   9,   9,
       11,  11,  11,  11,  12,  12,  12,  12,
       12,  12,  13,  13,  13,  13,  13,  13,
       13,  13,  13,  13,  13,  13,  14,  14,
       14,  14,  14,  14,  15,  15,  15,  15,
    };

    uint8_pixel_t exp_data_test_case_01[8 * 8] =
    {
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_pixel_t exp_data_test_case_02[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
    };

    uint8_pixel_t src_data_test_case_02[8 * 8] =
    {
        105, 105, 105, 105, 105, 105, 105, 105,
        105, 105, 105, 105, 105, 105, 105, 105,
        110, 110, 110, 110, 110, 110, 110, 110,
        110, 110, 110, 110, 110, 110, 110, 110,
        120, 120, 120, 120, 120, 120, 120, 120,
        120, 120, 120, 120, 120, 120, 120, 120,
        125, 125, 125, 125, 125, 125, 125, 125,
        125, 125, 125, 125, 125, 125, 125, 125,
    };

    uint8_pixel_t exp_data_test_case_03[8 * 8] =
    {
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_pixel_t exp_data_test_case_04[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
    };

    uint8_pixel_t dst_data[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

     typedef struct testcase_t
     {
         uint8_pixel_t *src_data;
         uint8_pixel_t *exp_data;
         eBrightness brightness;
     }testcase_t;

     // Compose array of test cases
     testcase_t testcases[] = {
         {src_data_test_case_01, exp_data_test_case_01, BRIGHTNESS_DARK},
         {src_data_test_case_01, exp_data_test_case_02, BRIGHTNESS_BRIGHT},
         {src_data_test_case_02, exp_data_test_case_03, BRIGHTNESS_DARK},
         {src_data_test_case_02, exp_data_test_case_04, BRIGHTNESS_BRIGHT},
     };

     // Prepare images
     image_t src = {8, 8, IMGTYPE_UINT8, NULL};
     image_t exp = {8, 8, IMGTYPE_UINT8, NULL};
     image_t dst = {8, 8, IMGTYPE_UINT8, dst_data};

     // Loop all test cases
     for(uint32_t i=0; i < (sizeof(testcases) / sizeof(testcase_t)); ++i)
     {
         // Set the data
         src.data = testcases[i].src_data;
         exp.data = testcases[i].exp_data;

         // Execute the operator
         threshold2Means(&src, &dst, testcases[i].brightness);

         // Set test case name
         char name[80] = "";
         sprintf(name, "Test case %d of %d", i+1, (uint32_t)(sizeof(testcases) / sizeof(testcase_t)));

 #if 0
         // Print testcase info
         printf("\n---------------------------------------\n");
         printf("%s\n", name);

         // Print image data
         prettyprint(&src, "src");
         prettyprint(&exp, "exp");
         prettyprint(&dst, "dst");

 #endif

         // Verify the result
         TEST_ASSERT_EQUAL_uint8_pixel_t_ARRAY_MESSAGE(exp.data, dst.data, (exp.cols * exp.rows), name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.type, dst.type, name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.cols, dst.cols, name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.rows, dst.rows, name);
     }
}

void test_thresholdOtsu(void)
{
    // Prepare images for testing
    uint8_pixel_t src_data_test_case_01[8 * 8] =
    {
        5,   5,   5,   5,   6,   6,   6,   6,
        6,   6,   7,   7,   7,   7,   7,   7,
        7,   7,   7,   7,   7,   7,   8,   8,
        8,   8,   8,   8,   9,   9,   9,   9,
       11,  11,  11,  11,  12,  12,  12,  12,
       12,  12,  13,  13,  13,  13,  13,  13,
       13,  13,  13,  13,  13,  13,  14,  14,
       14,  14,  14,  14,  15,  15,  15,  15,
    };

    uint8_pixel_t exp_data_test_case_01[8 * 8] =
    {
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_pixel_t exp_data_test_case_02[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
    };

    uint8_pixel_t src_data_test_case_02[8 * 8] =
    {
        105, 105, 105, 105, 105, 105, 105, 105,
        105, 105, 105, 105, 105, 105, 105, 105,
        110, 110, 110, 110, 110, 110, 110, 110,
        110, 110, 110, 110, 110, 110, 110, 110,
        120, 120, 120, 120, 120, 120, 120, 120,
        120, 120, 120, 120, 120, 120, 120, 120,
        125, 125, 125, 125, 125, 125, 125, 125,
        125, 125, 125, 125, 125, 125, 125, 125,
    };

    uint8_pixel_t exp_data_test_case_03[8 * 8] =
    {
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_pixel_t exp_data_test_case_04[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
    };

    uint8_pixel_t dst_data[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

     typedef struct testcase_t
     {
         uint8_pixel_t *src_data;
         uint8_pixel_t *exp_data;
         eBrightness brightness;
     }testcase_t;

     // Compose array of test cases
     testcase_t testcases[] = {
         {src_data_test_case_01, exp_data_test_case_01, BRIGHTNESS_DARK},
         {src_data_test_case_01, exp_data_test_case_02, BRIGHTNESS_BRIGHT},
         {src_data_test_case_02, exp_data_test_case_03, BRIGHTNESS_DARK},
         {src_data_test_case_02, exp_data_test_case_04, BRIGHTNESS_BRIGHT},
     };

     // Prepare images
     image_t src = {8, 8, IMGTYPE_UINT8, NULL};
     image_t exp = {8, 8, IMGTYPE_UINT8, NULL};
     image_t dst = {8, 8, IMGTYPE_UINT8, dst_data};

     // Loop all test cases
     for(uint32_t i=0; i < (sizeof(testcases) / sizeof(testcase_t)); ++i)
     {
         // Set the data
         src.data = testcases[i].src_data;
         exp.data = testcases[i].exp_data;

         // Execute the operator
         thresholdOtsu(&src, &dst, testcases[i].brightness);

         // Set test case name
         char name[80] = "";
         sprintf(name, "Test case %d of %d", i+1, (uint32_t)(sizeof(testcases) / sizeof(testcase_t)));

 #if 0
         // Print testcase info
         printf("\n---------------------------------------\n");
         printf("%s\n", name);

         // Print image data
         prettyprint(&src, "src");
         prettyprint(&exp, "exp");
         prettyprint(&dst, "dst");

 #endif

         // Verify the result
         TEST_ASSERT_EQUAL_uint8_pixel_t_ARRAY_MESSAGE(exp.data, dst.data, (exp.cols * exp.rows), name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.type, dst.type, name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.cols, dst.cols, name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.rows, dst.rows, name);
     }
}

void test_lineDetector(void)
{
    // Prepare images for testing
    uint8_pixel_t src_data[8 * 8] =
    {
        3,   3,   3,   3,   3,   3,   3,   3,
        3,   3,   3,  50,  50,   3,   3,   3,
        3,   3,   3,  50,  50,   3,   3,   3,
        3,  50,  50,  50,  50,  50,  50,   3,
        3,  50,  50,  50,  50,  50,  50,   3,
        3,   3,   3,  50,  50,   3,   3,   3,
        3,   3,   3,  50,  50,   3,   3,   3,
        3,   3,   3,   3,   3,   3,   3,   3,
    };

    uint8_pixel_t exp_data_test_case_01[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,  47,  94,  94,  47,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,  94,  94,  47,  47,  94,  94,   0,
        0,  94,  94,  47,  47,  94,  94,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,  47,  94,  94,  47,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_pixel_t exp_data_test_case_02[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,  94,  94,   0,   0,   0,
        0,  47,   0,  94,  94,   0,  47,   0,
        0,  94,   0,  47,  47,   0,  94,   0,
        0,  94,   0,  47,  47,   0,  94,   0,
        0,  47,   0,  94,  94,   0,  47,   0,
        0,   0,   0,  94,  94,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_pixel_t exp_data_test_case_03[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,  47,  94,   0,   0,   0,   0,
        0,  47,   0,   0,  94,  47,   0,   0,
        0,  94,   0,   0,  47,  94,   0,   0,
        0,   0,  94,  47,   0,   0,  94,   0,
        0,   0,  47,  94,   0,   0,  47,   0,
        0,   0,   0,   0,  94,  47,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_pixel_t exp_data_test_case_04[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,  94,  47,   0,   0,
        0,   0,  47,  94,   0,   0,  47,   0,
        0,   0,  94,  47,   0,   0,  94,   0,
        0,  94,   0,   0,  47,  94,   0,   0,
        0,  47,   0,   0,  94,  47,   0,   0,
        0,   0,  47,  94,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_pixel_t dst_data[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

     typedef struct testcase_t
     {
         uint8_pixel_t *exp_data;
         int16_t m[3][3];
     }testcase_t;

     // Compose array of test cases
     testcase_t testcases[] = {
         {exp_data_test_case_01,{{-1,-1,-1},
                                 { 2, 2, 2},
                                 {-1,-1,-1}}},
         {exp_data_test_case_02,{{-1, 2,-1},
                                 {-1, 2,-1},
                                 {-1, 2,-1}}},
         {exp_data_test_case_03,{{ 2,-1,-1},
                                 {-1, 2,-1},
                                 {-1,-1, 2}}},
         {exp_data_test_case_04,{{-1,-1, 2},
                                 {-1, 2,-1},
                                 { 2,-1,-1}}},
     };

     // Prepare images
     image_t src = {8, 8, IMGTYPE_UINT8, src_data};
     image_t exp = {8, 8, IMGTYPE_UINT8, NULL};
     image_t dst = {8, 8, IMGTYPE_UINT8, dst_data};

     // Loop all test cases
     for(uint32_t i=0; i < (sizeof(testcases) / sizeof(testcase_t)); ++i)
     {
         // Set the data
         exp.data = testcases[i].exp_data;

         // Clear destination
         clearUint8Image(&dst);

         // Execute the operator
         lineDetector(&src, &dst,
                      testcases[i].m);

         // Set test case name
         char name[80] = "";
         sprintf(name, "Test case %d of %d", i+1, (uint32_t)(sizeof(testcases) / sizeof(testcase_t)));

 #if 0
         // Print testcase info
         printf("\n---------------------------------------\n");
         printf("%s\n", name);

         // Print image data
         prettyprint(&src, "src");
         prettyprint(&exp, "exp");
         prettyprint(&dst, "dst");

 #endif

         // Verify the result
         TEST_ASSERT_EQUAL_uint8_pixel_t_ARRAY_MESSAGE(exp.data, dst.data, (exp.cols * exp.rows), name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.type, dst.type, name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.cols, dst.cols, name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.rows, dst.rows, name);
     }
}
