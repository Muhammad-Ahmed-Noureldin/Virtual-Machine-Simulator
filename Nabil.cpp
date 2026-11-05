#include <iostream>
#include <string>
#include <iomanip>

#include "Muhammad.h"
#include "Eslam.h"
#include "Nabil.h"
#include "Abdelrahman.h"
using namespace std;

int InstructionRegister::hexToInt(char num)
{
   if (num < 60) return num - 48;
   else if (num > 60) return num - 55;
   return 0;
}

void InstructionRegister::set(string hex_str)
{
      value = hex_str;
}

int InstructionRegister::get_opcode()
{
   return hexToInt(value[0]);
}

int InstructionRegister::get_r()
{
    int x =  hexToInt(value[0]),y;
    if (x == 4)
    {
        y =  hexToInt(value[2]);
        return y;
    }
    else return  hexToInt(value[1]);
}

int InstructionRegister::get_s()
{
    int x =  hexToInt(value[0]),y=0;
    if (x > 4 && x < 10)
    {
        y =  hexToInt(value[2]);
    }
    else if (x == 4)
    {
        y =  hexToInt(value[3]);
    }
    return y;
}

int InstructionRegister::get_t()
{
    int x = hexToInt(value[0]),y=0;
    if (x > 4 && x < 10)
    {
        y =  hexToInt(value[3]);
    } 
    return y;
}

int InstructionRegister::get_xy()
{
    int x = hexToInt(value[0]),y;
    if ((x > 0 && x < 4) || x == 11)
    {
         y = hexToInt(value[2]);
         y = (y*16)+(hexToInt(value[3])); 
         return y;
    }  
    else return 0;
}

int InstructionRegister::get_x()
{
    int x = hexToInt(value[0]);
    if (x == 10)
    {
        // if (value[3] < 60 )
        // return value[3] - 48;
        // else if (value[3] > 60)
        // return value[3] - 65;
        return hexToInt(value[3]);
    } 
    else return 0;
}


////-------------------------------------------------------------------------------------


CPU::CPU(registers* Reg,programCounter* pc,InstructionRegister* ir,ALU* alu,CU* controlUnit)
 : Registers(Reg) , pc(pc) , ir(ir) , alu(alu) , controlUnit(controlUnit){}


void CPU::step()
{
    controlUnit->step();
}


void CPU::display_state()
{
    cout << "PC = " << hex << setw(2) << uppercase << setfill('0') << pc->get() << "\n";
    Registers->display();
}