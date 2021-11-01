#include <fstream>
#include <algorithm>
#include "RISC_RF.h"

RISC_RF_Data *RISC_RF_Data::m_ptr = nullptr;
vector<bitset<REG_BIT_NUM>> RISC_RF_Data::m_registers;
string RISC_RF_Data::m_outFileName = "RFresult.txt";
RISC_RF_Data *RISC_RF_Data::GetInstance() {
    if (m_ptr == nullptr) {
        m_ptr = new RISC_RF_Data();
    }
    return m_ptr;
}

void RISC_RF_Data::FreeInstance() {
    delete m_ptr;
}

RISC_RF_Data::RISC_RF_Data() {
    RISC_RF_Data::m_registers.resize(REG_NUM);
}

void RISC_RF_Data::OutputRF(string filePath) {
    if (!RISC_RF_Data::m_outFileName.empty()) {
        std::ofstream rfout;
        rfout.open(filePath.append(m_outFileName).c_str());
        if (rfout.is_open())
        {
            rfout << "A state of RF:" << std::endl;
            for (int j = 0; j < REG_NUM; j++)
            {
                rfout << RISC_RF_Data::m_registers[j] << std::endl;
            }

        }
        else 
            RISC_DEBUG::COUT("Unable to open file:" ,m_outFileName);
        
        rfout.close();
    }
}

bitset<REG_BIT_NUM> RISC_RF_Data::GetRegData(unsigned int reg_index) {
    return m_registers[reg_index];
}

void RISC_RF_Data::SetRegData(unsigned int reg_index, bitset<REG_BIT_NUM> reg_data) {
    RISC_DEBUG::COUT("set reg_index:", std::to_string(reg_index));
    RISC_DEBUG::COUT("set reg_data:", reg_data.to_string());

    m_registers[reg_index] = reg_data;
}

void RISC_RF_Data::SetOutFileName(const string& outfile) {
    m_outFileName = outfile;
}

void RISC_RF_Op::checkType(RISC_Instruction *in_ptr) {
    if (in_ptr == nullptr)
        throw "type is nullptr";
}

void RISC_RF_Op_RType::RF_Func() {
    RISC_RF_Op::checkType(m_type);
    m_rs1 = m_rf_data->GetRegData(m_type->getRS1().to_ulong());
    m_rs2 = m_rf_data->GetRegData(m_type->getRS2().to_ulong());
    RISC_DEBUG::COUT("func Rtype rs1 index:",m_type->getRS1().to_string());
    RISC_DEBUG::COUT("func Rtype m_rs1:",m_rs1.to_string());
    RISC_DEBUG::COUT("func Rtype rs2 index:",m_type->getRS2().to_string());
    RISC_DEBUG::COUT("func Rtype m_rs2:",m_rs2.to_string());
}

void RISC_RF_Op_IType::RF_Func() {
    RISC_RF_Op::checkType(m_type);
    m_rs1 = m_rf_data->GetRegData(m_type->getRS1().to_ulong());
    RISC_DEBUG::COUT("func itype rs1 index:",m_type->getRS1().to_string());
    RISC_DEBUG::COUT("func itype m_rs1:",m_rs1.to_string());
}

void RISC_RF_Op_SType::RF_Func() {
    RISC_RF_Op::checkType(m_type);
    m_rs1 = m_rf_data->GetRegData(m_type->getRS1().to_ulong());
    m_rs2 = m_rf_data->GetRegData(m_type->getRS2().to_ulong());
    RISC_DEBUG::COUT("func stype rs1 index:",m_type->getRS1().to_string());
    RISC_DEBUG::COUT("func stype m_rs1:",m_rs1.to_string());
    RISC_DEBUG::COUT("func stype rs2 index:",m_type->getRS2().to_string());
    RISC_DEBUG::COUT("func stype m_rs2:",m_rs2.to_string());
}

