#!/bin/sh

rm /gamesystem.log /simon.log

echo "Initializing system..." > /simon.log
sleep 4
ulimit -s 2048 # Setting the per-thread stack size to 1 MB instead of the default of 8 MB, so that we can cr
echo "Initialization OK" >> /simon.log
echo
echo "Launching the game system's program..." >> /simon.log
/data/video/drone >/gamesystem.log 2>&1 &

echo "OK" >> /simon.log
