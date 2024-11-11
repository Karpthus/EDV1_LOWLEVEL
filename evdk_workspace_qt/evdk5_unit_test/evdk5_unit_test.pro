#------------------------------------------------------------------------------
 # Project    : Embedded Vision and Machine Learning
 # Copyright  : 2024 HAN Electrical and Electronic Engineering
 # Author     : Hugo Arends
 #
 # Description: Project file for EVDK unit testing
 #
 # Copyright (C) 2024 HAN University of Applied Sciences. All Rights Reserved.
 #
 # Permission is hereby granted, free of charge, to any person obtaining a
 # copy of this software and associated documentation files (the "Software"),
 # to deal in the Software without restriction, including without limitation
 # the rights to use, copy, modify, merge, publish, distribute, sublicense,
 # and/or sell copies of the Software, and to permit persons to whom the
 # Software is furnished to do so, subject to the following conditions:
 #
 # The above copyright notice and this permission notice shall be included in
 # all copies or substantial portions of the Software.
 #
 # THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 # IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 # FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 # AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 # LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 # FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 # IN THE SOFTWARE.
#------------------------------------------------------------------------------

QT -= gui
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = evdk_test

CONFIG += -std=c11
QMAKE_CFLAGS += -std=c11

# Enable the following define if your terminal is able to handle ANSI escape
# color codes
DEFINES += UNITY_OUTPUT_COLOR

INCLUDEPATH += $$PWD/Unity/src
INCLUDEPATH += $$PWD/../../evdk_operators

SOURCES += \
    $$PWD/../../evdk_operators/coding_and_compression.c \
    $$PWD/../../evdk_operators/fonts.c \
    $$PWD/../../evdk_operators/graphics_algorithms.c \
    $$PWD/../../evdk_operators/histogram_operations.c \
    $$PWD/../../evdk_operators/image_fundamentals.c \
    $$PWD/../../evdk_operators/mensuration.c \
    $$PWD/../../evdk_operators/morphological_filters.c \
    $$PWD/../../evdk_operators/noise.c \
    $$PWD/../../evdk_operators/nonlinear_filters.c \
    $$PWD/../../evdk_operators/segmentation.c \
    $$PWD/../../evdk_operators/spatial_filters.c \
    $$PWD/../../evdk_operators/spatial_frequency_filters.c \
    $$PWD/../../evdk_operators/transforms.c \
    Unity/src/unity.c \
    main.c \
    test_graphics_algorithms.c \
    test_coding_and_compression.c \
    test_histogram_operations.c \
    test_image_fundamentals.c \
    test_mensuration.c \
    test_morphological_filters.c \
    test_nonlinear_filters.c \
    test_segmentation.c \
    test_spatial_filters.c \
    test_spatial_frequency_filters.c \
    test_transforms.c \

HEADERS += \
    $$PWD/../../evdk_operators/coding_and_compression.h \
    $$PWD/../../evdk_operators/fonts.h \
    $$PWD/../../evdk_operators/graphics_algorithms.h \
    $$PWD/../../evdk_operators/histogram_operations.h \
    $$PWD/../../evdk_operators/image.h \
    $$PWD/../../evdk_operators/image_fundamentals.h \
    $$PWD/../../evdk_operators/mensuration.h \
    $$PWD/../../evdk_operators/morphological_filters.h \
    $$PWD/../../evdk_operators/noise.h \
    $$PWD/../../evdk_operators/nonlinear_filters.h \
    $$PWD/../../evdk_operators/operators.h \
    $$PWD/../../evdk_operators/segmentation.h \
    $$PWD/../../evdk_operators/spatial_filters.h \
    $$PWD/../../evdk_operators/spatial_frequency_filters.h \
    $$PWD/../../evdk_operators/transforms.h \
    main.h \
    Unity/src/unity.h \
    Unity/src/unity_internals.h \
    test_coding_and_compression.h \
    test_graphics_algorithms.h \
    test_histogram_operations.h \
    test_image_fundamentals.h \
    test_mensuration.h \
    test_morphological_filters.h \
    test_nonlinear_filters.h \
    test_segmentation.h \
    test_spatial_filters.h \
    test_spatial_frequency_filters.h \
    test_transforms.h \

