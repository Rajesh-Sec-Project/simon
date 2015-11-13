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
    MainWindow.cpp \
    gamepad.cpp \
    commmanager.cpp

HEADERS  += \
    MainWindow.h \
    gamepad.h \
    commmanager.h

FORMS    += \
    MainWindow.ui \
    gamepad.ui

CONFIG += c++14
INCLUDEPATH += ../../libs.d/libcomm/inc
INCLUDEPATH += ../../libs.d/libconf/include
LIBS += -L../../libs.d/libcomm/bin -lcomm
LIBS += -L../../libs.d/libconf/bin -lconf
