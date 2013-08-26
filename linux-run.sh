#!/bin/sh

cd "$(dirname "$0")"

#LD_LIBRARY_PATH=$(pwd)/libs/:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$(pwd)/libs/:$LD_LIBRARY_PATH
echo $LD_LIBRARY_PATH
chmod u+xr ./Game
./Game
