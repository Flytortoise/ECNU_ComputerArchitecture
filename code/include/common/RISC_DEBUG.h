#ifndef RISC_DEBUG_H_
#define RISC_DEBUG_H_

#include <string>

using std::string;

#define DEBUG_ON    1

class RISC_DEBUG{
public:
    static void COUT(string out_str1, string out_str2 = "");
};

#endif
