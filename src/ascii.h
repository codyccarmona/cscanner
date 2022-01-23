#include <ctype.h>
#include <cstddef>

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
    FORWARD        = 47,
    ZERO           = 48,
    SEVEN          = 55
};
bool iscomment(int i, int j){
    return i == BACK && j == BACK || j == ASTERICK;
}
bool is_skippable_between_tokens(int i){
    return i == SPACE || i == TAB || i == LF || i == ASCII_NULL; 
}
// Read char will be casted to ascii char. Must be between 48 and 55 ascii to be octal
bool isoctal(int i) {
    return i >= ZERO && i <= SEVEN;
}
//schar = any ascii character except quote, ditto, carriage return and back
bool is_schar(int i) {
    return isascii(i) && i != QUOTE && i != DITTO && i != CR && i != BACK;
}
/*
| "(" | ")" | "," | "." | ":" | ";" | "?" | "[" | "]"

| "{" | "}" | "~” | "&&" | "||" | "++" | "--" | "->"
*/
bool istoken(int i, int j){
    
}