/*! ***************************************************************************
 *
 * \brief     Unit test functions for nonlinear filters
 * \file      test_nonlinear_filters.c
 * \author    Hugo Arends - HAN Embedded Vision and Machine Learning
 * \author    Renzo Broekman
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

void test_harmonic(void) {
    // Prepare images for testing
    uint8_pixel_t src_data_test_case_01[8 * 8] = {
        1,   2,   3,   4,   5,   6,   7,   8,
        9,   10,  11,  12,  13,  14,  15,  16,
        17,  18,  19,  20,  21,  22,  23,  24,
        25,  26,  27,  28,  29,  30,  31,  32,
        33,  34,  35,  36,  37,  38,  39,  40,
        41,  42,  43,  44,  45,  46,  47,  48,
        49,  50,  51,  52,  53,  54,  55,  56,
        57,  58,  59,  60,  61,  62,  63,  64,
    };

    uint8_pixel_t exp_data_test_case_01[8 * 8] = {
        2,   3,   4,   6,   7,   8,   9,  10,
        3,   4,   6,   8,   9,  10,  12,  12,
       15,  15,  17,  18,  19,  20,  21,  22,
       24,  24,  25,  26,  27,  29,  30,  30,
       32,  33,  34,  35,  36,  37,  38,  38,
       40,  41,  42,  43,  44,  45,  46,  47,
       49,  49,  50,  51,  52,  53,  54,  55,
       53,  54,  55,  56,  57,  58,  59,  59,
    };

    uint8_pixel_t src_data_test_case_02[8 * 8] = {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   1,   1,  30,  30,   1,   1,   0,
        0,   1,   1,  30,  30,   1,   1,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_pixel_t exp_data_test_case_02[8 * 8] = {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   2,   2,   0,   0,   0,
        0,   0,   0,   2,   2,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_pixel_t exp_data_test_case_03[8 * 8] = {
        4,   4,   5,   7,   9,  10,  11,  12,
        5,   6,   6,   9,  11,  12,  13,  14,
        6,   7,   7,  10,  12,  14,  15,  16,
       20,  21,  21,  23,  24,  25,  26,  26,
       30,  30,  31,  32,  33,  34,  35,  35,
       39,  39,  40,  41,  42,  43,  44,  44,
       44,  45,  45,  46,  47,  48,  49,  49,
       49,  50,  50,  51,  52,  53,  54,  54,
    };

    uint8_pixel_t exp_data_test_case_04[8 * 8] = {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_pixel_t dst_data[8 * 8] = {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    typedef struct testcase_t {
        uint8_pixel_t *src_data;
        uint8_pixel_t *exp_data;
        uint8_t n;
    } testcase_t;

    // Compose array of test cases
    testcase_t testcases[] = {
        {src_data_test_case_01, exp_data_test_case_01, 3},
        {src_data_test_case_02, exp_data_test_case_02, 3},
        {src_data_test_case_01, exp_data_test_case_03, 5},
        {src_data_test_case_02, exp_data_test_case_04, 5},
    };

    // Prepare images
    image_t src = {8, 8, IMGTYPE_UINT8, NULL};
    image_t exp = {8, 8, IMGTYPE_UINT8, NULL};
    image_t dst = {8, 8, IMGTYPE_UINT8, dst_data};

    // Loop all test cases
    for (uint32_t i = 0; i < (sizeof(testcases) / sizeof(testcase_t)); ++i) {
        // Set the data
        src.data = testcases[i].src_data;
        exp.data = testcases[i].exp_data;

        // Execute the operator
        harmonic(&src, &dst, testcases[i].n);

        // Set test case name
        char name[80] = "";
        sprintf(name, "Test case %d of %d", i + 1, (uint32_t)(sizeof(testcases) / sizeof(testcase_t)));

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

void test_mean(void) {
    // Prepare images for testing
    uint8_pixel_t src_data_test_case_01[8 * 8] = {
        1,   2,   3,   4,   5,   6,   7,   8,
        9,   10,  11,  12,  13,  14,  15,  16,
        17,  18,  19,  20,  21,  22,  23,  24,
        25,  26,  27,  28,  29,  30,  31,  32,
        33,  34,  35,  36,  37,  38,  39,  40,
        41,  42,  43,  44,  45,  46,  47,  48,
        49,  50,  51,  52,  53,  54,  55,  56,
        57,  58,  59,  60,  61,  62,  63,  64,
    };

    uint8_pixel_t exp_data_test_case_01[8 * 8] = {
        6,   6,   7,   8,   9,  10,  11,  12,
       10,  10,  11,  12,  13,  14,  15,  16,
       18,  18,  19,  20,  21,  22,  23,  24,
       26,  26,  27,  28,  29,  30,  31,  32,
       34,  34,  35,  36,  37,  38,  39,  40,
       42,  42,  43,  44,  45,  46,  47,  48,
       50,  50,  51,  52,  53,  54,  55,  56,
       54,  54,  55,  56,  57,  58,  59,  60,
    };

    uint8_pixel_t src_data_test_case_02[8 * 8] = {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,  30,  30,   0,   0,   0,
        0,   0,   0,  30,  30,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_pixel_t exp_data_test_case_02[8 * 8] = {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   3,   7,   7,   3,   0,   0,
        0,   0,   7,  13,  13,   7,   0,   0,
        0,   0,   7,  13,  13,   7,   0,   0,
        0,   0,   3,   7,   7,   3,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_pixel_t exp_data_test_case_03[8 * 8] = {
        10,  11,  11,  12,  13,  14,  15,  15,
        14,  15,  15,  16,  17,  18,  19,  19,
        18,  19,  19,  20,  21,  22,  23,  23,
        26,  27,  27,  28,  29,  30,  31,  31,
        34,  35,  35,  36,  37,  38,  39,  39,
        42,  43,  43,  44,  45,  46,  47,  47,
        46,  47,  47,  48,  49,  50,  51,  51,
        50,  51,  51,  52,  53,  54,  55,  55,
    };

    uint8_pixel_t exp_data_test_case_04[8 * 8] = {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   2,   3,   3,   3,   3,   2,   0,
        0,   3,   5,   5,   5,   5,   3,   0,
        0,   3,   5,   5,   5,   5,   3,   0,
        0,   3,   5,   5,   5,   5,   3,   0,
        0,   3,   5,   5,   5,   5,   3,   0,
        0,   2,   3,   3,   3,   3,   2,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_pixel_t dst_data[8 * 8] = {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };


    typedef struct testcase_t {
        uint8_pixel_t *src_data;
        uint8_pixel_t *exp_data;
        uint8_t n;
    } testcase_t;

    // Compose array of test cases
    testcase_t testcases[] = {
        {src_data_test_case_01, exp_data_test_case_01, 3},
        {src_data_test_case_02, exp_data_test_case_02, 3},
        {src_data_test_case_01, exp_data_test_case_03, 5},
        {src_data_test_case_02, exp_data_test_case_04, 5},
    };

    // Prepare images
    image_t src = {8, 8, IMGTYPE_UINT8, NULL};
    image_t exp = {8, 8, IMGTYPE_UINT8, NULL};
    image_t dst = {8, 8, IMGTYPE_UINT8, dst_data};

    // Loop all test cases
    for (uint32_t i = 0; i < (sizeof(testcases) / sizeof(testcase_t)); ++i) {
        // Set the data
        src.data = testcases[i].src_data;
        exp.data = testcases[i].exp_data;

        // Execute the operator
        mean(&src, &dst, testcases[i].n);

        // Set test case name
        char name[80] = "";
        sprintf(name, "Test case %d of %d", i + 1, (uint32_t)(sizeof(testcases) / sizeof(testcase_t)));

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

void test_meanFast(void) {
    // Prepare images for testing
    uint8_pixel_t src_data_test_case_01[8 * 8] = {
        1,   2,   3,   4,   5,   6,   7,   8,
        9,   10,  11,  12,  13,  14,  15,  16,
        17,  18,  19,  20,  21,  22,  23,  24,
        25,  26,  27,  28,  29,  30,  31,  32,
        33,  34,  35,  36,  37,  38,  39,  40,
        41,  42,  43,  44,  45,  46,  47,  48,
        49,  50,  51,  52,  53,  54,  55,  56,
        57,  58,  59,  60,  61,  62,  63,  64,
    };

    uint8_pixel_t exp_data_test_case_01[8 * 8] = {
         0,   0,   0,   0,   0,   0,   0,   0,
         0,  10,  11,  12,  13,  14,  15,   0,
         0,  18,  19,  20,  21,  22,  23,   0,
         0,  26,  27,  28,  29,  30,  31,   0,
         0,  34,  35,  36,  37,  38,  39,   0,
         0,  42,  43,  44,  45,  46,  47,   0,
         0,  50,  51,  52,  53,  54,  55,   0,
         0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_pixel_t src_data_test_case_02[8 * 8] = {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,  30,  30,   0,   0,   0,
        0,   0,   0,  30,  30,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_pixel_t exp_data_test_case_02[8 * 8] = {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   3,   7,   7,   3,   0,   0,
        0,   0,   7,  13,  13,   7,   0,   0,
        0,   0,   7,  13,  13,   7,   0,   0,
        0,   0,   3,   7,   7,   3,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_pixel_t src_data_test_case_03[8 * 8] = {
        1,   1,   1,   0,   0,   2,   2,   2,
        1,   1,   1,   0,   0,   2,   2,   2,
        1,   1,   1,   0,   0,   2,   2,   2,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        4,   4,   4,   0,   0,   3,   3,   3,
        4,   4,   4,   0,   0,   3,   3,   3,
        4,   4,   4,   0,   0,   3,   3,   3,
    };

    uint8_pixel_t exp_data_test_case_03[8 * 8] = {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   1,   1,   0,   1,   1,   2,   0,
        0,   1,   0,   0,   0,   1,   1,   0,
        0,   0,   0,   0,   0,   0,   1,   0,
        0,   1,   1,   0,   0,   1,   1,   0,
        0,   3,   2,   1,   1,   1,   2,   0,
        0,   4,   3,   1,   1,   2,   3,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_pixel_t dst_data[8 * 8] = {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };


    typedef struct testcase_t {
        uint8_pixel_t *src_data;
        uint8_pixel_t *exp_data;
    } testcase_t;

    // Compose array of test cases
    testcase_t testcases[] = {
        {src_data_test_case_01, exp_data_test_case_01},
        {src_data_test_case_02, exp_data_test_case_02},
        {src_data_test_case_03, exp_data_test_case_03},
    };

    // Prepare images
    image_t src = {8, 8, IMGTYPE_UINT8, NULL};
    image_t exp = {8, 8, IMGTYPE_UINT8, NULL};
    image_t dst = {8, 8, IMGTYPE_UINT8, dst_data};

    // Loop all test cases
    for (uint32_t i = 0; i < (sizeof(testcases) / sizeof(testcase_t)); ++i) {
        // Set the data
        src.data = testcases[i].src_data;
        exp.data = testcases[i].exp_data;

        clearUint8Image(&dst);

        // Execute the operator
        meanFast(&src, &dst);

        // Set test case name
        char name[80] = "";
        sprintf(name, "Test case %d of %d", i + 1, (uint32_t)(sizeof(testcases) / sizeof(testcase_t)));

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

void test_minimum(void) {
    // Prepare images for testing
    uint8_pixel_t src_data_test_case_01[8 * 8] = {
        1,   2,   3,   4,   5,   6,   7,   8,
        9,   10,  11,  12,  13,  14,  15,  16,
        17,  18,  19,  20,  21,  22,  23,  24,
        25,  26,  27,  28,  29,  30,  31,  32,
        33,  34,  35,  36,  37,  38,  39,  40,
        41,  42,  43,  44,  45,  46,  47,  48,
        49,  50,  51,  52,  53,  54,  55,  56,
        57,  58,  59,  60,  61,  62,  63,  64,
    };

    uint8_pixel_t exp_data_test_case_01[8 * 8] = {
        1,   1,   2,   3,   4,   5,   6,   7,
        1,   1,   2,   3,   4,   5,   6,   7,
        9,   9,  10,  11,  12,  13,  14,  15,
       17,  17,  18,  19,  20,  21,  22,  23,
       25,  25,  26,  27,  28,  29,  30,  31,
       33,  33,  34,  35,  36,  37,  38,  39,
       41,  41,  42,  43,  44,  45,  46,  47,
       49,  49,  50,  51,  52,  53,  54,  55,
    };

    uint8_pixel_t src_data_test_case_02[8 * 8] = {
        0,   0,   1,   5,   5,   1,   0,   0,
        0,   1,   5,  15,  15,   5,   1,   0,
        1,   5,  15,  25,  25,  15,   5,   1,
        5,  15,  25,  30,  30,  25,  15,   5,
        5,  15,  25,  30,  30,  25,  15,   5,
        1,   5,  15,  25,  25,  15,   5,   1,
        0,   1,   5,  15,  15,   5,   1,   0,
        0,   0,   1,   5,   5,   1,   0,   0,
    };

    uint8_pixel_t exp_data_test_case_02[8 * 8] = {
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   1,   5,   5,   1,   0,   0,
        1,   1,   5,  15,  15,   5,   1,   1,
        1,   1,   5,  15,  15,   5,   1,   1,
        0,   0,   1,   5,   5,   1,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
    };


    uint8_pixel_t exp_data_test_case_03[8 * 8] = {
        1,   1,   1,   2,   3,   4,   5,   6,
        1,   1,   1,   2,   3,   4,   5,   6,
        1,   1,   1,   2,   3,   4,   5,   6,
        9,   9,   9,  10,  11,  12,  13,  14,
       17,  17,  17,  18,  19,  20,  21,  22,
       25,  25,  25,  26,  27,  28,  29,  30,
       33,  33,  33,  34,  35,  36,  37,  38,
       41,  41,  41,  42,  43,  44,  45,  46,
    };

    uint8_pixel_t exp_data_test_case_04[8 * 8] = {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_pixel_t dst_data[8 * 8] = {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    typedef struct testcase_t {
        uint8_pixel_t *src_data;
        uint8_pixel_t *exp_data;
        uint8_t n;
    } testcase_t;

    // Compose array of test cases
    testcase_t testcases[] = {
        {src_data_test_case_01, exp_data_test_case_01, 3},
        {src_data_test_case_02, exp_data_test_case_02, 3},
        {src_data_test_case_01, exp_data_test_case_03, 5},
        {src_data_test_case_02, exp_data_test_case_04, 5},
    };

    // Prepare images
    image_t src = {8, 8, IMGTYPE_UINT8, NULL};
    image_t exp = {8, 8, IMGTYPE_UINT8, NULL};
    image_t dst = {8, 8, IMGTYPE_UINT8, dst_data};

    // Loop all test cases
    for (uint32_t i = 0; i < (sizeof(testcases) / sizeof(testcase_t)); ++i) {
        // Set the data
        src.data = testcases[i].src_data;
        exp.data = testcases[i].exp_data;

        // Execute the operator
        minimum(&src, &dst, testcases[i].n);

        // Set test case name
        char name[80] = "";
        sprintf(name, "Test case %d of %d", i + 1, (uint32_t)(sizeof(testcases) / sizeof(testcase_t)));

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

void test_maximum(void) {
    // Prepare images for testing
    uint8_pixel_t src_data_test_case_01[8 * 8] = {
        1,   2,   3,   4,   5,   6,   7,   8,
        9,   10,  11,  12,  13,  14,  15,  16,
        17,  18,  19,  20,  21,  22,  23,  24,
        25,  26,  27,  28,  29,  30,  31,  32,
        33,  34,  35,  36,  37,  38,  39,  40,
        41,  42,  43,  44,  45,  46,  47,  48,
        49,  50,  51,  52,  53,  54,  55,  56,
        57,  58,  59,  60,  61,  62,  63,  64,
    };

    uint8_pixel_t exp_data_test_case_01[8 * 8] = {
        10,  11,  12,  13,  14,  15,  16,  16,
        18,  19,  20,  21,  22,  23,  24,  24,
        26,  27,  28,  29,  30,  31,  32,  32,
        34,  35,  36,  37,  38,  39,  40,  40,
        42,  43,  44,  45,  46,  47,  48,  48,
        50,  51,  52,  53,  54,  55,  56,  56,
        58,  59,  60,  61,  62,  63,  64,  64,
        58,  59,  60,  61,  62,  63,  64,  64,
    };

    uint8_pixel_t src_data_test_case_02[8 * 8] = {
        0,   0,   1,   5,   5,   1,   0,   0,
        0,   1,   5,  15,  15,   5,   1,   0,
        1,   5,  15,  25,  25,  15,   5,   1,
        5,  15,  25,  30,  30,  25,  15,   5,
        5,  15,  25,  30,  30,  25,  15,   5,
        1,   5,  15,  25,  25,  15,   5,   1,
        0,   1,   5,  15,  15,   5,   1,   0,
        0,   0,   1,   5,   5,   1,   0,   0,
    };

    uint8_pixel_t exp_data_test_case_02[8 * 8] = {
        1,   5,  15,  15,  15,  15,   5,   1,
        5,  15,  25,  25,  25,  25,  15,   5,
       15,  25,  30,  30,  30,  30,  25,  15,
       15,  25,  30,  30,  30,  30,  25,  15,
       15,  25,  30,  30,  30,  30,  25,  15,
       15,  25,  30,  30,  30,  30,  25,  15,
        5,  15,  25,  25,  25,  25,  15,   5,
        1,   5,  15,  15,  15,  15,   5,   1,
    };

    uint8_pixel_t exp_data_test_case_03[8 * 8] = {
        19,  20,  21,  22,  23,  24,  24,  24,
        27,  28,  29,  30,  31,  32,  32,  32,
        35,  36,  37,  38,  39,  40,  40,  40,
        43,  44,  45,  46,  47,  48,  48,  48,
        51,  52,  53,  54,  55,  56,  56,  56,
        59,  60,  61,  62,  63,  64,  64,  64,
        59,  60,  61,  62,  63,  64,  64,  64,
        59,  60,  61,  62,  63,  64,  64,  64,
    };

    uint8_pixel_t exp_data_test_case_04[8 * 8] = {
        15,  25,  25,  25,  25,  25,  25,  15,
        25,  30,  30,  30,  30,  30,  30,  25,
        25,  30,  30,  30,  30,  30,  30,  25,
        25,  30,  30,  30,  30,  30,  30,  25,
        25,  30,  30,  30,  30,  30,  30,  25,
        25,  30,  30,  30,  30,  30,  30,  25,
        25,  30,  30,  30,  30,  30,  30,  25,
        15,  25,  25,  25,  25,  25,  25,  15,
    };

    uint8_pixel_t dst_data[8 * 8] = {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    typedef struct testcase_t {
        uint8_pixel_t *src_data;
        uint8_pixel_t *exp_data;
        uint8_t n;
    } testcase_t;

    // Compose array of test cases
    testcase_t testcases[] = {
        {src_data_test_case_01, exp_data_test_case_01, 3},
        {src_data_test_case_02, exp_data_test_case_02, 3},
        {src_data_test_case_01, exp_data_test_case_03, 5},
        {src_data_test_case_02, exp_data_test_case_04, 5},
    };

    // Prepare images
    image_t src = {8, 8, IMGTYPE_UINT8, NULL};
    image_t exp = {8, 8, IMGTYPE_UINT8, NULL};
    image_t dst = {8, 8, IMGTYPE_UINT8, dst_data};

    // Loop all test cases
    for (uint32_t i = 0; i < (sizeof(testcases) / sizeof(testcase_t)); ++i) {
        // Set the data
        src.data = testcases[i].src_data;
        exp.data = testcases[i].exp_data;

        // Execute the operator
        maximum(&src, &dst, testcases[i].n);

        // Set test case name
        char name[80] = "";
        sprintf(name, "Test case %d of %d", i + 1, (uint32_t)(sizeof(testcases) / sizeof(testcase_t)));

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

void test_range(void) {
    // Prepare images for testing
    uint8_pixel_t src_data_test_case_01[8 * 8] = {
        1,   2,   3,   4,   5,   6,   7,   8,
        9,   10,  11,  12,  13,  14,  15,  16,
        17,  18,  19,  20,  21,  22,  23,  24,
        25,  26,  27,  28,  29,  30,  31,  32,
        33,  34,  35,  36,  37,  38,  39,  40,
        41,  42,  43,  44,  45,  46,  47,  48,
        49,  50,  51,  52,  53,  54,  55,  56,
        57,  58,  59,  60,  61,  62,  63,  64,
    };

    uint8_pixel_t exp_data_test_case_01[8 * 8] = {
        9,  10,  10,  10,  10,  10,  10,   9,
       17,  18,  18,  18,  18,  18,  18,  17,
       17,  18,  18,  18,  18,  18,  18,  17,
       17,  18,  18,  18,  18,  18,  18,  17,
       17,  18,  18,  18,  18,  18,  18,  17,
       17,  18,  18,  18,  18,  18,  18,  17,
       17,  18,  18,  18,  18,  18,  18,  17,
        9,  10,  10,  10,  10,  10,  10,   9,
    };

    uint8_pixel_t src_data_test_case_02[8 * 8] = {
        0,   0,   1,   5,   5,   1,   0,   0,
        0,   1,   5,  15,  15,   5,   1,   0,
        1,   5,  15,  25,  25,  15,   5,   1,
        5,  15,  25,  30,  30,  25,  15,   5,
        5,  15,  25,  30,  30,  25,  15,   5,
        1,   5,  15,  25,  25,  15,   5,   1,
        0,   1,   5,  15,  15,   5,   1,   0,
        0,   0,   1,   5,   5,   1,   0,   0,
    };

    uint8_pixel_t exp_data_test_case_02[8 * 8] = {
        1,   5,  15,  14,  14,  15,   5,   1,
        5,  15,  25,  24,  24,  25,  15,   5,
       15,  25,  29,  25,  25,  29,  25,  15,
       14,  24,  25,  15,  15,  25,  24,  14,
       14,  24,  25,  15,  15,  25,  24,  14,
       15,  25,  29,  25,  25,  29,  25,  15,
        5,  15,  25,  24,  24,  25,  15,   5,
        1,   5,  15,  14,  14,  15,   5,   1,
    };

    uint8_pixel_t exp_data_test_case_03[8 * 8] = {
        18,  19,  20,  20,  20,  20,  19,  18,
        26,  27,  28,  28,  28,  28,  27,  26,
        34,  35,  36,  36,  36,  36,  35,  34,
        34,  35,  36,  36,  36,  36,  35,  34,
        34,  35,  36,  36,  36,  36,  35,  34,
        34,  35,  36,  36,  36,  36,  35,  34,
        26,  27,  28,  28,  28,  28,  27,  26,
        18,  19,  20,  20,  20,  20,  19,  18,
    };

    uint8_pixel_t exp_data_test_case_04[8 * 8] = {
        15,  25,  25,  25,  25,  25,  25,  15,
        25,  30,  30,  30,  30,  30,  30,  25,
        25,  30,  30,  30,  30,  30,  30,  25,
        25,  30,  30,  29,  29,  30,  30,  25,
        25,  30,  30,  29,  29,  30,  30,  25,
        25,  30,  30,  30,  30,  30,  30,  25,
        25,  30,  30,  30,  30,  30,  30,  25,
        15,  25,  25,  25,  25,  25,  25,  15,
    };

    uint8_pixel_t dst_data[8 * 8] = {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };


    typedef struct testcase_t {
        uint8_pixel_t *src_data;
        uint8_pixel_t *exp_data;
        uint8_t n;
    } testcase_t;

    // Compose array of test cases
    testcase_t testcases[] = {
        {src_data_test_case_01, exp_data_test_case_01, 3},
        {src_data_test_case_02, exp_data_test_case_02, 3},
        {src_data_test_case_01, exp_data_test_case_03, 5},
        {src_data_test_case_02, exp_data_test_case_04, 5},
    };

    // Prepare images
    image_t src = {8, 8, IMGTYPE_UINT8, NULL};
    image_t exp = {8, 8, IMGTYPE_UINT8, NULL};
    image_t dst = {8, 8, IMGTYPE_UINT8, dst_data};

    // Loop all test cases
    for (uint32_t i = 0; i < (sizeof(testcases) / sizeof(testcase_t)); ++i) {
        // Set the data
        src.data = testcases[i].src_data;
        exp.data = testcases[i].exp_data;

        // Execute the operator
        range(&src, &dst, testcases[i].n);

        // Set test case name
        char name[80] = "";
        sprintf(name, "Test case %d of %d", i + 1, (uint32_t)(sizeof(testcases) / sizeof(testcase_t)));

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

void test_median() {
    // Prepare images for testing
    uint8_pixel_t src_data_test_case_01[8 * 8] = {
        1,   2,   3,   4,   5,   6,   7,   8,
        9,   10,  11,  12,  13,  14,  15,  16,
        17,  18,  19,  20,  21,  22,  23,  24,
        25,  26,  27,  28,  29,  30,  31,  32,
        33,  34,  35,  36,  37,  38,  39,  40,
        41,  42,  43,  44,  45,  46,  47,  48,
        49,  50,  51,  52,  53,  54,  55,  56,
        57,  58,  59,  60,  61,  62,  63,  64,
    };

    uint8_pixel_t exp_data_test_case_01[8 * 8] = {
        6,   6,   7,   8,   9,  10,  11,  12,
       10,  10,  11,  12,  13,  14,  15,  16,
       18,  18,  19,  20,  21,  22,  23,  24,
       26,  26,  27,  28,  29,  30,  31,  32,
       34,  34,  35,  36,  37,  38,  39,  40,
       42,  42,  43,  44,  45,  46,  47,  48,
       50,  50,  51,  52,  53,  54,  55,  56,
       54,  54,  55,  56,  57,  58,  59,  60,
    };

    uint8_pixel_t src_data_test_case_02[8 * 8] = {
        0,   0,   1,   5,   5,   1,   0,   0,
        0,   1,   5,  15,  15,   5,   1,   0,
        1,   5,  15,  25,  25,  15,   5,   1,
        5,  15,  25,  30,  30,  25,  15,   5,
        5,  15,  25,  30,  30,  25,  15,   5,
        1,   5,  15,  25,  25,  15,   5,   1,
        0,   1,   5,  15,  15,   5,   1,   0,
        0,   0,   1,   5,   5,   1,   0,   0,
    };

    uint8_pixel_t exp_data_test_case_02[8 * 8] = {
        0,   1,   3,   5,   5,   3,   1,   0,
        1,   1,   5,  15,  15,   5,   1,   1,
        3,   5,  15,  25,  25,  15,   5,   3,
        5,  15,  25,  25,  25,  25,  15,   5,
        5,  15,  25,  25,  25,  25,  15,   5,
        3,   5,  15,  25,  25,  15,   5,   3,
        1,   1,   5,  15,  15,   5,   1,   1,
        0,   1,   3,   5,   5,   3,   1,   0,
    };

    uint8_pixel_t exp_data_test_case_03[8 * 8] = {
        10,  11,  11,  12,  13,  14,  15,  15,
        14,  15,  15,  16,  17,  18,  19,  19,
        18,  19,  19,  20,  21,  22,  23,  23,
        26,  27,  27,  28,  29,  30,  31,  31,
        34,  35,  35,  36,  37,  38,  39,  39,
        42,  43,  43,  44,  45,  46,  47,  47,
        46,  47,  47,  48,  49,  50,  51,  51,
        50,  51,  51,  52,  53,  54,  55,  55,
    };

    uint8_pixel_t exp_data_test_case_04[8 * 8] = {
        1,   3,   5,   5,   5,   5,   3,   1,
        3,   5,   5,  15,  15,   5,   5,   3,
        5,   5,  15,  15,  15,  15,   5,   5,
        5,  15,  15,  15,  15,  15,  15,   5,
        5,  15,  15,  15,  15,  15,  15,   5,
        5,   5,  15,  15,  15,  15,   5,   5,
        3,   5,   5,  15,  15,   5,   5,   3,
        1,   3,   5,   5,   5,   5,   3,   1,
    };

    uint8_pixel_t dst_data[8 * 8] = {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };


    typedef struct testcase_t {
        uint8_pixel_t *src_data;
        uint8_pixel_t *exp_data;
        uint8_t n;
    } testcase_t;

    // Compose array of test cases
    testcase_t testcases[] = {
        {src_data_test_case_01, exp_data_test_case_01, 3},
        {src_data_test_case_02, exp_data_test_case_02, 3},
        {src_data_test_case_01, exp_data_test_case_03, 5},
        {src_data_test_case_02, exp_data_test_case_04, 5},
    };

    // Prepare images
    image_t src = {8, 8, IMGTYPE_UINT8, NULL};
    image_t exp = {8, 8, IMGTYPE_UINT8, NULL};
    image_t dst = {8, 8, IMGTYPE_UINT8, dst_data};

    // Loop all test cases
    for (uint32_t i = 0; i < (sizeof(testcases) / sizeof(testcase_t)); ++i) {
        // Set the data
        src.data = testcases[i].src_data;
        exp.data = testcases[i].exp_data;

        // Execute the operator
        median(&src, &dst, testcases[i].n);

        // Set test case name
        char name[80] = "";
        sprintf(name, "Test case %d of %d", i + 1, (uint32_t)(sizeof(testcases) / sizeof(testcase_t)));

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

void test_midpoint() {
    // Prepare images for testing
    uint8_pixel_t src_data_test_case_01[8 * 8] = {
        1,   2,   3,   4,   5,   6,   7,   8,
        9,   10,  11,  12,  13,  14,  15,  16,
        17,  18,  19,  20,  21,  22,  23,  24,
        25,  26,  27,  28,  29,  30,  31,  32,
        33,  34,  35,  36,  37,  38,  39,  40,
        41,  42,  43,  44,  45,  46,  47,  48,
        49,  50,  51,  52,  53,  54,  55,  56,
        57,  58,  59,  60,  61,  62,  63,  64,
    };

    uint8_pixel_t exp_data_test_case_01[8 * 8] = {
        6,   6,   7,   8,   9,  10,  11,  12,
       10,  10,  11,  12,  13,  14,  15,  16,
       18,  18,  19,  20,  21,  22,  23,  24,
       26,  26,  27,  28,  29,  30,  31,  32,
       34,  34,  35,  36,  37,  38,  39,  40,
       42,  42,  43,  44,  45,  46,  47,  48,
       50,  50,  51,  52,  53,  54,  55,  56,
       54,  54,  55,  56,  57,  58,  59,  60,
    };

    uint8_pixel_t src_data_test_case_02[8 * 8] = {
        0,   0,   1,   5,   5,   1,   0,   0,
        0,   1,   5,  15,  15,   5,   1,   0,
        1,   5,  15,  25,  25,  15,   5,   1,
        5,  15,  25,  30,  30,  25,  15,   5,
        5,  15,  25,  30,  30,  25,  15,   5,
        1,   5,  15,  25,  25,  15,   5,   1,
        0,   1,   5,  15,  15,   5,   1,   0,
        0,   0,   1,   5,   5,   1,   0,   0,
    };

    uint8_pixel_t exp_data_test_case_02[8 * 8] = {
        1,   3,   8,   8,   8,   8,   3,   1,
        3,   8,  13,  13,  13,  13,   8,   3,
        8,  13,  16,  18,  18,  16,  13,   8,
        8,  13,  18,  23,  23,  18,  13,   8,
        8,  13,  18,  23,  23,  18,  13,   8,
        8,  13,  16,  18,  18,  16,  13,   8,
        3,   8,  13,  13,  13,  13,   8,   3,
        1,   3,   8,   8,   8,   8,   3,   1,
    };

    uint8_pixel_t exp_data_test_case_03[8 * 8] = {
        10,  11,  11,  12,  13,  14,  15,  15,
        14,  15,  15,  16,  17,  18,  19,  19,
        18,  19,  19,  20,  21,  22,  23,  23,
        26,  27,  27,  28,  29,  30,  31,  31,
        34,  35,  35,  36,  37,  38,  39,  39,
        42,  43,  43,  44,  45,  46,  47,  47,
        46,  47,  47,  48,  49,  50,  51,  51,
        50,  51,  51,  52,  53,  54,  55,  55,
    };

    uint8_pixel_t exp_data_test_case_04[8 * 8] = {
        8,  13,  13,  13,  13,  13,  13,   8,
       13,  15,  15,  15,  15,  15,  15,  13,
       13,  15,  15,  15,  15,  15,  15,  13,
       13,  15,  15,  16,  16,  15,  15,  13,
       13,  15,  15,  16,  16,  15,  15,  13,
       13,  15,  15,  15,  15,  15,  15,  13,
       13,  15,  15,  15,  15,  15,  15,  13,
        8,  13,  13,  13,  13,  13,  13,   8,
    };

    uint8_pixel_t dst_data[8 * 8] = {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    typedef struct testcase_t {
        uint8_pixel_t *src_data;
        uint8_pixel_t *exp_data;
        uint8_t n;
    } testcase_t;

    // Compose array of test cases
    testcase_t testcases[] = {
        {src_data_test_case_01, exp_data_test_case_01, 3},
        {src_data_test_case_02, exp_data_test_case_02, 3},
        {src_data_test_case_01, exp_data_test_case_03, 5},
        {src_data_test_case_02, exp_data_test_case_04, 5},
    };

    // Prepare images
    image_t src = {8, 8, IMGTYPE_UINT8, NULL};
    image_t exp = {8, 8, IMGTYPE_UINT8, NULL};
    image_t dst = {8, 8, IMGTYPE_UINT8, dst_data};

    // Loop all test cases
    for (uint32_t i = 0; i < (sizeof(testcases) / sizeof(testcase_t)); ++i) {
        // Set the data
        src.data = testcases[i].src_data;
        exp.data = testcases[i].exp_data;

        // Execute the operator
        midpoint(&src, &dst, testcases[i].n);

        // Set test case name
        char name[80] = "";
        sprintf(name, "Test case %d of %d", i + 1, (uint32_t)(sizeof(testcases) / sizeof(testcase_t)));

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
