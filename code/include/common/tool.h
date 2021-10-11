#ifndef RISC_TOOL_H_
#define RISC_TOOL_H_

#include "common.h"

bitset<32> ConvertEndian(const bitset<32> in_data);
bitset<1> isITypeFunc(const std::string& str);

bool CompareInsStr(const string &str1, const string &str2);
string RevertStr(const string &str);
string GetBitSetValue(bitset<32> &data, unsigned int begin_index, unsigned int count);

#endif