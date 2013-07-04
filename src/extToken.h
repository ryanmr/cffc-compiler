/* 
    ExtToken: an extension of the Token class with new methods for parsing 
    (led, nud, lbp) and describing the token.

    Author: EVW
*/

#ifndef EXTTOKEN_H
#define EXTTOKEN_H

#include "scanner.h"
#include "parser.h"

class ExtToken {
public:
    ExtToken (Parser *p, Token *t) 
        : lexeme(t->lexeme), terminal(t->terminal), parser(p) { }
    ExtToken (Parser *p, Token *t, std::string d) 
        : lexeme(t->lexeme), terminal(t->terminal), parser(p), descStr(d) { }

    virtual ~ExtToken () { } ;

    virtual ParseResult nud () {
        throw ( parser->makeErrorMsg (parser->currToken->terminal) ) ;
    }

    virtual ParseResult led (ParseResult left) {
        throw ( parser->makeErrorMsg (parser->currToken->terminal) ) ;
    }
    std::string lexeme ;
    tokenType terminal ;
    ExtToken *next ;
    Parser *parser;

    virtual int lbp() { return 0 ; }
    virtual std::string description() { return descStr ; }

private:
    ExtToken () : parser(NULL) { } 
    std::string descStr ;
} ;

ExtToken *extendToken (Parser *p, Token *tokens) ;
ExtToken *extendTokenList (Parser *p, Token *tokens) ;

/* For each terminal symbol that will play some unique role in the
   semantic analysis of the program, we need a unique subclass of
   ExtToken.
 */

// True Kwd
class TrueKwdToken : public ExtToken {
public:
    TrueKwdToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseTrueKwd (); }
    std::string description() { return "true const"; }
} ;

// False Kwd
class FalseKwdToken : public ExtToken {
public:
    FalseKwdToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseFalseKwd (); }
    std::string description() { return "false const"; }
} ;

// Int Const
class IntConstToken : public ExtToken {
public:
    IntConstToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseIntConst (); }
    std::string description() { return "int const"; }
} ;

// Float Const
class FloatConstToken : public ExtToken {
public:
    FloatConstToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseFloatConst (); }
    std::string description() { return "float const"; }
} ;

// String Const
class StringConstToken : public ExtToken {
public:
    StringConstToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseStringConst (); }
    std::string description() { return "string const"; }
} ;

// Char Const
class CharConstToken : public ExtToken {
public:
    CharConstToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseCharConst (); }
    std::string description() { return "char const"; }
} ;

// Variable Name
class VariableNameToken : public ExtToken {
public:
    VariableNameToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseVariableName (); }
    std::string description() { return "variable name"; }
} ;


// Left Paren
class LeftParenToken : public ExtToken {
public:
    LeftParenToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseNestedExpr () ; }
    std::string description() { return "'('"; }
    int lbp() { return 80; }
} ;

// Plus Sign
class PlusSignToken : public ExtToken {
public:
    PlusSignToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult led (ParseResult left) {
        return parser->parseAddition (left) ; 
    }
    std::string description() { return "'+'"; }
    int lbp() { return 50; }
} ;

// Star
class StarToken : public ExtToken {
public:
    StarToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult led (ParseResult left) {
        return parser->parseMultiplication (left) ; 
    }
    std::string description() { return "'*'"; }
    int lbp() { return 60; }
} ;

// Dash
class DashToken : public ExtToken {
public:
    DashToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult led (ParseResult left) {
        return parser->parseSubtraction (left) ; 
    }
    std::string description() { return "'-'"; }
    int lbp() { return 50; }
} ;

// ForwardSlash
class ForwardSlashToken : public ExtToken {
public:
    ForwardSlashToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult led (ParseResult left) {
        return parser->parseDivision (left) ; 
    }
    std::string description() { return "/"; }
    int lbp() { return 60; }
} ;

// Relational Op
class RelationalOpToken : public ExtToken {
public:
    RelationalOpToken (Parser *p, Token *t, std::string d) : ExtToken(p,t,d) { }
    ParseResult led (ParseResult left) {
        return parser->parseRelationalExpr (left) ; 
    }
    int lbp() { return 30; }
} ;


// End of File
class EndOfFileToken : public ExtToken {
public:
    EndOfFileToken (Parser *p, Token *t) : ExtToken(p,t) { }
    std::string description() { return "end of file"; }
} ;


#endif /* EXTTOKEN_H */
