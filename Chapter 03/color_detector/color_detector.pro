# -------------------------------------------------
# Project created by QtCreator 2010-09-10T15:33:45
# -------------------------------------------------
TARGET = color_detector
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    colordetector.cpp \
    colordetector.cpp \
    colorDetectController.cpp
HEADERS += mainwindow.h \
    colordetector.h \
    colorDetectController.h
FORMS += mainwindow.ui
INCLUDEPATH += C:\OpenCV2.2\include\
LIBS += -LC:\OpenCV2.2\lib \
    -lopencv_core220 \
    -lopencv_highgui220 \
    -lopencv_imgproc220 \
    -lopencv_features2d220 \
    -lopencv_calib3d220
