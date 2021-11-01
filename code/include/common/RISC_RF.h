#ifndef RISC_RF_H_
#define RISC_RF_H_

#include "common.h"
#include "RISC_Instruction.h"

#define REG_NUM          32
#define REG_BIT_NUM      64

class RISC_RF_Op;
RISC_RF_Op* CreateRiscOp(RISC_Instruction *ins);

/*
contains 32 64-bit registers defined as a private member. 
Remember that register $0 is always 0. 
Your job is to implement the implement the ReadWrite() member function that provides read and write access to the register file.
*/

class RISC_RF_Data {
public:
    static RISC_RF_Data *GetInstance();
    static void FreeInstance();

    static bitset<REG_BIT_NUM> GetRegData(unsigned int reg_index);
    static void SetRegData(unsigned int reg_index, bitset<REG_BIT_NUM> reg_data);
    static void SetOutFileName(const string& outfile);

    static void OutputRF(string);

private:
    RISC_RF_Data();

    static RISC_RF_Data *m_ptr;
    static vector<bitset<REG_BIT_NUM>> m_registers;
    static string m_outFileName;
};

class RISC_RF_Op {
public:
    RISC_RF_Op() { m_rf_data = RISC_RF_Data::GetInstance();}
    virtual ~RISC_RF_Op() = default;

    virtual void RF_Func() = 0;
    virtual void RF_Func_back() = 0;
    static void checkType(RISC_Instruction *);
protected:
    RISC_RF_Data *m_rf_data;
};

class RISC_RF_Op_Halt : public RISC_RF_Op {
public:
    RISC_RF_Op_Halt(RISC_Halt* type) :m_type(type) {}
    void RF_Func() {}
    void RF_Func_back() {}

private:
    RISC_Halt* m_type;
};


class RISC_RF_Op_RType : public RISC_RF_Op{
public:
    RISC_RF_Op_RType(RISC_RType *type):m_type(type) {}
    void RF_Func();
    void RF_Func_back();

    bitset<REG_BIT_NUM> getRs1Data() { return m_rs1; }
    bitset<REG_BIT_NUM> getRs2Data() { return m_rs2; }
    void setRdData(bitset<REG_BIT_NUM> data) { m_rd = data; }

private:
    RISC_RType *m_type;
    bitset<REG_BIT_NUM> m_rs1;
    bitset<REG_BIT_NUM> m_rs2;
    bitset<REG_BIT_NUM> m_rd;
};

class RISC_RF_Op_IType : public RISC_RF_Op{
public:
    RISC_RF_Op_IType(RISC_IType *type):m_type(type) {}
    void RF_Func();
    void RF_Func_back();

    bitset<REG_BIT_NUM> getRs1Data() { return m_rs1; }
    void setRdData(bitset<REG_BIT_NUM> data) { m_rd = data; 
        RISC_DEBUG::COUT("rf itype m_rd:", m_rd.to_string());
    }

private:
    RISC_IType *m_type;
    bitset<REG_BIT_NUM> m_rs1;
    bitset<REG_BIT_NUM> m_rd;
};

class RISC_RF_Op_Abstract_SType : public RISC_RF_Op{
public:
    RISC_RF_Op_Abstract_SType(RISC_Abstract_SType *type):m_type(type) {}
    void RF_Func_back();

    bitset<REG_BIT_NUM> getRs1Data() { return m_rs1; }
    bitset<REG_BIT_NUM> getRs2Data() { return m_rs2; }
    
protected:
    RISC_Abstract_SType *m_type;
    bitset<REG_BIT_NUM> m_rs1;
    bitset<REG_BIT_NUM> m_rs2;
};

class RISC_RF_Op_SType : public RISC_RF_Op_Abstract_SType{
public:
    RISC_RF_Op_SType(RISC_SType *type) : RISC_RF_Op_Abstract_SType(type){}
    void RF_Func();
};

class RISC_RF_Op_SBType : public RISC_RF_Op_Abstract_SType{
public:
    RISC_RF_Op_SBType(RISC_SBType *type) : RISC_RF_Op_Abstract_SType(type){}
    void RF_Func();
};

class RISC_RF_Op_Abstract_UType : public RISC_RF_Op{
public:
    RISC_RF_Op_Abstract_UType(RISC_Abstract_UType *type):m_type(type) {}
    void RF_Func_back();

    void setRdData(bitset<REG_BIT_NUM> data) { m_rd = data; }

protected:
    RISC_Abstract_UType *m_type;
    bitset<REG_BIT_NUM> m_rd;
};

class RISC_RF_Op_UType : public RISC_RF_Op_Abstract_UType{
public:
    RISC_RF_Op_UType(RISC_UType *type) : RISC_RF_Op_Abstract_UType(type){}
    void RF_Func();
};

class RISC_RF_Op_UJType : public RISC_RF_Op_Abstract_UType{
public:
    RISC_RF_Op_UJType(RISC_UJType *type) : RISC_RF_Op_Abstract_UType(type){}
    void RF_Func();
};


#endif