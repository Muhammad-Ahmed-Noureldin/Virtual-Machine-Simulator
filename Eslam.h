#ifndef ESLAM_H
#define ESLAM_H


class Memory
{

    private:
    static const int size=256;
    int cells[size];
    public:
 Memory();
     
int read(int address) const;

void write(int address,int val);

void display_matrix();

};

class Screen
{
    private:
    vector<int>output_buffer;
    
    public:
    
    void write(int val);

    void display();

    void clear();
};
#endif
