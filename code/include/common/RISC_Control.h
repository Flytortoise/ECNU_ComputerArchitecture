#ifndef RISC_CONTROL_H_
#define RISC_CONTROL_H_

#include "RISC_Instruction.h"
#include "RISC_RF.h"
#include "RISC_ALU.h"

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
private:
    RISC_Control();
    static RISC_Control *m_Instance;
    static map<EM_RISC_INS, std::pair<RISC_Instruction *, RISC_RF_Op *>> m_map;
    static EM_RISC_INS m_last_get_em;
    static bitset<REG_BIT_NUM> m_alu_result;
};

#endif
