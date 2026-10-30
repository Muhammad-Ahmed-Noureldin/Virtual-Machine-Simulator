#include "Eslam.h"
#include <iostream>
#include <cstdlib>
#include <iomanip>
using namespace std;

Memory::Memory()
{
  for(int i=0;i<size;i++)
    cells[i]=0;
}

int Memory::read(int address) const
{
   if(address<0 || address>=size)
     exit(EXIT_FAILURE);
  else 
     return
       cells[address];
}

void Memory::write(int address,int val)
{
    if(address<0 || address>=size)
     exit(EXIT_FAILURE);
  else
      cells[address]=val;
}

void Memory::display_matrix()
{
  cout<<"\tAddress "<<"\tvalue\n";
    
    for(int i=0;i<size;i++)
    {
    cout<<"\t0x"<<uppercase<<hex<<setfill('0')<<setw(2)<<i;
    cout<<"\t"<<dec<<cells[i]<<"\n"; 
    }
 
}
