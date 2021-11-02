#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <queue>

using namespace std;

class PipeVector {
public:
    PipeVector(vector<string> data) : m_data(data) {}

    PipeVector(const PipeVector& data) : m_data(data.m_data) , my_index(data.my_index+1){}

    void dump() {
        cout << m_data[m_curr_index++] << " " << my_index << "   ";
    }

    bool isEnd() {
        return m_curr_index == m_data.size() ? true : false;
    }
    int my_index;
private:
    int m_curr_index;
    vector<string> m_data;
    
};

class PipeLine {
public:
    PipeLine(const PipeVector& data) : m_pipe_vector(data){ 
        
    }
    void run() {
        if (!stop) {
            m_ins_vec.push_back(new PipeVector(m_pipe_vector));
            m_pipe_vector.my_index++;
        }
        for (list<PipeVector*>::iterator it = m_ins_vec.begin(); it != m_ins_vec.end(); ++it) {
            (*it)->dump();
        }
        if (!m_ins_vec.empty() && m_ins_vec.front()->isEnd()) {
            PipeVector* tmp = m_ins_vec.front();
            m_ins_vec.pop_front();
            delete tmp;
        }
        cout << endl;
    }

    bool isContinue() {
        return !m_ins_vec.empty();
    }

    bool stop = false;
private:
    PipeVector m_pipe_vector;
    list<PipeVector *> m_ins_vec;
};


int main() {
    vector<string> in_data{ "IF", "ID", "EX", "ME", "WB" };
    PipeVector m_vector(in_data);
    m_vector.my_index = 0;
    PipeLine p(m_vector);

    int count = 12;
    p.run();
    while (p.isContinue()) {
        count--;
        p.run();
        if (count == 4)
            p.stop = true;
    }

    return 0;
}