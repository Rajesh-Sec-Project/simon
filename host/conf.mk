PROJECT=simon_gui
PRODUCT=simon_gui
TMP_DIR=build
BIN_DIR=bin
LIB_DIR=../libs.d

CROSS = 0
DEPENDS = libconf
QMAKE_FLAGS = LIBS+=-lconf