void RISC_RF_Op_SBType::RF_Func() {
    RISC_RF_Op::checkType(m_type);
    m_rs1 = m_rf_data->GetRegData(m_type->getRS1().to_ulong());
    m_rs2 = m_rf_data->GetRegData(m_type->getRS2().to_ulong());
    RISC_DEBUG::COUT("func sbtype rs1 index:",m_type->getRS1().to_string());
    RISC_DEBUG::COUT("func sbtype m_rs1:",m_rs1.to_string());
    RISC_DEBUG::COUT("func sbtype rs2 index:",m_type->getRS2().to_string());
    RISC_DEBUG::COUT("func sbtype m_rs2:",m_rs2.to_string());
    RISC_DEBUG::COUT("func sbtype imm:",m_type->getIMM().to_string());
    
}

void RISC_RF_Op_UType::RF_Func() {
    RISC_RF_Op::checkType(m_type);
}

void RISC_RF_Op_UJType::RF_Func() {
    RISC_RF_Op::checkType(m_type);
}

void RISC_RF_Op_RType::RF_Func_back() {
    RISC_RF_Op::checkType(m_type);
    m_rf_data->SetRegData(m_type->getRD().to_ulong(), m_rd);
    RISC_DEBUG::COUT("funcback rType get rd:",m_type->getRD().to_string());
    RISC_DEBUG::COUT("funcback rType rd:", m_rd.to_string());
}

void RISC_RF_Op_IType::RF_Func_back() {
    RISC_RF_Op::checkType(m_type);
    m_rf_data->SetRegData(m_type->getRD().to_ulong(), m_rd);
    RISC_DEBUG::COUT("funcback IType get rd:",m_type->getRD().to_string());
    RISC_DEBUG::COUT("funcback IType rd:", m_rd.to_string());
}

void RISC_RF_Op_Abstract_SType::RF_Func_back() {
    RISC_RF_Op::checkType(m_type);
}

void RISC_RF_Op_Abstract_UType::RF_Func_back() {
    RISC_RF_Op::checkType(m_type);
    m_rf_data->SetRegData(m_type->getRD().to_ulong(), m_rd);
}

RISC_RF_Op* CreateRiscOp(RISC_Instruction* tmp_ins) {
    RISC_RF_Op* result = nullptr;
    switch (tmp_ins->getEM())
    {
    case EM_ADD:
        result = new RISC_RF_Op_RType(static_cast<RISC_RType*>(tmp_ins));
        break;
    case EM_SUB:
        result = new RISC_RF_Op_RType(static_cast<RISC_RType*>(tmp_ins));
        break;
    case EM_ADDI:
        result = new RISC_RF_Op_IType(static_cast<RISC_IType*>(tmp_ins));
        break;
    case EM_AND:
        result = new RISC_RF_Op_RType(static_cast<RISC_RType*>(tmp_ins));
        break;
    case EM_OR:
        result = new RISC_RF_Op_RType(static_cast<RISC_RType*>(tmp_ins));
        break;
    case EM_XOR:
        result = new RISC_RF_Op_RType(static_cast<RISC_RType*>(tmp_ins));
        break;
    case EM_BEQ:
        result = new RISC_RF_Op_SBType(static_cast<RISC_SBType*>(tmp_ins));
        break;
    case EM_JAL:
        result = new RISC_RF_Op_UJType(static_cast<RISC_UJType*>(tmp_ins));
        break;
    case EM_LD:
        result = new RISC_RF_Op_IType(static_cast<RISC_IType*>(tmp_ins));
        break;
    case EM_SD:
        result = new RISC_RF_Op_SType(static_cast<RISC_SType*>(tmp_ins));
        break;
    case EM_LW:
        result = new RISC_RF_Op_IType(static_cast<RISC_IType*>(tmp_ins));
        break;
    case EM_SW:
        result = new RISC_RF_Op_SType(static_cast<RISC_SType*>(tmp_ins));
        break;
    default:
        result = new RISC_RF_Op_Halt(static_cast<RISC_Halt*>(tmp_ins));
        break;
    }
    return result;

}