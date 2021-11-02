#include "RISC_Control.h"

RISC_Control *RISC_Control::m_Instance = nullptr;
vector<RISC_Instruction *> RISC_Control::m_ins_vector;
bitset<32> RISC_Control::m_load32_value;
bitset<64> RISC_Control::m_load64_value;
DataMem RISC_Control::m_data_mem;
std::string RISC_Control::m_filepath = "./";
INSMem RISC_Control::m_ins;
bitset<32> RISC_Control::m_PC;
bool RISC_Control::m_is_halt = false;
bool RISC_Control::m_is_pause = false;

std::list<bitset<REG_BIT_NUM>> RISC_Control::m_alu_result;
std::list<std::pair<RISC_Instruction*, RISC_RF_Op*> *> RISC_Control::m_if_id;
std::list<std::pair<RISC_Instruction*, RISC_RF_Op*> *> RISC_Control::m_id_ex;
std::list<std::pair<RISC_Instruction*, RISC_RF_Op*> *> RISC_Control::m_ex_me;
std::list<std::pair<RISC_Instruction*, RISC_RF_Op*> *> RISC_Control::m_me_wb;

RISC_Control *RISC_Control::GetInstance() {
    if (m_Instance == nullptr) {
        m_Instance = new RISC_Control();
    }
    return m_Instance;
}

void RISC_Control::FreeInstance() {
    m_data_mem.OutputDataMem();

    for (auto it = m_ins_vector.begin(); it != m_ins_vector.end(); ++it) {
        delete *it;
    }
    delete m_Instance;
}

RISC_Control::RISC_Control() {
    m_ins_vector.push_back(CreateRiscINS(EM_HALT));
    m_ins_vector.push_back(CreateRiscINS(EM_ADD));
    m_ins_vector.push_back(CreateRiscINS(EM_SUB));
    m_ins_vector.push_back(CreateRiscINS(EM_ADDI));
    m_ins_vector.push_back(CreateRiscINS(EM_AND));
    m_ins_vector.push_back(CreateRiscINS(EM_OR));
    m_ins_vector.push_back(CreateRiscINS(EM_XOR));
    m_ins_vector.push_back(CreateRiscINS(EM_BEQ));
    m_ins_vector.push_back(CreateRiscINS(EM_JAL));
    m_ins_vector.push_back(CreateRiscINS(EM_LD));
    m_ins_vector.push_back(CreateRiscINS(EM_SD));
    m_ins_vector.push_back(CreateRiscINS(EM_LW));
    m_ins_vector.push_back(CreateRiscINS(EM_SW));
}

RISC_Instruction *RISC_Control::GetINSObject(const bitset<INS_SIZE> &in_data) {

    for (auto it = m_ins_vector.begin(); it != m_ins_vector.end(); ++it) {
        (*it)->setInstruction(in_data);
        if ((*it)->isReady()) {
            RISC_Instruction*tmp_ins = CreateRiscINS((*it)->getEM());
            tmp_ins->setInstruction(in_data);
            std::pair<RISC_Instruction*, RISC_RF_Op*>* tmp_pair = 
                    new std::pair<RISC_Instruction*, RISC_RF_Op*>(tmp_ins, CreateRiscOp(tmp_ins));
            m_if_id.push_back(tmp_pair);
            return tmp_ins;
        }
    }

    return nullptr;
}

bool RISC_Control::IF() {
    RISC_DEBUG::COUT("----------IF start----------");
    RISC_DEBUG::COUT("start PC:", std::to_string(m_PC.to_ulong()));
    // 1. Fetch Instruction
    bitset<32> instruction = m_ins.ReadMemory(m_PC);
    RISC_Instruction* ins_obj = RISC_Control::GetINSObject(instruction);
    if (ins_obj != nullptr) {
        ins_obj->print();
        if (ins_obj->getEM() == EM_HALT) {
            RISC_DEBUG::COUT("this is 'Halt instruction', save data mem to dmemresult.txt and exit simulator");
            m_is_halt = true;
            return false;
        }
    }
    else {
        RISC_DEBUG::COUT("FAILED: instruction is incorrect, eixt !");
        m_is_halt = true;
        return false;
    }

    // Update pc
    /*if (RISC_Control::GetCurrentINSEm() == EM_BEQ &&
        RISC_Control::GetCurrentALUResult().to_ulong() == 0) {
        m_PC = bitset<32>(m_PC.to_ulong() + RISC_Control::GetBEQImm().to_ulong());
    }
    else if (RISC_Control::GetCurrentINSEm() == EM_JAL) {
        m_PC = bitset<32>(m_PC.to_ulong() + 4 + RISC_Control::GetCurrentALUResult().to_ulong());
    }
    else {*/
        m_PC = bitset<32>(m_PC.to_ulong() + 4);
    //}
    RISC_DEBUG::COUT("after update PC:", std::to_string(m_PC.to_ulong()));
    RISC_DEBUG::COUT("----------IF end----------");
    return true;
}

