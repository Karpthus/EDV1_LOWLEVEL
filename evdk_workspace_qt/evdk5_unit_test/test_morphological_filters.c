/*! ***************************************************************************
 *
 * \brief     Unit test functions for morphological filters
 * \file      test_morphological_filters.c
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

void test_dilation(void)
{
    // Prepare images for testing
    uint8_pixel_t src_data[8 * 8] =
    {
        0,   0,   1,   0,   0,   0,   0,   1,
        0,   1,   0,   0,   0,   0,   0,   1,
        1,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_t mask_test_case_01[9] =
    {
        1,1,0,
        1,1,1,
        0,1,1,
    };

    uint8_pixel_t exp_data_test_case_01[8 * 8] =
    {
        1,   1,   1,   1,   0,   0,   1,   1,
        1,   1,   1,   1,   0,   0,   1,   1,
        1,   1,   1,   1,   1,   1,   0,   1,
        1,   1,   1,   1,   1,   1,   1,   0,
        0,   1,   1,   1,   1,   1,   1,   0,
        0,   1,   1,   1,   1,   1,   1,   0,
        0,   1,   1,   1,   1,   1,   1,   0,
        0,   0,   1,   1,   1,   1,   1,   0,
    };

    uint8_t mask_test_case_02[9] =
    {
        1,0,0,
        0,1,0,
        0,0,0,
    };

    uint8_pixel_t exp_data_test_case_02[8 * 8] =
    {
        0,   0,   1,   0,   0,   0,   0,   1,
        0,   1,   0,   1,   0,   0,   0,   1,
        1,   0,   1,   0,   0,   0,   0,   0,
        0,   1,   1,   1,   1,   1,   0,   0,
        0,   0,   1,   1,   1,   1,   1,   0,
        0,   0,   1,   1,   1,   1,   1,   0,
        0,   0,   1,   1,   1,   1,   1,   0,
        0,   0,   0,   1,   1,   1,   1,   0,
    };

    uint8_t mask_test_case_03[25] =
    {
        0,0,0,0,1,
        0,0,0,1,0,
        0,0,1,0,0,
        0,1,0,0,0,
        1,0,0,0,0,
    };

    uint8_pixel_t exp_data_test_case_03[8 * 8] =
    {
        0,   0,   1,   0,   0,   0,   0,   1,
        0,   1,   0,   0,   1,   1,   1,   1,
        1,   0,   0,   1,   1,   1,   1,   1,
        0,   0,   1,   1,   1,   1,   1,   1,
        0,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   0,
        1,   1,   1,   1,   1,   1,   0,   0,
        1,   1,   1,   1,   1,   0,   0,   0,
    };

    uint8_t mask_test_case_04[25] =
    {
        0,0,0,0,0,
        0,0,0,0,0,
        0,0,0,0,0,
        0,0,0,0,0,
        1,0,0,0,0,
    };

    uint8_pixel_t exp_data_test_case_04[8 * 8] =
    {
        0,   0,   1,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   1,   1,   1,   1,
        0,   0,   0,   0,   1,   1,   1,   1,
        0,   0,   0,   0,   1,   1,   1,   1,
        0,   0,   0,   0,   1,   1,   1,   1,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
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
         uint8_t *mask;
         uint8_t n;
     }testcase_t;

     // Compose array of test cases
     testcase_t testcases[] = {
         {src_data, exp_data_test_case_01, mask_test_case_01, 3},
         {src_data, exp_data_test_case_02, mask_test_case_02, 3},
         {src_data, exp_data_test_case_03, mask_test_case_03, 5},
         {src_data, exp_data_test_case_04, mask_test_case_04, 5},
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
         dilation(&src, &dst, testcases[i].mask, testcases[i].n);

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

void test_dilationGray(void)
{
    // Prepare images for testing
    uint8_pixel_t src_data[8 * 8] =
    {
        0,   0,   1,   0,   0,   0,   0,   2,
        0,   1,   0,   0,   0,   0,   0,   2,
        1,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   5,   5,   5,   5,   0,   0,
        0,   0,   5,   2,   2,   5,   0,   0,
        0,   0,   5,   4,   4,   5,   0,   0,
        0,   0,   5,   5,   5,   5,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_t mask_test_case_01[9] =
    {
        2,2,0,
        2,1,3,
        0,3,3,
    };

    uint8_pixel_t exp_data_test_case_01[8 * 8] =
    {
        4,   4,   3,   3,   3,   3,   5,   5,
        4,   3,   3,   3,   3,   3,   5,   4,
        3,   8,   8,   8,   8,   8,   5,   4,
        3,   8,   8,   8,   8,   8,   7,   3,
        3,   8,   8,   7,   8,   8,   7,   3,
        3,   8,   8,   8,   8,   8,   7,   3,
        3,   8,   8,   8,   8,   7,   7,   3,
        3,   5,   7,   7,   7,   7,   7,   2,
    };

    uint8_t mask_test_case_02[9] =
    {
        1,0,0,
        0,1,0,
        0,0,0,
    };

    uint8_pixel_t exp_data_test_case_02[8 * 8] =
    {
        1,   1,   2,   1,   1,   1,   2,   3,
        1,   2,   1,   2,   1,   1,   2,   3,
        2,   5,   5,   5,   5,   5,   5,   2,
        1,   5,   6,   6,   6,   6,   5,   1,
        1,   5,   6,   6,   6,   6,   6,   1,
        1,   5,   6,   6,   5,   6,   6,   1,
        1,   5,   6,   6,   6,   6,   6,   1,
        1,   5,   5,   6,   6,   6,   6,   1,
    };

    uint8_t mask_test_case_03[25] =
    {
        0,0,0,0,1,
        0,0,0,1,0,
        0,0,1,0,0,
        0,1,0,0,0,
        1,0,0,0,0,
    };

    uint8_pixel_t exp_data_test_case_03[8 * 8] =
    {
        1,   1,   2,   1,   1,   2,   2,   3,
        5,   5,   5,   5,   6,   6,   6,   6,
        5,   5,   5,   6,   6,   6,   6,   6,
        5,   5,   6,   6,   6,   6,   6,   6,
        5,   6,   6,   6,   6,   6,   6,   6,
        6,   6,   6,   6,   6,   6,   6,   5,
        6,   6,   6,   6,   6,   6,   5,   5,
        6,   6,   6,   6,   6,   5,   5,   5,
    };

    uint8_t mask_test_case_04[25] =
    {
        0,0,0,0,0,
        0,0,0,0,0,
        0,0,0,0,0,
        0,0,0,0,0,
        1,0,0,0,0,
    };

    uint8_pixel_t exp_data_test_case_04[8 * 8] =
    {
        1,   1,   2,   1,   1,   2,   2,   2,
        5,   5,   5,   5,   6,   6,   6,   6,
        5,   5,   5,   5,   6,   5,   5,   6,
        5,   5,   5,   5,   6,   5,   5,   6,
        5,   5,   5,   5,   6,   6,   6,   6,
        5,   5,   5,   5,   5,   5,   5,   5,
        5,   5,   5,   5,   5,   5,   5,   5,
        5,   5,   5,   5,   5,   5,   5,   5,
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
         uint8_t *mask;
         uint8_t n;
     }testcase_t;

     // Compose array of test cases
     testcase_t testcases[] = {
         {src_data, exp_data_test_case_01, mask_test_case_01, 3},
         {src_data, exp_data_test_case_02, mask_test_case_02, 3},
         {src_data, exp_data_test_case_03, mask_test_case_03, 5},
         {src_data, exp_data_test_case_04, mask_test_case_04, 5},
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
         dilationGray(&src, &dst, testcases[i].mask, testcases[i].n);

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

void test_erosion(void)
{
    // Prepare images for testing
    uint8_pixel_t src_data[8 * 8] =
    {
        0,   0,   1,   0,   0,   0,   0,   1,
        0,   1,   0,   0,   0,   0,   0,   1,
        1,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_t mask_test_case_01[9] =
    {
        1,1,0,
        1,1,1,
        0,1,1,
    };

    uint8_pixel_t exp_data_test_case_01[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_t mask_test_case_02[9] =
    {
        1,0,0,
        0,1,0,
        0,0,0,
    };

    uint8_pixel_t exp_data_test_case_02[8 * 8] =
    {
        0,   0,   1,   0,   0,   0,   0,   1,
        0,   0,   0,   0,   0,   0,   0,   0,
        1,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   1,   1,   1,   0,   0,
        0,   0,   0,   1,   1,   1,   0,   0,
        0,   0,   0,   1,   1,   1,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_t mask_test_case_03[25] =
    {
        0,0,0,0,1,
        0,0,0,1,0,
        0,0,1,0,0,
        0,1,0,0,0,
        1,0,0,0,0,
    };

    uint8_pixel_t exp_data_test_case_03[8 * 8] =
    {
        0,   0,   1,   0,   0,   0,   0,   0,
        0,   1,   0,   0,   0,   0,   0,   0,
        1,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_t mask_test_case_04[25] =
    {
        0,0,0,0,0,
        0,0,0,0,0,
        0,0,0,0,0,
        0,0,0,0,0,
        1,0,0,0,0,
    };

    uint8_pixel_t exp_data_test_case_04[8 * 8] =
    {
        1,   1,   1,   0,   0,   0,   0,   0,
        1,   1,   0,   0,   1,   1,   1,   1,
        1,   1,   0,   0,   1,   1,   1,   1,
        1,   1,   0,   0,   1,   1,   1,   1,
        1,   1,   0,   0,   1,   1,   1,   1,
        1,   1,   0,   0,   0,   0,   0,   0,
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
         uint8_t *mask;
         uint8_t n;
     }testcase_t;

     // Compose array of test cases
     testcase_t testcases[] = {
         {src_data, exp_data_test_case_01, mask_test_case_01, 3},
         {src_data, exp_data_test_case_02, mask_test_case_02, 3},
         {src_data, exp_data_test_case_03, mask_test_case_03, 5},
         {src_data, exp_data_test_case_04, mask_test_case_04, 5},
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
         erosion(&src, &dst, testcases[i].mask, testcases[i].n);

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

void test_erosionGray(void)
{
    // Prepare images for testing
    uint8_pixel_t src_data[8 * 8] =
    {
        0,   0,   1,   0,   0,   0,   0,   2,
        0,   1,   0,   0,   0,   0,   0,   2,
        1,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   5,   5,   5,   5,   0,   0,
        0,   0,   5,   8,   8,   5,   0,   0,
        0,   0,   5,   9,   9,   5,   0,   0,
        0,   0,   5,   5,   5,   5,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_t mask_test_case_01[9] =
    {
        2,2,0,
        2,1,3,
        0,3,3,
    };

    uint8_pixel_t exp_data_test_case_01[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   3,   2,   0,   0,   0,
        0,   0,   0,   2,   2,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_t mask_test_case_02[9] =
    {
        1,0,0,
        0,1,0,
        0,0,0,
    };

    uint8_pixel_t exp_data_test_case_02[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   4,   4,   0,   0,   0,
        0,   0,   0,   4,   5,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_t mask_test_case_03[25] =
    {
        0,0,0,0,1,
        0,0,0,1,0,
        0,0,1,0,0,
        0,1,0,0,0,
        1,0,0,0,0,
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
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_t mask_test_case_04[25] =
    {
        0,0,0,0,0,
        0,0,0,0,0,
        0,0,0,0,0,
        0,0,0,0,0,
        1,0,0,0,0,
    };

    uint8_pixel_t exp_data_test_case_04[8 * 8] =
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
         uint8_t *mask;
         uint8_t n;
     }testcase_t;

     // Compose array of test cases
     testcase_t testcases[] = {
         {src_data, exp_data_test_case_01, mask_test_case_01, 3},
         {src_data, exp_data_test_case_02, mask_test_case_02, 3},
         {src_data, exp_data_test_case_03, mask_test_case_03, 5},
         {src_data, exp_data_test_case_04, mask_test_case_04, 5},
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
         erosionGray(&src, &dst, testcases[i].mask, testcases[i].n);

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

void test_fillHolesIterative(void)
{
    // Prepare images for testing
    uint8_pixel_t src_data_test_cases_0102[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   0,   1,   0,   0,   1,   0,   0,
        0,   0,   1,   0,   0,   1,   0,   0,
        0,   0,   1,   1,   1,   0,   0,   1,
        0,   0,   0,   0,   0,   0,   1,   0,
        0,   0,   0,   0,   0,   1,   0,   0,
    };

    // FOUR connected expected result
    uint8_pixel_t exp_data_test_case_01[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   0,   1,   1,   1,   0,   0,   1,
        0,   0,   0,   0,   0,   0,   1,   0,
        0,   0,   0,   0,   0,   1,   0,   0,
    };

    // EIGHT connected expected result
    uint8_pixel_t exp_data_test_case_02[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   0,   1,   0,   0,   1,   0,   0,
        0,   0,   1,   0,   0,   1,   0,   0,
        0,   0,   1,   1,   1,   0,   0,   1,
        0,   0,   0,   0,   0,   0,   1,   0,
        0,   0,   0,   0,   0,   1,   0,   0,
    };

    // Prepare images for testing
    uint8_pixel_t src_data_test_cases_0304[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   1,   1,   1,   1,   1,   1,   0,
        0,   1,   0,   0,   0,   0,   1,   0,
        0,   1,   0,   1,   0,   0,   1,   0,
        0,   1,   0,   0,   1,   0,   1,   0,
        0,   1,   0,   0,   1,   0,   1,   0,
        0,   1,   1,   1,   1,   0,   1,   0,
        0,   0,   0,   0,   0,   0,   1,   0,
    };

    // FOUR connected expected result
    uint8_pixel_t exp_data_test_case_03[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   1,   1,   1,   1,   1,   1,   0,
        0,   1,   0,   0,   0,   0,   1,   0,
        0,   1,   0,   1,   0,   0,   1,   0,
        0,   1,   0,   0,   1,   0,   1,   0,
        0,   1,   0,   0,   1,   0,   1,   0,
        0,   1,   1,   1,   1,   0,   1,   0,
        0,   0,   0,   0,   0,   0,   1,   0,
    };

    // EIGHT connected expected result
    uint8_pixel_t exp_data_test_case_04[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   1,   1,   1,   1,   1,   1,   0,
        0,   1,   0,   0,   0,   0,   1,   0,
        0,   1,   0,   1,   0,   0,   1,   0,
        0,   1,   0,   0,   1,   0,   1,   0,
        0,   1,   0,   0,   1,   0,   1,   0,
        0,   1,   1,   1,   1,   0,   1,   0,
        0,   0,   0,   0,   0,   0,   1,   0,
    };

    // Prepare images for testing
    uint8_pixel_t src_data_test_cases_0506[8 * 8] =
    {
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   0,   0,   0,   0,   0,   0,   1,
        1,   0,   1,   1,   1,   1,   0,   1,
        1,   0,   1,   0,   0,   1,   0,   1,
        1,   0,   1,   0,   0,   1,   0,   1,
        1,   0,   1,   1,   1,   0,   0,   1,
        1,   0,   0,   0,   0,   0,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
    };

    // FOUR connected expected result
    uint8_pixel_t exp_data_test_case_05[8 * 8] =
    {
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
    };

    // EIGHT connected expected result
    uint8_pixel_t exp_data_test_case_06[8 * 8] =
    {
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
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
        eConnected c;
    }testcase_t;

    // Compose array of test cases
    testcase_t testcases[] = {
        {src_data_test_cases_0102, exp_data_test_case_01, CONNECTED_FOUR},
        {src_data_test_cases_0102, exp_data_test_case_02, CONNECTED_EIGHT},
        {src_data_test_cases_0304, exp_data_test_case_03, CONNECTED_FOUR},
        {src_data_test_cases_0304, exp_data_test_case_04, CONNECTED_EIGHT},
        {src_data_test_cases_0506, exp_data_test_case_05, CONNECTED_FOUR},
        {src_data_test_cases_0506, exp_data_test_case_06, CONNECTED_EIGHT},
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
        fillHolesIterative(&src, &dst, testcases[i].c);

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

void test_fillHolesTwoPass(void)
{
    // Prepare images for testing
    uint8_pixel_t src_data_test_cases_0102[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   0,   1,   0,   0,   1,   0,   0,
        0,   0,   1,   0,   0,   1,   0,   0,
        0,   0,   1,   1,   1,   0,   0,   1,
        0,   0,   0,   0,   0,   0,   1,   0,
        0,   0,   0,   0,   0,   1,   0,   0,
    };

    // FOUR connected expected result
    uint8_pixel_t exp_data_test_case_01[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   0,   1,   1,   1,   0,   0,   1,
        0,   0,   0,   0,   0,   0,   1,   0,
        0,   0,   0,   0,   0,   1,   0,   0,
    };

    // EIGHT connected expected result
    uint8_pixel_t exp_data_test_case_02[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   0,   1,   0,   0,   1,   0,   0,
        0,   0,   1,   0,   0,   1,   0,   0,
        0,   0,   1,   1,   1,   0,   0,   1,
        0,   0,   0,   0,   0,   0,   1,   0,
        0,   0,   0,   0,   0,   1,   0,   0,
    };

    // Prepare images for testing
    uint8_pixel_t src_data_test_cases_0304[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   1,   1,   1,   1,   1,   1,   0,
        0,   1,   0,   0,   0,   0,   1,   0,
        0,   1,   0,   1,   0,   0,   1,   0,
        0,   1,   0,   0,   1,   0,   1,   0,
        0,   1,   0,   0,   1,   0,   1,   0,
        0,   1,   1,   1,   1,   0,   1,   0,
        0,   0,   0,   0,   0,   0,   1,   0,
    };

    // FOUR connected expected result
    uint8_pixel_t exp_data_test_case_03[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   1,   1,   1,   1,   1,   1,   0,
        0,   1,   0,   0,   0,   0,   1,   0,
        0,   1,   0,   1,   0,   0,   1,   0,
        0,   1,   0,   0,   1,   0,   1,   0,
        0,   1,   0,   0,   1,   0,   1,   0,
        0,   1,   1,   1,   1,   0,   1,   0,
        0,   0,   0,   0,   0,   0,   1,   0,
    };

    // EIGHT connected expected result
    uint8_pixel_t exp_data_test_case_04[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   1,   1,   1,   1,   1,   1,   0,
        0,   1,   0,   0,   0,   0,   1,   0,
        0,   1,   0,   1,   0,   0,   1,   0,
        0,   1,   0,   0,   1,   0,   1,   0,
        0,   1,   0,   0,   1,   0,   1,   0,
        0,   1,   1,   1,   1,   0,   1,   0,
        0,   0,   0,   0,   0,   0,   1,   0,
    };

    // Prepare images for testing
    uint8_pixel_t src_data_test_cases_0506[8 * 8] =
    {
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   0,   0,   0,   0,   0,   0,   1,
        1,   0,   1,   1,   1,   1,   0,   1,
        1,   0,   1,   0,   0,   1,   0,   1,
        1,   0,   1,   0,   0,   1,   0,   1,
        1,   0,   1,   1,   1,   0,   0,   1,
        1,   0,   0,   0,   0,   0,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
    };

    // FOUR connected expected result
    uint8_pixel_t exp_data_test_case_05[8 * 8] =
    {
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
    };

    // EIGHT connected expected result
    uint8_pixel_t exp_data_test_case_06[8 * 8] =
    {
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
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
        eConnected c;
    }testcase_t;

    // Compose array of test cases
    testcase_t testcases[] = {
        {src_data_test_cases_0102, exp_data_test_case_01, CONNECTED_FOUR},
        {src_data_test_cases_0102, exp_data_test_case_02, CONNECTED_EIGHT},
        {src_data_test_cases_0304, exp_data_test_case_03, CONNECTED_FOUR},
        {src_data_test_cases_0304, exp_data_test_case_04, CONNECTED_EIGHT},
        {src_data_test_cases_0506, exp_data_test_case_05, CONNECTED_FOUR},
        {src_data_test_cases_0506, exp_data_test_case_06, CONNECTED_EIGHT},
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
        fillHolesTwoPass(&src, &dst, testcases[i].c, 128);

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

void test_hitmiss(void)
{
    // Prepare images for testing
    uint8_pixel_t src_data[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   1,   1,   1,   1,   1,   1,   0,
        0,   1,   1,   1,   1,   1,   1,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   0,   1,   1,   1,   1,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_t m1_test_case_01[9] =
    {
        0,0,0,
        0,1,1,
        0,0,0,
    };

    uint8_t m2_test_case_01[9] =
    {
        0,0,0,
        1,0,0,
        0,0,0,
    };

    uint8_pixel_t exp_data_test_case_01[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   1,   0,   0,   0,   0,   0,   0,
        0,   1,   0,   0,   0,   0,   0,   0,
        0,   0,   1,   0,   0,   0,   0,   0,
        0,   0,   1,   0,   0,   0,   0,   0,
        0,   0,   0,   1,   0,   0,   0,   0,
        0,   0,   0,   1,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_t m1_test_case_02[9] =
    {
        0,0,0,
        1,1,0,
        0,0,0,
    };

    uint8_t m2_test_case_02[9] =
    {
        0,0,0,
        0,0,1,
        0,0,0,
    };

    uint8_pixel_t exp_data_test_case_02[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   1,   0,
        0,   0,   0,   0,   0,   0,   1,   0,
        0,   0,   0,   0,   0,   1,   0,   0,
        0,   0,   0,   0,   0,   1,   0,   0,
        0,   0,   0,   0,   1,   0,   0,   0,
        0,   0,   0,   0,   1,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_t m1_test_case_03[9] =
    {
        0,1,0,
        1,1,0,
        0,0,0,
    };

    uint8_t m2_test_case_03[9] =
    {
        0,0,0,
        0,0,0,
        0,0,1,
    };

    uint8_pixel_t exp_data_test_case_03[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   1,   1,   0,
        0,   0,   0,   0,   0,   1,   0,   0,
        0,   0,   0,   0,   1,   1,   0,   0,
        0,   0,   0,   0,   1,   0,   0,   0,
        0,   0,   0,   0,   1,   0,   0,   0,
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
         uint8_t *m1;
         uint8_t *m2;
     }testcase_t;

     // Compose array of test cases
     testcase_t testcases[] = {
         {src_data, exp_data_test_case_01, m1_test_case_01, m2_test_case_01},
         {src_data, exp_data_test_case_02, m1_test_case_02, m2_test_case_02},
         {src_data, exp_data_test_case_03, m1_test_case_03, m2_test_case_03},
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
         hitmiss(&src, &dst, testcases[i].m1, testcases[i].m2);

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

void test_removeBorderBlobsIterative(void)
{
    // Prepare images for testing
    uint8_pixel_t src_data_test_cases_0102[8 * 8] =
    {
        1,   0,   0,   0,   0,   0,   0,   0,
        0,   1,   0,   1,   1,   1,   1,   0,
        0,   1,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   1,   1,   0,   0,   1,   1,   1,
        1,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    // FOUR connected expected result
    uint8_pixel_t exp_data_test_case_01[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   1,   0,   1,   1,   1,   1,   0,
        0,   1,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   1,   1,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    // EIGHT connected expected result
    uint8_pixel_t exp_data_test_case_02[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   1,   1,   1,   1,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    // Prepare images for testing
    uint8_pixel_t src_data_test_cases_0304[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   1,   1,   1,   1,   1,   1,   0,
        0,   1,   0,   0,   0,   0,   1,   0,
        0,   1,   0,   1,   0,   0,   1,   0,
        0,   1,   0,   0,   1,   0,   1,   0,
        0,   1,   0,   0,   1,   0,   1,   0,
        0,   1,   1,   1,   1,   0,   1,   0,
        0,   0,   0,   0,   0,   0,   1,   0,
    };

    // FOUR connected expected result
    uint8_pixel_t exp_data_test_case_03[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   1,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    // EIGHT connected expected result
    uint8_pixel_t exp_data_test_case_04[8 * 8] =
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
        {src_data_test_cases_0102, exp_data_test_case_01, CONNECTED_FOUR},
        {src_data_test_cases_0102, exp_data_test_case_02, CONNECTED_EIGHT},
        {src_data_test_cases_0304, exp_data_test_case_03, CONNECTED_FOUR},
        {src_data_test_cases_0304, exp_data_test_case_04, CONNECTED_EIGHT},
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
        removeBorderBlobsIterative(&src, &dst, testcases[i].c);

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

void test_removeBorderBlobsTwoPass(void)
{
    // Prepare images for testing
    uint8_pixel_t src_data_test_cases_0102[8 * 8] =
    {
        1,   0,   0,   0,   0,   0,   0,   0,
        0,   1,   0,   1,   1,   1,   1,   0,
        0,   1,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   1,   1,   0,   0,   1,   1,   1,
        1,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    // FOUR connected expected result
    uint8_pixel_t exp_data_test_case_01[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   1,   0,   1,   1,   1,   1,   0,
        0,   1,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   1,   1,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    // EIGHT connected expected result
    uint8_pixel_t exp_data_test_case_02[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   1,   1,   1,   1,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    // Prepare images for testing
    uint8_pixel_t src_data_test_cases_0304[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   1,   1,   1,   1,   1,   1,   0,
        0,   1,   0,   0,   0,   0,   1,   0,
        0,   1,   0,   1,   0,   0,   1,   0,
        0,   1,   0,   0,   1,   0,   1,   0,
        0,   1,   0,   0,   1,   0,   1,   0,
        0,   1,   1,   1,   1,   0,   1,   0,
        0,   0,   0,   0,   0,   0,   1,   0,
    };

    // FOUR connected expected result
    uint8_pixel_t exp_data_test_case_03[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   1,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    // EIGHT connected expected result
    uint8_pixel_t exp_data_test_case_04[8 * 8] =
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
        {src_data_test_cases_0102, exp_data_test_case_01, CONNECTED_FOUR},
        {src_data_test_cases_0102, exp_data_test_case_02, CONNECTED_EIGHT},
        {src_data_test_cases_0304, exp_data_test_case_03, CONNECTED_FOUR},
        {src_data_test_cases_0304, exp_data_test_case_04, CONNECTED_EIGHT},
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
        removeBorderBlobsTwoPass(&src, &dst, testcases[i].c, 128);

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

void test_skeleton(void)
{
    // Prepare images for testing
    uint8_pixel_t src_data[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   1,   1,   1,   1,   1,   1,   0,
        0,   1,   1,   1,   1,   1,   1,   0,
        0,   1,   1,   1,   1,   1,   1,   0,
        0,   1,   1,   1,   1,   1,   1,   0,
        0,   1,   1,   1,   1,   1,   1,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_t mask_test_case_01[9] =
    {
        0,1,0,
        1,1,1,
        0,1,0,
    };

    uint8_pixel_t exp_data_test_case_01[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   1,   0,   0,   0,   0,   1,   0,
        0,   0,   1,   0,   0,   1,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   1,   0,   0,   1,   0,   0,
        0,   1,   0,   0,   0,   0,   1,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_t mask_test_case_02[9] =
    {
        1,1,1,
        1,1,1,
        1,1,1,
    };

    uint8_pixel_t exp_data_test_case_02[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   1,   1,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
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
         uint8_t *mask;
         uint8_t n;
     }testcase_t;

     // Compose array of test cases
     testcase_t testcases[] = {
         {src_data, exp_data_test_case_01, mask_test_case_01, 3},
         {src_data, exp_data_test_case_02, mask_test_case_02, 3},
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
         skeleton(&src, &dst, testcases[i].mask, testcases[i].n);

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
