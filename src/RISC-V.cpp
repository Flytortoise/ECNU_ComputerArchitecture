#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
#include <string>

using namespace std;

#define RISC_V_OP_SIZE  7
#define RISC_V_REG_SIZE 5

#define RF_OUTFILE      "RFresult.txt"
#define INS_INFILE      "imem.txt"
#define DATE_INFILE     "dmem.txt"
#define DATE_OUTFILE    "dmemresult.txt"

#define ADDU 1
#define SUBU 3
#define AND 4
#define OR  5
#define NOR 7

// Memory size.
// In reality, the memory size should be 2^32, but for this lab and space reasons,
// we keep it as this large number, but the memory is still 32-bit addressable.
#define MemSize 65536

/*
contains 32 64-bit registers defined as a private member. 
Remember that register $0 is always 0. 
Your job is to implement the implement the ReadWrite() member function that provides read and write access to the register file.
*/
class RF
{
public:
    bitset<64> ReadData1, ReadData2;        // change 32 -> 64 by ZhangJunTao
    RF(const string &outFileName) : m_outFileName(outFileName)
    {
        Registers.resize(32);
        Registers[0] = bitset<64>(0);
    }

    void ReadWrite(bitset<RISC_V_REG_SIZE> RdReg1, bitset<RISC_V_REG_SIZE> RdReg2, bitset<RISC_V_REG_SIZE> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable)
    {
        // TODO: implement!
        

    }

    void OutputRF()
    {
        ofstream rfout;
        rfout.open(m_outFileName.c_str(), std::ios_base::app);
        if (rfout.is_open())
        {
            //cout << "out RF to file:" << m_outFileName << endl;
            rfout << "A state of RF:" << endl;
            for (int j = 0; j < 32; j++)
            {
                rfout << Registers[j] << endl;
            }

        }
        else cout << "Unable to open file:" << m_outFileName << endl;
        rfout.close();

    }
private:
    vector<bitset<64> >Registers;
    string m_outFileName;
};

/*
 implements the ALU. 
 Your job is to implement ALUOperation() member function that performs the appropriate operation on two 64 bit operands based on ALUOP. 
 See Table 1 for more details.
*/
class ALU
{
public:
    bitset<64> ALUresult;
    bitset<64> ALUOperation(bitset<3> ALUOP, bitset<64> oprand1, bitset<64> oprand2)
    {
        // TODO: implement!
        return ALUresult;       // TODO this is tmp by zhangyan 20210925
    }
};

/*
a Byte addressable memory that contains instructions. 
The constructor InsMem() initializes the contents of instruction memory from the file imem.txt (you are expected to write a program). 
Your job is to implement the member function ReadMemory() that provides read access to instruction memory. 
An access to the instruction memory class returns 4 bytes of data; i.e., the byte pointed to by the address and the three subsequent bytes.
*/
class INSMem
{
public:
    bitset<32> Instruction;
    INSMem(const string &inFileName) : m_inFileName(inFileName)
    {
        IMem.resize(MemSize);
        ifstream imem;
        string line;
        int i = 0;
        imem.open(m_inFileName.c_str());
        if (imem.is_open())
        {
            cout << "read INSMem from file:" << m_inFileName << endl;
            while (getline(imem, line))
            {
                IMem[i] = bitset<8>(line.substr(0, 8));
                i++;
            }

        }
        else cout << "Unable to open file:" << m_inFileName<< endl;
        imem.close();
#if 0       // out IMem
        for (int j = 0; j < i; j++) {
            cout << "index:" << j << "  data:  " << IMem[j].to_string() << endl;
        }
#endif
    }

    bitset<32> ReadMemory(bitset<32> ReadAddress)
    {
        // TODO: implement!
        // (Read the byte at the ReadAddress and the following three byte).
        Instruction.reset();            // memecpy zero
        auto IMem_Index = ReadAddress.to_ullong();
        int IMen_ItemSize = IMem[0].size();     // 8
        int tmp_value = Instruction.size()/IMen_ItemSize;   // 4
        if ((IMem_Index < IMem.size()) && (IMem_Index % tmp_value == 0)) {
            int Instruction_Index = tmp_value - 1;    // 3
            for (int i = 0; i < tmp_value; i++) {
                for (int j = 0; j < IMen_ItemSize; j++) {
                    Instruction[Instruction_Index * IMen_ItemSize + j] = IMem[IMem_Index+i][j];     // Big Endian
                }
                Instruction_Index--;
            }
        }
        //
        return Instruction;
    }

private:
    vector<bitset<8>> IMem;
    string m_inFileName;
};

