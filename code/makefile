PWD_PATH:=$(shell pwd)
OUT_PATH:=$(PWD_PATH)/../out/
COMMON_TARGET:=$(OUT_PATH)/common.so
COMPILER_TARGET:=$(OUT_PATH)/compiler.bin
SIMULATOR_TARGET:=$(OUT_PATH)/simulator.bin
CACHE_SIMULATOR_TARGET:=$(OUT_PATH)/cachesimulator.bin
# PIPELINE_TARGET:=$(OUT_PATH)/pipeline.bin

BUILD:=g++
# HIDE:=@$(BUILD)
HIDE:=$(BUILD)
INCLUDE:=-I $(PWD_PATH)/include/
COMMON_INCLUDE:=$(INCLUDE)/common/
COMPILER_INCLUDE:=$(INCLUDE)/compiler/ $(COMMON_INCLUDE)
SIMULATOR_INCLUDE:=$(INCLUDE)/simulator/  $(COMMON_INCLUDE)

SRC:=$(PWD_PATH)/src/
COMMON_SRC:=$(SRC)/common/*.cpp
COMPILER_SRC:=$(SRC)/compiler/*.cpp
SIMULATOR_SRC:=$(SRC)/simulator/*.cpp
# PIPELINE_SRC:=$(SRC)/pipeline/*.cpp
CACHE_SIMULATOR_SRC:=$(SRC)/cachesimulator.cpp

CC_FLAG:=-fPIC


common:$(COMMON_SRC)
	$(HIDE)  $^ -std=c++11 $(COMMON_INCLUDE) -shared $(CC_FLAG) -o $(COMMON_TARGET)
$(COMMON_TARGET):common

simulator:$(SIMULATOR_SRC) $(COMMON_TARGET) 
	$(HIDE) $^ -std=c++11 $(SIMULATOR_INCLUDE) -o $(SIMULATOR_TARGET)

compiler:$(COMPILER_SRC) $(COMMON_TARGET) 
	$(HIDE) $^ -std=c++11 $(COMPILER_INCLUDE) -o $(COMPILER_TARGET)

cachesimulator:$(CACHE_SIMULATOR_SRC)
	$(HIDE) $^ -std=c++11 -o $(CACHE_SIMULATOR_TARGET)
# pip:$(PIPELINE_SRC) $(COMMON_TARGET)
# 	$(HIDE) $^ -std=c++11  $(COMPILER_INCLUDE) -o $(PIPELINE_TARGET)

clean:
	@rm -rf $(OUT_PATH)/*
