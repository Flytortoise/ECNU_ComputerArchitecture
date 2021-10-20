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
int main_risc_ins(int argc, char *argv[]);      // experiment 1
int main_pipeline(int argc, char *argv[]);      // experiment 2


int main(int argc, char *argv[])  {
    //main_risc_ins(argc, argv);
    main_pipeline(argc, argv);
    
    return 0;
}

int main_pipeline(int argc, char *argv[])
{
    if (argc != 2) {
        RISC_DEBUG::COUT("Please input file path !");
        return -1;
    }

    string filePath(argv[1]);
    RISC_DEBUG::COUT("input file path:", filePath);
    INSMem myInsMem(string(filePath).append(INS_INFILE));

    // RISC_RF_Op myRF;
    DataMem myDataMem(string(filePath).append(DATE_INFILE),string(filePath).append(DATE_OUTFILE));
    stateStruct newState;
    struct stateStruct state{0};
    state.IF.nop = false;
    state.ID.nop = true;
    state.EX.nop = true;
    state.MEM.nop = true;
    state.WB.nop = true;
    state.EX.alu_op = true;
    int cycle = 0;

    cout << "start pipline" << endl;
    while (1) {
        break;

        /* --------------------- WB stage --------------------- */






        /* --------------------- MEM stage --------------------- */






        /* --------------------- EX stage --------------------- */




        /* --------------------- ID stage --------------------- */





        /* --------------------- IF stage --------------------- */





        /* --------------------- Stall unit--------------------- */







        if (state.IF.nop && state.ID.nop && state.EX.nop && state.MEM.nop && state.WB.nop)
            break;

        printState(newState, cycle); //print states after executing cycle 0, cycle 1, cycle 2 ... 

        cycle += 1;
        state = newState; /*The end of the cycle and updates the current state with the values calculated in this cycle */

    }

    // myRF.outputRF(); // dump RF;	
    myDataMem.OutputDataMem(); // dump data mem 

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
    INSMem myInsMem(string(filePath).append(INS_INFILE));
    DataMem myDataMem(string(filePath).append(DATE_INFILE),string(filePath).append(DATE_OUTFILE));

    RISC_Control *risc_control = RISC_Control::GetInstance();

    bitset<32> PC;

    while (1) {
        RISC_DEBUG::COUT("start PC:", to_string(PC.to_ulong()));
        // 1. Fetch Instruction
        bitset<32> instruction = myInsMem.ReadMemory(PC);
        RISC_Instruction *ins_obj = risc_control->GetINSObject(instruction);
        if (ins_obj != nullptr){
            ins_obj->print();
        }else {
            RISC_DEBUG::COUT("Instruction is :", instruction.to_string());
            if (instruction.to_ulong() == 0xffffffff) {
                RISC_DEBUG::COUT("this is 'Halt instruction', save data mem to dmemresult.txt and exit simulator");
                break;
            } else {
                RISC_DEBUG::COUT("FAILED: instruction is incorrect, eixt !");
                break;
            }
        }

        // 2. Register File Instruction
        risc_control->RF_Func();

        // 3. Execuete alu operation
        risc_control->ALU_Func();

        // 4. Read/Write Mem(Memory Access)
        risc_control->DataMem_Func(myDataMem);

        // 5. Register File Update(Write Back)
        risc_control->RF_Func_back();

        // Update pc
        if (risc_control->GetCurrentINSEm() == EM_BEQ && 
            risc_control->GetCurrentALUResult().to_ulong() == 0) {
            PC = bitset<32>(PC.to_ulong() + risc_control->GetBEQImm().to_ulong());
        } else if (risc_control->GetCurrentINSEm() == EM_JAL) {
            PC = bitset<32>(PC.to_ulong() + 4 + risc_control->GetCurrentALUResult().to_ulong());
        } else {
            PC = bitset<32>(PC.to_ulong() + 4);
        }
        RISC_DEBUG::COUT("after update PC:", to_string(PC.to_ulong()));

        RISC_RF_Data::OutputRF(filePath);
    }

    myDataMem.OutputDataMem(); // dump data memX
    return 0;
}

