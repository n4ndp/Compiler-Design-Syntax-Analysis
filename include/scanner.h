#ifndef Syntax_Analysis_SCANNER_H
#define Syntax_Analysis_SCANNER_H

#include "token.h"

class Scanner {
private:
    std::string input;
    int first, current, state;
    std::unordered_map<std::string, Token::Type> reserved;

public:
    Scanner(const std::string&);
    Token* next_token();

private:
    char next_char();
    void roll_back();
    void start_lexeme();
    void incr_start_lexeme();
    std::string get_lexeme();
    Token::Type check_reserved(const std::string&);
};

char Scanner::next_char() {
    int c = input[current];
    current++;
    return c;
}

void Scanner::roll_back() { current--; }
void Scanner::start_lexeme() { first = current - 1; }
void Scanner::incr_start_lexeme() { first++; }
std::string Scanner::get_lexeme() { return input.substr(first, current - first); }

Token::Type Scanner::check_reserved(const std::string& lexeme) { 
    std::unordered_map<std::string, Token::Type>::const_iterator it = reserved.find(lexeme);
    return it == reserved.end() ? Token::ERR : it->second;
}

Scanner::Scanner(const std::string&  input): input(input), first(0), current(0) {
    reserved["push"] = Token::PUSH;
    reserved["jmpeq"] = Token::JMPEQ;
    reserved["jmpgt"] = Token::JMPGT;
    reserved["jmpge"] = Token::JMPGE;
    reserved["jmplt"] = Token::JMPLT;
    reserved["jmple"] = Token::JMPLE;
    reserved["goto"] = Token::GOTO;
    reserved["skip"] = Token::SKIP;
    reserved["pop"] = Token::POP;
    reserved["dup"] = Token::DUP;
    reserved["swap"] = Token::SWAP;
    reserved["add"] = Token::ADD;   
    reserved["sub"] = Token::SUB;
    reserved["mul"] = Token::MUL;
    reserved["div"] = Token::DIV;
    reserved["store"] = Token::STORE;
    reserved["load"] = Token::LOAD;
    reserved["print"] = Token::PRINT;
}

Token* Scanner::next_token() {
    Token* token;
    char c = next_char();

    while (c == ' ') c = this->next_char();
    if (c == '\0') return new Token(Token::END);
    if (c == '%') {
        while (c != '\n' && c != '\0') c = this->next_char();
        if (c == '\n') return this->next_token();
    }

    this->start_lexeme();
    state = 0;

    while (true) {
        switch (state) {
            case 0:
                if (isalpha(c)) state = 1;
                else if (isdigit(c)) state = 2;
                else if (c == '\n') state = 4;
                else return new Token(Token::ERR, c);
                break;
            case 1:
                while (isalpha(c) || isdigit(c) || c == '_') c = this->next_char();
                if (c == ':') {
                    state = 3;
                    break;
                }
                else {
                    this->roll_back();
                    std::string lexeme = this->get_lexeme();
                    Token::Type token_type = this->check_reserved(lexeme);

                    if (token_type != Token::ERR) return new Token(token_type);
                    else return new Token(Token::ID, this->get_lexeme());
                }
            case 2:
                this->start_lexeme();
                while (isdigit(c)) c = this->next_char();
                this->roll_back();

                return new Token(Token::NUM, this->get_lexeme());
            case 3:
                this->roll_back();
                token = new Token(Token::LABEL, this->get_lexeme());
                c = this->next_char();

                return token;
            case 4:
                while (c == '\n') c = this->next_char();
                this->roll_back();

                return new Token(Token::EOL);
            default:
                std::cout << "Programming Error ... quitting" << std::endl;
                exit(0);
        }
    }
}

#endif // Syntax_Analysis_SCANNER_H
