#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <bitset>
#include <cstdint>
#include <cstdlib>

#include "Muhammad.h"
#include "Eslam.h"
#include "Nabil.h"
#include "Abdelrahman.h"
void programCounter::set( int Address )
{
    this->Address=Address;
}

int programCounter::get()
{
    return Address;
}

void programCounter::increment()
{
    Address+=2;
}




//------------------------------------------------------------------------


registers::registers()
{
    count =0x10 ;
    cells.resize(count,0);
}

int registers::read(int index)
{
    if(index >=0x10 || index<0)
    {
        exit(EXIT_FAILURE);
    }
    return cells[index];
} 

void registers::write(int index , int value)
{
    if(index >=0x10 || index<0)
    {
        exit(EXIT_FAILURE);
    }
    cells[index]=value;
}

void registers::display()
{
    cout << "    Registers (CPU)\n";
    cout << "************************\n";
    cout << "Address" << "      Value(Hexa)    \n";
    cout << "************************\n";
    for(int i=0 ; i<count ; i++)
    {
        cout << "  ";
        cout << hex << uppercase << setfill('0') << setw(2) << i << "          " << setw(2)<< cells[i] << '\n';
    }

    cout << dec;
}

voleMachine::voleMachine(Memory *memory, CPU * cpu ,Screen * screen)
{
    this->memory=memory;
    this->cpu=cpu;
    this->screen=screen;
}

void voleMachine::load_program_from_file(string filePath ,int address)
{
    string textLine;
    ifstream fin(filePath);
    while(getline(fin,textLine))
    {
        int start=textLine.find_first_not_of(" \t");
        if(start==string::npos || textLine[start]==';') continue;
        string inst_string=textLine.substr(start,4);
        stringstream s;
        s << inst_string;
        int  inst_int;
        s >> hex >> inst_int;
        uint16_t valu1= inst_int>>8;
        memory->write(address,valu1);
        address++;
        uint16_t valu2 =inst_int<<8;
        valu2 = valu2 >> 8;
        memory->write(address,valu2);
        address++;
    }
}
