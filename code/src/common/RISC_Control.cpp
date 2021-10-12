#include "RISC_Control.h"

RISC_Control *RISC_Control::m_Instance = nullptr;
map<EM_RISC_INS, std::pair<RISC_Instruction *, RISC_RF_Op *>> RISC_Control::m_map;
EM_RISC_INS RISC_Control::m_last_get_em = EM_ADD;
bitset<REG_BIT_NUM> RISC_Control::m_alu_result;
bitset<32> RISC_Control::m_load_value;
RISC_Control *RISC_Control::GetInstance() {
    if (m_Instance == nullptr) {
        m_Instance = new RISC_Control();
    }
    return m_Instance;
}

void RISC_Control::FreeInstance() {
    delete m_Instance;
}

RISC_Control::RISC_Control() {
    RISC_Instruction *tmp_ins = new RISC_ADD();
    RISC_RF_Op *tmp_op = new RISC_RF_Op_RType(static_cast<RISC_RType *>(tmp_ins));
    m_map[EM_ADD] = std::pair<RISC_Instruction *, RISC_RF_Op *>(tmp_ins, tmp_op);

    tmp_ins = new RISC_SUB();
    tmp_op = new RISC_RF_Op_RType(static_cast<RISC_RType *>(tmp_ins));
    m_map[EM_SUB] = std::pair<RISC_Instruction *, RISC_RF_Op *>(tmp_ins, tmp_op);

    tmp_ins = new RISC_ADDI();
    tmp_op = new RISC_RF_Op_IType(static_cast<RISC_IType *>(tmp_ins));
    m_map[EM_ADDI] = std::pair<RISC_Instruction *, RISC_RF_Op *>(tmp_ins, tmp_op);

    tmp_ins = new RISC_AND();
    tmp_op = new RISC_RF_Op_RType(static_cast<RISC_RType *>(tmp_ins));
    m_map[EM_AND] = std::pair<RISC_Instruction *, RISC_RF_Op *>(tmp_ins, tmp_op);

    tmp_ins = new RISC_OR();
    tmp_op = new RISC_RF_Op_RType(static_cast<RISC_RType *>(tmp_ins));
    m_map[EM_OR] = std::pair<RISC_Instruction *, RISC_RF_Op *>(tmp_ins, tmp_op);

    tmp_ins = new RISC_XOR();
    tmp_op = new RISC_RF_Op_RType(static_cast<RISC_RType *>(tmp_ins));
    m_map[EM_XOR] = std::pair<RISC_Instruction *, RISC_RF_Op *>(tmp_ins, tmp_op);

    tmp_ins = new RISC_BEQ();
    tmp_op = new RISC_RF_Op_SBType(static_cast<RISC_SBType *>(tmp_ins));
    m_map[EM_BEQ] = std::pair<RISC_Instruction *, RISC_RF_Op *>(tmp_ins, tmp_op);

    tmp_ins = new RISC_JAL();
    tmp_op = new RISC_RF_Op_UJType(static_cast<RISC_UJType *>(tmp_ins));
    m_map[EM_JAL] = std::pair<RISC_Instruction *, RISC_RF_Op *>(tmp_ins, tmp_op);

    tmp_ins = new RISC_LD();
    tmp_op = new RISC_RF_Op_IType(static_cast<RISC_IType *>(tmp_ins));
    m_map[EM_LD] = std::pair<RISC_Instruction *, RISC_RF_Op *>(tmp_ins, tmp_op);

    tmp_ins = new RISC_SD();
    tmp_op = new RISC_RF_Op_SType(static_cast<RISC_SType *>(tmp_ins));
    m_map[EM_SD] = std::pair<RISC_Instruction *, RISC_RF_Op *>(tmp_ins, tmp_op);

    tmp_ins = new RISC_LW();
    tmp_op = new RISC_RF_Op_IType(static_cast<RISC_IType *>(tmp_ins));
    m_map[EM_LW] = std::pair<RISC_Instruction *, RISC_RF_Op *>(tmp_ins, tmp_op);

    tmp_ins = new RISC_SW();
    tmp_op = new RISC_RF_Op_SType(static_cast<RISC_SType *>(tmp_ins));
    m_map[EM_SW] = std::pair<RISC_Instruction *, RISC_RF_Op *>(tmp_ins, tmp_op);
    
}

