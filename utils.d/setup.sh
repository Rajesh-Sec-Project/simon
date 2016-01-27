#!/bin/sh

echo "Starting drone setup..."

export FTP_PORT=21
export NC_PORT=23
export IP=192.168.1.1

echo "********* Pushing binary to drone"

ftp -v -n $IP $FTP_PORT <<END
binary
lcd docs.d/binaries
cd /data/video
put drone
quit
END

echo "********* Setting up USB port..."

ftp -v -n $IP $FTP_PORT <<END
binary
lcd docs.d/binaries
cd /data/video
put cdc-acm.ko
quit
END

function send {
	echo "$1; exit" | nc -v $IP $NC_PORT
}

function send_and_wait {
    {
        echo "$1"
        while :
        do
            sleep 1
        done
    } | nc -v $IP $NC_PORT
}

send "insmod /data/video/cdc-acm.ko"

read -p "******* Please plug in the LED board, press any key when done..."  -n1 -s

echo "******** Starting remote application"

send "chmod +x data/video/drone && killall -9 drone && killall -9 program.elf"
sleep 1
send_and_wait "/data/video/drone" &

echo "******** Starting GUI"

host/bin/simon_gui &
