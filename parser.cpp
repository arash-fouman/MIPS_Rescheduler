//
//  parser.cpp
//  
//
//  Created by Arash Fouman on 4/10/19.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <map>

#define nop "00000000000000000000000000000000"


using namespace std;

string toStr( double );

class instruction{
public:
    
    instruction(int t, string s , int op1, int op2, int prevOp1, int prevOp2): exeTime(t), operation(s), operand1(op1), operand2(op2), prevOperand1(prevOp1), prevOperand2(prevOp2){
        
      /*  initializeInstructionGroup();
        if(m.find(operation) == m.end()){
            cout<<operation<<endl;
          cout<<"error\n";
        }
        m[operation]++;*/
        
    }
    
    void initializeInstructionGroup();
    
    friend ostream& operator<< (ostream& out , instruction i){
        
        map<string,double>::iterator it;
       
 
        out<<setw(35)<<toStr(i.operand1^i.prevOperand1)<<setw(35)
        <<toStr(i.operand2^i.prevOperand2)<<setw(35)<<i.exeTime;//<<toStr(i.operand1)<<setw(35)

        return out;
    }
    
//private:
    int exeTime;
    string operation;
    int operand1;
    int operand2;
    int prevOperand1;
    int prevOperand2;
    map<string,double> m;
};

vector<instruction> readFile (string);
string toBin( string );
string toBin( char );
string parse ( string );
string parseInst( string );
string parseOpCode( string , string );
bool isBEQZ( string );
bool isMult( string );
int toInt( string );


int main(){
    
    
    vector<instruction> vi = readFile("prof.txt");
    ofstream fout("test.txt");

    for( int i = 0 ; i < vi.size() ; ++i ){
        fout<<vi[i].exeTime<<endl;
        

    }

    return 0;
}

void instruction::initializeInstructionGroup(){
    
    
    m.insert(pair<string,double>("COP",0));
    m.insert(pair<string,double>("add",0));
    m.insert(pair<string,double>("branch",0));
    m.insert(pair<string,double>("div",0));
    m.insert(pair<string,double>("j",0));
    m.insert(pair<string,double>("jr",0));
    m.insert(pair<string,double>("lbu",0));
    m.insert(pair<string,double>("logic",0));
    m.insert(pair<string,double>("lui",0));
    m.insert(pair<string,double>("mfhi",0));
    m.insert(pair<string,double>("move",0));
    m.insert(pair<string,double>("mult",0));
    m.insert(pair<string,double>("nop",0));
    m.insert(pair<string,double>("shift",0));
    m.insert(pair<string,double>("slt",0));
    m.insert(pair<string,double>("store",0));
    m.insert(pair<string,double>("xor",0));
    
    m.insert(pair<string,double>("and",0));
    
    
}


vector<instruction> readFile (string fileName){
    
    ifstream fin(fileName.c_str());
    
    int time1, time2, exeTime;
    string inst, op1, op2, pc;
    string pInst, pOp1, pOp2;
    string garbage;
    vector<instruction> vi;
    
    getline(fin,garbage);
    getline(fin,garbage);
    while(fin>>pc>>time1>>time2>>inst>>op1>>op2){
        if(pc == garbage || pc == "00800000")
            continue;
        garbage = pc;
        if(isMult(parse(toBin(inst))))
            exeTime = time2;
        else
            exeTime = time1;
        
        if(exeTime == 0)
            continue;
        
        inst = parse(toBin(inst));
        op1  = toBin(op1);
        op2  = toBin(op2);
        //pOp1 = toBin(pOp1);
        //pOp2 = toBin(pOp2);
        
        if(inst == "and"  )
            vi.push_back(instruction(exeTime, inst, toInt(op1), toInt(op2), toInt(pOp1), toInt(pOp2)));
    }
    
    return vi;
    
}

string toBin( string s ){
    int size = s.size();
    string res = "";
    
    for( int i = 0 ; i < size ; ++i ){
        res += toBin( s[i] );
    }
    
    return res;
}

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

