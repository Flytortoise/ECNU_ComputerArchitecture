#include <fstream>
#include "RISC_DataMem.h"
#include "RISC_DEBUG.h"

using std::ifstream;
using std::ofstream;

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

bitset<32> DataMem::MemoryAccess(bitset<64> Address, bitset<64> WriteData, bitset<1> readmem, bitset<1> writemem) {
    // TODO: implement!
    return bitset<32>(0);
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