//
//  mips.hpp
//  
//
//  Created by Arash Fouman on 7/25/19.
//

#ifndef mips_h
#define mips_h

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;


#define nop "00000000000000000000000000000000"
#define opCode    temp.substr(0,6)
#define imm       toIntSigned(temp.substr(16,16))
#define rs_addr   toIntUnsigned(temp.substr(6,5))
#define rt_addr   toIntUnsigned(temp.substr(11,5))
#define rd_addr   toIntUnsigned(temp.substr(16,5))
#define sa_val    toIntUnsigned(temp.substr(21,5))

string toBin( string );
string toBin (char );
string parse ( string );
string parseInst( string );
string parseOpCode( string, string );
bool isBEQZ( string);
bool isMult( string );
int toIntUnsigned( string );
int toIntSigned( string );


class instruction {
public:
    
    instruction(){
        return;
    }
    
    instruction (string inst){
        temp = inst;
        decode();
        
    }
    
    void decode(){
        
        operation = parse( temp );
        rs = rs_addr;
        rt = rt_addr;
        rd = rd_addr;
        sa = sa_val;
        immediate = imm;
        imm_Rnot = opCode != "000000";
        
    }
    
    friend ostream& operator<< (ostream& outStream , instruction i){
        outStream<<"inst"<<setw(7)<<"rs"<<setw(7)<<"rt"
        <<setw(7)<<"rd"<<setw(7)<<"sa"<<setw(7)<<"imm"<<setw(7)
        <<"immOrR"<<endl;
        
        outStream<<setw(7)<<i.operation<<setw(7)<<i.rs<<setw(7)
        <<i.rt<<setw(7)<<i.rd<<setw(7)<<i.sa<<setw(7)
        <<i.immediate<<setw(7)<<i.imm_Rnot<<endl;
        
        return outStream;
    }
    
    friend bool operator==(instruction i, string s){
        return (i.operation == s);
    }
    
    string temp;
    string operation;
    int rs;
    int rt;
    int rd;
    int sa;
    bool imm_Rnot;
    int immediate;
};

class entity {
public:
    
    entity( string PC, string instruction1, string operand1, string operand2, int time ): pc(PC), instr(instruction1), op1(operand1), op2(operand2), exeTime(time)
    {
        
        turn=0;
        instruction alaki(toBin(instruction1));        
        inst = alaki;
        //cout<<"Co: "<<inst<<endl;
        
    }
    
    entity (string s){
        turn=0;
        instruction alaki(s);
        inst = alaki;
    }
    
    entity(){
        instruction alaki(nop);
        inst = alaki;
        turn=0;
        return;
    }
    
    bool setTurn( int i ){
        if(inst == "nop")
            return false;
        turn = i;
        return true;
    }
    
    friend bool operator < (entity i, entity j){
        return (i.turn < j.turn);
    }
    
    string pc;
    string instr;
    string op1;
    string op2;
    int exeTime;
    
    int turn;
    instruction inst;
    
};


string toBin( char c ){
    
    switch (c) {
        case '0':
            return "0000";
        case '1':
            return "0001";
        case '2':
            return "0010";
        case '3':
            return "0011";
        case '4':
            return "0100";
        case '5':
            return "0101";
        case '6':
            return "0110";
        case '7':
            return "0111";
        case '8':
            return "1000";
        case '9':
            return "1001";
        case 'a':
            return "1010";
        case 'b':
            return "1011";
        case 'c':
            return "1100";
        case 'd':
            return "1101";
        case 'e':
            return "1110";
        case 'f':
            return "1111";
            
        default:
            return "xxxx";
    }
    
}
string toBin( string s ){
    int size = s.size();
    string res = "";
    
    for( int i = 0 ; i < size ; ++i ){
        res += toBin( s[i] );
    }
    
    return res;
}



bool isBEQZ( string name ){
    
    if( name.substr(11,5) == "00000" )
        return true;
    return false;
    
}

bool isMult( string s ){
    
    if(s == "mult" || s == "multu" || s == "div" || s == "divu" )
        return 1;
    
    return 0;
}