int toInt( string s ){
    int res = 0;
    int size = s.size()-1;
    
    for( int i = size ; i >= 0 ; --i){
        res += (s[size-i]-48)*(1<<i);
    }
    return res;
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

string parseInst( string s ){
    
    if( s == "000000" ){
        
        return "special";
        
    }
    else if( s == "010001" ){
        
        return "COP";
        
    }
    else if( s == "001000"){
        //return "addi";
        return "add";
        
    }
    else if( s == "001001"){
        //return "addiu";
        return "add";
        
    }
    else if( s == "001100"){
        //return "andi";
        //return "logic";
        return "and";
        
    }
    else if( s == "001101"){
        //return "ori";
        //return "logic";
        return "or";
        
    }
    else if( s == "001010"){
        //return "slti";
        return "slt";
        
    }
    else if( s == "001011"){
        //return "sltiu";
        return "slt";
        
    }
    else if( s == "001110"){
        //return "xori";
        return "xor";
        
    }
    else if( s == "001111"){
        return "lui";
        
    }
    else if( s == "000010"){
        return "j";
        
    }
    else if( s == "000011"){
        //return "jal";
        return "j";
        
    }
    else if( s == "100100"){
        return "lbu";
        
    }
    else if( s == "100011" ){
        return "lw";
        //return "store";
        
    }
    
    else if( s == "100000" ){
        //return "lb";
        return "lbu";
        
    }
    else if(s == "100001" ){
        //return "lh";
        return "lbu";
        
    }
    else if( s == "100101" ){
        //return "lhu";
        return "lbu";
        
    }
    
    
    else if( s == "101011" ){
        return "sw";
        //return "store";
        
    }
    
    else if(s == "101000" ){
        //return "sb";
        return "store";
        
    }
    else if(s == "101001" ){
        //return "sh";
        return "store";
        
    }
    
    else if( s == "000001"){
        return "regimm";
        
    }
    
    
    else if( s == "000100"){
        //return "beq";
        return "branch";
        
    }
    
    else if( s == "000111"){
        //return "bgtz";
        return "branch";
        
    }
    else if( s == "000110"){
        //return "blez";
        return "branch";
        
    }
    else if( s == "000101"){
        return "bne";
        //return "branch";
        
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
            //return "addu";
            return "add";
        else if( temp == "100010" )
            return "sub";
            //return "add";
        else if( temp == "100011" )
            return "subu";
            //return "add";
        
        else if( temp == "100100" )
            return "and";
            //return "logic";
        
        else if( temp == "100101" )
            return "or";
            //return "logic";
        else if( temp == "100110" )
            return "xor";
        else if( temp == "100111" )
            //return "nor";
            return "logic";
        
        else if( temp == "010000" )
            return "mfhi";
        else if( temp == "010001" )
            //return "mthi";
            return "mfhi";
        
        else if( temp == "010010" )
            //return "mflo";
            return "mfhi";
        
        else if( temp == "010011" )
            //return "mtlo";
            return "mfhi";
        
        
        else if( temp == "000010" )
            //return "srl";
            return "shift";
        
        else if( temp == "000110" )
            //return "srlv";
            return "shift";
        
        else if( temp == "000100" )
            //return "sllv";
            return "shift";
        
        else if( temp == "000000" )
            //return "sll";
            return "shift";
        else if( temp == "000111" )
            //return "srav";
            return "shift";
        
        else if( temp == "000011" )
            //return "sra";
            return "shift";
        
        
        else if( temp == "011000" )
            return "mult";
        else if( temp == "011001" )
            //return "multu";
            return "mult";
        else if( temp == "011010" )
            return "div";
        else if( temp == "011011" )
            //return "divu";
            return "div";
        
        else if( temp == "101010" )
            return "slt";
        else if( temp == "101011" )
            //return "sltu";
            return "slt";
        
        
        else if( temp == "001100" )
            return "syscall";
        else if( temp == "001101" )
            return "break";
        else if( temp == "001001" )
            //return "jalr";
            return "jr";
        else if( temp == "001000" )
            return "jr";
        
        
    }
    
    
    else if( op == "regimm" ){
        string temp = s.substr(11,5);
        
        if( temp == "00001" )
            //return "bgez";
            return "branch";
        else if( temp == "10001" )
            //return "bgezal";
            return "branch";
        else if( temp == "00000" )
            //return "bltz";
            return "branch";
        else if( temp == "10000" )
            //return "bltzal";
            return "branch";
        
    }
    
    return op;
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

string toStr( double i ){
    
    string str = "";
    while(i>=1){
        str = char((int(i)%2)+48) + str;
        i /= 2;
        
    }
    
    for(int k = str.size() ; k < 32 ; ++k )
        str = '0' + str;
    
    
    return str;
}
