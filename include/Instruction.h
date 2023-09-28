#ifndef Syntax_Analysis_INSTRUCTION_H
#define Syntax_Analysis_INSTRUCTION_H

#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <unordered_map>

struct Instruction {
    enum IType { IPUSH=0, IPOP, IDUP, ISWAP, IADD, ISUB, IMUL, IDIV, 
                 IGOTO, IJMPEQ, IJMPGT, IJMPGE, IJMPLT, IJMPLE, 
                 ISKIP, ISTORE, ILOAD, IPRINT };
    static const char* snames[18];

    IType itype;
    bool hasarg;
    std::string label, jmp_label;
    int argument_int;

    Instruction(std::string, IType);
    Instruction(std::string, IType, int);
    Instruction(std::string, IType, std::string);
};

const char* Instruction::snames[18] = { "push", "pop", "dup", "swap", "add", "sub", "mult", "div", 
                                        "goto", "jmpeq", "jmpgt", "jmpge", "jmplt", "jmple", "skip", 
                                        "store", "load", "print" };

Instruction::Instruction(std::string label, IType itype): label(label), itype(itype), hasarg(false) {}
Instruction::Instruction(std::string label, IType itype, int argument): label(label), itype(itype), hasarg(true), argument_int(argument) { jmp_label = ""; }
Instruction::Instruction(std::string label, IType itype, std::string argument): label(label), itype(itype), hasarg(true), jmp_label(argument) {}



class SVM {
private:
    int registers[8];
    std::stack<int> opstack;

    std::vector<Instruction*> instructions;
    std::unordered_map<std::string, int> labels;

    int pc;

private:
    void perror(std::string);
    void execute(Instruction*);
    void register_write(int, int);
    int register_read(int);

public:
    SVM(std::list<Instruction*>&);
    void execute();
    void print_stack();
    void print();
    int top();
};

void SVM::perror(std::string msg) {
    std::cout << "error: " << msg << std::endl;
    exit(0);
}

void SVM::execute(Instruction* instruction) {
    Instruction::IType itype = instruction->itype;
    int next, top;

    if (itype==Instruction::IPOP || itype==Instruction::IDUP || itype==Instruction::IPRINT || 
        itype==Instruction::ISKIP) {
        switch (itype) {
            case (Instruction::IPOP):
                if (opstack.empty()) this->perror("Can't pop from an empty stack");
                opstack.pop();
                break;
            case (Instruction::IDUP):
                if (opstack.empty()) this->perror("Can't dup from an empty stack");
                opstack.push(opstack.top());
                break;
            case (Instruction::IPRINT):
                this->print_stack(); 
                break;
            case (Instruction::ISKIP): 
                break;
            default: this->perror("Programming Error 1");
        }
        pc++;
    } else if (itype==Instruction::IPUSH || itype==Instruction::ISTORE || itype==Instruction::ILOAD) {
        int argument = instruction->argument_int;

        switch (itype) {
            case (Instruction::IPUSH): 
                opstack.push(instruction->argument_int); 
                break;
            case (Instruction::ISTORE):
                if (opstack.empty()) this->perror("Can't store from an empty stack");
                this->register_write(instruction->argument_int, opstack.top()); 
                opstack.pop(); 
                break;
            case (Instruction::ILOAD):
                opstack.push(register_read(instruction->argument_int)); 
                break;
            default: this->perror("Programming Error 2");
        }
        pc++;
    } else if (itype==Instruction::IJMPEQ || itype==Instruction::IJMPGT || itype==Instruction::IJMPGE || 
               itype==Instruction::IJMPLT || itype==Instruction::IJMPLE) {
        top = opstack.top();
        opstack.pop();
        next = opstack.top();
        opstack.pop();

        bool jump = false;
        switch(itype) {
            case(Instruction::IJMPEQ): 
                jump = (next==top); 
                break;
            case(Instruction::IJMPGT): 
                jump = (next>top);
                break;
            case(Instruction::IJMPGE): 
                jump = (next>=top);
                break;
            case(Instruction::IJMPLT): 
                jump = (next<top);
                break;
            case(Instruction::IJMPLE): 
                jump = (next<=top);
                break;
            default: this->perror("Programming Error 3");
        }

        if (jump) pc = instruction->argument_int;
        else pc++;
    } else if (itype==Instruction::IADD || itype==Instruction::ISUB || itype==Instruction::IMUL || 
               itype==Instruction::IDIV || itype==Instruction::ISWAP) {
        top = opstack.top();
        opstack.pop();
        next = opstack.top();
        opstack.pop();

        switch(itype) {
            case(Instruction::IADD): 
                opstack.push(next+top);
                break;
            case(Instruction::ISUB): 
                opstack.push(next-top);
                break;
            case(Instruction::IMUL): 
                opstack.push(next*top);
                break;
            case(Instruction::IDIV): 
                opstack.push(next/top);
                break;
            case(Instruction::ISWAP): 
                opstack.push(top); 
                opstack.push(next); 
                break;
            default: perror("Programming Error 4");
        }
        pc++;
    } else if (itype == Instruction::IGOTO) {
        pc = instruction->argument_int;
    } else {
        std::cout << "Programming Error: execute instruction" << std::endl;
        exit(0);
    }
}

void SVM::register_write(int reg, int value) {
    if (reg > 7 || reg < 0) this->perror("Invalid register number");
    registers[reg] = value;
}

int SVM::register_read(int reg) {
    if (reg > 7 || reg < 0) this->perror("Invalid register number");
    return registers[reg];
}

SVM::SVM(std::list<Instruction*>& sl) {
    pc = 0;

    instructions.reserve(sl.size());
    std::copy(begin(sl), end(sl), back_inserter(instructions));
    for(int i=0; i < instructions.size(); i++) {
        std::string label = instructions[i]->label;
        if (label != "") labels[label] = i;
    }

    for(int i=0; i < instructions.size(); i++) {
        std::string jmp_label = instructions[i]->jmp_label;
        if (jmp_label != "") {
            std::unordered_map<std::string,int>::const_iterator it = labels.find(jmp_label);
            if (it == labels.end()) {
                std::cout << "No label found: " << jmp_label << std::endl;
                exit(0);
            } else instructions[i]->argument_int = it->second;
        }
    }
}

void SVM::execute() {
    while (true) {
        if (pc >= instructions.size()) break;
        this->execute(instructions[pc]);
    }
}

void SVM::print_stack() {
    std::stack<int> local;

    std::cout << "stack [ ";
    while(!opstack.empty()) {
        std::cout << opstack.top() << " ";
        local.push(opstack.top());
        opstack.pop();
    }
    std::cout << "]" << std::endl;

    while(!local.empty()) {
        opstack.push(local.top());
        local.pop();
    }
}

void SVM::print() {
    for(int i= 0; i < instructions.size(); i++) {
        Instruction* s = instructions[i];

        if (s->label != "") std::cout << s->label << ": ";
        std::cout << Instruction::snames[s->itype] << " ";
        if (s->hasarg) {
            if (s->jmp_label == "") std::cout << s->argument_int;
            else std::cout << s->jmp_label;
        }
        std::cout << std::endl;
    }
}

int SVM::top() {
    if (opstack.empty()) perror("Can't get top of an empty stack");
    return opstack.top();
}

#endif // Syntax_Analysis_INSTRUCTION_H
