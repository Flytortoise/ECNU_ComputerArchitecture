#include <fstream>
#include <algorithm>
#include "RISC_INSMem.h"
#include "RISC_DEBUG.h"

INSMem::INSMem(const string &inFileName) : m_inFileName(inFileName)
{
    IMem.resize(MEMSIZE);
    std::ifstream imem;
    string line;
    int i = 0;
    imem.open(m_inFileName.c_str());
    if (imem.is_open())
    {
        RISC_DEBUG::COUT("read INSMem from file:", m_inFileName);
        while (getline(imem, line))
        {
            IMem[i] = bitset<8>(line.substr(0, 8));
            i++;
        }

    }
    else 
        RISC_DEBUG::COUT("Unable to open file:", m_inFileName);

    imem.close();
}

bitset<32> INSMem::ReadMemory(bitset<32> ReadAddress)
{
    // TODO: implement!
    // (Read the byte at the ReadAddress and the following three byte).
    Instruction.reset();            // memecpy zero
    auto IMem_Index = ReadAddress.to_ullong();
    int IMen_ItemSize = IMem[0].size();     // 8
    int tmp_value = Instruction.size()/IMen_ItemSize;   // 4
    if ((IMem_Index < IMem.size()) && (IMem_Index % tmp_value == 0)) {
        string tmp_str;
        for (int i = 0; i < tmp_value; i++) {
            tmp_str += IMem[IMem_Index + i].to_string();
        }
        Instruction = bitset<32>(tmp_str);
        // int Instruction_Index = tmp_value - 1;    // 3
        // for (int i = 0; i < tmp_value; i++) {
        //     for (int j = 0; j < IMen_ItemSize; j++) {
        //         Instruction[Instruction_Index * IMen_ItemSize + j] = IMem[IMem_Index+i][j];     // Big Endian
        //     }
        //     Instruction_Index--;
        // }
    }
    //
    return Instruction;
}