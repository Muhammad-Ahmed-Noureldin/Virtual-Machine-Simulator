#include <iostream>
#include <string>
#include "Nabil.h"
using namespace std;


void InstructionRegister::set(string hex_str)
{
      value = hex_str;
}

int InstructionRegister::get_opcode()
{
   int x;
   x = value[0] - 30;

//    for (int i=0;i<4;i++)
//    {
//        x = (x*10)+(value[i+1]-30);
//    }
 
   return x;
}

int InstructionRegister::get_r()
{
    int x = value[1] - 30,y;
    if (x == 4)
    {
        y = value[2] - 30;
        return y;
    }
    else return value[1] - 30;
}

int InstructionRegister::get_s()
{
    int x = value[0] - 30,y;
    if (x > 4 && x < 10)
    {
        y = value[2] - 30;
    }
    else if (x == 4)
    {
        y = value[3] - 30;
    }
    return y;
}

int InstructionRegister::get_t()
{
    int x = value[0] - 30,y;
    if (x > 4 && x < 10)
    {
        y = value[3] - 30;
    } 
    return y;
}

int InstructionRegister::get_xy()
{
    int x = value[0] - 30,y;
    if ((x > 0 && x < 4) || x == 11)
    {
         y = value[2] - 30;
         y = (y*10)+(value[3]-30); 
         return y;
    }  
}

int InstructionRegister::get_x()
{
    int x = value[0] - 30;
    if (x == 10)
    {
        return value[3] - 30;
    } 
}