#ifndef RISC_INSTRUCTION_H_
#define RISC_INSTRUCTION_H_

#include <bitset>
#include <string>
#include <vector>
using std::bitset;
using std::string;
using std::vector;

#define INS_SIZE        32
#define OP_SIZE         7
#define RD_SIZE         5
#define FUNT3_SIZE      3
#define RS1_SIZE        5
#define RS2_SIZE        5
#define R_FUNT7         (INS_SIZE-OP_SIZE-RD_SIZE-FUNT3_SIZE-RS1_SIZE-RS2_SIZE)
#define I_IMM           (INS_SIZE-OP_SIZE-RD_SIZE-FUNT3_SIZE-RS1_SIZE)
#define U_IMM           (INS_SIZE-OP_SIZE-RD_SIZE)
#define UJ_IMM          (INS_SIZE-OP_SIZE-RD_SIZE)

enum EM_RISC_INS {
    EM_HALT = 0,
    EM_ADD,
    EM_SUB,
    EM_ADDI,
    EM_AND,
    EM_OR,
    EM_XOR,
    EM_BEQ,
    EM_JAL,
    EM_LD,
    EM_SD,
    EM_LW,
    EM_SW,
    EM_END
};

class RISC_Instruction;
RISC_Instruction* CreateRiscINS(enum EM_RISC_INS);

class RISC_Instruction{
public:
    virtual ~RISC_Instruction() = default;
    virtual bool Sync();
    virtual void print();
    virtual bool isReady();
    virtual bool isDependence(RISC_Instruction*);

    bool setInstruction(const bitset<INS_SIZE>&);
    enum EM_RISC_INS getEM() { return em_flag; }
    bool getDependence() { return m_is_dependence; }
    void setDependence(bool data) { m_is_dependence = data; }
    bitset<INS_SIZE> getINS() { return m_Instruction; }

protected:
    bitset<INS_SIZE> m_Instruction;
    bitset<OP_SIZE> m_operation;
    string m_name;
    string m_limit_op;
    enum EM_RISC_INS em_flag;
    bool m_is_dependence = false;
};

class RISC_Halt : public RISC_Instruction {
public:
    RISC_Halt() {
        m_limit_op = "1111111";
        m_name = "halt";
        em_flag = EM_HALT;
    }
};

class RISC_RType : public RISC_Instruction{
public:
    bool Sync();
    bool isReady();
    bool isDependence(RISC_Instruction*);

    bitset<RD_SIZE> getRD() { return m_rd; }
    bitset<RS1_SIZE> getRS1() { return m_rs1; }
    bitset<RS2_SIZE> getRS2() { return m_rs2; }

protected:
    bitset<RD_SIZE> m_rd;
    bitset<FUNT3_SIZE> m_funt3;
    bitset<RS1_SIZE> m_rs1;
    bitset<RS2_SIZE> m_rs2;
    bitset<R_FUNT7> m_funt7;

    string m_limit_funt3;
    string m_limit_funt7;
};

class RISC_IType : public RISC_Instruction{
public:
    bool Sync();
    bool isReady();
    bool isDependence(RISC_Instruction*);

    bitset<RD_SIZE> getRD() { return m_rd; }
    bitset<RS1_SIZE> getRS1() { return m_rs1; }
    bitset<INS_SIZE> getIMM() { return m_imm; }

protected:
    bitset<RD_SIZE> m_rd;
    bitset<FUNT3_SIZE> m_funt3;
    bitset<RS1_SIZE> m_rs1;
    bitset<INS_SIZE> m_imm;

    string m_limit_funt3;
};

class RISC_Abstract_SType : public RISC_Instruction {
public:
    bool isReady();
    bool isDependence(RISC_Instruction*);

    bitset<RS1_SIZE> getRS1() { return m_rs1; }
    bitset<RS2_SIZE> getRS2() { return m_rs2; }
    bitset<INS_SIZE> getIMM() { return m_imm; }

protected:
    bitset<FUNT3_SIZE> m_funt3;
    bitset<RS1_SIZE> m_rs1;
    bitset<RS2_SIZE> m_rs2;
    bitset<INS_SIZE> m_imm;

    string m_limit_funt3;
};

class RISC_SType : public RISC_Abstract_SType{
public:
    bool Sync();
};

class RISC_SBType : public RISC_Abstract_SType{
public:
    bool Sync();
};

class RISC_Abstract_UType : public RISC_Instruction {
public:
    bitset<RD_SIZE> getRD() { return m_rd; }
    bitset<INS_SIZE> getIMM() { return m_imm; }
protected:
    bitset<RD_SIZE> m_rd;
    bitset<INS_SIZE> m_imm;
};

class RISC_UType : public RISC_Abstract_UType{
public:
    bool Sync();
};

class RISC_UJType : public RISC_Abstract_UType{
public:
    bool Sync();
};

class RISC_ADD : public RISC_RType {
public:
    RISC_ADD(){
        m_limit_op = "0110011";
        m_limit_funt3 = "000";
        m_limit_funt7 = "0000000";
        m_name = "add";
        em_flag = EM_ADD;
    }
};

class RISC_SUB : public RISC_RType {
public:
    RISC_SUB(){
        m_limit_op = "0110011";
        m_limit_funt3 = "000";
        m_limit_funt7 = "0100000";
        m_name = "sub";
        em_flag = EM_SUB;
    }
};

class RISC_ADDI : public RISC_IType {
public:
    RISC_ADDI(){
        m_limit_op = "0010011";
        m_limit_funt3 = "000";
        m_name = "addi";
        em_flag = EM_ADDI;
    }
};

class RISC_AND : public RISC_RType {
public:
    RISC_AND(){
        m_limit_op = "0110011";
        m_limit_funt3 = "111";
        m_limit_funt7 = "0000000";
        m_name = "and";
        em_flag = EM_AND;
    }
};

class RISC_OR : public RISC_RType {
public:
    RISC_OR(){
        m_limit_op = "0110011";
        m_limit_funt3 = "110";
        m_limit_funt7 = "0000000";
        m_name = "or";
        em_flag = EM_OR;
    }
};

class RISC_XOR : public RISC_RType {
public:
    RISC_XOR(){
        m_limit_op = "0110011";
        m_limit_funt3 = "100";
        m_limit_funt7 = "0000000";
        m_name = "xor";
        em_flag = EM_XOR;
    }
};

class RISC_BEQ : public RISC_SBType {
public:
    RISC_BEQ(){
        m_limit_op = "1100011";
        m_limit_funt3 = "000";
        m_name = "beq";
        em_flag = EM_BEQ;
    }
};

class RISC_JAL : public RISC_UJType {
public:
    RISC_JAL(){
        m_limit_op = "1101111";
        m_name = "jal";
        em_flag = EM_JAL;
    }
};

class RISC_LD : public RISC_IType {
public:
    RISC_LD(){
        m_limit_op = "0000011";
        m_limit_funt3 = "011";
        m_name = "ld";
        em_flag = EM_LD;
    }
};

class RISC_SD : public RISC_SType {
public:
    RISC_SD(){
        m_limit_op = "0100011";
        m_limit_funt3 = "011";
        m_name = "sd";
        em_flag = EM_SD;
    }
};

class RISC_LW : public RISC_IType {
public:
    RISC_LW(){
        m_limit_op = "0000011";
        m_limit_funt3 = "010";
        m_name = "lw";
        em_flag = EM_LW;
    }
};

class RISC_SW : public RISC_SType {
public:
    RISC_SW(){
        m_limit_op = "0100011";
        m_limit_funt3 = "010";
        m_name = "sw";
        em_flag = EM_SW;
    }
};


#endif