/************************************************
 * Name: Cody Carmona
 * Class: COP4020
 * Assignment: Proj 1 (Implementing a C scanner) 
 * Compile: "gcc -g -o cscan.exe cscan.cpp"	 
 ************************************************/
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <cassert>
#include "asciiautomata.h"
#include <map>

#define MAXTOK 256 /* maximum token size */

int cur;  /* current character being processed */
int peek; /* next charcter to be processed */
std::map<std::string, int> tokens;

void advance_char_itr()
{
    cur = peek;
    if (peek != EOF)
        peek = std::fgetc(stdin);
}
void skip_comment()
{
    if (is_start_of_comment(cur, peek))
        return;

    bool block_comment = cur == peek;
    while (cur != EOF && peek)
    {
        advance_char_itr();
        if (peek == LF && !block_comment ||
            peek == FORWARD && block_comment && cur == ASTERICK)
        {
            peek = std::fgetc(stdin);
            advance_char_itr();
            return;
        }
    }
}
void skip_space()
{
    while (is_skippable_between_tokens(cur))
        advance_char_itr();
}
/**
 * Blanks, tabs, newlines, and comments are ignored
 *  between tokens
 *  - Blanks   '\0'
 *  - Tabs     '\t'
 *  - Newlines '\n'
 *  - Comment lines 
 *  - Comment blocks
 * @return end char after skip is finished or EOF
 */
int skip()
{
    /* SKIP OVER WHITESPACEs
     * IT IS A TEMPLATE. 
     * NOTE:: YOU NEED TO IMPLEMENT 
     */
    while (is_skippable_between_tokens(cur))
        advance_char_itr();
    return cur;
}

void add_token(std::string key)
{
    auto itr = tokens.find(key);
    if (itr != tokens.end())
        itr->second++;
    else
        tokens.insert(std::pair<std::string, int>{key, 1});
}
int scan(char *lexeme)
{
    int i = 0;

    /* skip over whitespaces and check for EOF */
    if (skip() == EOF)
        return EOF;

    // indent
    //alpha (alpha | digit)*
    else if (isalpha(cur) || cur == '_')
    {
        // ID TOKEN
        while (isalpha(cur) || isdigit(cur) || cur == '_')
        {
            lexeme[i++] = cur;
            advance_char_itr();
        }
        lexeme[i] = '\0';
        return i; // return any value other than EOF
    }
    else
    {
        if (isdigit(cur)){
            while(isdigit(cur)){
                lexeme[i++] = cur;
                advance_char_itr();
            }
            lexeme[i] = '\0';
        }
        else if (cur == DITTO)
        {
            while()
            lexeme[i++] = cur;
            advance_char_itr();
            do
            {
                lexeme[i++] = cur;
                advance_char_itr();
            } while (CharAutomata::is_in_alphabet(cur));
            lexeme[i++] = cur;
            return i;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    char lexeme[MAXTOK];
    int result;
    char input;

    /* setup for scanning */
    cur = peek = std::fgetc(stdin);

    if (cur != EOF)
        peek = std::fgetc(stdin);

    while ((result = scan(lexeme)) != EOF)
    {
        std::cout << lexeme << std::endl;
    }

    return 0;
}

/* Find a token and store the chars into lexeme buffer */
/*
    token   = digit+                                                    #number
    token   = alpha (alpha | digit)*                                    #indent
    token   = quote char quote                                          #char
    token   = ditto str+ ditto                                          #string
    token   = | "(" | ")" | "," | "." | ":" | ";" | "?" | "[" | "]"
                | "{" | "}" | "~” | "&&" | "||" | "++" | "--" | "->"
                | ("|" | "ˆ" | "&" | "+" | "-" | "%" | "*" | "/" |
                "=" | "!" | ">" | ">>" | "<" | "<<") ("=" | null)
    quote   = "’"
    ditto   = """
    back    = "\"
    octal   = "0" | "1" | ... | "7"
    octch   = back octal (octal | null) (octal | null)
    digit   = "0" | "1" | ... | "9"
    alpha   = "a" | ... | "z" | "A" | ... | "Z" | "_"
    schar   = any ascii character except quote, ditto,carriage return and back
    char    = ((back | null) (schar | ditto)) | back back| back quote | octch
    
*/
