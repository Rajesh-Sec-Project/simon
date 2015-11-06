#!/bin/bash

source vars.sh

ftp -v -n $FTP_IP $FTP_PORT <<END
binary
lcd $PREFIX/$BIN_DIR
cd $DIST_PATH
put $PRODUCT
quit
END
