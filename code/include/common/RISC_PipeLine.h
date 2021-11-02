#ifndef RISC_PIPE_LINE_H_
#define RISC_PIPE_LINE_H_

#include <bitset>
#include <vector>
#include <string>
#include <list>
#include "RISC_RF.h"
using std::bitset;
typedef bool(*RISC_Func)();

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

class PipeVector {
public:
    PipeVector(std::vector<RISC_Func> data);

    PipeVector(const PipeVector& data);

    bool dump();

    bool isEnd() {
        return m_curr_index == m_data.size() ? true : false;
    }

private:
    int m_curr_index = 0;
    std::vector<RISC_Func> m_data;

};

class PipeLine {
public:
    PipeLine(const PipeVector& data);

    void run();
    void start() { run();  }

    bool isContinue() {
        return !m_ins_vec.empty();
    }

private:
    PipeVector m_pipe_vector;
    std::list<PipeVector*> m_ins_vec;
};



#endif