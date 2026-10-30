#include <iostream>
#include "Abdelrahman.h"

programCounter::programCounter( int value )
{
    this->value=value;
}
int programCounter::getInstructionAddress()
{
    value+=2;
    return value-2;
}
