#!/bin/bash

source vars.sh

mkdir -p $PREFIX/$DBG_DIR
./versatile_gdb.sh remote &> $PREFIX/$DBG_DIR/gdbserver.log &
./versatile_gdb.sh local
