//#include "../include/scanner.h"
#include "../include/instruction.h"
#include <fstream>
#include <sstream>

void test_only_instruction() {
    SVM* svm;

    std::list<Instruction*> sl;

    sl.push_back(new Instruction("",Instruction::IPUSH, 30));
    sl.push_back(new Instruction("",Instruction::IPUSH, 3));
    sl.push_back(new Instruction("",Instruction::IDIV));
    sl.push_back(new Instruction("",Instruction::IDUP));
    sl.push_back(new Instruction("",Instruction::IPUSH, 2));  
    sl.push_back(new Instruction("",Instruction::IADD));
    sl.push_back(new Instruction("",Instruction::ISWAP));
    sl.push_back(new Instruction("",Instruction::IJMPGE,"L20"));
    sl.push_back(new Instruction("",Instruction::IPUSH, 10));		  
    sl.push_back(new Instruction("",Instruction::IGOTO, "LEND"));
    sl.push_back(new Instruction("L20",Instruction::IPUSH, 20));
    sl.push_back(new Instruction("LEND",Instruction::ISKIP));

    svm = new SVM(sl);

    std::cout << "Program:" << std::endl;
    svm->print();
    std::cout << "----------------" << std::endl;

    
    std::cout << "Running ...." << std::endl;
    svm->execute();
    std::cout << "Finished" << std::endl;

    svm->print_stack();
}

int main(int argc, char** argv) {
    test_only_instruction();
    return 0;
}
