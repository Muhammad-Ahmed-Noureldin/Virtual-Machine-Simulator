#ifndef ABDELRHMAN_HPP
#define ABDELRHMAN_HPP
#include <iostream>
#include <vector>
using namespace std;

class programCounter
{
    private:
    int Address;
    public:
    void setAddress( int Address );

    int getInstructionAddress();
};

class registers
{
    private:
    int count;
    vector<int>cells;
    public:
    registers();
    
    int read(int index);
    
    void write(int index , int value);

    void display();

};
#endif
