/*
Cache Simulator
Level one L1 and level two L2 cache parameters are read from file (block size, line per set and set per cache).
The 32 bit address is divided into tag bits (t), set index bits (s) and block offset bits (b)
s = log2(#sets)   b = log2(block size)  t=32-s-b
*/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <cmath>
#include <bitset>

using namespace std;
//access state:
#define NA 0 // no action
#define RH 1 // read hit
#define RM 2 // read miss
#define WH 3 // Write hit
#define WM 4 // write miss


const string cache_name = "cacheconfig.txt";
const string trace_name = "trace.txt";

struct config{
       int L1blocksize;
       int L1setsize;
       int L1size;
       int L2blocksize;
       int L2setsize;
       int L2size;

void dump() {
    cout << "L1blocksize:" << this->L1blocksize << endl;
    cout << "L1setsize:" << this->L1setsize << endl;
    cout << "L1size:" << this->L1size << endl;
    cout << "L2blocksize:" << this->L2blocksize << endl;
    cout << "L2setsize:" << this->L2setsize << endl;
    cout << "L2size:" << this->L2size << endl;
}
};

/* you can define the cache class here, or design your own data structure for L1 and L2 cache
class cache {
      
      }
*/       
class block{
    private:
        int tagNumOfBits;
        bool validBit;
        vector<unsigned char> data;
        bitset<32> address;
    public:
        block(int tagNum, int offsetSize){
            tagNumOfBits= tagNum;
            data        = vector<unsigned char>(offsetSize);
            validBit    = false;
        }
        block(const block &input){
            tagNumOfBits= input.tagNumOfBits;
            data        = input.data;
            validBit    = input.validBit ;
            address     = input.address;
        }
        bool isEmpty(){
            return !validBit ;
        }
        bool isValid(bitset<32> lookUpAddress){
            // Compare tag
            if(address.to_string().substr(0, tagNumOfBits) == 
                lookUpAddress.to_string().substr(0, tagNumOfBits)){
                if(validBit)
                    return true;
            }
            return false;
        }
        void updateFromWrite(bitset<32> inputAdd){
            validBit = true;
            address = inputAdd;
        }
        void updateFromLowLevel(bitset<32> inputAdd){
            validBit = true;
            address = inputAdd;
        }
        bitset<32> query(){
            return address;
        };
        void inValid(bitset<32> inputAdd){
            validBit = false;
        }
};

class cache{
    private:
        int tagNumOfBits;
        int idxNumOfBits;
        int numOfWays;
        vector<vector<block> > myCache;
        vector<int>  blockEvictCounter;

        int numOfBits(int a){
            int res = 0;
            int maxNum = 1;
            while(a > maxNum){
                res++;
                maxNum <<= 1;
            }
            return res;
        }
    public:
        cache(int block_size, int ways_num, int cache_size){
            int cacheLength = 0;
            if(ways_num == 0){
                ways_num = cache_size * 1024 / block_size;
            }
            cacheLength = cache_size * 1024 / ways_num;
            int blockNum = cacheLength / block_size;

            int blockNumOfBits = numOfBits(block_size);
            idxNumOfBits = numOfBits(blockNum);
            tagNumOfBits = 32 - idxNumOfBits - blockNumOfBits;
            numOfWays = ways_num;
            blockEvictCounter = vector<int>(blockNum, 0);

            for(int j=0; j<ways_num; j++){
                vector<block> temp;
                for(int i=0; i < blockNum; i++){
                    block blkInstance(tagNumOfBits, block_size);
                    temp.push_back(blkInstance);
                }
                myCache.push_back(temp);
            }
        };

        bool isValid(bitset<32> address){
            // Compare tag
            unsigned int idx = bitset<32>(address.to_string().substr(tagNumOfBits, idxNumOfBits)).to_ulong();
            for(int i=0; i<numOfWays; i++){
                if(myCache[i][idx].isValid(address)){
                    return true;
                }
            }
            return false;
        }

        bitset<32> query(bitset<32> address, bool *isEvict){
            *isEvict = false;
            unsigned int idx = bitset<32>(address.to_string().substr(tagNumOfBits, idxNumOfBits)).to_ulong();

            for(int i=0; i<numOfWays; i++){
                if(myCache[i][idx].isEmpty()){
                    return bitset<32>(0);
                }
            }

            *isEvict = true;
            return myCache[blockEvictCounter[idx]][idx].query();
        }

        void updateFromLowLevel(bitset<32> address){
            unsigned int idx = bitset<32>(address.to_string().substr(tagNumOfBits, idxNumOfBits)).to_ulong();

            // Round-Robin Policy
            for(int i=0; i<numOfWays; i++){
                if(myCache[i][idx].isEmpty()){
                    myCache[i][idx].updateFromLowLevel(address);
                    return ;
                }
            }
            myCache[blockEvictCounter[idx]][idx].updateFromLowLevel(address);
            blockEvictCounter[idx] = (blockEvictCounter[idx] + 1) % numOfWays;
        }

        void updateFromHighLevel(bitset<32> address){
            unsigned int idx = bitset<32>(address.to_string().substr(tagNumOfBits, idxNumOfBits)).to_ulong();
            for(int i=0; i<numOfWays; i++){
                if(myCache[i][idx].isValid(address)){
                    myCache[i][idx].updateFromWrite(address);
                    return ;
                }
            }
        }

