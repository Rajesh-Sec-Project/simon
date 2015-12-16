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

QEMU_ARM=qemu-arm
QEMU_SYSROOT=$(HOME)/x-tools/arm-cortex_a8-linux-gnueabi/arm-cortex_a8-linux-gnueabi

#####################
### Project names ###
#####################

PROJECT=drone-sandbox
PRODUCT=drone-sandbox

#############
### Flags ###
#############

CX_FLAGS  = -std=gnu++14 -static-libstdc++ -Wall #-Wsuggest-override
CX_FLAGS += -I$(LIB_DIR)/libpcap/include

release: CX_FLAGS += -O3
debug:   CX_FLAGS += -O0 -g

LD_FLAGS  = -lm -static-libstdc++ -lcomm -lconf -lcontrol -lpthread
LD_FLAGS += -L$(LIB_DIR)/libpcap/lib -lpcap

CROSS = 1
DEPENDS = libcomm libconf libcontrol

#############
### Tools ###
#############

CX=arm-cortex_a8-linux-gnueabi-g++
LD=$(CX)
