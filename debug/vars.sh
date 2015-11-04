#!/bin/bash

source conf.sh

export GDB_IP=$REMOTE_IP
export NC_IP=$REMOTE_IP
export FTP_IP=$REMOTE_IP

export MK_FILE=$PREFIX/$CONF_MK
export PRODUCT="$(cat $MK_FILE | sed -n 's/.*PRODUCT=\(.*\)/\1/p')"
export BIN_DIR="$(cat $MK_FILE | sed -n 's/.*BIN_DIR=\(.*\)/\1/p')"
export DBG_DIR="$(cat $MK_FILE | sed -n 's/.*DBG_DIR=\(.*\)/\1/p')"
export GDB="$(cat $MK_FILE | sed -n 's/.*CX=\(.*\)-g++/\1-gdb/p')"
