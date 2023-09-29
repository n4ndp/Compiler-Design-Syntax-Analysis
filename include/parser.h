#ifndef Syntax_Analysis_PARSER_H
#define Syntax_Analysis_PARSER_H

#include "scanner.h"

class Parser {
private:
    Scanner* scanner;
    Token *current, *previous;

private:
    bool isAtEnd();
    bool check(Token::Type);
    bool match(Token::Type);
    bool advance();
    Instruction* parseInstruction();

public:
    Parser(Scanner*);
    SVM* parse();
};

bool Parser::isAtEnd() { return (current->type == Token::END); }
bool Parser::check(Token::Type token_type) { return this->isAtEnd() ? false : current->type == token_type;}

bool Parser::match(Token::Type token_type) {
    if (this->check(token_type)) {
        this->advance();
        return true;
    }
    return false;
}

bool Parser::advance() {
    if (!this->isAtEnd()) {
        Token* temp = current;

        if (previous) delete previous;
        current = scanner->next_token();
        previous = temp;

        if (this->check(Token::ERR)) {
            std::cout << "Parse error, unrecognised character: " << current->lexeme << std::endl;
            exit(0);
        }
        return true;
    }
    return false;
}

Instruction* Parser::parseInstruction() {
    Instruction* instr = nullptr;
    std::string label = "";
    std::string jmp_label;
    Token::Type token_type;
    int argument_int;
    int type_instr = 0;

    if (this->match(Token::LABEL)) label = previous->lexeme;

    if (this->match(Token::POP) || this->match(Token::ADD) || this->match(Token::SUB) ||
        this->match(Token::MUL) || this->match(Token::DIV) || this->match(Token::SKIP) ||
        this->match(Token::DUP) || this->match(Token::SWAP) || this->match(Token::PRINT)) {
        type_instr = 0;
        token_type = previous->type;
    } else if (this->match(Token::PUSH) || this->match(Token::STORE) || this->match(Token::LOAD)) {
        type_instr = 1;
        token_type = previous->type;
    } else if (this->match(Token::GOTO) || this->match(Token::JMPEQ) ||
               this->match(Token::JMPGT) || this->match(Token::JMPGE) ||
               this->match(Token::JMPLT) || this->match(Token::JMPLE)) {
        type_instr = 2;
        token_type = previous->type;
    } else {
        std::cout << "Error: couldn't find a match for " << *current << std::endl;
        exit(0);
    }

    if (type_instr == 1) {
        if (!this->match(Token::NUM)) {
            std::cout << "Error: Expected number" << std::endl;
            exit(0);
        }
        argument_int = std::stoi(previous->lexeme);
    } else if (type_instr == 2) {
        if (!this->match(Token::ID)) {
            std::cout << "Error: Expected id" << std::endl;
            exit(0);
        }
        jmp_label = previous->lexeme;
    }

    if (!this->match(Token::EOL)) {
        std::cout << "Error: Expected end of line" << std::endl;
        exit(0);
    }

    if(type_instr == 0) {
        instr = new Instruction(label, Token::tokenToIType(token_type));
    } else if (type_instr == 1) {
        instr = new Instruction(label, Token::tokenToIType(token_type), argument_int);
    } else if (type_instr == 2) {
        instr = new Instruction(label, Token::tokenToIType(token_type), jmp_label);
    } else {
        std::cout << "Error: Unable to recognize the instruction type" << std::endl;
        exit(0);
    }

    return instr;
}

Parser::Parser(Scanner* scanner): scanner(scanner) { previous = current = nullptr; };

SVM* Parser::parse() {
    current = scanner->next_token();

    if (this->check(Token::ERR)) {
        std::cout << "Error: Invalid character" << std::endl;
        exit(0);
    }

    Instruction* instr = nullptr;
    std::list<Instruction*> sl;

    while (current->type != Token::END) {
        instr = this->parseInstruction();
        sl.push_back(instr);
    }

    if (current->type != Token::END) {
        std::cout << "Error: Expected end of input, found: " << *current << std::endl;
    }
    if (current) delete current;

    return new SVM(sl);
}

#endif // Syntax_Analysis_PARSER_H
