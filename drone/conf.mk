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
PRODUCT=simon.elf

#############
### Flags ###
#############

CX_FLAGS = -std=gnu++11

release: CX_FLAGS += -O3
debug:   CX_FLAGS += -O0 -g

LD_FLAGS = -lm

#############
### Tools ###
#############

CX=armv7a-softfloat-linux-gnueabi-g++
LD=$(CX)
