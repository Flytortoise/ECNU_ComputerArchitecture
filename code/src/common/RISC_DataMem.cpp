#include <fstream>
#include <string>
#include "RISC_DataMem.h"
#include "RISC_DEBUG.h"

using std::ifstream;
using std::ofstream;
using std::to_string;

DataMem::DataMem(const string &inFileName, const string &outFileName) : m_inFileName(inFileName), m_outFileName(outFileName) {
    DMem.resize(MEMSIZE);
    ifstream dmem;
    string line;
    int i = 0;
    dmem.open(m_inFileName.c_str());
    if (dmem.is_open())
    {   
        RISC_DEBUG::COUT("read data from file:", m_inFileName);
        while (getline(dmem, line))
        {
            DMem[i] = bitset<8>(line.substr(0, 8));
            i++;
        }
    }
    else 
        RISC_DEBUG::COUT("Unable to open file:", m_inFileName);
    
    dmem.close();

}

// bitset<32> DataMem::MemoryAccess(bitset<64> Address, bitset<64> WriteData, bitset<1> readmem, bitset<1> writemem) {
//     // TODO: implement!
//     return bitset<32>(0);
// }

bitset<32> DataMem::LoadMemory(bitset<64> Address) {
    if (Address.to_ulong()%4 !=0 || Address.to_ulong() >= MEMSIZE)
        throw "Load Memory: Address is error";
    
    string tmp_str;
    for (int i = 0; i < 4; i++) {
        tmp_str += DMem[Address.to_ulong() + i].to_string();
    }
    return bitset<32>(tmp_str);
}

void DataMem::StoreMemory(bitset<64> Address, bitset<32> data) {
    if (Address.to_ulong()%4 !=0 || Address.to_ulong() >= MEMSIZE)
        throw "Store Memory: Address is error";

    // RISC_DEBUG::COUT("store Address 32:", to_string(Address.to_ullong()));
    RISC_DEBUG::COUT("store data 32:", data.to_string());

    for (int i = 0; i < 4; i++) {
        string str_tmp = GetBitSetValue(data, (3-i)*8, 8);
        RISC_DEBUG::COUT("STORE DATA:", str_tmp);
        DMem[Address.to_ulong()+i] = bitset<8>(str_tmp);
    }
    RISC_DEBUG::COUT("store data 32 end");
}


void DataMem::StoreMemory(bitset<64> Address, bitset<64> data) {
    RISC_DEBUG::COUT("store Address:", Address.to_string());
    RISC_DEBUG::COUT("store data:", data.to_string());
    if (Address.to_ulong()%4 !=0 || Address.to_ulong() >= MEMSIZE)
        throw "Store Memory: Address is error";

    string tmp_str = data.to_string();
    string low_str = tmp_str.substr(32,32);
    string high_str = tmp_str.substr(0,32);
    this->StoreMemory(Address, bitset<32>(high_str));
    this->StoreMemory(bitset<64>(Address.to_ulong()+4), bitset<32>(low_str));
}


void DataMem::OutputDataMem() {
    ofstream dmemout;
    dmemout.open(m_outFileName.c_str());
    if (dmemout.is_open())
    {
        RISC_DEBUG::COUT("write data to file:", m_outFileName);
        for (int j = 0; j < 1000; j++)
        {
            dmemout << DMem[j] << std::endl;
        }

    }
    else 
        RISC_DEBUG::COUT("Unable to open file:", m_outFileName);

    dmemout.close();

}