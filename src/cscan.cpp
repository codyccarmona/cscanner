/*
 *  STUDENT C++ TEMPLATE FOR CSCAN PROJECT
 */

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <cassert>
#include <cstring>
#include <algorithm>
#include <vector>
#include "ascii.h"

#define MAXTOK 256      /* maximum token size */
#define ident_key "ident"
#define str_key "string"
#define char_key "char"
#define digit_key "digit"

int cur;                                                    /* current character being processed */
int peek;                                                   /* next charcter to be processed */
int line_number = 1;                                        /* hold current line number */
std::vector<std::pair<std::string, int>> token_count;       /* hold token counts  using key */
struct map_sort_comp{                                       /* used to quickly sort map for print */
    template<typename T>
    bool operator()(const T &l, const T &r) const
    {
        if (l.second != r.second) {
            return l.second > r.second;
        }

        return l.first > r.first;
    }
};
enum State{                             /* arbitrary values to represent states of nfa*/
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
    switch(s){
        case A:
            if(i == BACK) return C;
            else if((is_schar(i) && i != LF) || (!str && i == DITTO) || (str && i == QUOTE)) return B;
            break;
        case B:
            break;
        case C:
            if(i == BACK || is_schar(i) || i == DITTO || i == QUOTE || (str && i == LF)) return B;
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
bool sortvec(const std::pair<std::string, int> &l, const std::pair<std::string, int> &r){
    if(l.second != r.second)
        return l.second > r.second;
    else{
        if(l.first.size() == r.first.size())
            return l.first < r.first;
        return l.first.size() > r.first.size();
    }
}
/* Checks if set of states is accept state for nfa */
bool is_accept_state(State state){
    return state == State::B || state == State::D || state == State::E;
}

/* Advances to next byte from stdin */
void advance(int num_advances = 1){
    while(num_advances > 0){
        if(cur == LF)
            line_number++;
        cur = peek;
        if (peek != EOF)
            peek = std::fgetc(stdin);
        num_advances--;
    }
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
            while((cur != ASTERICK || cur == ASTERICK && peek != FORWARD) && cur != EOF)
                advance();
            if(cur == ASTERICK && peek == FORWARD)
                advance(2);
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

/* uses struct to create set obj with sorted keys and counts, then prints them */
void print_token_summary(){
    std::sort(token_count.begin(), token_count.end(), sortvec);
    //std::set<std::pair<std::string, int>, map_sort_comp> sorted_token_count(token_count.begin(), token_count.end());
    printf("\n%13s%15s", "token", "count\n");
    printf("--------------------- -----\n");
    for(auto itr = token_count.begin(); itr != token_count.end(); itr++){     
        printf("%21s %5d\n", itr->first.begin(), itr->second);
    }
}

/* called by scan when it encounters valid token */
void record_token(std::string key){
    auto itr = token_count.begin();
    for(; itr != token_count.end(); itr++){
        if(itr->first == key){
            itr->second++;
            break;
        }           
    }
    if(itr == token_count.end())
        token_count.push_back(std::pair<std::string, int>(key, 1));
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
        record_token(ident_key);
	    lexeme[i] = '\0';
        return i; // return any value other than EOF
    }
    else{
        /* char array token */
        if(cur == QUOTE || cur == DITTO){
            State state = A;
            lexeme[i++] = cur;
            advance();
            /* Check for char token */
            if(cur == QUOTE){
                while(cur != QUOTE && cur != EOF && state != REJECT){
                    lexeme[i++] = cur;
                    state = move(state, cur, false);
                    advance();
                }
                if(cur == QUOTE){
                    record_token(char_key);
                    lexeme[i++] = cur;
                    advance();
                }
                else{
                    std::fprintf(stderr ,"missing %c for %s on line %i", QUOTE, lexeme, line_number);
                    lexeme[0] = 0;
                }
            }
            /* Check for str token */
            else{
                while((cur != DITTO || cur == DITTO &&  lexeme[i - 1] == BACK) && cur != EOF && state != REJECT){
                    state = move(state, cur, true); 
                    lexeme[i++] = cur;
                    advance();               
                    if(state == B)
                        state = A;
                }
                if(cur == DITTO){            
                    record_token(str_key);
                    lexeme[i++] = cur;
                    advance();
                }
                else{ 
                    //missing " for "This is a test with conton line 9
                    std::fprintf(stderr ,"missing %c for %s on line %i\n", DITTO, lexeme, (lexeme[i - 1] == LF ? line_number - 1 : line_number));
                    lexeme[0] = 0;
                }                
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
            record_token(digit_key);
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
            record_token(std::string(lexeme, i));  
            lexeme[i] = '\0';       
            return i;
        }
        else{
            printf("illegal characer: %c on line %i", cur, line_number);
            lexeme[0] = 0;
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
    print_token_summary();
    return 0;
}
