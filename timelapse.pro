#-------------------------------------------------
#
# Project created by QtCreator 2015-07-05T07:39:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = timelapse
TEMPLATE = app


INCLUDEPATH += "../../opencv/build/include"
LIBS += -L"../../opencv/build/x64/vc12/lib"

debug {
    LIBS += -lopencv_imgproc2410d \
    -lopencv_highgui2410d \
    -lopencv_video2410d \
    -lopencv_core2410d
}
release {
    LIBS += -lopencv_imgproc2410 \
    -lopencv_highgui2410 \
    -lopencv_video2410 \
    -lopencv_core2410
}




SOURCES += main.cpp\
        mainwindow.cpp \
    utility.cpp

HEADERS  += mainwindow.h \
    utility.h

FORMS    += mainwindow.ui
