#include <iostream>
#include "RISC_DEBUG.h"

using std::cout;
using std::endl;

void RISC_DEBUG::COUT(string out_str1, string out_str2) {
#if DEBUG_ON == 1
    cout << out_str1 << out_str2 << endl;
#endif
}