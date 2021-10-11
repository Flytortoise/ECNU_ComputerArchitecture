#include <algorithm>
#include "tool.h"
#include "RISC_Instruction.h"
#include "RISC_DEBUG.h"

bool RISC_Instruction::Sync() {
    m_operation = bitset<OP_SIZE>(GetBitSetValue(m_Instruction, 0, OP_SIZE));
    return true;
}

void RISC_Instruction::print() {
    RISC_DEBUG::COUT(m_name + ':', m_Instruction.to_string());
}

bool RISC_Instruction::isReady() {
    return !m_limit_op.compare(m_operation.to_string());
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

bool RISC_Abstract_SType::isReady() {
    return (RISC_Instruction::isReady()) && !m_limit_funt3.compare(m_funt3.to_string());
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
        m_imm = bitset<INS_SIZE>(string(19, '1') + GetBitSetValue(m_Instruction, 31, 1) + GetBitSetValue(m_Instruction, 7, 1) +
                                        GetBitSetValue(m_Instruction, 25, 6) + GetBitSetValue(m_Instruction, 8, 4) + string("0"));
    } else {
        m_imm = bitset<INS_SIZE>(string(19, '0') + GetBitSetValue(m_Instruction, 31, 1) + GetBitSetValue(m_Instruction, 7, 1) +
                                        GetBitSetValue(m_Instruction, 25, 6) + GetBitSetValue(m_Instruction, 8, 4) + string("0"));
    }

    if (m_name.empty())
        m_name = "SBType";
    return true;
}

bool RISC_UType::Sync() {
    RISC_Instruction::Sync();
    int tmp_index = OP_SIZE;
    m_rd = bitset<RD_SIZE>(GetBitSetValue(m_Instruction, tmp_index, RD_SIZE)); tmp_index += m_rd.size();
    if (m_Instruction[INS_SIZE-1] == 1) {       // this is need ??? TBD
        m_imm = bitset<INS_SIZE>(string(INS_SIZE-U_IMM, '1') + GetBitSetValue(m_Instruction, tmp_index, U_IMM));
    } else {
        m_imm = bitset<INS_SIZE>(string(INS_SIZE-U_IMM, '0') + GetBitSetValue(m_Instruction, tmp_index, U_IMM));
    }
    
    if (m_name.empty())
        m_name = "UType";
    return true;
}

bool RISC_UJType::Sync() {
    RISC_Instruction::Sync();
    int tmp_index = OP_SIZE;
    m_rd = bitset<RD_SIZE>(GetBitSetValue(m_Instruction, tmp_index, RD_SIZE)); tmp_index += m_rd.size();
    if (m_Instruction[INS_SIZE-1] == 1) {        // this is need ??? TBD
        m_imm = bitset<INS_SIZE>(string(11, '1') + GetBitSetValue(m_Instruction, 31, 1) + GetBitSetValue(m_Instruction, 12, 8)  +
                                        GetBitSetValue(m_Instruction, 20, 1)+ GetBitSetValue(m_Instruction, 21, 10)+string("0"));
    } else {
        m_imm = bitset<INS_SIZE>(string(11, '0') + GetBitSetValue(m_Instruction, 31, 1) + GetBitSetValue(m_Instruction, 12, 8)  +
                                        GetBitSetValue(m_Instruction, 20, 1)+ GetBitSetValue(m_Instruction, 21, 10)+string("0"));
    }

    if (m_name.empty())
        m_name = "UJType";
    return true;
}

