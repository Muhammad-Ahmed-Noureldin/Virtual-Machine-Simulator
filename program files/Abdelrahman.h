#ifndef ABDELRHMAN_HPP
#define ABDELRHMAN_HPP
class CPU;
class Screen;
class Memory;
#include <vector>

using namespace std;

class programCounter
{
    private:
    
    int Address;


    public:

    void set( int Address );
    int get();
    void increment();


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

class voleMachine
{
    private:
    Memory *memory;
    CPU * cpu;
    Screen * screen;
    public:
    voleMachine(Memory *memory, CPU * cpu ,Screen * screen);

    void load_program_from_file(string filePath ,int address);


};
#endif
