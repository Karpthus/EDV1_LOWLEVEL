/*! ***************************************************************************
 *
 * \brief     Unit test functions for mensuration functions
 * \file      test_mensuration.c
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

void test_area(void)
{
    // Prepare images for testing
    uint8_pixel_t src_data_01[8 * 8] =
    {
        5,   5,   5,   5,   6,   6,   6,   6,
        5,   5,   5,   5,   6,   6,   6,   6,
        5,   5,   5,   5,   6,   6,   6,   6,
        5,   5,   5,   5,   6,   6,   6,   6,
        250, 250, 250, 250, 250, 250, 250, 250,
        250, 250, 250, 250, 250, 250, 250, 250,
        251, 251, 251, 251, 251, 251, 251, 251,
        251, 251, 251, 251, 251, 251, 251, 251,
    };

    uint8_pixel_t src_data_02[8 * 8] =
    {
        1,   0,   0,   0,   0,   0,   0,   1,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        1,   0,   0,   0,   0,   0,   0,   1,
    };

    typedef struct testcase_t
    {
        uint32_t blobnr;
        uint32_t exp_area;
        uint8_pixel_t *src_data;
    }testcase_t;

    // Compose array of test cases
    testcase_t testcases[] = {
        {  0,  0, src_data_01},
        {  5, 16, src_data_01},
        {250, 16, src_data_01},
        {251, 16, src_data_01},
        {  0, 60, src_data_02},
        {  1,  4, src_data_02},
    };

    // Prepare images
    image_t src = {8,8, IMGTYPE_UINT8, NULL};

    // Loop all test cases
    for(uint32_t i=0; i < (sizeof(testcases) / sizeof(testcase_t)); ++i)
    {
        blobinfo_t blobinfo;

        // Set the data
        src.data = testcases[i].src_data;

        // Execute the operator
        area(&src, &blobinfo, testcases[i].blobnr);

        // Set test case name
        char name[80] = "";
        sprintf(name, "Test case %d of %d", i+1, (uint32_t)(sizeof(testcases) / sizeof(testcase_t)));

#if 0
        // Print testcase info
        printf("\n---------------------------------------\n");
        printf("%s\n", name);

        // Print image data
        prettyprint(&src, "src");
        printf("expected area  : %d\n", testcases[i].exp_area);
        printf("calculated area: %d\n", blobinfo.area);

#endif

        // Verify the result
        TEST_ASSERT_EQUAL_MESSAGE(testcases[i].exp_area, blobinfo.area, name);
    }
}

void test_labelIterative(void)
{
    // Prepare images for testing
    uint8_pixel_t src_data_test_case_0102[8 * 8] =
        {
            1,   1,   0,   0,   0,   0,   0,   0,
            1,   1,   0,   0,   0,   1,   1,   0,
            0,   0,   1,   1,   0,   1,   1,   0,
            0,   0,   1,   1,   0,   0,   1,   0,
            0,   0,   0,   1,   0,   0,   0,   0,
            0,   0,   0,   1,   0,   1,   0,   0,
            0,   1,   0,   1,   0,   1,   0,   0,
            0,   0,   0,   0,   1,   0,   0,   0,
        };

    uint8_pixel_t src_data_test_case_0304[8 * 8] =
        {
            0,   0,   0,   0,   0,   0,   0,   0,
            0,   0,   0,   0,   0,   0,   1,   0,
            0,   0,   0,   1,   1,   0,   1,   0,
            0,   1,   1,   1,   1,   1,   1,   0,
            0,   0,   0,   0,   0,   0,   0,   0,
            0,   0,   1,   0,   1,   0,   0,   0,
            0,   0,   0,   1,   0,   0,   0,   0,
            0,   0,   1,   0,   1,   0,   0,   0,
        };

    // FOUR connected expected result
    uint8_pixel_t exp_data_test_case_01[8 * 8] =
        {
            1,   1,   0,   0,   0,   0,   0,   0,
            1,   1,   0,   0,   0,   2,   2,   0,
            0,   0,   3,   3,   0,   2,   2,   0,
            0,   0,   3,   3,   0,   0,   2,   0,
            0,   0,   0,   3,   0,   0,   0,   0,
            0,   0,   0,   3,   0,   4,   0,   0,
            0,   5,   0,   3,   0,   4,   0,   0,
            0,   0,   0,   0,   6,   0,   0,   0,
        };

    // EIGHT connected expected result
    uint8_pixel_t exp_data_test_case_02[8 * 8] =
        {
            1,   1,   0,   0,   0,   0,   0,   0,
            1,   1,   0,   0,   0,   2,   2,   0,
            0,   0,   1,   1,   0,   2,   2,   0,
            0,   0,   1,   1,   0,   0,   2,   0,
            0,   0,   0,   1,   0,   0,   0,   0,
            0,   0,   0,   1,   0,   1,   0,   0,
            0,   3,   0,   1,   0,   1,   0,   0,
            0,   0,   0,   0,   1,   0,   0,   0,
        };

    // FOUR connected expected result
    uint8_pixel_t exp_data_test_case_03[8 * 8] =
        {
            0,   0,   0,   0,   0,   0,   0,   0,
            0,   0,   0,   0,   0,   0,   1,   0,
            0,   0,   0,   1,   1,   0,   1,   0,
            0,   1,   1,   1,   1,   1,   1,   0,
            0,   0,   0,   0,   0,   0,   0,   0,
            0,   0,   2,   0,   3,   0,   0,   0,
            0,   0,   0,   4,   0,   0,   0,   0,
            0,   0,   5,   0,   6,   0,   0,   0,
        };

    // EIGHT connected expected result
    uint8_pixel_t exp_data_test_case_04[8 * 8] =
        {
            0,   0,   0,   0,   0,   0,   0,   0,
            0,   0,   0,   0,   0,   0,   1,   0,
            0,   0,   0,   1,   1,   0,   1,   0,
            0,   1,   1,   1,   1,   1,   1,   0,
            0,   0,   0,   0,   0,   0,   0,   0,
            0,   0,   2,   0,   2,   0,   0,   0,
            0,   0,   0,   2,   0,   0,   0,   0,
            0,   0,   2,   0,   2,   0,   0,   0,
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
        eConnected c;
    }testcase_t;

    // Compose array of test cases
    testcase_t testcases[] = {
        {src_data_test_case_0102, exp_data_test_case_01, CONNECTED_FOUR},
        {src_data_test_case_0102, exp_data_test_case_02, CONNECTED_EIGHT},
        {src_data_test_case_0304, exp_data_test_case_03, CONNECTED_FOUR},
        {src_data_test_case_0304, exp_data_test_case_04, CONNECTED_EIGHT},
    };

    // Prepare images
    image_t src = {8,8, IMGTYPE_UINT8, NULL};
    image_t exp = {8,8, IMGTYPE_UINT8, NULL};
    image_t dst = {8,8, IMGTYPE_UINT8, dst_data};

    // Loop all test cases
    for(uint32_t i=0; i < (sizeof(testcases) / sizeof(testcase_t)); ++i)
    {
        // Set the data
        src.data = testcases[i].src_data;
        exp.data = testcases[i].exp_data;

        // Execute the operator
        labelIterative(&src, &dst, testcases[i].c);

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

void test_labelTwoPass(void)
{
    // Prepare images for testing
    uint8_pixel_t src_data_test_case_0102[8 * 8] =
        {
            1,   1,   0,   0,   0,   0,   0,   0,
            1,   1,   0,   0,   0,   1,   1,   0,
            0,   0,   1,   1,   0,   1,   1,   0,
            0,   0,   1,   1,   0,   0,   1,   0,
            0,   0,   0,   1,   0,   0,   0,   0,
            0,   0,   0,   1,   0,   1,   0,   0,
            0,   1,   0,   1,   0,   1,   0,   0,
            0,   0,   0,   0,   1,   0,   0,   0,
        };

    uint8_pixel_t src_data_test_case_0304[8 * 8] =
        {
            0,   0,   0,   0,   0,   0,   0,   0,
            0,   0,   0,   0,   0,   0,   1,   0,
            0,   0,   0,   1,   1,   0,   1,   0,
            0,   1,   1,   1,   1,   1,   1,   0,
            0,   0,   0,   0,   0,   0,   0,   0,
            0,   0,   1,   0,   1,   0,   0,   0,
            0,   0,   0,   1,   0,   0,   0,   0,
            0,   0,   1,   0,   1,   0,   0,   0,
        };

    // FOUR connected expected result
    uint8_pixel_t exp_data_test_case_01[8 * 8] =
        {
            0,   0,   0,   0,   0,   0,   0,   0,
            0,   1,   0,   0,   0,   2,   2,   0,
            0,   0,   3,   3,   0,   2,   2,   0,
            0,   0,   3,   3,   0,   0,   2,   0,
            0,   0,   0,   3,   0,   0,   0,   0,
            0,   0,   0,   3,   0,   4,   0,   0,
            0,   5,   0,   3,   0,   4,   0,   0,
            0,   0,   0,   0,   0,   0,   0,   0,
        };

    // EIGHT connected expected result
    uint8_pixel_t exp_data_test_case_02[8 * 8] =
        {
            0,   0,   0,   0,   0,   0,   0,   0,
            0,   1,   0,   0,   0,   2,   2,   0,
            0,   0,   1,   1,   0,   2,   2,   0,
            0,   0,   1,   1,   0,   0,   2,   0,
            0,   0,   0,   1,   0,   0,   0,   0,
            0,   0,   0,   1,   0,   3,   0,   0,
            0,   4,   0,   1,   0,   3,   0,   0,
            0,   0,   0,   0,   0,   0,   0,   0,
        };

    // FOUR connected expected result
    uint8_pixel_t exp_data_test_case_03[8 * 8] =
        {
            0,   0,   0,   0,   0,   0,   0,   0,
            0,   0,   0,   0,   0,   0,   1,   0,
            0,   0,   0,   1,   1,   0,   1,   0,
            0,   1,   1,   1,   1,   1,   1,   0,
            0,   0,   0,   0,   0,   0,   0,   0,
            0,   0,   2,   0,   3,   0,   0,   0,
            0,   0,   0,   4,   0,   0,   0,   0,
            0,   0,   0,   0,   0,   0,   0,   0,
        };

    // EIGHT connected expected result
    uint8_pixel_t exp_data_test_case_04[8 * 8] =
        {
            0,   0,   0,   0,   0,   0,   0,   0,
            0,   0,   0,   0,   0,   0,   1,   0,
            0,   0,   0,   1,   1,   0,   1,   0,
            0,   1,   1,   1,   1,   1,   1,   0,
            0,   0,   0,   0,   0,   0,   0,   0,
            0,   0,   2,   0,   2,   0,   0,   0,
            0,   0,   0,   2,   0,   0,   0,   0,
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
        uint8_pixel_t *src_data;
        uint8_pixel_t *exp_data;
        eConnected c;
        uint32_t maxLabels;
    }testcase_t;

    // Compose array of test cases
    testcase_t testcases[] = {
        {src_data_test_case_0102, exp_data_test_case_01, CONNECTED_FOUR, 64},
        {src_data_test_case_0102, exp_data_test_case_02, CONNECTED_EIGHT, 64},
        {src_data_test_case_0304, exp_data_test_case_03, CONNECTED_FOUR, 64},
        {src_data_test_case_0304, exp_data_test_case_04, CONNECTED_EIGHT, 64},
    };

    // Prepare images
    image_t src = {8,8, IMGTYPE_UINT8, NULL};
    image_t exp = {8,8, IMGTYPE_UINT8, NULL};
    image_t dst = {8,8, IMGTYPE_UINT8, dst_data};

    // Loop all test cases
    for(uint32_t i=0; i < (sizeof(testcases) / sizeof(testcase_t)); ++i)
    {
        // Set the data
        src.data = testcases[i].src_data;
        exp.data = testcases[i].exp_data;

        // Execute the operator
        labelTwoPass(&src, &dst, testcases[i].c, testcases[i].maxLabels);

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

void test_perimeter(void)
{
    // Prepare images for testing
    uint8_pixel_t src_data_01[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   1,   0,   0,   0,   0,   0,
        0,   0,   1,   1,   0,   0,   0,   0,
        0,   0,   1,   1,   1,   0,   0,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   0,   1,   1,   1,   1,   1,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_pixel_t src_data_02[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_pixel_t src_data_03[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   1,   1,   1,   1,   1,   1,   0,
        0,   1,   1,   1,   1,   1,   1,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    typedef struct testcase_t
    {
        uint32_t blobnr;
        float exp_perimeter;
        uint8_pixel_t *src_data;
    }testcase_t;

    // Compose array of test cases
    testcase_t testcases[] = {
        {1, 13.48f, src_data_01},
        {1, 12.00f, src_data_02},
        {1, 14.60f, src_data_03},
    };

    // Prepare images
    image_t src = {8,8, IMGTYPE_UINT8, NULL};

    // Loop all test cases
    for(uint32_t i=0; i < (sizeof(testcases) / sizeof(testcase_t)); ++i)
    {
        blobinfo_t blobinfo;

        // Set the data
        src.data = testcases[i].src_data;

        // Execute the operator
        perimeter(&src, &blobinfo, testcases[i].blobnr);

        // Set test case name
        char name[80] = "";
        sprintf(name, "Test case %d of %d", i+1, (uint32_t)(sizeof(testcases) / sizeof(testcase_t)));

#if 0
        // Print testcase info
        printf("\n---------------------------------------\n");
        printf("%s\n", name);

        // Print image data
        prettyprint(&src, "src");
        printf("expected perimeter  : %f\n", testcases[i].exp_perimeter);
        printf("calculated perimeter: %f\n", blobinfo.perimeter);

#endif

        // Verify the result
        TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.01f, testcases[i].exp_perimeter, blobinfo.perimeter, name);
    }
}
