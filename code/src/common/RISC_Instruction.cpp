#include <algorithm>
#include "tool.h"
#include "RISC_Instruction.h"
#include "RISC_DEBUG.h"

bool RISC_Instruction::Sync() {
    m_operation = bitset<OP_SIZE>(GetBitSetValue(m_Instruction, 0, OP_SIZE));
    return true;
}

void RISC_Instruction::print() {
    RISC_DEBUG::COUT(string("\n") + m_name + ':', m_Instruction.to_string());
}

bool RISC_Instruction::isReady() {
    return !m_limit_op.compare(m_operation.to_string());
}

bool RISC_Instruction::isDependence(RISC_Instruction* in_ins)
{
    return false;
}

bool RISC_Instruction::setInstruction(const bitset<INS_SIZE> &in_data) {
    m_Instruction = bitset<INS_SIZE>(in_data);
    return this->Sync();
}

bool RISC_RType::Sync() {
    RISC_Instruction::Sync();
    int tmp_index = OP_SIZE;
    m_rd = bitset<RD_SIZE>(GetBitSetValue(m_Instruction, tmp_index, RD_SIZE)); tmp_index += m_rd.size();
    m_funt3 = bitset<FUNT3_SIZE>(GetBitSetValue(m_Instruction, tmp_index, FUNT3_SIZE)); tmp_index += m_funt3.size();
    m_rs1 = bitset<RS1_SIZE>(GetBitSetValue(m_Instruction, tmp_index, RS1_SIZE)); tmp_index += m_rs1.size();
    m_rs2 = bitset<RS2_SIZE>(GetBitSetValue(m_Instruction, tmp_index, RS2_SIZE)); tmp_index += m_rs2.size();
    m_funt7 = bitset<R_FUNT7>(GetBitSetValue(m_Instruction, tmp_index, R_FUNT7));

    if (m_name.empty())
        m_name = "RType";
    return true;
}

bool RISC_RType::isReady() {
    return (RISC_Instruction::isReady()) && !m_limit_funt3.compare(m_funt3.to_string()) 
                                         && !m_limit_funt7.compare(m_funt7.to_string());
}

bool RISC_RType::isDependence(RISC_Instruction* in_ins)
{
    bitset<RD_SIZE> in_rd = bitset<RD_SIZE>(GetBitSetValue(in_ins->getINS(), OP_SIZE, RD_SIZE));

    return m_rs1 == in_rd || m_rs2 == in_rd;
}

bool RISC_IType::Sync() {
    RISC_Instruction::Sync();
    int tmp_index = OP_SIZE;
    m_rd = bitset<RD_SIZE>(GetBitSetValue(m_Instruction, tmp_index, RD_SIZE)); tmp_index += m_rd.size();
    m_funt3 = bitset<FUNT3_SIZE>(GetBitSetValue(m_Instruction, tmp_index, FUNT3_SIZE)); tmp_index += m_funt3.size();
    m_rs1 = bitset<RS1_SIZE>(GetBitSetValue(m_Instruction, tmp_index, RS1_SIZE)); tmp_index += m_rs1.size();

    if (m_Instruction[INS_SIZE-1] == 1) {
        m_imm = bitset<INS_SIZE>(string(20,'1') + GetBitSetValue(m_Instruction, tmp_index, I_IMM));
    } else {
        m_imm = bitset<INS_SIZE>(string(20,'0') + GetBitSetValue(m_Instruction, tmp_index, I_IMM));
    }

    if (m_name.empty())
        m_name = "IType";
    return true;
}

bool RISC_IType::isReady() {
    // RISC_DEBUG::COUT("ITYPE-ins:", m_Instruction.to_string());
    // RISC_DEBUG::COUT("ITYPE-op:", m_operation.to_string());
    // RISC_DEBUG::COUT("ITYPE-limit_op:", m_limit_op);
    // RISC_DEBUG::COUT("ITYPE-funt3:", m_funt3.to_string());
    // RISC_DEBUG::COUT("ITYPE-limit_funt3:", m_limit_funt3);
    // RISC_DEBUG::COUT("ITYPE-RISC_Instruction:", RISC_Instruction::isReady()?"true":"false");
    // RISC_DEBUG::COUT("ITYPE-m_limit_funt3:", (!m_limit_funt3.compare(m_funt3.to_string()))?"true":"false");
    return (RISC_Instruction::isReady()) && !m_limit_funt3.compare(m_funt3.to_string());
}

bool RISC_IType::isDependence(RISC_Instruction* in_ins)
{
    bitset<RD_SIZE> in_rd = bitset<RD_SIZE>(GetBitSetValue(in_ins->getINS(), OP_SIZE, RD_SIZE));

    return m_rs1 == in_rd;
}

bool RISC_Abstract_SType::isReady() {
    return (RISC_Instruction::isReady()) && !m_limit_funt3.compare(m_funt3.to_string());
}

bool RISC_Abstract_SType::isDependence(RISC_Instruction* in_ins)
{
    bitset<RD_SIZE> in_rd = bitset<RD_SIZE>(GetBitSetValue(in_ins->getINS(), OP_SIZE, RD_SIZE));

    return m_rs1 == in_rd || m_rs2 == in_rd;
}