RISC_Instruction *RISC_Control::GetINSObject(const bitset<INS_SIZE> &in_data) {
    m_map[m_last_get_em].first->setInstruction(bitset<INS_SIZE>());  //init empty

    for (auto it = m_map.begin(); it != m_map.end(); ++it) {
        it->second.first->setInstruction(in_data);
        if (it->second.first->isReady()) {
            m_last_get_em = it->first;
            return it->second.first;
        }
    }
    return nullptr;
}

void RISC_Control::RF_Func() {
    RISC_RF_Op *tmp_op = m_map[m_last_get_em].second;
    tmp_op->RF_Func();
}

void RISC_Control::ALU_Func() {
    m_alu_result.reset();
    RISC_RF_Op *tmp_op = m_map[m_last_get_em].second;
    bitset<REG_BIT_NUM> op1, op2;
    switch (m_last_get_em)
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
            RISC_IType *tmp_ins = static_cast<RISC_IType *>(m_map[m_last_get_em].first);
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
            RISC_UJType *tmp_ins = static_cast<RISC_UJType *>(m_map[m_last_get_em].first);
            op1 = bitset<REG_BIT_NUM>(tmp_ins->getIMM().to_string());
            break;
        }
            // S-TYPE
        case EM_SD:
        case EM_SW:
        {
            RISC_RF_Op_SType *tmp_type = static_cast<RISC_RF_Op_SType *>(tmp_op);
            op1 = tmp_type->getRs1Data();
            bitset<32> tmp_value = static_cast<RISC_Abstract_SType *>(m_map[m_last_get_em].first)->getIMM();
            op2 = bitset<64>(tmp_value.to_string());
            break;
        }
        default:
            break;
    }

    m_alu_result = RISC_ALU::ALUOperation(m_last_get_em, op1, op2);
}

void RISC_Control::RF_Func_back() {
    RISC_RF_Op *tmp_op = m_map[m_last_get_em].second;
    switch (m_last_get_em)
    {
            // R-TYPE
        case EM_ADD:
        case EM_SUB:
        case EM_AND:
        case EM_OR:
        case EM_XOR: 
        {
            RISC_RF_Op_RType *tmp_type = static_cast<RISC_RF_Op_RType *>(tmp_op);
            tmp_type->setRdData(m_alu_result);
            break;
        }
            // I-Type
        case EM_ADDI:
        {
            RISC_RF_Op_IType *tmp_type = static_cast<RISC_RF_Op_IType *>(tmp_op);
            tmp_type->setRdData(m_alu_result);
            break;
        }
        case EM_LD:
        case EM_LW:
        {
            RISC_RF_Op_IType *tmp_type = static_cast<RISC_RF_Op_IType *>(tmp_op);
            tmp_type->setRdData(bitset<64>(m_load_value.to_string()));
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
            RISC_RF_Op_UJType *tmp_type = static_cast<RISC_RF_Op_UJType *>(tmp_op);
            tmp_type->setRdData(m_alu_result);
            break;
        }
            // S-TYPE
        case EM_SD:
        case EM_SW:
        {
            RISC_DEBUG::COUT("control sw func back");
            RISC_RF_Op_SType *tmp_type = static_cast<RISC_RF_Op_SType *>(tmp_op);
            break;
        }
        default:
            break;
    }
    tmp_op->RF_Func_back();
}

void RISC_Control::DataMem_Func(DataMem& in_dataMem) {
    switch (m_last_get_em)
    {
        case EM_LD:
        case EM_LW:
        {
            m_load_value.reset();
            m_load_value = in_dataMem.LoadMemory(m_alu_result);
            RISC_DEBUG::COUT("load value:", m_load_value.to_string());
            break;
        }
            // S-TYPE
        case EM_SD:
        case EM_SW:
        {
            RISC_DEBUG::COUT("control DataMem_Func");
            RISC_RF_Op_SType *tmp_type = static_cast<RISC_RF_Op_SType *>(m_map[m_last_get_em].second);
            in_dataMem.StoreMemory(m_alu_result, tmp_type->getRs2Data());
            RISC_DEBUG::COUT("control DataMem_Func end");
            break;
        }
        default:
            break;
    }
}