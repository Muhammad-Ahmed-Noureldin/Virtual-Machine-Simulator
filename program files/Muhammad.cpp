#include "Muhammad.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include "Abdelrahman.h"
#include "Eslam.h"
#include "Nabil.h"
using namespace std;
#include <cmath>



    float ALU::decode(int val){

        float sign = (val >> 7 )& 0x01;
        float exponent = (val >> 4) & 0x07;
        float mantissa = val & 0x0f;

        exponent -=4;
        mantissa /= 16;

        float result = mantissa * (pow(2 , exponent));

        if (sign){
            result *= -1;
        }

        return result ;
    }

    int ALU::encode(float val){

        if(!val){
            return 0;
        }

        int sign = 0;

        if (val < 0){
            sign = 1;
            val *= -1;
        }

        int exponent = 0;

        while(val >= 1){
            val /= 2;
            exponent++;
        }

        while(val < 0.5){
            val *= 2;
            exponent--;
        }

        exponent += 4;

        if(exponent > 7){
            exponent=7;
            int mantissa = 15;

            int result = (sign << 7) | (exponent << 4) | (mantissa);

            return result;
        } 

        if(exponent<0) return 0x00;

        val *= 16;

        int mantissa = static_cast<int>(val);

        int result = (sign << 7) | (exponent << 4) | (mantissa);


        return result;
    }
    


    ////////////////////////////////////////////////////////////////////////////////////////
    int ALU::add_twos_complement(int val1 ,int val2){

        int result = val1 + val2;

        result &= 0xff;

        return result;


        
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    int ALU::add_floating_point(int val1 , int val2){

        val1 &= 0xff;
        val2 &= 0xff;

        float x = decode(val1);
        float y = decode(val2);

        return encode(x + y);
    }

    ////////////////////////////////////////////////////////////////////////////////////////    
    int ALU::bitwise_or(int val1 , int val2){

        int result = val1 | val2;

        result &= 0xff;
        
        return result;
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    int ALU::bitwise_and(int val1 , int val2){

        int result = val1 & val2;
        
        result &= 0xff;

        return result;
        
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    int ALU::bitwise_xor(int val1 , int val2){

        int result = val1 ^ val2;

        result &= 0xff;
        
        return result;
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    int ALU::rotate_right(int val , int steps){
        
        val &= 0xff;

        steps %= 8;

        int result = (val >> steps) | (val << (8 - steps));

        result &= 0xff;

        return result;
    }


    ////-------------------------------------------------------------------------------

    CU::CU(programCounter* pc,InstructionRegister* ir , Memory* memory, registers* reg, ALU* alu , Screen* screen)
    : pc(pc) , ir(ir) , memory(memory) , reg(reg) , alu(alu) , screen(screen) , halted(false){}


    bool CU::is_halted(){

        return halted;
    }

    void CU::fetch(){

        int address = pc->get(); 
        int val1 = memory->read(address);   //0x1f -->0001 1111 --> 31
        int val2 = memory->read(address+1); ///0x04 -->4



        pc->increment();

        stringstream ss;

        ss << uppercase << hex << setfill('0') << setw(2) << val1 << setw(2) << val2 ;

        ir->set(ss.str());
    }

    void CU::decode_and_execute(){

        int opcode = ir->get_opcode();
        int r = ir->get_r() ;
        int s = ir->get_s();
        int t = ir->get_t();
        int xy = ir->get_xy();
        int x = ir->get_x();

        if(opcode == OP_LOAD_MEM){

            int var = memory->read(xy);
            reg->write(r , var);
        }


        else if(opcode == OP_LOAD_VAL){
            reg->write(r , xy);
        }


        else if(opcode == OP_STORE){
            if(xy == 0xff){
                screen->write(reg->read(r));
            }
            else{
                memory->write( xy , reg->read(r) );
            }
        }


        else if(opcode == OP_MOVE){
            reg->write(s , reg->read(r));
        }


        else if(opcode == OP_ADD_INT){
            reg->write(r, alu->add_twos_complement(reg->read(s) , reg->read(t)));
        }


        else if(opcode == OP_ADD_FLOAT){
            reg->write(r , alu->add_floating_point(reg->read(s) , reg->read(t)));
        }


        else if(opcode == OR){
            reg->write(r, alu->bitwise_or(reg->read(s) , reg->read(t)));
        }


        else if(opcode == AND){
            reg->write(r , alu->bitwise_and(reg->read(s) , reg->read(t)));
        }


        else if(opcode == XOR){
            reg->write(r , alu->bitwise_xor(reg->read(s) , reg->read(t)));
        }


        else if(opcode == ROTATE){
            reg->write(r , alu->rotate_right(reg->read(r) , x));
        }


        else if(opcode == JUMP){
            if(reg->read(r) == reg->read(0)){
                pc->set(xy);
            }
        }

        
        else{
            halted = true;
        }

    }

    void CU::step(){

        if(is_halted()){
            return;
        }
        else{
            fetch();
            decode_and_execute();
        }
    }