bool RISC_SType::Sync() {
    RISC_Instruction::Sync();
    int tmp_index = OP_SIZE + RD_SIZE;
    m_funt3 = bitset<FUNT3_SIZE>(GetBitSetValue(m_Instruction, tmp_index, FUNT3_SIZE)); tmp_index += m_funt3.size();
    m_rs1 = bitset<RS1_SIZE>(GetBitSetValue(m_Instruction, tmp_index, RS1_SIZE)); tmp_index += m_rs1.size();
    m_rs2 = bitset<RS2_SIZE>(GetBitSetValue(m_Instruction, tmp_index, RS2_SIZE)); tmp_index += m_rs2.size();
    if (m_Instruction[INS_SIZE-1] == 1) {
        m_imm = bitset<INS_SIZE>(string(20,'1') + GetBitSetValue(m_Instruction, 25, R_FUNT7) + GetBitSetValue(m_Instruction, 7, RD_SIZE));
    } else {
        m_imm = bitset<INS_SIZE>(string(20,'0') + GetBitSetValue(m_Instruction, 25, R_FUNT7) + GetBitSetValue(m_Instruction, 7, RD_SIZE));
    }

    if (m_name.empty())
        m_name = "SType";
    return true;
}


bool RISC_SBType::Sync() {
    RISC_Instruction::Sync();
    int tmp_index = OP_SIZE + RD_SIZE;
    m_funt3 = bitset<FUNT3_SIZE>(GetBitSetValue(m_Instruction, tmp_index, FUNT3_SIZE)); tmp_index += m_funt3.size();
    m_rs1 = bitset<RS1_SIZE>(GetBitSetValue(m_Instruction, tmp_index, RS1_SIZE)); tmp_index += m_rs1.size();
    m_rs2 = bitset<RS2_SIZE>(GetBitSetValue(m_Instruction, tmp_index, RS2_SIZE)); tmp_index += m_rs2.size();
    if (m_Instruction[INS_SIZE-1] == 1) {
        m_imm = bitset<INS_SIZE>(string(18, '1') + GetBitSetValue(m_Instruction, 31, 1) + GetBitSetValue(m_Instruction, 7, 1) +
                                        GetBitSetValue(m_Instruction, 25, 6) + GetBitSetValue(m_Instruction, 8, 4) + string("00"));
    } else {
        m_imm = bitset<INS_SIZE>(string(18, '0') + GetBitSetValue(m_Instruction, 31, 1) + GetBitSetValue(m_Instruction, 7, 1) +
                                        GetBitSetValue(m_Instruction, 25, 6) + GetBitSetValue(m_Instruction, 8, 4) + string("00"));
    }

    if (m_name.empty())
        m_name = "SBType";
    return true;
}

bool RISC_UType::Sync() {
    RISC_Instruction::Sync();
    int tmp_index = OP_SIZE;
    m_rd = bitset<RD_SIZE>(GetBitSetValue(m_Instruction, tmp_index, RD_SIZE)); tmp_index += m_rd.size();
    m_imm = bitset<INS_SIZE>(GetBitSetValue(m_Instruction, tmp_index, U_IMM) + string(INS_SIZE-U_IMM, '0'));
    
    if (m_name.empty())
        m_name = "UType";
    return true;
}

bool RISC_UJType::Sync() {
    RISC_Instruction::Sync();
    int tmp_index = OP_SIZE;
    m_rd = bitset<RD_SIZE>(GetBitSetValue(m_Instruction, tmp_index, RD_SIZE)); tmp_index += m_rd.size();
    if (m_Instruction[INS_SIZE-1] == 1) {
        m_imm = bitset<INS_SIZE>(string(10, '1') + GetBitSetValue(m_Instruction, 31, 1) + GetBitSetValue(m_Instruction, 12, 8)  +
                                        GetBitSetValue(m_Instruction, 20, 1)+ GetBitSetValue(m_Instruction, 21, 10)+string("00"));
    } else {
        m_imm = bitset<INS_SIZE>(string(10, '0') + GetBitSetValue(m_Instruction, 31, 1) + GetBitSetValue(m_Instruction, 12, 8)  +
                                        GetBitSetValue(m_Instruction, 20, 1)+ GetBitSetValue(m_Instruction, 21, 10)+string("00"));
    }

    if (m_name.empty())
        m_name = "UJType";
    return true;
}

RISC_Instruction* CreateRiscINS(enum EM_RISC_INS in_flag) {
    RISC_Instruction* result = nullptr;
    switch (in_flag)
    {
    case EM_ADD:
        result = new RISC_ADD();
        break;
    case EM_SUB:
        result = new RISC_SUB();
        break;
    case EM_ADDI:
        result = new RISC_ADDI();
        break;
    case EM_AND:
        result = new RISC_AND();
        break;
    case EM_OR:
        result = new RISC_OR();
        break;
    case EM_XOR:
        result = new RISC_XOR();
        break;
    case EM_BEQ:
        result = new RISC_BEQ();
        break;
    case EM_JAL:
        result = new RISC_JAL();
        break;
    case EM_LD:
        result = new RISC_LD();
        break;
    case EM_SD:
        result = new RISC_SD();
        break;
    case EM_LW:
        result = new RISC_LW();
        break;
    case EM_SW:
        result = new RISC_SW();
        break;
    default:
        result = new RISC_Halt();
        break;
    }
    return result;

}