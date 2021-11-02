#ifndef RISC_INS_MEM_H_
#define RISC_INS_MEM_H_

#include "common.h"

/*
a Byte addressable memory that contains instructions. 
The constructor InsMem() initializes the contents of instruction memory from the file imem.txt (you are expected to write a program). 
Your job is to implement the member function ReadMemory() that provides read access to instruction memory. 
An access to the instruction memory class returns 4 bytes of data; i.e., the byte pointed to by the address and the three subsequent bytes.
*/
class INSMem
{
public:
    INSMem() = default;
    INSMem(const string& inFileName);
    bitset<32> Instruction;
    bitset<32> ReadMemory(bitset<32> ReadAddress);
    void SetFileName(const string& filename);

private:
    vector<bitset<8>> IMem;
};


#endif