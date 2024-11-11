/*! ***************************************************************************
 *
 * \brief     Unit test functions for spatial filters
 * \file      test_spatial_filters.c
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

void test_gaussianFilter(void)
{
    // Prepare images for testing
    int16_pixel_t src_data[8 * 8] =
    {
        5,   5,   5,   5,   6,   6,   6,   6,
        5,   5,   5,   5,   6,   6,   6,   6,
        5,   5,   5,   5,   6,   6,   6,   6,
        5,   5,   5,   5,   6,   6,   6,   6,
        10, 10,  10,  10,  20,  20,  20,  20,
        10, 10,  10,  10,  20,  20,  20,  20,
        10, 10,  10,  10,  20,  20,  20,  20,
        10, 10,  10,  10,  20,  20,  20,  20,
    };

    int16_pixel_t exp_data_test_case_01[8 * 8] =
    {
         45,    60,    60,    63,    69,    72,    72,    54,
         60,    80,    80,    84,    92,    96,    96,    72,
         60,    80,    80,    84,    92,    96,    96,    72,
         75,   100,   100,   113,   139,   152,   152,   114,
        105,   140,   140,   171,   233,   264,   264,   198,
        120,   160,   160,   200,   280,   320,   320,   240,
        120,   160,   160,   200,   280,   320,   320,   240,
         90,   120,   120,   150,   210,   240,   240,   180,
    };

    int16_pixel_t exp_data_test_case_02[8 * 8] =
    {
         660,   890,   962,  1008,  1082,  1128,  1068,   792,
         890,  1200,  1296,  1358,  1458,  1520,  1440,  1068,
        1010,  1360,  1476,  1578,  1748,  1850,  1760,  1308,
        1240,  1670,  1842,  2088,  2492,  2738,  2628,  1952,
        1610,  2170,  2440,  2920,  3693,  4173,  4028,  2988,
        1840,  2480,  2806,  3430,  4437,  5061,  4896,  3632,
        1780,  2400,  2720,  3340,  4340,  4960,  4800,  3560,
        1320,  1780,  2020,  2480,  3220,  3680,  3560,  2640,
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
         int16_pixel_t *exp_data;
         void (*fp)(const image_t *, image_t *);
     }testcase_t;

     // Compose array of test cases
     testcase_t testcases[] = {{exp_data_test_case_01, gaussianFilter_3x3},
                               {exp_data_test_case_02, gaussianFilter_5x5}};

     // Prepare images
     image_t src = {8, 8, IMGTYPE_INT16, (uint8_t *)src_data};
     image_t exp = {8, 8, IMGTYPE_INT16, NULL};
     image_t dst = {8, 8, IMGTYPE_INT16, (uint8_t *)dst_data};

     // Loop all test cases
     for(uint32_t i=0; i < (sizeof(testcases) / sizeof(testcase_t)); ++i)
     {
         // Set the data
         exp.data = (uint8_t *)testcases[i].exp_data;

         // Clear destination
         clearInt16Image(&dst);

         // Execute the operator
         testcases[i].fp(&src, &dst);

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
         TEST_ASSERT_EQUAL_UINT16_ARRAY_MESSAGE(exp.data, dst.data, (exp.cols * exp.rows), name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.type, dst.type, name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.cols, dst.cols, name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.rows, dst.rows, name);
     }
}

void test_laplacian(void)
{
    // Prepare images for testing
    int16_pixel_t src_data[8 * 8] =
    {
        5,   5,   5,   5,   6,   6,   6,   6,
        5,   5,   5,   5,   6,   6,   6,   6,
        5,   5,   5,   5,   6,   6,   6,   6,
        5,   5,   5,   5,   6,   6,   6,   6,
        10, 10,  10,  10,  20,  20,  20,  20,
        10, 10,  10,  10,  20,  20,  20,  20,
        10, 10,  10,  10,  20,  20,  20,  20,
        10, 10,  10,  10,  20,  20,  20,  20,
    };

    int16_pixel_t exp_data_test_case_01[8 * 8] =
    {
        10,     5,     5,     4,     7,     6,     6,    12,
         5,     0,     0,    -1,     1,     0,     0,     6,
         5,     0,     0,    -1,     1,     0,     0,     6,
         0,    -5,    -5,    -6,   -13,   -14,   -14,    -8,
        15,     5,     5,    -5,    24,    14,    14,    34,
        10,     0,     0,   -10,    10,     0,     0,    20,
        10,     0,     0,   -10,    10,     0,     0,    20,
        20,    10,    10,     0,    30,    20,    20,    40,
    };

    int16_pixel_t exp_data_test_case_02[8 * 8] =
    {
        80,    65,    47,    44,    66,    63,    78,    96,
        65,    45,    21,    17,    38,    34,    54,    78,
        35,     5,   -39,   -53,   -42,   -56,   -26,    18,
        20,   -15,   -73,   -96,   -94,  -117,   -82,   -24,
       130,    90,    18,   -14,   204,   172,   212,   284,
       115,    70,   -16,   -57,   152,   111,   156,   242,
       130,    90,    10,   -30,   180,   140,   180,   260,
       160,   130,    70,    40,   260,   230,   260,   320,
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
         int16_pixel_t *exp_data;
         void (*fp)(const image_t *, image_t *);
     }testcase_t;

     // Compose array of test cases
     testcase_t testcases[] = {{exp_data_test_case_01, laplacianFilter_3x3},
                               {exp_data_test_case_02, laplacianFilter_5x5}};

     // Prepare images
     image_t src = {8, 8, IMGTYPE_INT16, (uint8_t *)src_data};
     image_t exp = {8, 8, IMGTYPE_INT16, NULL};
     image_t dst = {8, 8, IMGTYPE_INT16, (uint8_t *)dst_data};

     // Loop all test cases
     for(uint32_t i=0; i < (sizeof(testcases) / sizeof(testcase_t)); ++i)
     {
         // Set the data
         exp.data = (uint8_t *)testcases[i].exp_data;

         // Clear destination
         clearInt16Image(&dst);

         // Execute the operator
         testcases[i].fp(&src, &dst);

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
         TEST_ASSERT_EQUAL_UINT16_ARRAY_MESSAGE(exp.data, dst.data, (exp.cols * exp.rows), name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.type, dst.type, name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.cols, dst.cols, name);
         TEST_ASSERT_EQUAL_MESSAGE(exp.rows, dst.rows, name);
     }
}

void test_sobel(void)
{
    // Prepare images for testing
    int16_pixel_t src_data[8 * 8] =
    {
        5,   5,   5,   5,   6,   6,   6,   6,
        5,   5,   5,   5,   6,   6,   6,   6,
        5,   5,   5,   5,   6,   6,   6,   6,
        5,   5,   5,   5,   6,   6,   6,   6,
        10, 10,  10,  10,  20,  20,  20,  20,
        10, 10,  10,  10,  20,  20,  20,  20,
        10, 10,  10,  10,  20,  20,  20,  20,
        10, 10,  10,  10,  20,  20,  20,  20,
    };

    int16_pixel_t exp_data_mag[8 * 8] =
    {
        30,    20,    20,    24,    26,    24,    24,    36,
        20,     0,     0,     4,     4,     0,     0,    24,
        20,     0,     0,     4,     4,     0,     0,    24,
        40,    20,    20,    42,    60,    56,    56,    80,
        50,    20,    20,    60,    78,    56,    56,   108,
        40,     0,     0,    40,    40,     0,     0,    80,
        40,     0,     0,    40,    40,     0,     0,    80,
        60,    40,    40,    80,   100,    80,    80,   120,
    };

  //float_pixel_t exp_data_dir[8 * 8] =
  //{
  //    0.785,  0.000,  0.000,  0.142,  0.130,  0.000,  0.000, -0.785,
  //    1.571,    NAN,    NAN,  1.571,  1.571,    NAN,    NAN, -1.571,
  //    1.571,    NAN,    NAN,  1.571,  1.571,    NAN,    NAN, -1.571,
  //    1.030,  0.000,  0.000,  0.421,  0.270,  0.000,  0.000, -0.735,
  //    1.166,  0.000,  0.000,  0.819,  0.583,  0.000,  0.000, -1.004,
  //    1.571,    NAN,    NAN,  1.571,  1.571,    NAN,    NAN, -1.571,
  //    1.571,    NAN,    NAN,  1.571,  1.571,    NAN,    NAN, -1.571,
  //   -0.785, -0.000, -0.000, -0.540, -0.405, -0.000, -0.000,  0.785,
  //};

    int16_pixel_t dst_data_mag[8 * 8] =
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

    float_pixel_t dst_data_dir[8 * 8] =
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

     // Prepare images
     image_t src = {8, 8, IMGTYPE_INT16, (uint8_t *)src_data};
     image_t exp_mag = {8, 8, IMGTYPE_INT16, (uint8_t *)exp_data_mag};
   //image_t exp_dir = {8, 8, IMGTYPE_FLOAT, (uint8_t *)exp_data_dir};
     image_t dst_mag = {8, 8, IMGTYPE_INT16, (uint8_t *)dst_data_mag};
     image_t dst_dir = {8, 8, IMGTYPE_FLOAT, (uint8_t *)dst_data_dir};

     // Clear destination
     clearInt16Image(&dst_mag);
     clearFloatImage(&dst_dir);

     // Execute the operator
     sobel(&src, &dst_mag, &dst_dir);

     // Set test case name
     char name[80] = "";
     sprintf(name, "Test case 1 of 1");

 #if 0
     // Print testcase info
     printf("\n---------------------------------------\n");
     printf("%s\n", name);

     // Print image data
     prettyprint(&src, "src");
     prettyprint(&exp_mag, "exp_mag");
     prettyprint(&dst_mag, "dst_mag");
     prettyprint(&exp_dir, "exp_dir");
     prettyprint(&dst_dir, "dst_dir");

 #endif

     // Verify the result
     // Magnitude only
     TEST_ASSERT_EQUAL_UINT16_ARRAY_MESSAGE(exp_mag.data, dst_mag.data, (exp_mag.cols * exp_mag.rows), name);
     TEST_ASSERT_EQUAL_MESSAGE(exp_mag.type, dst_mag.type, name);
     TEST_ASSERT_EQUAL_MESSAGE(exp_mag.cols, dst_mag.cols, name);
     TEST_ASSERT_EQUAL_MESSAGE(exp_mag.rows, dst_mag.rows, name);
}

void test_sobelFast(void)
{
    // Prepare images for testing
    int16_pixel_t src_data[8 * 8] =
    {
        5,   5,   5,   5,   6,   6,   6,   6,
        5,   5,   5,   5,   6,   6,   6,   6,
        5,   5,   5,   5,   6,   6,   6,   6,
        5,   5,   5,   5,   6,   6,   6,   6,
        10, 10,  10,  10,  20,  20,  20,  20,
        10, 10,  10,  10,  20,  20,  20,  20,
        10, 10,  10,  10,  20,  20,  20,  20,
        10, 10,  10,  10,  20,  20,  20,  20,
    };

    int16_pixel_t exp_data_mag[8 * 8] =
    {
        0,     0,     0,     0,     0,     0,     0,   0,
        0,     0,     0,     4,     4,     0,     0,   0,
        0,     0,     0,     4,     4,     0,     0,   0,
        0,    20,    20,    42,    60,    56,    56,   0,
        0,    20,    20,    60,    78,    56,    56,   0,
        0,     0,     0,    40,    40,     0,     0,   0,
        0,     0,     0,    40,    40,     0,     0,   0,
        0,     0,     0,     0,     0,     0,     0,   0,
    };

    int16_pixel_t dst_data_mag[8 * 8] =
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

     // Prepare images
     image_t src =     {8, 8, IMGTYPE_INT16, (uint8_t *)src_data};
     image_t exp_mag = {8, 8, IMGTYPE_INT16, (uint8_t *)exp_data_mag};
     image_t dst_mag = {8, 8, IMGTYPE_INT16, (uint8_t *)dst_data_mag};

     // Clear destination
     clearInt16Image(&dst_mag);

     // Execute the operator
     sobelFast(&src, &dst_mag);

     // Set test case name
     char name[80] = "";
     sprintf(name, "Test case 1 of 1");

#if 0
     // Print testcase info
     printf("\n---------------------------------------\n");
     printf("%s\n", name);

     // Print image data
     prettyprint(&src, "src");
     prettyprint(&exp_mag, "exp_mag");
     prettyprint(&dst_mag, "dst_mag");

#endif

     // Verify the result
     // Magnitude only
     TEST_ASSERT_EQUAL_UINT16_ARRAY_MESSAGE(exp_mag.data, dst_mag.data, (exp_mag.cols * exp_mag.rows), name);
     TEST_ASSERT_EQUAL_MESSAGE(exp_mag.type, dst_mag.type, name);
     TEST_ASSERT_EQUAL_MESSAGE(exp_mag.cols, dst_mag.cols, name);
     TEST_ASSERT_EQUAL_MESSAGE(exp_mag.rows, dst_mag.rows, name);
}
