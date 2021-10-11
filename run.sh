#!/bin/bash

WORK_PATH=$(pwd)
OUT_PATH=${WORK_PATH}/out/
code_PATH=${WORK_PATH}/code/

echo "-----run.sh begin $(date) -----"

if [ ! -d ${OUT_PATH} ];then
    mkdir ${OUT_PATH};fi

if [ ! -d ${code_PATH} ];then
    echo "FAILED:no ${code_PATH}, exit";exit 1;fi

if [ ! -n "$1" ];then
    echo "make simulator and run it"
    cd ${code_PATH} && make clean && make simulator && cd ${OUT_PATH}
    ./simulator.bin
else
    echo "make compiler and run it"
    cd ${code_PATH} && make clean && make compiler && cd ${OUT_PATH}
    ./compiler.bin;fi

echo "-----run.sh end   $(date) -----"


