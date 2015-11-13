#################################
### Directories configuration ###
#################################

SRC_DIR=src
INC_DIR=inc
TST_DIR=tests
TMP_DIR=build
BIN_DIR=bin
DBG_DIR=debug
LIB_DIR=../libs.d

#####################
### Project names ###
#####################

PROJECT=host-sandbox
PRODUCT=host-sandbox

#############
### Flags ###
#############

CX_FLAGS = -std=gnu++14

release: CX_FLAGS += -O3
debug:   CX_FLAGS += -O0 -g

LD_FLAGS = -lm -lcomm -lconf -lpthread

CROSS = 0
DEPENDS = libcomm libconf

#############
### Tools ###
#############

CX=g++
LD=$(CX)
