#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <cassert>
#include "../src/ascii.h"
#define MAXTOK 256 /* maximum token size */
int cur;    /* current character being processed */
int peek;   /* next charcter to be processed */
enum State{
    START = 0,
    A = 1,
    B = 2,
    C = 3,
    D = 4,
    E = 5,
    F = 6,
    REJECT = EOF,
};
void advance(){
    cur = peek;
    if (peek != EOF)
        peek = std::fgetc(stdin);
}
class TokenNfa{
public:
    virtual bool is_accept_state(State state);
    virtual State move(State, int);
};
class CharToken : TokenNfa{
public:
    bool is_accept_state(State state){
        return state == State::B || state == State::D || state == State::E;
    }
    State move(State s, int i, bool str){
        if(s == START)
            return A;
        switch(s){
            case A:
                if(i == BACK) return C;
                else if(is_schar(i) || (!str && i == DITTO) || (str && i == QUOTE)) return B;
                break;
            case B:
                break;
            case C:
                if(i == BACK || is_schar(i) || i == DITTO || i == QUOTE) return B;
                else if(isoctal(i)) return D;
                break;
            case D:
                if(isoctal(i)) return E;
                break;
            case E:
                if(isoctal(i)) return E;
                break;
            default:
                break;
        }
        return REJECT;
    }
};
bool id_char_token(){
    if(cur == QUOTE){
        CharToken nfa;
        State state = START;
        advance();
        while(cur != QUOTE && cur != EOF && state != REJECT){
            state = nfa.move(state, cur);
            advance();
        }
        return nfa.is_accept_state(state) && cur == QUOTE;
    }
    return false;
}
/* Find a token and store the chars into lexeme buffer */
int scan(char *lexeme) {
    int i = 0;
    /* skip over whitespaces and check for EOF */
    return 0;
}

int main(int argc, char *argv[])
{
    char lexeme[MAXTOK];
    int  result;
    
    /* setup for scanning */
    cur = peek = std::fgetc(stdin);
    if (cur != EOF)
        peek = std::fgetc(stdin);

    printf("Is char token: %i\n", id_char_token());
    return 0;
}

/*
            "--" 
            "->"
            "-" 
            "-="
            
            "^"     "%"     "*"     "/"     "="     "!" 
            "^="    "%="    "*="    "/="    "=="    "!="

            ">"     "<"  
            ">="    "<=" 
            ">>"    "<<" 
            ">>="   "<<="           
         *