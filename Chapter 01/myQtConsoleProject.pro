#-------------------------------------------------
#
# Project created by QtCreator
#
# As other .pro files in the example code, this is
# just an example template.
#
# You have to configure INCLUDEPATH and LIBS to
# fit your system.
#
#-------------------------------------------------

QT       -= gui

TARGET = myQtConsoleProject
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

INCLUDEPATH += C:\OpenCV2.2\include\

LIBS += -LC:\OpenCV2.2\lib \
    -lopencv_core220 \
    -lopencv_highgui220 \
    -lopencv_imgproc220 \
    -lopencv_features2d220 \
    -lopencv_calib3d220
