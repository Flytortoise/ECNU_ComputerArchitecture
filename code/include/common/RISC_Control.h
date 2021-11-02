#ifndef RISC_CONTROL_H_
#define RISC_CONTROL_H_

#include "RISC_Instruction.h"
#include "RISC_RF.h"
#include "RISC_ALU.h"
#include "RISC_DataMem.h"
#include "RISC_INSMem.h"

#include <vector>
#include <string>
#include <queue>
#include <list>
using std::vector;

class RISC_Control {
public:
    static RISC_Control *GetInstance();
    static void FreeInstance();
    static void SetFilePath(string path) { 
        RISC_Control::m_filepath = path; 
        m_ins.SetFileName(string(m_filepath).append(INS_INFILE));
        m_data_mem.SetFileName(string(m_filepath).append(DATA_INFILE), string(m_filepath).append(DATA_OUTFILE));
    }

    // experiment 2
    static bool IF();
    static bool ID() { 
        RISC_DEBUG::COUT("----------ID start----------");
        RISC_Control::RF_Func(); 
        RISC_DEBUG::COUT("----------ID end----------");
        return !isPause();
    }

    static bool EX() { 
        RISC_DEBUG::COUT("----------EX start----------");
        RISC_Control::ALU_Func(); 
        RISC_DEBUG::COUT("----------EX end----------");
        return true; 
    }

    static bool ME() { 
        RISC_DEBUG::COUT("----------ME start----------");
        RISC_Control::DataMem_Func(); 
        RISC_DEBUG::COUT("----------ME end----------");
        return true; 
    }

    static bool WB() { 
        RISC_DEBUG::COUT("----------WB start----------");
        RISC_Control::RF_Func_back(); 
        RISC_DEBUG::COUT("----------WB end----------");
        return true; 
    }

    // experiment 1
    static RISC_Instruction *GetINSObject(const bitset<INS_SIZE> &);
    static void RF_Func();
    static void ALU_Func();
    static void RF_Func_back();
    static void DataMem_Func();
    //static bitset<REG_BIT_NUM> GetCurrentALUResult() { 
    //    RISC_DEBUG::COUT("control return alu result:", m_alu_result.to_string());
    //    return m_alu_result; 
    //}
    //static bitset<32> GetBEQImm() {
    //    return static_cast<RISC_SBType *>(m_ins_vector[EM_BEQ].first)->getIMM();
    //}

    static bool isHalt() { return m_is_halt;  }
    static bool isPause() { return m_is_pause; }

private:
    RISC_Control();
    static std::string m_filepath;
    static RISC_Control *m_Instance;
    static vector<RISC_Instruction *> m_ins_vector;
    //static bitset<REG_BIT_NUM> m_alu_result;
    static bitset<32> m_load32_value;
    static bitset<64> m_load64_value;
    static DataMem m_data_mem;
    static INSMem m_ins;
    static bitset<32> m_PC;
    static bool m_is_halt;
    static bool m_is_pause;

    static std::list<std::pair<RISC_Instruction*, RISC_RF_Op*> *> m_if_id;
    static std::list<std::pair<RISC_Instruction*, RISC_RF_Op*> *> m_id_ex;
    static std::list<std::pair<RISC_Instruction*, RISC_RF_Op*> *> m_ex_me;
    static std::list<std::pair<RISC_Instruction*, RISC_RF_Op*> *> m_me_wb;
    static std::list<bitset<REG_BIT_NUM>> m_alu_result;
};

#endif
