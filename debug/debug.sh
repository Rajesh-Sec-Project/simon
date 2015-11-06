#!/bin/bash

source vars.sh

function start_local_gdb {
   $GDB -ex "target extended $GDB_IP:$GDB_PORT" \
        -ex "file $PREFIX/$BIN_DIR/$PRODUCT"
}

function send_and_wait {
    {
        echo "$1"
        while :
        do
            sleep 1
        done
    } | nc -v $NC_IP $NC_PORT
}

function send {
    echo "$1; exit" | nc -v $NC_IP $NC_PORT
}

function stop_remote_gdb {
    send "killall gdbserver"
}

function start_remote_gdb {
    send "chmod +x $DIST_PATH/$PRODUCT"
    send_and_wait "gdbserver localhost:$GDB_PORT $DIST_PATH/$PRODUCT"
}

mkdir -p $PREFIX/$DBG_DIR
stop_remote_gdb &> /dev/null
#TODO: append to log and add some header w/ date, etc.
start_remote_gdb &> $PREFIX/$DBG_DIR/gdbserver.log &
sleep 1
start_local_gdb
stop_remote_gdb &> /dev/null
