//
//  mips.cpp
//  
//
//  Created by Arash Fouman on 7/25/19.
//

#include "mips.h"
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <set>

#define nop "00000000000000000000000000000000"


using namespace std;

queue <entity> q0;
queue <entity> q1;
queue <entity> q2;

set <int> toCommit;

int inOrder;
int state = 1;
bool done;

vector<entity> readMem();
entity decode_stage   ( entity& );
entity issue_stage    ( entity& );
entity execute_stage  ( entity& );
entity memAccess_stage( entity& );
entity writeBack_stage( entity& );
entity commit_stage   ( entity& );

int predictor (int i0 , int i1 ){ return (i0*i1)>0;}
bool isReady(const instruction&);

bool writePending( int, int );

int main(){
    
    vector<entity> entityMem = readMem();
    
    int programCounter = 0;
    inOrder = 0;
    done = 0;
    
    entity fe_de   ;
    entity de_is   ;
    entity is_exe  ;
    entity exe_mem ;
    entity mem_wb  ;
    entity wb_comm ;
    
    entity temp1, temp2, temp3, temp4, temp5, temp6, temp7;
    
    do{
//        cout<<"state: "<<state<<endl;
//        cout<<"pc: "<<programCounter<<endl;

        temp7 = commit_stage(wb_comm);
        temp6 = writeBack_stage(mem_wb);
        temp5 = memAccess_stage(exe_mem);
        temp4 = execute_stage(is_exe);
        temp3 = issue_stage(de_is);
        temp2 = decode_stage(fe_de);
        
        temp1 = programCounter < entityMem.size() ?
            entityMem[programCounter] : entity(nop);
        
        
        fe_de = temp1;
        de_is = temp2;
        is_exe = temp3;
        exe_mem = temp4;
        mem_wb = temp5;
        wb_comm = temp6;

/*        cout<<"DE: "<<de_is.inst<<endl;
        cout<<"IS: "<<is_exe.inst<<endl;
        cout<<"EX: "<<exe_mem.inst<<endl;
        cout<<"ME: "<<mem_wb.inst<<endl;
        cout<<"WB: "<<wb_comm.inst<<endl;
*/
        
        programCounter++;
        //cout<<endl;
    }while(!done);
    
}

vector<entity> readMem(){
    
    vector<entity> vEn;
    string pc, instruction, op1, op2;
    int time1, time2, time;
    
    while(cin>>pc>>time1>>time2>>instruction>>op1>>op2){
        
        if( isMult(parse(toBin(instruction))) )
            time = time2;
        else
            time = time1;
        
        if(pc == "00800000" || time == 0)
            continue;
        
        vEn.push_back(entity(pc,instruction,op1,op2, time));
    }
    return vEn;
}

entity decode_stage( entity& ent_in ){
    
    static vector <entity> ve;
    static int turnCounter  = 1;
    
    entity temp;
    
    if( ent_in.setTurn(turnCounter) ){
        toCommit.insert(turnCounter);
        turnCounter++;
    }
    
    if(inOrder)
        return ent_in;
    

    if( !ve.empty() && isReady(ve[0].inst) ){
        temp = ve[0];
        ve.erase(ve.begin());
        ve.push_back(ent_in);
        return temp;
        
    }
    
    if( !isReady(ent_in.inst) ){
        ve.push_back(ent_in);
        return entity(nop);
    }
    
    
    return ent_in;
    
}

bool isReady( const instruction& inst){
    
    
    if( inst.imm_Rnot ){
        if( writePending(inst.rs, 2) )
            return false;
    }
    else
        if( writePending(inst.rs, 2) || writePending(inst.rt,2) ){
            return false;
        }
    
    if( inst.imm_Rnot ){
        writePending(inst.rt,1);
    }
    else
        writePending(inst.rd,1);
    return true;
    
}

bool writePending( int index, int val ){

    static bool regFileStatus [32] = {0};
    
    if(index == 0){
        return false;
    }
    if( val == 2 )  //read
        return regFileStatus[index];
    else            //update
        regFileStatus[index] = val;
    return true;
    
}

entity issue_stage( entity& ent_queue ){
    
    int Qnumber;
    entity temp;
    
    if(inOrder)
        return ent_queue;
    
    
    if( ent_queue.inst.imm_Rnot )
        Qnumber = predictor( ent_queue.inst.rs, ent_queue.inst.immediate );
    else
        Qnumber = predictor( ent_queue.inst.rs, ent_queue.inst.rt );
    
    
    state = 0;
    Qnumber = 0;
    if( Qnumber == 0 && ent_queue.inst.operation != "nop")
        q0.push( ent_queue );
    if( Qnumber == 1 && ent_queue.inst.operation != "nop")
        q1.push( ent_queue );
    if( Qnumber == 2 && ent_queue.inst.operation != "nop")
        q2.push( ent_queue );
    
    
    
    if(state == 0 && !q0.empty()){
        temp = q0.front();
        q0.pop();
        return temp;
    }
    if(state == 1 && !q1.empty()){
        temp = q1.front();
        q1.pop();
        return temp;
    }
    if(state == 2 && !q2.empty()){
        temp = q2.front();
        q2.pop();
        return temp;
    }
    
    
    return entity(nop);
}

entity execute_stage( entity& ent_exe ){
    
    if(ent_exe.inst.imm_Rnot)
        writePending( ent_exe.inst.rt , 0);
    
    else
        writePending( ent_exe.inst.rd, 0);
    
    return ent_exe;
}

entity memAccess_stage( entity& ent_mem ){
    
    return ent_mem;
    
}

entity writeBack_stage( entity& ent_wb ){
    
    return ent_wb;
    
}

entity commit_stage( entity& ent_comm ){
    
    static set<entity> pending;
    static int alaki = 0;
    entity temp;
    
    int head = *toCommit.begin();
 
    if(alaki > 2)
        done = 1;
    
    if(toCommit.empty()){
        alaki++;
        return entity(nop);
    }
    
    
    
    if(ent_comm.turn == *toCommit.begin()){
     
        if(ent_comm.inst.operation != "nop")
            cout<<"Re:"<<ent_comm.inst<<endl;
        
        toCommit.erase(toCommit.begin());
        return ent_comm;
    }
    
    
    if( ent_comm.turn > *toCommit.begin()){
        pending.insert(ent_comm);
    }
    
    temp = *pending.begin();
    if(temp.turn == *toCommit.begin()){
 
        if(temp.inst.operation != "nop")
            cout<<"Re:"<<temp.inst<<endl;
        
        toCommit.erase(toCommit.begin());
        pending.erase(pending.begin());
        return ent_comm;
    }
        
    
    
    
    return ent_comm;
    
}
