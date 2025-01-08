/*! ***************************************************************************
 *
 * \brief     Unit test functions for image fundamentals
 * \file      test_image_fundamentals.c
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

void test_convertUyvyToUint8(void)
{
    // Prepare images for testing
    uyvy_pixel_t src_data_test_case_01[8 * 8] =
    {
        0x4080, 0xC080, 0x4000, 0xC000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x4000, 0xC080, 0x40FF, 0xC0FF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
        0x4080, 0xC080, 0x4000, 0xC000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x4000, 0xC080, 0x40FF, 0xC0FF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
        0x4080, 0xC080, 0x4000, 0xC000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x4000, 0xC080, 0x40FF, 0xC0FF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
        0x4080, 0xC080, 0x4000, 0xC000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x4000, 0xC080, 0x40FF, 0xC0FF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    };

    uint8_pixel_t exp_data_test_case_01[8 * 8] =
    {
        64, 192,  64, 192,   0,   0,   0,   0,
        64, 192,  64, 192, 255, 255, 255, 255,
        64, 192,  64, 192,   0,   0,   0,   0,
        64, 192,  64, 192, 255, 255, 255, 255,
        64, 192,  64, 192,   0,   0,   0,   0,
        64, 192,  64, 192, 255, 255, 255, 255,
        64, 192,  64, 192,   0,   0,   0,   0,
        64, 192,  64, 192, 255, 255, 255, 255,
    };

    uyvy_pixel_t src_data_test_case_02[8 * 8] =
    {
        0x0001, 0x0010, 0x4080, 0xC080, 0x4000, 0xC000, 0x0100, 0x1000,
        0xFFFF, 0xFFFF, 0x4000, 0xC080, 0x40FF, 0xC0FF, 0xFFFF, 0xFFFF,
        0x000F, 0x00F0, 0x4080, 0xC080, 0x4000, 0xC000, 0x0F00, 0xF000,
        0xFFFF, 0xFFFF, 0x4000, 0xC080, 0x40FF, 0xC0FF, 0xFFFF, 0xFFFF,
        0x0000, 0x0000, 0x4080, 0xC080, 0x4000, 0xC000, 0x0000, 0x0000,
        0xFFFF, 0xFFFF, 0x4000, 0xC080, 0x40FF, 0xC0FF, 0xFFFF, 0xFFFF,
        0x0000, 0x0000, 0x4080, 0xC080, 0x4000, 0xC000, 0x0000, 0x0000,
        0xFFFF, 0xFFFF, 0x4000, 0xC080, 0x40FF, 0xC0FF, 0xFFFF, 0xFFFF,
    };

    uint8_pixel_t exp_data_test_case_02[8 * 8] =
    {
          0,   0,  64, 192,  64, 192,   1,  16,
        255, 255,  64, 192,  64, 192, 255, 255,
          0,   0,  64, 192,  64, 192,  15, 240,
        255, 255,  64, 192,  64, 192, 255, 255,
          0,   0,  64, 192,  64, 192,   0,   0,
        255, 255,  64, 192,  64, 192, 255, 255,
          0,   0,  64, 192,  64, 192,   0,   0,
        255, 255,  64, 192,  64, 192, 255, 255,
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
        uyvy_pixel_t *src_data;
        uint8_pixel_t *exp_data;
    }testcase_t;

    // Compose array of test cases
    testcase_t testcases[] =
    {
        {src_data_test_case_01, exp_data_test_case_01},
        {src_data_test_case_02, exp_data_test_case_02},
    };

    // Prepare images
    image_t src = {8,8, IMGTYPE_UYVY, NULL};
    image_t exp = {8,8, IMGTYPE_UINT8, NULL};
    image_t dst = {8,8, IMGTYPE_UINT8, dst_data};

    // Loop all test cases
    for(uint32_t i=0; i < (sizeof(testcases) / sizeof(testcase_t)); ++i)
    {
        // Set the data
        src.data = (uint8_t *)(testcases[i].src_data);
        exp.data = testcases[i].exp_data;

        // Execute the operator
        convertUyvyToUint8(&src, &dst);

        // Set test case name
        char name[80] = "";
        sprintf(name, "Test case %d of %d", i+1, (uint32_t)(sizeof(testcases) / sizeof(testcase_t)));

#if 0 // Change 0 to 1 to enable printing

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

void test_scaleFast(void)
{
    // Prepare images for testing
    uint8_pixel_t src_data_test_case_01[8 * 8] =
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
       36,  36,  36,  36,  36,  36,  36,  36,
       73,  73,  73,  73,  73,  73,  73,  73,
      109, 109, 109, 109, 109, 109, 109, 109,
      146, 146, 146, 146, 146, 146, 146, 146,
      182, 182, 182, 182, 182, 182, 182, 182,
      219, 219, 219, 219, 219, 219, 219, 219,
      255, 255, 255, 255, 255, 255, 255, 255,
    };

    uint8_pixel_t src_data_test_case_02[8 * 8] =
    {
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   2,
    };

    uint8_pixel_t exp_data_test_case_02[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0, 255,
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
    }testcase_t;

    // Compose array of test cases
    testcase_t testcases[] =
    {
        {src_data_test_case_01, exp_data_test_case_01},
        {src_data_test_case_02, exp_data_test_case_02},
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
        scaleFast(&src, &dst);

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

void test_convolve(void)
{
    // Prepare images for testing
    int16_pixel_t src_data_test_case_01[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   1,   1,   1,   1,   1,   1,   0,
        0,   2,   2,   2,   2,   2,   2,   0,
        0,   3,   3,   3,   3,   3,   3,   0,
        0,   4,   4,   4,   4,   4,   4,   0,
        0,   5,   5,   5,   5,   5,   5,   0,
        0,   6,   6,   6,   6,   6,   6,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    int16_pixel_t msk_data_test_case_0101[3 * 3] =
    {
        0, 0, 0,
        0, 1, 0,
        0, 0, 0,
    };

    int16_pixel_t msk_data_test_case_0102[3 * 3] =
    {
         0,-1, 0,
        -1, 5,-1,
         0,-1, 0,
    };

    int16_pixel_t exp_data_test_case_0101[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   1,   1,   1,   1,   1,   1,   0,
        0,   2,   2,   2,   2,   2,   2,   0,
        0,   3,   3,   3,   3,   3,   3,   0,
        0,   4,   4,   4,   4,   4,   4,   0,
        0,   5,   5,   5,   5,   5,   5,   0,
        0,   6,   6,   6,   6,   6,   6,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    int16_pixel_t exp_data_test_case_0102[8 * 8] =
    {
         0,   -1,  -1,  -1,  -1,  -1,  -1,   0,
        -1,    2,   1,   1,   1,   1,   2,  -1,
        -2,    4,   2,   2,   2,   2,   4,  -2,
        -3,    6,   3,   3,   3,   3,   6,  -3,
        -4,    8,   4,   4,   4,   4,   8,  -4,
        -5,   10,   5,   5,   5,   5,  10,  -5,
        -6,   19,  13,  13,  13,  13,  19,  -6,
         0,   -6,  -6,  -6,  -6,  -6,  -6,   0,
    };

    int16_pixel_t src_data_test_case_02[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    int16_pixel_t msk_data_test_case_0201[3 * 3] =
    {
        -1,-1,-1,
        -1, 8,-1,
        -1,-1,-1,
    };

    int16_pixel_t msk_data_test_case_0202[3 * 3] =
    {
         0,-1, 0,
        -1, 5,-1,
         0,-1, 0,
    };

    int16_pixel_t exp_data_test_case_0201[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,  -1,  -2,  -2,  -1,   0,   0,
        0,   0,  -2,   5,   5,  -2,   0,   0,
        0,   0,  -2,   5,   5,  -2,   0,   0,
        0,   0,  -1,  -2,  -2,  -1,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    int16_pixel_t exp_data_test_case_0202[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,  -1,  -1,   0,   0,   0,
        0,   0,  -1,   3,   3,  -1,   0,   0,
        0,   0,  -1,   3,   3,  -1,   0,   0,
        0,   0,   0,  -1,  -1,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    int16_pixel_t src_data_test_case_03[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    int16_pixel_t msk_data_test_case_0301[3 * 3] =
    {
        1,  1,  1,
        0,  0,  0,
        0,  0,  0,
    };

    int16_pixel_t msk_data_test_case_0302[3 * 3] =
    {
         0,  0, -1,
         0,  0, -1,
        -1, -1, -1,
    };

    int16_pixel_t exp_data_test_case_0301[8 * 8] =
    {
        0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     1,     2,     2,     1,     0,     0,
        0,     0,     1,     2,     2,     1,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,
    };

    int16_pixel_t exp_data_test_case_0302[8 * 8] =
    {
        0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,    -1,    -1,     0,     0,
        0,     0,     0,     0,    -2,    -2,     0,     0,
        0,     0,    -1,    -2,    -3,    -2,     0,     0,
        0,     0,    -1,    -2,    -2,    -1,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,
    };

    int16_pixel_t dst_data[8 * 8] =
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
        int16_pixel_t *src_data;
        int16_pixel_t *exp_data;
        int16_pixel_t *msk_data;
    }testcase_t;

    // Compose array of test cases
    testcase_t testcases[] =
    {
        {src_data_test_case_01, exp_data_test_case_0101,msk_data_test_case_0101},
        {src_data_test_case_01, exp_data_test_case_0102,msk_data_test_case_0102},
        {src_data_test_case_02, exp_data_test_case_0201,msk_data_test_case_0201},
        {src_data_test_case_02, exp_data_test_case_0202,msk_data_test_case_0202},
        {src_data_test_case_03, exp_data_test_case_0301,msk_data_test_case_0301},
        {src_data_test_case_03, exp_data_test_case_0302,msk_data_test_case_0302},
    };

    // Prepare images
    image_t src = {8,8, IMGTYPE_INT16, NULL};
    image_t msk = {3,3, IMGTYPE_INT16, NULL};
    image_t exp = {8,8, IMGTYPE_INT16, NULL};
    image_t dst = {8,8, IMGTYPE_INT16, (uint8_t *)dst_data};

    // Loop all test cases
    for(uint32_t i=0; i < (sizeof(testcases) / sizeof(testcase_t)); ++i)
    {
        // Set the data
        src.data = (uint8_t *)testcases[i].src_data;
        msk.data = (uint8_t *)testcases[i].msk_data;
        exp.data = (uint8_t *)testcases[i].exp_data;

        // Execute the operator
        convolve(&src, &dst, &msk);

        // Set test case name
        char name[80] = "";
        sprintf(name, "Test case %d of %d", i+1, (uint32_t)(sizeof(testcases) / sizeof(testcase_t)));

#if 0
        // Print testcase info
        printf("\n---------------------------------------\n");
        printf("%s\n", name);

        // Print image data
        prettyprint(&src, "src");
        prettyprint(&msk, "msk");
        prettyprint(&exp, "exp");
        prettyprint(&dst, "dst");

#endif

        // Verify the result
        TEST_ASSERT_EQUAL_int16_pixel_t_ARRAY_MESSAGE(exp.data, dst.data, (exp.cols * exp.rows), name);
        TEST_ASSERT_EQUAL_MESSAGE(exp.type, dst.type, name);
        TEST_ASSERT_EQUAL_MESSAGE(exp.cols, dst.cols, name);
        TEST_ASSERT_EQUAL_MESSAGE(exp.rows, dst.rows, name);
    }
}

void test_convolveFast(void)
{
    // Prepare images for testing
    int16_pixel_t src_data_test_case_01[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   1,   1,   1,   1,   1,   1,   0,
        0,   2,   2,   2,   2,   2,   2,   0,
        0,   3,   3,   3,   3,   3,   3,   0,
        0,   4,   4,   4,   4,   4,   4,   0,
        0,   5,   5,   5,   5,   5,   5,   0,
        0,   6,   6,   6,   6,   6,   6,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    int16_pixel_t msk_data_test_case_0101[3 * 3] =
    {
        0, 0, 0,
        0, 1, 0,
        0, 0, 0,
    };

    int16_pixel_t msk_data_test_case_0102[3 * 3] =
    {
         0,-1, 0,
        -1, 5,-1,
         0,-1, 0,
    };

    int16_pixel_t exp_data_test_case_0101[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   1,   1,   1,   1,   1,   1,   0,
        0,   2,   2,   2,   2,   2,   2,   0,
        0,   3,   3,   3,   3,   3,   3,   0,
        0,   4,   4,   4,   4,   4,   4,   0,
        0,   5,   5,   5,   5,   5,   5,   0,
        0,   6,   6,   6,   6,   6,   6,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    int16_pixel_t exp_data_test_case_0102[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   2,   1,   1,   1,   1,   2,   0,
        0,   4,   2,   2,   2,   2,   4,   0,
        0,   6,   3,   3,   3,   3,   6,   0,
        0,   8,   4,   4,   4,   4,   8,   0,
        0,  10,   5,   5,   5,   5,  10,   0,
        0,  19,  13,  13,  13,  13,  19,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    int16_pixel_t src_data_test_case_02[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    int16_pixel_t msk_data_test_case_0201[3 * 3] =
    {
        -1,-1,-1,
        -1, 8,-1,
        -1,-1,-1,
    };

    int16_pixel_t msk_data_test_case_0202[3 * 3] =
    {
         0,-1, 0,
        -1, 5,-1,
         0,-1, 0,
    };

    int16_pixel_t exp_data_test_case_0201[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,  -1,  -2,  -2,  -1,   0,   0,
        0,   0,  -2,   5,   5,  -2,   0,   0,
        0,   0,  -2,   5,   5,  -2,   0,   0,
        0,   0,  -1,  -2,  -2,  -1,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0
    };

    int16_pixel_t exp_data_test_case_0202[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,  -1,  -1,   0,   0,   0,
        0,   0,  -1,   3,   3,  -1,   0,   0,
        0,   0,  -1,   3,   3,  -1,   0,   0,
        0,   0,   0,  -1,  -1,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    int16_pixel_t src_data_test_case_03[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    int16_pixel_t msk_data_test_case_0301[3 * 3] =
    {
        1,  1,  1,
        0,  0,  0,
        0,  0,  0,
    };

    int16_pixel_t msk_data_test_case_0302[3 * 3] =
    {
        0,  0, -1,
        0,  0, -1,
        -1, -1, -1,
    };

    int16_pixel_t exp_data_test_case_0301[8 * 8] =
    {
        0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     1,     2,     2,     1,     0,     0,
        0,     0,     1,     2,     2,     1,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,
    };

    int16_pixel_t exp_data_test_case_0302[8 * 8] =
    {
        0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,    -1,    -1,     0,     0,
        0,     0,     0,     0,    -2,    -2,     0,     0,
        0,     0,    -1,    -2,    -3,    -2,     0,     0,
        0,     0,    -1,    -2,    -2,    -1,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,
    };

    int16_pixel_t dst_data[8 * 8] =
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
        int16_pixel_t *src_data;
        int16_pixel_t *exp_data;
        int16_pixel_t *msk_data;
    }testcase_t;

    // Compose array of test cases
    testcase_t testcases[] =
    {
        {src_data_test_case_01, exp_data_test_case_0101,msk_data_test_case_0101},
        {src_data_test_case_01, exp_data_test_case_0102,msk_data_test_case_0102},
        {src_data_test_case_02, exp_data_test_case_0201,msk_data_test_case_0201},
        {src_data_test_case_02, exp_data_test_case_0202,msk_data_test_case_0202},
        {src_data_test_case_03, exp_data_test_case_0301,msk_data_test_case_0301},
        {src_data_test_case_03, exp_data_test_case_0302,msk_data_test_case_0302},
    };

    // Prepare images
    image_t src = {8,8, IMGTYPE_INT16, NULL};
    image_t msk = {3,3, IMGTYPE_INT16, NULL};
    image_t exp = {8,8, IMGTYPE_INT16, NULL};
    image_t dst = {8,8, IMGTYPE_INT16, (uint8_t *)dst_data};

    // Loop all test cases
    for(uint32_t i=0; i < (sizeof(testcases) / sizeof(testcase_t)); ++i)
    {
        // Set the data
        src.data = (uint8_t *)testcases[i].src_data;
        msk.data = (uint8_t *)testcases[i].msk_data;
        exp.data = (uint8_t *)testcases[i].exp_data;

        // Execute the operator
        convolveFast(&src, &dst, &msk);

        // Set test case name
        char name[80] = "";
        sprintf(name, "Test case %d of %d", i+1, (uint32_t)(sizeof(testcases) / sizeof(testcase_t)));

#if 0
        // Print testcase info
        printf("\n---------------------------------------\n");
        printf("%s\n", name);

        // Print image data
        prettyprint(&src, "src");
        prettyprint(&msk, "msk");
        prettyprint(&exp, "exp");
        prettyprint(&dst, "dst");

#endif

        // Verify the result
        TEST_ASSERT_EQUAL_int16_pixel_t_ARRAY_MESSAGE(exp.data, dst.data, (exp.cols * exp.rows), name);
        TEST_ASSERT_EQUAL_MESSAGE(exp.type, dst.type, name);
        TEST_ASSERT_EQUAL_MESSAGE(exp.cols, dst.cols, name);
        TEST_ASSERT_EQUAL_MESSAGE(exp.rows, dst.rows, name);
    }
}
