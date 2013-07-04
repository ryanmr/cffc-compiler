#ifndef SCANNER_H
#define SCANNER_H

#include <regex.h>
#include "regex.h"
#include <string>
#include <map>
#include <vector>

class Token;

/* This enumerated type is used to keep track of what kind of
   construct was matched. 
*/
enum tokenEnumType { 

    // Keywords
    nameKwd, platformKwd, initialKwd, stateKwd, 
    gotoKwd, whenKwd, performingKwd, exitKwd,

    intKwd, floatKwd, booleanKwd, stringKwd, charKwd,

    trueKwd, falseKwd,

    // Constants
    intConst, floatConst, stringConst, charConst,

    // Names ; 19
    variableName ,

    // Punctuation
    leftParen, rightParen, 
    leftCurly, rightCurly, // 24

    leftAngle, rightAngle,
    colon, comma, semiColon, // 28
    assign, // 29

    plusSign, star, dash, forwardSlash,
    equalsEquals, lessThanEquals, greaterThanEquals, notEquals, // 37

    // Special terminal types
    endOfFile , // 38
    lexicalError
};
typedef enum tokenEnumType tokenType;


class Token {
    public:
        Token();
        Token(const char*, tokenType, Token*);
        ~Token();

        tokenType terminal;
        std::string lexeme;
        Token *next;
};

class Scanner {
    public:
        std::map<int, regex_t*> expressions;
        std::vector<tokenType> order;
        std::vector<regex_t *> ignore_expressions;

        Scanner();
        ~Scanner();
        Token *scan(const char*);
        int _consume(const char*);
};


#endif /* SCANNER_H */
