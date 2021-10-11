#ifndef RISC_COMMON_H_
#define RISC_COMMON_H_

#include <bitset>
#include <vector>
#include <string>
using std::bitset;
using std::string;
using std::vector;

#define RISC_V_OP_SIZE  7
#define RISC_V_REG_SIZE 5

#include "tool.h"
#include "RISC_DEBUG.h"

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
#define MEMSIZE 65536

#endif