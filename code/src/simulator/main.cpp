#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
#include <string>

#include "common.h"
#include "RISC_Instruction.h"
#include "RISC_INSMem.h"
#include "RISC_DataMem.h"
#include "RISC_Control.h"
#include "RISC_PipeLine.h"

using namespace std;
int main_risc_ins(int argc, char *argv[]);

int main(int argc, char *argv[])  {
    main_risc_ins(argc, argv);
    
    return 0;
}

int main_risc_ins(int argc, char *argv[])
{
    if (argc != 2) {
        RISC_DEBUG::COUT("Please input file path !");
        return -1;
    }

    string filePath(argv[1]);
    RISC_DEBUG::COUT("input file path:", filePath);

    RISC_Control *risc_control = RISC_Control::GetInstance();
    risc_control->SetFilePath(filePath);

    vector<RISC_Func> in_data{ risc_control->IF, risc_control->ID, risc_control->EX, risc_control->ME, risc_control->WB };
    PipeVector m_vector(in_data);
    PipeLine pipeline(m_vector);

    pipeline.start();
    while (pipeline.isContinue()) {
        pipeline.run();
    }

    risc_control->FreeInstance();
    return 0;
}