void RISC_Control::RF_Func() {
    std::pair<RISC_Instruction*, RISC_RF_Op*>* tmp_pair = m_if_id.front();

    std::pair<RISC_Instruction*, RISC_RF_Op*>* check_pair_1 = nullptr;
    std::pair<RISC_Instruction*, RISC_RF_Op*>* check_pair_2 = nullptr;
   
    if (m_ex_me.size() > 0) {
        check_pair_1 = m_ex_me.front();
    }

    if (m_me_wb.size() > 0) {
        check_pair_2 = m_me_wb.front();
    }

    if (CheckDependence(tmp_pair, check_pair_1, check_pair_2)) {
        m_is_pause = true;
        return;
    }

    tmp_pair->second->RF_Func();
    
    m_if_id.pop_front();
    m_id_ex.push_back(tmp_pair);
}

void RISC_Control::ALU_Func() {
    std::pair<RISC_Instruction*, RISC_RF_Op*>* tmp_pair = m_id_ex.front();

    RISC_RF_Op *tmp_op = tmp_pair->second;
    bitset<REG_BIT_NUM> op1, op2;
    switch (tmp_pair->first->getEM())
    {
            // R-TYPE
        case EM_ADD:
        case EM_SUB:
        case EM_AND:
        case EM_OR:
        case EM_XOR: 
        {
            RISC_RF_Op_RType *tmp_type = static_cast<RISC_RF_Op_RType *>(tmp_op);
            op1 = tmp_type->getRs1Data();
            op2 = tmp_type->getRs2Data();
            break;
        }
            // I-Type
        case EM_ADDI:
        case EM_LD:
        case EM_LW:
        {
            RISC_RF_Op_IType *tmp_type = static_cast<RISC_RF_Op_IType *>(tmp_op);
            op1 = tmp_type->getRs1Data();
            RISC_IType *tmp_ins = static_cast<RISC_IType *>(tmp_pair->first);
            bitset<INS_SIZE> tmp_imm = tmp_ins->getIMM();
            RISC_DEBUG::COUT("control itype tmp_imm:", tmp_imm.to_string());
            if (tmp_imm[INS_SIZE-1] == 1) {
                op2 = bitset<REG_BIT_NUM>(string(REG_BIT_NUM-INS_SIZE,'1') + tmp_imm.to_string());
            } else {
                op2 = bitset<REG_BIT_NUM>(string(REG_BIT_NUM-INS_SIZE,'0') + tmp_imm.to_string());
            }
            break;
        }
            // SB-TYPE
        case EM_BEQ:
        {
            RISC_RF_Op_SBType *tmp_type = static_cast<RISC_RF_Op_SBType *>(tmp_op);
            op1 = tmp_type->getRs1Data();
            op2 = tmp_type->getRs2Data();
            break;
        }
            // UJ-TYPE
        case EM_JAL:
        {
            // RISC_RF_Op_UJType *tmp_type = static_cast<RISC_RF_Op_UJType *>(tmp_op);
            RISC_UJType *tmp_ins = static_cast<RISC_UJType *>(tmp_pair->first);
            op1 = bitset<REG_BIT_NUM>(tmp_ins->getIMM().to_string());
            break;
        }
            // S-TYPE
        case EM_SD:
        case EM_SW:
        {
            RISC_RF_Op_SType *tmp_type = static_cast<RISC_RF_Op_SType *>(tmp_op);
            op1 = tmp_type->getRs1Data();
            bitset<32> tmp_value = static_cast<RISC_Abstract_SType *>(tmp_pair->first)->getIMM();
            op2 = bitset<64>(tmp_value.to_string());
            break;
        }

        default:
            break;
    }

    m_alu_result.push_back(RISC_ALU::ALUOperation(tmp_pair->first->getEM(), op1, op2));
    m_id_ex.pop_front();
    m_ex_me.push_back(tmp_pair);
}

