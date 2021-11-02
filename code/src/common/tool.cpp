#include "tool.h"
#include <string>
#include <algorithm>

using std::string;

bitset<32> ConvertEndian(const bitset<32> in_data) {
    string result;
    string tmp_data = in_data.to_string();

    for (int i = 0; i < 4; i++) {
        result += tmp_data.substr(i*8, 8);
    }
    return bitset<32>(result);
}

bitset<1> isITypeFunc(const string& str) {
    bitset<1> result = (str.substr(0, RISC_V_OP_SIZE) == string("1100111") ||
                        str.substr(0, RISC_V_OP_SIZE) == string("0000011") ||
                        str.substr(0, RISC_V_OP_SIZE) == string("0010011") ||
                        str.substr(0, RISC_V_OP_SIZE) == string("0011011") ||   // ADDIW
                        str.substr(0, RISC_V_OP_SIZE) == string("0001111") );   // FENCE.I
    if (result.to_ulong()) {
        return result;
    } else if (str.substr(0, RISC_V_OP_SIZE) == string("1010011")){
        result = (str.substr(20,5) == string("00000") || str.substr(20,5)==string("00001"));
    }
    return result;
}

bool CompareInsStr(const string &str1, const string &str2) {
    if (str1.size() != str2.size()) {
        return false;
    }

    string tmp_str(str1);
    std::reverse(tmp_str.begin(), tmp_str.end());
    return tmp_str.compare(str2) == 0;
}

string RevertStr(const string &str) {
    string result(str);
    std::reverse(result.begin(), result.end());
    return result;
}

string GetBitSetValue(string data, unsigned int begin_index, const unsigned int count) {
    std::reverse(data.begin(), data.end());

    string result = data.substr(begin_index, count);
    std::reverse(result.begin(), result.end());
    return result;
}

bool CheckDependence(std::pair<RISC_Instruction*, RISC_RF_Op*>* it,
    std::pair<RISC_Instruction*, RISC_RF_Op*>* it_1,
    std::pair<RISC_Instruction*, RISC_RF_Op*>* it_2) {

    if (it_2 != nullptr) {
        if (it->first->isDependence(it_2->first)) {
            it_2->first->setDependence(true);
            return true;
        }
    }

    if (it_1 != nullptr) {
        if (it->first->isDependence(it_1->first)) {
            it_1->first->setDependence(true);
            return true;
        }
    }

    return false;
}