/*
 *  STUDENT C++ TEMPLATE FOR CSCAN PROJECT
 */

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <cassert>

#define MAXTOK      256 /* maximum token size */

enum Ascii{
    ASCII_NULL     = 0,
    BACKSPACE      = 8,
    BACK           = 92,
    DITTO          = 34,
    QUOTE          = 39,
    ASTERICK       = 42,
    LF             = 10,
    CR             = 13,
    TAB            =  9,
    SPACE          = 32,
    FORWARD  
};

int cur;    /* current character being processed */
int peek;   /* next charcter to be processed */

int skip() {
    /* SKIP OVER WHITESPACEs
     * IT IS A TEMPLATE. 
     * NOTE:: YOU NEED TO IMPLEMENT 
     */
  while (isspace(cur)) {
    cur = peek;
    peek = std::fgetc(stdin);
  }
  return cur;
}
/* Find a token and store the chars into lexeme buffer */
int scan(char *lexeme) {
    int i = 0;
    /* skip over whitespaces and check for EOF */
    if (skip() == EOF)
        return EOF;
    else if (isalpha(cur) || cur == '_') {
        // ID TOKEN 
        while (isalpha(cur) || isdigit(cur) || cur == '_') {
            lexeme[i++] = cur;
            cur = peek;
            if (peek != EOF)
                peek = std::fgetc(stdin);

        }
	    lexeme[i] = '\0';
        return i; // return any value other than EOF
    }
    else {
        // YOU NEED TO IMEPLEMENT THE REST
        if(cur == QUOTE){
            while(cur != QUOTE){
                lexeme[i++] = cur;
                cur = peek;
                if (peek != EOF)
                    peek = std::fgetc(stdin);
            }
        }
        else{
            lexeme[i++] = cur;
            cur = peek;
            if (peek != EOF)
                peek = std::fgetc(stdin);
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    char lexeme[MAXTOK];
    int  result;
    char x = '&&';
    /* setup for scanning */
    cur = peek = std::fgetc(stdin);
    if (cur != EOF)
        peek = std::fgetc(stdin);

    while ((result = scan(lexeme)) != EOF) {
        std::cout << lexeme << std::endl;
    }
    return 0;
}
