#!/bin/bash

WORK_PATH=$(pwd)
OUT_PATH=${WORK_PATH}/out/
CODE_PATH=${WORK_PATH}/code/
FILE_PATH=${WORK_PATH}/file/


echo "shell  -----run.sh begin $(date) -----"

if [ ! -d ${OUT_PATH} ];then
    mkdir ${OUT_PATH};fi

if [ ! -d ${CODE_PATH} ];then
    echo "shell  FAILED:no ${CODE_PATH}, exit";exit 1;fi

if [ ! -n "$1" ];then
    echo "shell  make simulator and run it"
    make clean && make simulator && cd ${OUT_PATH}
    echo "shell ----- start simulator -----"
    ./simulator.bin ${FILE_PATH}
    echo "shell ----- end simulator -----"
else
    echo "shell  make compiler and run it"
    make clean && make compiler && cd ${OUT_PATH}
    echo "shell ----- start compiler -----"
    ./compiler.bin
    echo "shell ----- end compiler -----"
fi;

echo "shell  -----run.sh end   $(date) -----"


