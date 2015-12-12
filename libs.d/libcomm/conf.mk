#################################
### Directories configuration ###
#################################

SRC_DIR=src
INC_DIR=inc
TMP_DIR=build
BIN_DIR=bin
LIB_DIR=../

#####################
### Project names ###
#####################

PROJECT=libcomm
PRODUCT=libcomm.a

#############
### Flags ###
#############

CX_FLAGS = -std=c++14 -I../libconf/include -Wall #-Wsuggest-override

release: CX_FLAGS += -O3
debug:   CX_FLAGS += -O0 -g

#############
### Tools ###
#############

CX=g++
LD=$(CX)
AR=ar
