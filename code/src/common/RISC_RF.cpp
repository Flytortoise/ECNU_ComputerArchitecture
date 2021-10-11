#include <fstream>
#include <algorithm>
#include "RISC_RF.h"

RISC_RF_Data *RISC_RF_Data::m_ptr = nullptr;
vector<bitset<REG_BIT_NUM>> RISC_RF_Data::m_registers;
string RISC_RF_Data::m_outFileName;
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

void RISC_RF_Data::OutputRF() {
    if (!RISC_RF_Data::m_outFileName.empty()) {
        std::ofstream rfout;
        rfout.open(m_outFileName.c_str(), std::ios_base::app);
        if (rfout.is_open())
        {
            rfout << "A state of RF:" << std::endl;
            for (int j = 0; j < REG_BIT_NUM; j++)
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
}

void RISC_RF_Op_SBType::RF_Func() {
    RISC_RF_Op::checkType(m_type);
    m_rs1 = m_rf_data->GetRegData(m_type->getRS1().to_ulong());
    m_rs2 = m_rf_data->GetRegData(m_type->getRS2().to_ulong());
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
