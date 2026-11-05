#ifndef MUHAMMAD_H
#define MUHAMMAD_H
class InstructionRegister;
class programCounter;
class  Memory;
class  registers;
class  Screen;

class ALU{

    private:

    float decode(int val);

    int encode(float val);


    public:

    int add_twos_complement(int val1 ,int val2);

    int add_floating_point(int val1 , int val2);

    int bitwise_or(int val1 , int val2);

    int bitwise_and(int val1 , int val2);

    int bitwise_xor(int val1 , int val2);

    int rotate_right(int val , int steps);

};

class CU{
    
    private:

    static const int OP_LOAD_MEM = 0X1;
    static const int OP_LOAD_VAL = 0X2;
    static const int OP_STORE = 0X3;
    static const int OP_MOVE = 0X4;
    static const int OP_ADD_INT = 0X5;
    static const int OP_ADD_FLOAT = 0X6;
    static const int OR = 0X7;
    static const int AND = 0X8;
    static const int XOR = 0X9;
    static const int ROTATE = 0XA;
    static const int JUMP = 0XB;
    static const int HALT = 0XC;
    
    bool halted;

    programCounter* pc; // 0x01334AAF934
    InstructionRegister* ir;
    Memory* memory;
    registers* reg;
    ALU* alu;
    Screen* screen;

    public:

    CU(programCounter* pc,InstructionRegister* ir , Memory* memory, registers* reg, ALU* alu , Screen* screen);

    bool is_halted();
    void fetch();
    void decode_and_execute();
    void step();
};


#endif