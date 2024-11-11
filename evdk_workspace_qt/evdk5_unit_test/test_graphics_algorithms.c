/*! ***************************************************************************
 *
 * \brief     Unit test functions for graphics algorithms
 * \file      test_grapic_algorithms.c
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

void test_affineTransformation(void)
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
        1,   1,   1,   1,   1,   1,   1,   1,
        2,   2,   2,   2,   2,   2,   2,   2,
        3,   3,   3,   3,   3,   3,   3,   3,
        4,   4,   4,   4,   4,   4,   4,   4,
        5,   5,   5,   5,   5,   5,   5,   5,
        6,   6,   6,   6,   6,   6,   6,   6,
        7,   7,   7,   7,   7,   7,   7,   7,
    };

    uint8_pixel_t exp_data_test_case_03[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        1,   1,   1,   1,   1,   1,   1,   1,
        0,   0,   0,   0,   0,   0,   0,   0,
        2,   2,   2,   2,   2,   2,   2,   2,
        0,   0,   0,   0,   0,   0,   0,   0,
        3,   3,   3,   3,   3,   3,   3,   3,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    uint8_pixel_t exp_data_test_case_04[8 * 8] =
    {
        0,   2,   4,   6,   0,   0,   0,   0,
        1,   3,   5,   7,   0,   0,   0,   0,
        2,   4,   6,   0,   0,   0,   0,   0,
        3,   5,   7,   0,   0,   0,   0,   0,
        4,   6,   0,   0,   0,   0,   0,   0,
        5,   7,   0,   0,   0,   0,   0,   0,
        6,   0,   0,   0,   0,   0,   0,   0,
        7,   0,   0,   0,   0,   0,   0,   0,
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
         eTransformDirection d;
         float m[2][3];
     }testcase_t;

     // Compose array of test cases
     testcase_t testcases[] = {
         {exp_data_test_case_01,
          TRANSFORM_FORWARD,
          {{1.0f, 0.0f, 0.0f},
           {0.0f, 1.0f, 0.0f}}},
         {exp_data_test_case_02,
          TRANSFORM_BACKWARD,
          {{1.0f, 0.0f, 0.0f},
           {0.0f, 1.0f, 0.0f}}},
         {exp_data_test_case_03,
          TRANSFORM_FORWARD,
          {{1.0f, 0.0f, 0.5f},
           {0.0f, 2.0f, 0.0f}}},
         {exp_data_test_case_04,
          TRANSFORM_BACKWARD,
          {{1.0f, 0.0f, 0.1f},
           {2.0f, 1.0f, 0.0f}}},
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
         affineTransformation(&src, &dst,
                              testcases[i].d, testcases[i].m);

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
         TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(exp.data, dst.data, (exp.cols * exp.rows), name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.type, dst.type, name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.cols, dst.cols, name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.rows, dst.rows, name);
     }
}

void test_warpPerspective(void)
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
        0,   0,   0,   0,   0,   0,   0,   0,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1
    };

    uint8_pixel_t exp_data_test_case_03[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        1,   1,   1,   1,   1,   1,   1,   1,
        2,   1,   1,   1,   1,   1,   1,   1,
        2,   2,   2,   2,   2,   2,   2,   1,
        3,   3,   2,   2,   2,   2,   2,   2,
        3,   3,   3,   3,   3,   3,   2,   2,
        4,   3,   3,   3,   3,   3,   3,   3,
        4,   4,   4,   4,   3,   3,   3,   3,
    };

    uint8_pixel_t exp_data_test_case_04[8 * 8] =
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
         uint8_pixel_t *exp_data;
         point_t from[4];
         point_t to[4];
         eTransformDirection d;
     }testcase_t;

     // Compose array of test cases
     testcase_t testcases[] = {
         {exp_data_test_case_01,
          {{0,0},{7,0},{7,7},{0,7}},
          {{0,0},{7,0},{7,7},{0,7}},
          TRANSFORM_BACKWARD},
         {exp_data_test_case_02,
          {{0,0},{1,0},{1,1},{0,1}},
          {{0,0},{7,0},{7,7},{0,7}},
          TRANSFORM_BACKWARD},
         {exp_data_test_case_03,
          {{0,0},{2,0},{3,3},{2,4}},
          {{0,0},{7,0},{7,7},{0,7}},
          TRANSFORM_BACKWARD},
         {exp_data_test_case_04,
          {{1,1},{1,1},{1,1},{1,1}},
          {{0,0},{7,0},{7,7},{0,7}},
          TRANSFORM_BACKWARD},
     };

     // Prepare images
     image_t src = {8, 8,  IMGTYPE_UINT8, src_data};
     image_t exp = {8, 8,  IMGTYPE_UINT8, NULL};
     image_t dst = {8, 8,  IMGTYPE_UINT8, dst_data};

     // Loop all test cases
     for(uint32_t i=0; i < (sizeof(testcases) / sizeof(testcase_t)); ++i)
     {
         // Set the data
         exp.data = testcases[i].exp_data;

         // Clear destination
         clearUint8Image(&dst);

         // Execute the operator
         warpPerspective(&src, &dst,
                         testcases[i].from, testcases[i].to, testcases[i].d);

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
         TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(exp.data, dst.data, (exp.cols * exp.rows), name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.type, dst.type, name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.cols, dst.cols, name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.rows, dst.rows, name);
     }
}

void test_warpPerspectiveFast(void)
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
        0,   0,   0,   0,   0,   0,   0,   0,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1
    };

    uint8_pixel_t exp_data_test_case_03[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        1,   1,   1,   1,   1,   1,   1,   1,
        2,   1,   1,   1,   1,   1,   1,   1,
        2,   2,   2,   2,   2,   2,   2,   1,
        3,   3,   2,   2,   2,   2,   2,   2,
        3,   3,   3,   3,   3,   3,   2,   2,
        4,   3,   3,   3, 3,   3,   3,   3,
        4,   4,   4,   4,   3,   3,   3,   3
    };

    uint8_pixel_t exp_data_test_case_04[8 * 8] =
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
         uint8_pixel_t *exp_data;
         point_t from[4];
         eTransformDirection d;
     }testcase_t;

     // Compose array of test cases
     testcase_t testcases[] = {
         {exp_data_test_case_01,
             {{0,0},{7,0},{7,7},{0,7}},
             TRANSFORM_BACKWARD},
         {exp_data_test_case_02,
             {{0,0},{1,0},{1,1},{0,1}},
             TRANSFORM_BACKWARD},
         {exp_data_test_case_03,
             {{0,0},{2,0},{3,3},{2,4}},
             TRANSFORM_BACKWARD},
         {exp_data_test_case_04,
             {{1,1},{1,1},{1,1},{1,1}},
             TRANSFORM_BACKWARD},
     };

     // Prepare images
     image_t src = {8, 8,  IMGTYPE_UINT8, src_data};
     image_t exp = {8, 8,  IMGTYPE_UINT8, NULL};
     image_t dst = {8, 8,  IMGTYPE_UINT8, dst_data};

     // Loop all test cases
     for(uint32_t i=0; i < (sizeof(testcases) / sizeof(testcase_t)); ++i)
     {
         // Set the data
         exp.data = testcases[i].exp_data;

         // Clear destination
         clearUint8Image(&dst);

         // Execute the operator
         warpPerspectiveFast(&src, &dst,
                             testcases[i].from, testcases[i].d);

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
         TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(exp.data, dst.data, (exp.cols * exp.rows), name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.type, dst.type, name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.cols, dst.cols, name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.rows, dst.rows, name);
     }
}

void test_zoom(void)
{
    // Prepare images for testing
    uint8_pixel_t src_data_test_case_01[4 * 4] =
    {
        0,   0,   0,   0,
        1,   1,   1,   1,
        2,   2,   2,   2,
        3,   3,   3,   3,
    };

    uint8_pixel_t exp_data_test_case_01[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        2,   2,   2,   2,   2,   2,   2,   2,
        2,   2,   2,   2,   2,   2,   2,   2,
        3,   3,   3,   3,   3,   3,   3,   3,
        3,   3,   3,   3,   3,   3,   3,   3,
    };

    uint8_pixel_t dst_data_test_case_01[8 * 8] =
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

    uint8_pixel_t src_data_test_case_02[8 * 8] =
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

    uint8_pixel_t exp_data_test_case_02[4 * 4] =
    {
        0,   0,   0,   0,
        2,   2,   2,   2,
        4,   4,   4,   4,
        6,   6,   6,   6,
    };

    uint8_pixel_t dst_data_test_case_02[4 * 4] =
    {
        0,   0,   0,   0,
        0,   0,   0,   0,
        0,   0,   0,   0,
        0,   0,   0,   0,
    };

     typedef struct testcase_t
     {
        int32_t src_cols;
        int32_t src_rows;
         uint8_pixel_t *src_data;
         int32_t exp_cols;
         int32_t exp_rows;
         uint8_pixel_t *exp_data;
         int32_t dst_cols;
         int32_t dst_rows;
         uint8_pixel_t *dst_data;
         int32_t x;
         int32_t y;
         int32_t hor;
         int32_t ver;
         eZoom zd;
     }testcase_t;

     // Compose array of test cases
     testcase_t testcases[] = {
         {4, 4, src_data_test_case_01,
          8 ,8, exp_data_test_case_01,
          8, 8, dst_data_test_case_01,
          0,0,4,4,ZOOM_IN},
         {8, 8, src_data_test_case_02,
          4, 4, exp_data_test_case_02,
          4, 4, dst_data_test_case_02,
          0,0,8,8,ZOOM_OUT},
     };

     // Prepare images
     image_t src = {0, 0,  IMGTYPE_UINT8, NULL};
     image_t exp = {0, 0,  IMGTYPE_UINT8, NULL};
     image_t dst = {0, 0,  IMGTYPE_UINT8, NULL};

     // Loop all test cases
     for(uint32_t i=0; i < (sizeof(testcases) / sizeof(testcase_t)); ++i)
     {
         // Set the data
         src.data = testcases[i].src_data;
         src.cols = testcases[i].src_cols;
         src.rows = testcases[i].src_rows;
         exp.data = testcases[i].exp_data;
         exp.cols = testcases[i].exp_cols;
         exp.rows = testcases[i].exp_rows;
         dst.data = testcases[i].dst_data;
         dst.cols = testcases[i].dst_cols;
         dst.rows = testcases[i].dst_rows;

         // Execute the operator
         zoom(&src, &dst,
              testcases[i].x, testcases[i].y,
              testcases[i].hor, testcases[i].ver,
              testcases[i].zd);

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
         TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(exp.data, dst.data, (exp.cols * exp.rows), name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.type, dst.type, name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.cols, dst.cols, name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.rows, dst.rows, name);
     }
}

void test_zoomFactor(void)
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
        2,   2,   2,   2,   2,   2,   2,   2,
        2,   2,   2,   2,   2,   2,   2,   2,
        3,   3,   3,   3,   3,   3,   3,   3,
        3,   3,   3,   3,   3,   3,   3,   3,
    };

    uint8_pixel_t exp_data_test_case_02[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        2,   2,   2,   2,   0,   0,   0,   0,
        4,   4,   4,   4,   0,   0,   0,   0,
        6,   6,   6,   6,   0,   0,   0,   0,
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
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   1,   1,   1,   1,   1,   1,
        2,   2,   2,   2,   2,   2,   2,   2,
        2,   2,   2,   2,   2,   2,   2,   2,
    };

    uint8_pixel_t exp_data_test_case_04[8 * 8] =
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

    uint8_pixel_t exp_data_test_case_05[8 * 8] =
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

    uint8_pixel_t exp_data_test_case_06[8 * 8] =
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        5,   5,   0,   0,   0,   0,   0,   0,
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
         uint8_pixel_t *exp_data;
         int32_t x;
         int32_t y;
         int32_t hor;
         int32_t ver;
         eZoom zd;
         int16_t factor;
     }testcase_t;

     // Compose array of test cases
     testcase_t testcases[] = {
         {exp_data_test_case_01, 0,0, 4,4,  ZOOM_IN, 2},
         {exp_data_test_case_02, 0,0, 8,8, ZOOM_OUT, 2},
         {exp_data_test_case_03, 0,0, 4,4,  ZOOM_IN, 3},
         {exp_data_test_case_04, 0,0, 8,8, ZOOM_OUT, 1},
         {exp_data_test_case_05, 0,0, 4,4,  ZOOM_IN, 7},
         {exp_data_test_case_06, 0,0, 8,8, ZOOM_OUT, 5},
     };

     // Prepare images
     image_t src = {8, 8,  IMGTYPE_UINT8, src_data};
     image_t exp = {8, 8,  IMGTYPE_UINT8, NULL};
     image_t dst = {8, 8,  IMGTYPE_UINT8, dst_data};

     // Loop all test cases
     for(uint32_t i=0; i < (sizeof(testcases) / sizeof(testcase_t)); ++i)
     {
         // Set the data
         exp.data = testcases[i].exp_data;

         // Clear destination
         clearUint8Image(&dst);

         // Execute the operator
         zoomFactor(&src, &dst,
                    testcases[i].x, testcases[i].y,
                    testcases[i].hor, testcases[i].ver,
                    testcases[i].zd, testcases[i].factor);

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
         TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(exp.data, dst.data, (exp.cols * exp.rows), name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.type, dst.type, name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.cols, dst.cols, name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.rows, dst.rows, name);
     }
}
