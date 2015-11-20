#!/bin/sh

# At startup time, wait a few seconds before calling this script, to let time for the program.elf to setup the drone and open sockets.
rm /data/video/gamesystem.log

# Setting the per-thread stack size to 2 MB instead of the default of 8 MB, so that we can create more than a few execution threads.
ulimit -s 2048

if [ $DRONE_STARTUP -eq 1 ]; then

# Launching the program, redirecting its output to a file
/data/video/drone >/data/video/gamesystem.log 2>&1 &

else

# Launching the program, redirecting its output to a file
/data/video/drone 2>&1 | tee /data/video/gamesystem.log

fi
