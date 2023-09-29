#ifndef Syntax_Analysis_TOKEN_H
#define Syntax_Analysis_TOKEN_H

#include "instruction.h"

struct Token {
    enum Type { ID=0, LABEL, NUM, EOL, ERR, END, 
                PUSH, JMPEQ, JMPGT, JMPGE, JMPLT, JMPLE, 
                GOTO, SKIP, POP, DUP, SWAP, 
                ADD, SUB, MUL, DIV, STORE, LOAD, PRINT };
    static const char* token_names[24];

    Type type;
    std::string lexeme;

    Token(Type);
    Token(Type, char);
    Token(Type, const std::string&);
    static Instruction::IType tokenToIType(Type token_type);
};

const char* Token::token_names[24] = { "ID", "LABEL", "NUM", "EOL", "ERR", "END", 
                                       "PUSH", "JMEPEQ", "JMPGT", "JMPGE", "JMPLT", "JMPLE", 
                                       "GOTO", "SKIP", "POP", "DUP", "SWAP", 
                                       "ADD", "SUB", "MUL", "DIV", "STORE", "LOAD", "PRINT" };

Token::Token(Type t): type(t) { lexeme = ""; }
Token::Token(Type t, char c): type(t) { lexeme = c; }
Token::Token(Type t, const std::string& source): type(t) { lexeme = source; }

std::ostream& operator<<(std::ostream& stream, const Token& token) {
    stream << Token::token_names[token.type];
    if (!token.lexeme.empty()) stream << "(" << token.lexeme << ")";
    
    return stream;
}

Instruction::IType Token::tokenToIType(Token::Type token_type) {
    Instruction::IType itype;
    switch (token_type) {
    case (Token::PUSH): itype = Instruction::IPUSH; break;
    case (Token::POP): itype = Instruction::IPOP; break;
    case (Token::DUP): itype = Instruction::IDUP; break;
    case (Token::SWAP): itype = Instruction::ISWAP; break;
    case (Token::ADD): itype = Instruction::IADD; break;
    case (Token::SUB): itype = Instruction::ISUB; break;
    case (Token::MUL): itype = Instruction::IMUL; break;
    case (Token::DIV): itype = Instruction::IDIV; break;
    case (Token::GOTO): itype = Instruction::IGOTO; break;
    case (Token::JMPEQ): itype = Instruction::IJMPEQ; break;
    case (Token::JMPGT): itype = Instruction::IJMPGT; break;
    case (Token::JMPGE): itype = Instruction::IJMPGE; break;
    case (Token::JMPLT): itype = Instruction::IJMPLT; break;
    case (Token::JMPLE): itype = Instruction::IJMPLE; break;
    case (Token::SKIP): itype = Instruction::ISKIP; break;
    case (Token::STORE): itype = Instruction::ISTORE; break;
    case (Token::LOAD): itype = Instruction::ILOAD; break;
    case (Token::PRINT): itype = Instruction::IPRINT; break;
    default:
        std::cout << "Error: Unknown Keyword type" << std::endl;
        exit(0);
    }
    
    return itype;
}

#endif // Syntax_Analysis_TOKEN_H
