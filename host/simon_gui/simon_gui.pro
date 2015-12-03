#-------------------------------------------------
#
# Project created by QtCreator 2015-11-06T09:56:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simon_gui
TEMPLATE = app

SOURCES += main.cpp \
    debugwindow.cpp \
    gamewindow.cpp \
    gamepad.cpp \
    commmanager.cpp

HEADERS  += \
    debugwindow.h \
    gamewindow.h \
    gamepad.h \
    commmanager.h

FORMS    += \
    debugwindow.ui \
    gamewindow.ui \
    gamepad.ui

CONFIG += c++14
INCLUDEPATH += ../../libs.d/libcomm/inc
INCLUDEPATH += ../../libs.d/libconf/include
LIBS += -L../../libs.d/libcomm/bin -lcomm
LIBS += -L../../libs.d/libconf/bin -lconf
