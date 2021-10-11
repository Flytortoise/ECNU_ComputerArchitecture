#ifndef RISC_ALU_H_
#define RISC_ALU_H_

#include "common.h"
#include "RISC_RF.h"
#include "RISC_Instruction.h"

/*
 implements the ALU. 
 Your job is to implement ALUOperation() member function that performs the appropriate operation on two 64 bit operands based on ALUOP. 
 See Table 1 for more details.
*/
class RISC_ALU
{
public:
    static RISC_ALU *GetInstance();
    static void FreeInstance();
    
    static bitset<REG_BIT_NUM> ALUOperation(EM_RISC_INS ALUOP, bitset<REG_BIT_NUM> oprand1, bitset<REG_BIT_NUM> oprand2);

private:
    RISC_ALU() = default;
    static RISC_ALU *m_ptr;
};


#endif