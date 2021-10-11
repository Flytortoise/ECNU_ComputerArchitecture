PWD_PATH:=$(shell pwd)
OUT_PATH:=$(PWD_PATH)/../out/
COMMON_TARGET:=$(OUT_PATH)/common.o
COMPILER_TARGET:=$(OUT_PATH)/compiler.bin
SIMULATOR_TARGET:=$(OUT_PATH)/simulator.bin

BUILD:=g++
HIDE:=@$(BUILD)
INCLUDE:=-I $(PWD_PATH)/include/
COMMON_INCLUDE:=$(INCLUDE)/common/
COMPILER_INCLUDE:=$(INCLUDE)/compiler/ $(COMMON_INCLUDE)
SIMULATOR_INCLUDE:=$(INCLUDE)/simulator/  $(COMMON_INCLUDE)

SRC:=$(PWD_PATH)/src/
COMMON_SRC:=$(SRC)/common/*.cpp
COMPILER_SRC:=$(SRC)/compiler/*.cpp
SIMULATOR_SRC:=$(SRC)/simulator/*.cpp


common:$(COMMON_SRC)
	$(HIDE) -c $^ -std=c++11 $(COMMON_INCLUDE) -o $(COMMON_TARGET)
$(COMMON_TARGET):common

simulator:$(SIMULATOR_SRC) $(COMMON_TARGET) 
	$(HIDE) $^ -std=c++11 $(SIMULATOR_INCLUDE) -o $(SIMULATOR_TARGET)

compiler:$(COMPILER_SRC) $(COMMON_TARGET) 
	$(HIDE) $^ -std=c++11 $(COMPILER_INCLUDE) -o $(COMPILER_TARGET)

clean:
	@rm -rf $(OUT_PATH)/*
