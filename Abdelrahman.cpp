#include <iostream>
#include <vector>
#include <iomanip>
#include "Abdelrahman.h"
using namespace std;

void programCounter::setAddress( int Address )
{
    this->Address=Address;
}
int programCounter::getInstructionAddress()
{
    Address+=2;
    return Address-2;
}

registers::registers()
{
    count =0x10 ;
    cells.resize(count,0);
}

int registers::read(int index)
{
    return cells[index];
} 

void registers::write(int index , int value)
{
    cells[index]=value;
}

void registers::display()
{
    cout << "    Registers (CPU)\n";
    cout << "************************\n";
    cout << "Address" << "      Value(Hexa)    \n";
    cout << "************************\n";
    for(int i=0;i<count;i++)
    {
        cout << "  ";
        cout << hex << uppercase << setfill('0') << setw(2) << i << "          " << setw(2)<< cells[i] << '\n';
    }
}