/*
is similar to the instruction memory, except that it provides both read and write access, 
and an access to data memory class returns 8 bytes of data.
*/
class DataMem
{
public:
    bitset<32> readdata;
    DataMem(const string &inFileName, const string &outFileName) : m_inFileName(inFileName), m_outFileName(outFileName)
    {
        DMem.resize(MemSize);
        ifstream dmem;
        string line;
        int i = 0;
        dmem.open(m_inFileName.c_str());
        if (dmem.is_open())
        {   
            cout << "read data from file:" << m_inFileName << endl;
            while (getline(dmem, line))
            {
                DMem[i] = bitset<8>(line.substr(0, 8));
                i++;
            }
        }
        else cout << "Unable to open file:" << m_inFileName << endl;
        dmem.close();

    }
    bitset<32> MemoryAccess(bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem)
    {
        // TODO: implement!
        return Address;       // TODO this is tmp by zhangyan 20210925
    }

    void OutputDataMem()
    {
        ofstream dmemout;
        dmemout.open(m_outFileName.c_str());
        if (dmemout.is_open())
        {
            cout << "write data to file:" << m_outFileName << endl;
            for (int j = 0; j < 1000; j++)
            {
                dmemout << DMem[j] << endl;
            }

        }
        else cout << "Unable to open file:" << m_outFileName << endl;
        dmemout.close();

    }

private:
    vector<bitset<8> > DMem;
    string m_inFileName;
    string m_outFileName;
};

