#################################
### Directories configuration ###
#################################

SRC_DIR=src
INC_DIR=inc
TMP_DIR=build
BIN_DIR=bin
LIB_DIR=../../libs.d

#####################
### Project names ###
#####################

PROJECT=libcomm
PRODUCT=libcomm.a

#############
### Flags ###
#############

CX_FLAGS = -std=gnu++11 -I../libconf/include

release: CX_FLAGS += -O3
debug:   CX_FLAGS += -O0 -g

#############
### Tools ###
#############

CX=g++
LD=$(CX)
AR=ar
