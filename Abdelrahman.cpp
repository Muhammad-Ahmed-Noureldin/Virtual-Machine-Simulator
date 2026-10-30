#include <iostream>
#include "Abdelrahman.h"

programCounter::programCounter( int Address )
{
    this->Address=Address;
}
int programCounter::getInstructionAddress()
{
    Address+=2;
    return Address-2;
}
