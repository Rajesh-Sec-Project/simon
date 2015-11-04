#!/bin/bash

source vars.sh

function usage
{
    echo "Usage: versatile_gdb.sh <verb>"
    echo "Available verbs:"
    echo "    local:  start the local gdb client"
    echo "    remote: start the remote gdb server"
}

if [[ $# -ne 1 ]]; then
    usage
    exit
fi

ACTION=$1

function start_local_gdb {
   $GDB -ex "target extended $GDB_IP:$GDB_PORT" \
        -ex "file $PREFIX/$BIN_DIR/$PRODUCT"
}

function start_remote_gdb {
    {
        echo "kill \$(ps | grep gdbserver | cut -d ' ' -f 2) 2> /dev/null"
        echo "chmod +x $DIST_PATH/$PRODUCT"
        echo "gdbserver localhost:$GDB_PORT $DIST_PATH/$PRODUCT"
        while :
        do
            sleep 1
        done
    } | nc -v $NC_IP $NC_PORT
}

if [[ $ACTION == "local" ]]; then
    start_local_gdb
elif [[ $ACTION == "remote" ]]; then
    start_remote_gdb
else
    usage
    exit
fi
