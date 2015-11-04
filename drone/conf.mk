#################################
### Directories configuration ###
#################################

SRC_DIR=src
INC_DIR=inc
TMP_DIR=build
BIN_DIR=bin

#####################
### Project names ###
#####################

PROJECT=simon_drone
PRODUCT=simon_drone

#############
### Flags ###
#############

CX_FLAGS = -std=c++14 -static-libstdc++

release: CX_FLAGS += -O3
debug:   CX_FLAGS += -O0 -g

LD_FLAGS = -lm -static-libstdc++

#############
### Tools ###
#############

CX=arm-cortex_a8-linux-gnueabi-g++
LD=$(CX)
