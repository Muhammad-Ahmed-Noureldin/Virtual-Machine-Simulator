#ifndef _Nabil_HPP
#define _Nabil_HPP
#include <string>
using namespace std;

class InstructionRegister
{
    private:
        string value;
        int hexToInt(char num);
    public:
        void set(string hex_str);
        int get_opcode();
        int get_r();
        int get_s();
        int get_t();
        int get_xy();
        int get_x();
};


#endif