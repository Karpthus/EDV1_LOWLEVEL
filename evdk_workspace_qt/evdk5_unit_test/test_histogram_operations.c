/*! ***************************************************************************
 *
 * \brief     Unit test functions for histogram operations
 * \file      test_histogram_operations.c
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

void test_histogram(void)
{
    uint8_pixel_t src_data[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   1,   1,   1,   1,   1,   1,   1,
        2,   2,   2,   2,   2,   2,   2,   2,
        3,   3,   3,   3,   3,   3,   3,   3,
        4,   4,   4,   4,   4,   4,   4,   4,
        5,   5,   5,   5,   5,   5,   5,   5,
        6,   6,   6,   6,   6,   6,   6,   6,
        7,   7,   7,   7,   7,   7,   7, 255,
    };

    // Prepare image
    image_t src = {8, 8, IMGTYPE_UINT8, src_data};

    // Prepare histogram
    uint32_t hist[256];

    // Execute the operator
    histogram(&src, hist);

    // Verify the result
    TEST_ASSERT_EQUAL_MESSAGE(9, hist[0], "histogram value incorrect");
    TEST_ASSERT_EQUAL_MESSAGE(7, hist[1], "histogram value incorrect");
    TEST_ASSERT_EQUAL_MESSAGE(8, hist[2], "histogram value incorrect");
    TEST_ASSERT_EQUAL_MESSAGE(0, hist[254], "histogram value incorrect");
    TEST_ASSERT_EQUAL_MESSAGE(1, hist[255], "histogram value incorrect");
}

void test_brightness(void)
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
        10,  10,  10,  10,  10,  10,  10,  10,
        11,  11,  11,  11,  11,  11,  11,  11,
        12,  12,  12,  12,  12,  12,  12,  12,
        13,  13,  13,  13,  13,  13,  13,  13,
        14,  14,  14,  14,  14,  14,  14,  14,
        15,  15,  15,  15,  15,  15,  15,  15,
        16,  16,  16,  16,  16,  16,  16,  16,
        17,  17,  17,  17,  17,  17,  17,  17,
    };

    uint8_pixel_t exp_data_test_case_02[8 * 8] =
    {
        1,   1,   1,   1,   1,   1,   1,   1,
        2,   2,   2,   2,   2,   2,   2,   2,
        3,   3,   3,   3,   3,   3,   3,   3,
        4,   4,   4,   4,   4,   4,   4,   4,
        5,   5,   5,   5,   5,   5,   5,   5,
        6,   6,   6,   6,   6,   6,   6,   6,
        7,   7,   7,   7,   7,   7,   7,   7,
        8,   8,   8,   8,   8,   8,   8,   8,
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
        uint32_t b;
    }testcase_t;

    // Compose array of test cases
    testcase_t testcases[] =
    {
        {exp_data_test_case_01,10},
        {exp_data_test_case_02,1},
    };

    // Prepare images
    image_t src = {8,8, IMGTYPE_UINT8, src_data};
    image_t exp = {8,8, IMGTYPE_UINT8, NULL};
    image_t dst = {8,8, IMGTYPE_UINT8, dst_data};

    // Loop all test cases
    for(uint32_t i=0; i < (sizeof(testcases) / sizeof(testcase_t)); ++i)
    {
        // Set the data
        exp.data = testcases[i].exp_data;

        // Execute the operator
        brightness(&src, &dst, testcases[i].b);

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

void test_contrast(void)
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
        1,   1,   1,   1,   1,   1,   1,   1,
        2,   2,   2,   2,   2,   2,   2,   2,
        3,   3,   3,   3,   3,   3,   3,   3,
        4,   4,   4,   4,   4,   4,   4,   4,
        5,   5,   5,   5,   5,   5,   5,   5,
        6,   6,   6,   6,   6,   6,   6,   6,
        7,   7,   7,   7,   7,   7,   7,   7,
    };

    uint8_pixel_t exp_data_test_case_02[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        2,   2,   2,   2,   2,   2,   2,   2,
        4,   4,   4,   4,   4,   4,   4,   4,
        6,   6,   6,   6,   6,   6,   6,   6,
        8,   8,   8,   8,   8,   8,   8,   8,
       10,  10,  10,  10,  10,  10,  10,  10,
    };

    uint8_pixel_t exp_data_test_case_03[8 * 8] =
    {
        1,   1,   1,   1,   1,   1,   1,   1,
        2,   2,   2,   2,   2,   2,   2,   2,
        2,   2,   2,   2,   2,   2,   2,   2,
        3,   3,   3,   3,   3,   3,   3,   3,
        3,   3,   3,   3,   3,   3,   3,   3,
        4,   4,   4,   4,   4,   4,   4,   4,
        4,   4,   4,   4,   4,   4,   4,   4,
        5,   5,   5,   5,   5,   5,   5,   5,
    };

    uint8_pixel_t exp_data_test_case_04[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        8,   8,   8,   8,   8,   8,   8,   8,
       18,  18,  18,  18,  18,  18,  18,  18,
       28,  28,  28,  28,  28,  28,  28,  28,
       38,  38,  38,  38,  38,  38,  38,  38,
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
        float c;
    }testcase_t;

    // Compose array of test cases
    testcase_t testcases[] = {
        {exp_data_test_case_01,  1.0f},
        {exp_data_test_case_02,  2.0f},
        {exp_data_test_case_03,  0.5f},
        {exp_data_test_case_04, 10.0f},
    };

    // Prepare images
    image_t src = {8,8, IMGTYPE_UINT8, src_data};
    image_t exp = {8,8, IMGTYPE_UINT8, NULL};
    image_t dst = {8,8, IMGTYPE_UINT8, dst_data};

    // Loop all test cases
    for(uint32_t i=0; i < (sizeof(testcases) / sizeof(testcase_t)); ++i)
    {
        // Set the data
        exp.data = testcases[i].exp_data;

        // Execute the operator
        contrast(&src, &dst, testcases[i].c);

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
        // Allow an offset of +1 and -1 for the data
        TEST_ASSERT_UINT8_ARRAY_WITHIN_MESSAGE(1, exp.data, dst.data, (exp.cols * exp.rows), name);
        TEST_ASSERT_EQUAL_MESSAGE(exp.type, dst.type, name);
        TEST_ASSERT_EQUAL_MESSAGE(exp.cols, dst.cols, name);
        TEST_ASSERT_EQUAL_MESSAGE(exp.rows, dst.rows, name);
    }
}
