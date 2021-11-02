#include "RISC_ALU.h"

RISC_ALU *RISC_ALU::m_ptr = nullptr;

RISC_ALU *RISC_ALU::GetInstance() {
    if (m_ptr == nullptr) {
        m_ptr = new RISC_ALU();
    }
    return m_ptr;
}

void RISC_ALU::FreeInstance() {
    delete m_ptr;
}

bitset<REG_BIT_NUM> RISC_ALU::ALUOperation(EM_RISC_INS ALUOP, bitset<REG_BIT_NUM> oprand1, bitset<REG_BIT_NUM> oprand2) {
    RISC_DEBUG::COUT("OP1:", oprand1.to_string());
    RISC_DEBUG::COUT("OP2:", oprand2.to_string());
    bitset<REG_BIT_NUM> result;
    switch (ALUOP)
    {
    case EM_ADD:
        result = bitset<REG_BIT_NUM>(oprand1.to_ulong() + oprand2.to_ulong());
        break;
    case EM_SUB:
        result = bitset<REG_BIT_NUM>(oprand1.to_ulong() - oprand2.to_ulong());
        break;
    case EM_ADDI:
        result = bitset<REG_BIT_NUM>(oprand1.to_ulong() + oprand2.to_ulong());
        break;
    case EM_AND:
        result = bitset<REG_BIT_NUM>(oprand1.to_ulong() & oprand2.to_ulong());
        break;
    case EM_OR:
        result = bitset<REG_BIT_NUM>(oprand1.to_ulong() | oprand2.to_ulong());
        break;
    case EM_XOR:
        result = bitset<REG_BIT_NUM>(oprand1.to_ulong() ^ oprand2.to_ulong());
        break;
    case EM_BEQ:
        result = bitset<REG_BIT_NUM>(oprand1.to_ulong() - oprand2.to_ulong());
        RISC_DEBUG::COUT("ALU BEQ reault:", result.to_string());
        break;
    case EM_JAL:
        result = oprand1;
        break;
    case EM_LD:
        result = bitset<REG_BIT_NUM>(oprand1.to_ulong() + oprand2.to_ulong());
        break;
    case EM_SD:
        result = bitset<REG_BIT_NUM>(oprand1.to_ulong() + oprand2.to_ulong());
        break;
    case EM_LW:
        result = bitset<REG_BIT_NUM>(oprand1.to_ulong() + oprand2.to_ulong());
        break;
    case EM_SW:
        result = bitset<REG_BIT_NUM>(oprand1.to_ulong() + oprand2.to_ulong());
        break;
    default:
        break;
    }
    return result;
}