        void inValid(bitset<32> address){
            unsigned int idx = bitset<32>(address.to_string().substr(tagNumOfBits, idxNumOfBits)).to_ulong();
            for(int i=0; i<numOfWays; i++){
                if(myCache[i][idx].isValid(address)){
                    myCache[i][idx].inValid(address);
                    return ;
                }
            }
        }

        void dump() {
            cout << "tagNumOfBits:" << tagNumOfBits << endl;
            cout << "idxNumOfBits:" << idxNumOfBits << endl;
            cout << "numOfWays:" << numOfWays << endl;
            cout << "myCache size:" << myCache.size() << endl;
            cout << "myCache[0] size:" << myCache[0].size() << endl;
            cout << "blockEvictCounter size:" << blockEvictCounter.size() << endl;
        }
};

int main(int argc, char* argv[]){
    if (argc != 2) {
        cout << "Please input cacheconfig.txt path" << endl;
        return 0;
    }

    config cacheconfig;
    ifstream cache_params;
    string dummyLine;
    cache_params.open(string(argv[1])+cache_name);
    while(!cache_params.eof())  // read config file
    {
      cache_params>>dummyLine;
      cache_params>>cacheconfig.L1blocksize;
      cache_params>>cacheconfig.L1setsize;              
      cache_params>>cacheconfig.L1size;
      cache_params>>dummyLine;              
      cache_params>>cacheconfig.L2blocksize;           
      cache_params>>cacheconfig.L2setsize;        
      cache_params>>cacheconfig.L2size;
    }
    
    cout << "cache config" << endl;
    cacheconfig.dump();
    cout << endl;
    
   // Implement by you: 
   // initialize the hirearch cache system with those configs
   // probably you may define a Cache class for L1 and L2, or any data structure you like

    cache L1(cacheconfig.L1blocksize, cacheconfig.L1setsize, cacheconfig.L1size);
    cache L2(cacheconfig.L2blocksize, cacheconfig.L2setsize, cacheconfig.L2size);
    L1.dump();
    cout << endl;
    L2.dump();
   
    int L1AcceState = 0; // L1 access state variable, can be one of NA, RH, RM, WH, WM;
    int L2AcceState = 0; // L2 access state variable, can be one of NA, RH, RM, WH, WM;
   
   
    ifstream traces;
    ofstream tracesout;
    string outname;
    outname = string(argv[1]) + "trace.out";
    
    traces.open(string(argv[1])+trace_name);
    tracesout.open(outname.c_str());
    
    string line;
    string accesstype;  // the Read/Write access type from the memory trace;
    string xaddr;       // the address from the memory trace store in hex;
    unsigned int addr;  // the address from the memory trace store in unsigned int;        
    bitset<32> accessaddr; // the address from the memory trace store in the bitset;
    
    if (traces.is_open()&&tracesout.is_open()){    
        while (getline (traces,line)){   // read mem access file and access Cache
            
            istringstream iss(line); 
            if (!(iss >> accesstype >> xaddr)) {break;}
            stringstream saddr(xaddr);
            saddr >> std::hex >> addr;
            accessaddr = bitset<32> (addr);
        //    cout << "accessaddr:" << addr << endl;
        //    cout << "accesstype:" << accesstype << endl;
           
           // access the L1 and L2 Cache according to the trace;
              if (accesstype.compare("R")==0)
              
             {    
                 //Implement by you:
                 // read access to the L1 Cache, 
                 //  and then L2 (if required), 
                 //  update the L1 and L2 access state variable;
                 
                 
                 // Check if L1 is valid for read, Hit
                if (L1.isValid(accessaddr)) {
                  // What should I do in hit
                  L1AcceState = RH;
                  L2AcceState = NA;
                }
                // Read Miss on L1
                else {
                  bool isEvict;
                  bitset<32> evictAddr = L1.query(accessaddr, &isEvict);
                  L1AcceState = RM;
                  // L1 Miss, L2 Hit
                  if (L2.isValid(accessaddr)) {
                    L2AcceState = RH;
                    L1.updateFromLowLevel(accessaddr);
                    L2.inValid(accessaddr); 
                  }
                  else { // If L2 is missed
                    L2AcceState = RM;
                    L1.updateFromLowLevel(accessaddr);
                  }
                  // If L1 evicts block
                  if (isEvict) {
                    L2.updateFromLowLevel(evictAddr);
                  }
                }
                 
                 }
             else       // write
             {    
                   //Implement by you:
                  // write access to the L1 Cache, 
                  //and then L2 (if required), 
                  //update the L1 and L2 access state variable;
                  
                  // If L1 is valid for writing
                  if (L1.isValid(accessaddr)) {
                    L1AcceState = WH;
                    L2AcceState = NA;
                    // It means hit
                    L1.updateFromHighLevel(accessaddr);
                  }
                  // If L1 is not valid, which means write miss
                  else {
                    // Check whether L2 is valid
                    L1AcceState = WM;
                    if (L2.isValid(accessaddr)) {
                      L2AcceState = WH;
                      L2.updateFromHighLevel(accessaddr);
                    } else {
                      L2AcceState = WM;
                    }
                    // Else, no action for writing
                  }
                  
                  
              }
              
              
             
            tracesout<< L1AcceState << " " << L2AcceState << endl;  // Output hit/miss results for L1 and L2 to the output file;
             
             
        }
        traces.close();
        tracesout.close(); 
    }
    else cout<< "Unable to open trace or traceout file ";

    return 0;
}
