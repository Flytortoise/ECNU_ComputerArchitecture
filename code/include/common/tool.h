#ifndef RISC_TOOL_H_
#define RISC_TOOL_H_

#include <list>
#include <bitset>
#include <string>
using std::string;
using std::bitset;

#include "RISC_Instruction.h"
#include "RISC_RF.h"
#include "RISC_DEBUG.h"

bitset<32> ConvertEndian(const bitset<32> in_data);
bitset<1> isITypeFunc(const std::string& str);

bool CompareInsStr(const string &str1, const string &str2);
string RevertStr(const string &str);
string GetBitSetValue(string data, unsigned int begin_index, unsigned int count);
inline string GetBitSetValue(bitset<32> data, unsigned int begin_index, unsigned int count) {
    return GetBitSetValue(data.to_string(), begin_index, count);
}

inline string GetBitSetValue(bitset<64> data, unsigned int begin_index, unsigned int count) {
    return GetBitSetValue(data.to_string(), begin_index, count);
}

class RISC_RF_Op;

bool CheckDependence(std::pair<RISC_Instruction*, RISC_RF_Op*>*, std::pair<RISC_Instruction*, RISC_RF_Op*>*, std::pair<RISC_Instruction*, RISC_RF_Op*>*);


#endif