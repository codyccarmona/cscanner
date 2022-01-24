/*
 *  STUDENT C++ TEMPLATE FOR CSCAN PROJECT
 */

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <cassert>
#include <map>
#include "ascii.h"

#define MAXTOK 256      /* maximum token size */

int cur;                /* current character being processed */
int peek;               /* next charcter to be processed */
enum State{             /* arbitrary values to represent states of nfa*/
    START = 0,
    A = 1,
    B = 2,
    C = 3,
    D = 4,
    E = 5,
    F = 6,
    REJECT = EOF,
};

/**
 * @param State current state of NFA (only ints defined in State enum handled)
 * @param int   current value to check against transition functions
 * @param str   whether NFA is for string or character token. this only changes
 *                  a check in one state, otherwise NFA is the same
 * @return      resulting state
 */
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
            if(isoctal(i)) return B;
            break;
        default:
            break;
    }
    return REJECT;
}

/* Checks if set of states is accept state for nfa */
bool is_accept_state(State state){
    return state == State::B || state == State::D || state == State::E;
}

/* Advances to next byte from stdin */
void advance(){
    cur = peek;
    if (peek != EOF)
        peek = std::fgetc(stdin);
}

/* Returns true is i is ascii value of skippalble character */
bool is_skippable(int i){
    return isspace(i) && (i == LF || i == TAB || i == SPACE);
}

// Blanks, tabs, newlines, and comments (enclosed between */, /*, //) are ignored between tokens */
int skip() {
    if(is_skippable(cur)){
        while(is_skippable(cur))
            advance();
    }
    if(iscomment(cur, peek)){
        if(peek == FORWARD){
            // Comment line
            while(cur != LF && cur != EOF && cur != BACK)
                advance();
            if(cur == LF)
                advance();
        }
        else{
            // Comment block
            int prev = EOF;
            while((cur != FORWARD || cur == FORWARD && prev != ASTERICK) && cur != EOF){
                prev = cur;
                advance();
            }
            if(cur == FORWARD)
                advance();
        }
  }
  if(iscomment(cur, peek) || is_skippable(cur))
    skip();
  return cur;
}

/* Returns true if int is ascii value of one character tokens defined in requirements */
bool is_single_char_token(int i){
    return i == '(' || i == ')' || i == ',' ||
           i == '.' || i == ':' || i == ';' ||
           i == '?' || i == '[' || i == ']' ||      
           i == '{' || i == '}' || i == '~';
}

/* Returns true if int matches a character that, when repeated, is a valid token */
bool is_repeatable_operator(int i){
    return i == '&' || i == '|' || i == '+' || i == '-' || i == '=' || i == '<' || i == '>';
}

/* Returns true if int matches ascii value of operator */
bool isoperator(int i){
    return i == '&' || i == '|' || i == '+' ||
           i == '^' || i == '%' || i == '*' || 
           i == '/' || i == '=' || i == '!' || 
           i == '>' || i == '<';
}

void print(std::map<int, int> tokens){
    for(auto itr = tokens.begin(); itr != tokens.end(); itr++){
        printf("token count\n");
        printf("-------------------------- -----\n");
        printf("%21s %5d\n", itr->first, itr->second);
    }
}

/* Find a token and store the chars into lexeme buffer */
int scan(char *lexeme) {
    int i = 0;

    /* skip over whitespaces, as well as comments and check for EOF */
    if (skip() == EOF)
        return EOF;
   
    else if (isalpha(cur) || cur == '_') {
        // ID TOKEN 
        while (isalpha(cur) || isdigit(cur) || cur == '_') {
            lexeme[i++] = cur;
            advance();
        }
	    lexeme[i] = '\0';
        return i; // return any value other than EOF
    }
    else{
        if(cur == QUOTE || cur == DITTO){
            State state = START;
            lexeme[i++] = cur;
            advance();
            /* Check for char token */
            if(cur == QUOTE){
                while(cur != QUOTE && cur != EOF && state != REJECT){
                    lexeme[i++] = cur;
                    state = move(state, cur, false);
                    advance();
                }
                lexeme[i++] = cur;
                advance();
            }
            /* Check for str token */
            else{
                while(cur != DITTO && cur != EOF && state != REJECT){
                    if(state == B)
                        state = START;                       
                    lexeme[i++] = cur;
                    state = move(state, cur, false);
                    advance();
                    if((cur == BACK && peek == DITTO) || cur == LF)
                        advance();
                }
                lexeme[i++] = cur;
                advance();
            }
            lexeme[i] = '\0';
            return i;
        }
        /* Grab digit ident */
        else if(isdigit(cur)){
            while(isdigit(cur)){
                lexeme[i++] = cur;
                advance();
            }
            lexeme[i] = '\0';
            return i;
        }
        
        /* Checks for all tokens that are not char, number, ident, or string */
        else if(isoperator(cur) || is_single_char_token(cur)){
            /* Matches token such as *=, or tokens such as == ++ --, or token ->, or single character token  */
            if((peek == '=' && !is_single_char_token(cur)) || (is_repeatable_operator(cur) && cur == peek) || (cur == '-' && peek == '>')){
                while(i < 2){
                    lexeme[i++] = cur;
                    advance();
                }
                /* If we found << or >>, check if we can grab >>= <<= */
                if((lexeme[0] == '>' || lexeme[0] == '<') && lexeme[0] == lexeme[1] && cur == '='){
                    lexeme[i++] = cur;
                    advance();
                }
            }
            /* Only grab operator as token or single char token that is not operator */
            else{
                lexeme[i++] = cur;
                advance();
            }
            lexeme[i] = '\0';
            return i;
        }
        else{
            printf("Unrecognized character: %i\n", cur);
            advance();
        }
    }
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

    while ((result = scan(lexeme)) != EOF) {
        std::cout << lexeme << std::endl;
    }
    return 0;
}