string parseInst( string s ){
    
    if( s == "000000" ){
        
        return "special";
        
    }
    else if( s == "010001" ){
        
        return "COP";
        
    }
    else if( s == "001000"){
        return "addi";
        
    }
    else if( s == "001001"){
        return "addiu";
        
    }
    else if( s == "001100"){
        return "andi";
        
    }
    else if( s == "001101"){
        return "ori";
        
    }
    else if( s == "001010"){
        return "slti";
        
    }
    else if( s == "001011"){
        return "sltiu";
        
    }
    else if( s == "001110"){
        return "xori";
        
    }
    else if( s == "001111"){
        return "lui";
        
    }
    else if( s == "000010"){
        return "j";
        
    }
    else if( s == "000011"){
        return "jal";
        
    }
    else if( s == "100100"){
        return "lbu";
        
    }
    else if( s == "100011" ){
        return "lw";
        
    }
    
    else if( s == "100000" ){
        return "lb";
        
    }
    else if(s == "100001" ){
        return "lh";
        
    }
    else if( s == "100101" ){
        return "lhu";
        
    }
    
    
    else if( s == "101011" ){
        return "sw";
        
    }
    
    else if(s == "101000" ){
        return "sb";
        
    }
    else if(s == "101001" ){
        return "sh";
        
    }
    
    else if( s == "000001"){
        return "regimm";
        
    }
    
    
    else if( s == "000100"){
        return "beq";
        
    }
    
    else if( s == "000111"){
        return "bgtz";
        
    }
    else if( s == "000110"){
        return "blez";
        
    }
    else if( s == "000101"){
        return "bne";
        
    }
    
    
    else if( s == "110001" ){
        //return "lwc1";
        return "COP";
        
    }
    else if(s == "111001" ){
        //return "swc1";
        return "COP";
    }
    
    
    else{
        
        return "none";
    }
}

string parseOpCode( string op, string s ){
    
    if( op == "special" ){
        
        string temp = s.substr(26,6);
        
        if( temp == "100000" )
            return "add";
        
        else if( temp == "100001" )
            return "addu";
        else if( temp == "100010" )
            return "sub";
        else if( temp == "100011" )
            return "subu";
        
        else if( temp == "100100" )
            return "and";
        else if( temp == "100101" )
            return "or";
        else if( temp == "100110" )
            return "xor";
        else if( temp == "100111" )
            return "nor";
        
        else if( temp == "010000" )
            return "mfhi";
        else if( temp == "010001" )
            return "mthi";
        
        else if( temp == "010010" )
            return "mflo";
        
        else if( temp == "010011" )
            return "mtlo";
        
        
        else if( temp == "000010" )
            return "srl";
        
        else if( temp == "000110" )
            return "srlv";
        
        else if( temp == "000100" )
            return "sllv";
        
        else if( temp == "000000" )
            return "sll";
        else if( temp == "000111" )
            return "srav";
        
        else if( temp == "000011" )
            return "sra";
        
        
        else if( temp == "011000" )
            return "mult";
        else if( temp == "011001" )
            return "multu";
        else if( temp == "011010" )
            return "div";
        else if( temp == "011011" )
            return "divu";
        
        else if( temp == "101010" )
            return "slt";
        else if( temp == "101011" )
            return "sltu";
        
        
        else if( temp == "001100" )
            return "syscall";
        else if( temp == "001101" )
            return "break";
        else if( temp == "001001" )
            return "jalr";
        else if( temp == "001000" )
            return "jr";
        
        
    }
    
    
    else if( op == "regimm" ){
        string temp = s.substr(11,5);
        
        if( temp == "00001" )
            return "bgez";
        else if( temp == "10001" )
            return "bgezal";
        else if( temp == "00000" )
            return "bltz";
        else if( temp == "10000" )
            return "bltzal";
        
    }
    
    return op;
}


string parse ( string vs ){
    
    string opcode,instruction;
    
    
    if( vs == nop ){
        return "nop";
    }
    
    opcode = parseInst( vs.substr(0,6) );
    instruction = parseOpCode( opcode , vs );
    
    if( instruction == "none" ){
        cout<<instruction<<endl;
    }
    
    if( instruction == "beq" ){
        return ( isBEQZ( vs ) ? "beqz" : instruction );
    }
    
    return instruction;
}


int toIntSigned( string s ){
    int res = 0;
    int size = s.size()-1;
    
    bool sign = s[0]-48;
    if(sign)
        for(int i = 0; i < s.size() ; ++i)
            s[i] = ((s[i]-48) ? '0' : '1');
    
    for( int i = size-1 ; i >= 0 ; --i){
        res += (s[size-i]-48)*(1<<i);
    }
    return (sign ? -(res+1) : res);
}
int toIntUnsigned( string s ){
    int res = 0;
    int size = s.size()-1;
    
    for( int i = size ; i >= 0 ; --i){
        res += (s[size-i]-48)*(1<<i);
    }
    return res;
}


#endif /* mips_h */
