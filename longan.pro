#-------------------------------------------------
#
# Project created by QtCreator 2018-09-17T10:08:00
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = longan
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

unix {
    LIBS += -L../longan/lib/ -Wl,-rpath=./:./HCNetSDKCom:../longan/lib -lhcnetsdk -lPlayCtrl -lAudioRender -lSuperRender

    LIBS += -L/usr/local/lib/ -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_imgcodecs

}

INCLUDEPATH += \
    -I./includeCn/ \
    -I./arc_face/ \
    /usr/local/include \
    /usr/local/include/opencv \
    /usr/local/include/opencv2

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    hkcamera.cpp \
    facesdata.cpp \
    cameractrl.cpp \
    framedata.cpp \
    cameraflowevent.cpp \
    convertimageformat.cpp \
    printexectime.cpp \
    framedataprocess.cpp

HEADERS += \
    mainwindow.h \
    includeCn/HCNetSDK.h \
    includeCn/LinuxPlayM4.h \
    includeCn/PlayM4.h \
    arc_face/amcomdef.h \
    arc_face/arcsoft_fsdk_face_recognition.h \
    arc_face/arcsoft_fsdk_face_tracking.h \
    arc_face/asvloffscreen.h \
    arc_face/merror.h \
    hkcamera.h \
    cpptoml/cpptoml.h \
    facesdata.h \
    cameractrl.h \
    framedata.h \
    cameraflowevent.h \
    convertimageformat.h \
    printexectime.h \
    framedataprocess.h

FORMS += \
        mainwindow.ui

DISTFILES += \
    config/config.toml \
    config/config.toml
