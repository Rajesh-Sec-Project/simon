PROJECT=host
PRODUCT=simon_gui
TMP_DIR=build
BIN_DIR=bin
LIB_DIR=../libs.d

CROSS = 0
DEPENDS = libconf libcomm
QMAKE = qtchooser --run-tool=qmake -qt=5
QMAKE_FLAGS = LIBS+='-lconf -lcomm' QMAKE_CXXFLAGS+='-std=c++14 -Wall'
