#!/bin/bash

WORK_PATH=$(pwd)
OUT_PATH=${WORK_PATH}/out/
CODE_PATH=${WORK_PATH}/code/
FILE_PATH=${WORK_PATH}/file/
SELECT_VALUE=$1
MAKE_TARGET=

CheckFile() {
    if [ ! -d ${OUT_PATH} ];then
        mkdir ${OUT_PATH};fi

    if [ ! -d ${CODE_PATH} ];then
        echo "shell  FAILED:no ${CODE_PATH}, exit";exit 1;fi
}

CleanAndMake() {
    echo "shell  -----make begin $(date) -----"
    make clean && make ${MAKE_TARGET}
    echo "shell  -----make end $(date) -----"
}

run_simulator() {
    cd ${OUT_PATH}
    ./cachesimulator.bin ${FILE_PATH}
}

main() {
    echo "shell  -----run.sh begin $(date) -----"
    CheckFile
    if [ "$SELECT_VALUE" = "test1" ];then
        echo "begin test1"
        FILE_PATH=${WORK_PATH}/test/test1/
    elif [ "$SELECT_VALUE" = "test2" ];then
        echo "begin test2"
        FILE_PATH=${WORK_PATH}/test/test2/
    elif [ "$SELECT_VALUE" = "test3" ];then
        echo "begin test3"
        FILE_PATH=${WORK_PATH}/test/test3/
    elif [ "$SELECT_VALUE" = "test4" ];then
        echo "begin test4"
        FILE_PATH=${WORK_PATH}/test/test4/
    else
        echo "make cache simulator"
        MAKE_TARGET=cachesimulator
        CleanAndMake
        exit 0
    fi;

    run_simulator

    # if [ ! -n "$1" ];then
    #     echo "shell  make simulator and run it"
    #     MAKE_TARGET=simulator
    # else
    #     echo "shell  make compiler and run it"
    #     MAKE_TARGET=compiler
    # fi;

    # CleanAndMake
    # echo "shell ----- start ${MAKE_TARGET} -----"
    # cd ${OUT_PATH}
    # ./${MAKE_TARGET}.bin  ${FILE_PATH}
    # echo "shell ----- end ${MAKE_TARGET} -----"

    echo "shell  -----run.sh end   $(date) -----"

}

main