// this function need to be QA by zhangyan 20210925
// I-Type in <RISC-V Instruction Set Manual-Volume 1>, page 130 -- 135
bitset<1> isITypeFunc(const string& str) {
    /*
    isIType = instruction.to_string().substr(0, 5) != string("00000") &&
            instruction.to_string().substr(0, 4) != string("0001");
    */

    bitset<1> result = (str.substr(0, RISC_V_OP_SIZE) == string("1100111") ||
                        str.substr(0, RISC_V_OP_SIZE) == string("0000011") ||
                        str.substr(0, RISC_V_OP_SIZE) == string("0010011") ||
                        str.substr(0, RISC_V_OP_SIZE) == string("0011011") ||   // ADDIW
                        str.substr(0, RISC_V_OP_SIZE) == string("0001111") );   // FENCE.I
    if (result.to_ulong()) {
        return result;
    } else if (str.substr(0, RISC_V_OP_SIZE) == string("1010011")){
        result = (str.substr(20,5) == string("00000") || str.substr(20,5)==string("00001"));
    }
    return result;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        cout << "Please input file path !" << endl;
        return -1;
    }

    string filePath(argv[1]);
    cout << "input file path:" << filePath << endl;

    RF myRF(string(RF_OUTFILE));
    ALU myALU;
    INSMem myInsMem(string(filePath).append(INS_INFILE));
    DataMem myDataMem(string(filePath).append(DATE_INFILE),string(DATE_OUTFILE));

    // Control Registers
    bitset<32> PC;
    bitset<1> wrtEnable;
    bitset<1> isJType;
    bitset<1> isIType;
    bitset<1> isLoad;
    bitset<1> isStore;
    bitset<1> isBranch;
    bitset<3> aluOp;

    while (1)
    {
        // 1. Fetch Instruction
        bitset<32> instruction = myInsMem.ReadMemory(PC);
        cout << "Instruction is [" << instruction.to_string() << "]" << endl;
        // If current insturciton is "11111111111111111111111111111111", then break;
        if (myInsMem.Instruction.to_ulong() == 0xffffffff) {
            cout << "this is 'Halt instruction', save data mem to dmemresult.txt and exit simulator" << endl;
            break;
        }
        // decode(Read RF)
        // Decoder
        isLoad = instruction.to_string().substr(0, RISC_V_OP_SIZE) == string("0000011");      // 100011 -> 0000011(RISC-V load)
        isStore = instruction.to_string().substr(0, RISC_V_OP_SIZE) == string("0100011");      // 101011 -> 0100011(RISC-V store)
        isJType = instruction.to_string().substr(0, RISC_V_OP_SIZE) == string("1101111");      // 000010 -> 1101111(RISC-V jal)
        isBranch = instruction.to_string().substr(0, RISC_V_OP_SIZE) == string("1100011");      // 000100 -> 1100011(RISC-V beq)
        isIType = isITypeFunc(instruction.to_string());         // I-Type in <RISC-V Instruction Set Manual-Volume 1>, page 130 -- 135
        wrtEnable = !(isStore.to_ulong() || isBranch.to_ulong() || isJType.to_ulong());
        aluOp = bitset<3>(instruction.to_string().substr(12, 3));        // funct3? this need to be QA by zhangyan 20210925
        // if (instruction.to_string().substr(0, RISC_V_OP_SIZE) == string("0000011") ||       // Load
        //     instruction.to_string().substr(0, RISC_V_OP_SIZE) == string("0100011")) {       // Store
        //     aluOp = bitset<3>("011");       // 001 -> 011 (LD or SD)
        // }
        // else if (instruction.to_string().substr(0, RISC_V_OP_SIZE) == string("0000000")) {
        //     aluOp = bitset<3>(instruction.to_string().substr(29, 3));
        // }
        // else {
        //     aluOp = bitset<3>(instruction.to_string().substr(12, 3));
        // }

        // 2. Register File Instruction
        myRF.ReadWrite(bitset<5>(instruction.to_string().substr(15, RISC_V_REG_SIZE)), 
                        isIType[0] ? bitset<5>(0) : bitset<5>(instruction.to_string().substr(20, RISC_V_REG_SIZE)),
                        bitset<5>(instruction.to_string().substr(7, RISC_V_REG_SIZE)),
                        bitset<32>(0), wrtEnable);
                    // if Type is I-Type, no reg2 in Instruction,so set reg2 Zero

        // 3. Execuete alu operation        TODO this need to by QA by zhangyan 20210925
        bitset<32> tmp(string(23, '0') + instruction.to_string().substr(25, 5) + instruction.to_string().substr(8, 5)); // if positive, 0 padded
        if (tmp[RISC_V_OP_SIZE] == true) {      // -
            tmp = bitset<32>(string(23, '1') + instruction.to_string().substr(25, 5) + instruction.to_string().substr(8, 5));
        }
        myALU.ALUOperation(aluOp, myRF.ReadData1, isIType[0] ? tmp : myRF.ReadData2);


        // 4. Read/Write Mem(Memory Access)
        myDataMem.MemoryAccess(myALU.ALUresult, myRF.ReadData2, isLoad, isStore);

        // 5. Register File Update(Write Back)
        myRF.ReadWrite(bitset<5>(instruction.to_string().substr(15, RISC_V_REG_SIZE)), 
                isIType[0] ? bitset<5>(0) : bitset<5>(instruction.to_string().substr(20, RISC_V_REG_SIZE)),
                bitset<5>(instruction.to_string().substr(7, RISC_V_REG_SIZE)),
                isLoad[0] ? myDataMem.readdata : myALU.ALUresult, wrtEnable);
                    // if Type is I-Type, no reg2 in Instruction,so set reg2 Zero
        // myRF.ReadWrite(bitset<5>(instruction.to_string().substr(6, 5)), bitset<5>(instruction.to_string().substr(11, 5)),
        //     isIType[0] ? bitset<5>(instruction.to_string().substr(11, 5)) : bitset<5>(instruction.to_string().substr(16, 5)),
        //     isLoad[0] ? myDataMem.readdata : myALU.ALUresult, wrtEnable);

        // Update PC
        if (isBranch[0] && myRF.ReadData1 == myRF.ReadData2) {      // beq need to be QA by zhangyan 20210925
            bitset<32> addressExtend;
            if (instruction[RISC_V_OP_SIZE] == true) {      // -
                addressExtend = bitset<32>(string(23, '1') + instruction.to_string().substr(25, 5) + instruction.to_string().substr(8, 5));
            }
            else {                                          // +
                addressExtend = bitset<32>(string(23, '0') + instruction.to_string().substr(25, 5) + instruction.to_string().substr(8, 5));
            }
            PC = bitset<32>(PC.to_ulong() + 4 + addressExtend.to_ulong());
        }
        else if (isJType[0]) {      // JAL   imm[20|10:1|11|19:12], this need to be QA by zhangyan 20210925
            PC = bitset<32>(string(RISC_V_OP_SIZE, '0') + instruction.to_string().substr(RISC_V_OP_SIZE, 25));
        }
        else {
            PC = bitset<32>(PC.to_ulong() + 4);
        }

        myRF.OutputRF(); // dump RF;    
    }
    myDataMem.OutputDataMem(); // dump data mem

    return 0;
}
