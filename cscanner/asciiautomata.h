#include <ctype.h>
#include <cstddef>

#define INDENTKEY "indent"
#define CHARKEY "char"
#define STRING "string"
#define NUMKEY "number"

enum Asciiiues{
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
    FORWARD        = 47
};
bool is_start_of_comment(int first, int second){
    return first == FORWARD && second == FORWARD || second == ASTERICK;
}
bool is_skippable_between_tokens(int i){
    return i == SPACE || i == TAB || i == LF || i == ASCII_NULL; 
}
// Read char will be casted to ascii char. Must be between 48 and 55 ascii to be octal
bool isoctal(int i) {
    return i >= 48 && i <= 55;
}
//schar = any ascii character except quote, ditto, carriage return and back
bool is_schar(int i) {
    return isascii(i) && i != QUOTE || i != DITTO || i != DITTO && i != CR;
}
struct StrAutomata{
    static bool is_in_alphabet(int c){
        if(isdigit(c))
            return isoctal(c);
        else if(is_schar(c))
            return true;
        else{
            return c == BACK || c == QUOTE || c == DITTO;
        }
    }
};
struct CharAutomata{
/**
 * Char grammar has terminals:
 * - Ditto
 * - Back
 * - Quote
 * - Digits within 0-7
 * - Null
 * - If it is none of these, then it has to be a schar
 * @return if char is in grammer
 */
    static bool is_in_alphabet(int c){
        if(isdigit(c))
            return isoctal(c);
        else
            return is_schar(c) || c == BACK || c == DITTO || c == QUOTE;
        
        return false;
    }
};