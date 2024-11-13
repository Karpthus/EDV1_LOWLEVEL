CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

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
       main.cpp \

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

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

opencv_path = c:/opencv-4.10.0

INCLUDEPATH += $$PWD/../../evdk_operators
INCLUDEPATH += $$opencv_path/build/install/include

LIBS += $$opencv_path/build/install/x64/mingw/bin/libopencv*.dll
