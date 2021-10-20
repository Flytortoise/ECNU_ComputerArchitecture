#ifndef RISC_PIPE_LINE_H_
#define RISC_PIPE_LINE_H_

#include <bitset>
#include "RISC_RF.h"
using std::bitset;

struct IFStruct {
    bitset<32>  PC;
    bool        nop;  
};

struct IDStruct {
    bitset<INS_SIZE>  Instr;
    bool        nop;  
};

struct EXStruct {
    bitset<64>  Read_data1;
    bitset<64>  Read_data2;
    bitset<16>  Imm;            // todo ? 2021/10/20
    bitset<RS1_SIZE>   Rs;
    bitset<RS2_SIZE>   Rt;
    bitset<RD_SIZE>   Wrt_reg_addr;
    bool        is_I_type;
    bool        rd_mem;
    bool        wrt_mem; 
    bool        alu_op;     //1 for addu, lw, sw, 0 for subu 
    bool        wrt_enable;
    bool        nop;  
};

struct MEMStruct {
    bitset<REG_BIT_NUM>  ALUresult;
    bitset<64>  Store_data;
    bitset<RS1_SIZE>   Rs;
    bitset<RS2_SIZE>   Rt;    
    bitset<RD_SIZE>   Wrt_reg_addr;
    bool        rd_mem;
    bool        wrt_mem; 
    bool        wrt_enable;    
    bool        nop;    
};

struct WBStruct {
    bitset<64>  Wrt_data;
    bitset<RS1_SIZE>   Rs;
    bitset<RS2_SIZE>   Rt;     
    bitset<RD_SIZE>   Wrt_reg_addr;
    bool        wrt_enable;
    bool        nop;     
};

struct stateStruct {
    IFStruct    IF;
    IDStruct    ID;
    EXStruct    EX;
    MEMStruct   MEM;
    WBStruct    WB;
};

void printState(stateStruct state, int cycle);

#endif