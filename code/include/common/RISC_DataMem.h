#ifndef RISC_DATA_MEM_H_
#define RISC_DATA_MEM_H_

#include "common.h"

/*
is similar to the instruction memory, except that it provides both read and write access, 
and an access to data memory class returns 8 bytes of data.
*/
class DataMem
{
public:
    DataMem(const string &inFileName, const string &outFileName);
    // bitset<32> MemoryAccess(bitset<64> Address, bitset<64> WriteData, bitset<1> readmem, bitset<1> writemem);

    bitset<32> LoadMemory(bitset<64> Address);
    bitset<64> Load64Memory(bitset<64> Address);
    void StoreMemory(bitset<64> Address, bitset<64> data);
    void StoreMemory(bitset<64> Address, bitset<32> data);

    void OutputDataMem();

private:
    vector<bitset<8> > DMem;
    string m_inFileName;
    string m_outFileName;
};


#endif