void RISC_Control::RF_Func_back() {
    std::pair<RISC_Instruction*, RISC_RF_Op*>* tmp_pair = m_me_wb.front();
    RISC_RF_Op *tmp_op = tmp_pair->second;
    switch (tmp_pair->first->getEM())
    {
            // R-TYPE
        case EM_ADD:
        case EM_SUB:
        case EM_AND:
        case EM_OR:
        case EM_XOR: 
        {
            bitset<REG_BIT_NUM> alu_result = m_alu_result.front();
            RISC_RF_Op_RType *tmp_type = static_cast<RISC_RF_Op_RType *>(tmp_op);
            tmp_type->setRdData(alu_result);
            m_alu_result.pop_front();
            break;
        }
            // I-Type
        case EM_ADDI:
        {
            bitset<REG_BIT_NUM> alu_result = m_alu_result.front();
            RISC_RF_Op_IType *tmp_type = static_cast<RISC_RF_Op_IType *>(tmp_op);
            tmp_type->setRdData(alu_result);
            m_alu_result.pop_front();
            break;
        }
        case EM_LD:
        {
            RISC_RF_Op_IType *tmp_type = static_cast<RISC_RF_Op_IType *>(tmp_op);
            tmp_type->setRdData(m_load64_value);
            break;
        }
        case EM_LW:
        {
            RISC_RF_Op_IType *tmp_type = static_cast<RISC_RF_Op_IType *>(tmp_op);
            tmp_type->setRdData(bitset<64>(m_load32_value.to_string()));
            break;
        }
            // SB-TYPE
        case EM_BEQ:
        {
            RISC_RF_Op_SBType *tmp_type = static_cast<RISC_RF_Op_SBType *>(tmp_op);
            break;
        }
            // UJ-TYPE
        case EM_JAL:
        {
            bitset<REG_BIT_NUM> alu_result = m_alu_result.front();
            RISC_RF_Op_UJType *tmp_type = static_cast<RISC_RF_Op_UJType *>(tmp_op);
            tmp_type->setRdData(alu_result);
            m_alu_result.pop_front();
            break;
        }
            // S-TYPE
        case EM_SD:
        case EM_SW:
        {
            RISC_DEBUG::COUT("control S-TYPE func back");
            RISC_RF_Op_SType *tmp_type = static_cast<RISC_RF_Op_SType *>(tmp_op);
            break;
        }
        default:
            break;
    }
    tmp_op->RF_Func_back();

    if (tmp_pair->first->getDependence() && m_is_pause) {
        tmp_pair->first->setDependence(false);
        m_is_pause = false;     //resume pipeline
    }

    m_me_wb.pop_front();
    delete tmp_pair->second;
    delete tmp_pair->first;
    delete tmp_pair;
}

void RISC_Control::DataMem_Func() {
    std::pair<RISC_Instruction*, RISC_RF_Op*>* tmp_pair = m_ex_me.front();
    bitset<REG_BIT_NUM> alu_result = m_alu_result.front();
    switch (tmp_pair->first->getEM())
    {
        case EM_LD:
        {
            m_load64_value.reset();
            m_load64_value = m_data_mem.Load64Memory(alu_result);
            m_alu_result.pop_front();
            RISC_DEBUG::COUT("load 64 value:", m_load64_value.to_string());
            break;
        }
        case EM_LW:
        {
            m_load32_value.reset();
            m_load32_value = m_data_mem.LoadMemory(alu_result);
            m_alu_result.pop_front();
            RISC_DEBUG::COUT("load 32 value:", m_load32_value.to_string());
            break;
        }
            // S-TYPE
        case EM_SD:
        {
            RISC_DEBUG::COUT("control SD DataMem_Func");
            RISC_RF_Op_SType *tmp_type = static_cast<RISC_RF_Op_SType *>(tmp_pair->second);
            m_data_mem.StoreMemory(alu_result, tmp_type->getRs2Data());
            m_alu_result.pop_front();
            RISC_DEBUG::COUT("control SD DataMem_Func end");
            break;
        }
        case EM_SW:
        {
            RISC_DEBUG::COUT("control SW DataMem_Func");
            RISC_RF_Op_SType *tmp_type = static_cast<RISC_RF_Op_SType *>(tmp_pair->second);
            m_data_mem.StoreMemory(alu_result, bitset<32>(GetBitSetValue(tmp_type->getRs2Data().to_string(), 0, 32)));
            m_alu_result.pop_front();
            RISC_DEBUG::COUT("control SW DataMem_Func end");
            break;
        }
        default:
            break;
    }
    m_ex_me.pop_front();
    m_me_wb.push_back(tmp_pair);
}