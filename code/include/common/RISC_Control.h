#ifndef RISC_CONTROL_H_
#define RISC_CONTROL_H_

#include "RISC_Instruction.h"
#include "RISC_RF.h"
#include "RISC_ALU.h"
#include "RISC_DataMem.h"

#include <map>
using std::map;

class RISC_Control {
public:
    static RISC_Control *GetInstance();
    static void FreeInstance();

    static RISC_Instruction *GetINSObject(const bitset<INS_SIZE> &);
    static void RF_Func();
    static void ALU_Func();
    static void RF_Func_back();
    static void DataMem_Func(DataMem&);
    static EM_RISC_INS GetCurrentINSEm() { return m_last_get_em;}
    static bitset<REG_BIT_NUM> GetCurrentALUResult() { 
        RISC_DEBUG::COUT("control return alu result:", m_alu_result.to_string());
        return m_alu_result; 
    }
    static bitset<32> GetBEQImm() {
        return static_cast<RISC_SBType *>(m_map[EM_BEQ].first)->getIMM();
    }

private:
    RISC_Control();
    static RISC_Control *m_Instance;
    static map<EM_RISC_INS, std::pair<RISC_Instruction *, RISC_RF_Op *>> m_map;
    static EM_RISC_INS m_last_get_em;
    static bitset<REG_BIT_NUM> m_alu_result;
    static bitset<32> m_load32_value;
    static bitset<64> m_load64_value;
};

#endif
