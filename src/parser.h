/*
    parser.h
    A recursive descent parser with operator precedence.

    Mostly written by Professor EVW with patterns by
        Douglas Crockford's chapter is available at 
        http://javascript.crockford.com/tdop/tdop.html

        Vaughan Pratt's paper is available at 
        http://portal.acm.org/citation.cfm?id=512931
    And adapter for CFF by Ryan Rampersad.
*/

#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "parseResult.h"
#include "ast.h"

#include <string>
#include <iostream>

class ExtToken ;

class Parser {

public:
    Parser();

    ParseResult parse (const char *text);
    ParseResult prank (const char *text, std::string type);

    // Parser methods for the nonterminals:
    // Program, Platform, Decls, Decl, States, State,
    // Transitions, Transition, Stmts, Stmt, Expr

    ParseResult parseProgram ();
    ParseResult parsePlatform ();

    /*
        parseDecls in its overloaded form
        handles the list form of a DeclList.
    */
    void parseDecls (SeqDecl *t);
    ParseResult parseDecls ();
    ParseResult parseDecl ();

    ParseResult parseType ();

    /*
        parseStates in its overloaded form
        handles the list form of State.
    */
    void parseStates (State *t);
    ParseResult parseStates ();
    ParseResult parseState ();

    /*
        parseTransitions in its overloaded form
        handles the list form of Transition.
    */
    void parseTransitions (Transition *t);
    ParseResult parseTransitions ();
    ParseResult parseTransition ();

    /*
        parseStmts in its overloaded form
        handles the list form of Stmt.
    */
    void parseStmts (Stmt *t);
    ParseResult parseStmts ();
    ParseResult parseStmt ();

    ParseResult parseExpr (int rbp);

    // methods for parsing productions for Expr
    ParseResult parseTrueKwd ();
    ParseResult parseFalseKwd ();
    ParseResult parseIntConst ();
    ParseResult parseFloatConst ();
    ParseResult parseStringConst ();
    ParseResult parseCharConst ();
    ParseResult parseVariableName ();

    ParseResult parseNestedExpr ();

    ParseResult parseAddition ( ParseResult left );
    ParseResult parseMultiplication ( ParseResult left );
    ParseResult parseSubtraction ( ParseResult left );
    ParseResult parseDivision ( ParseResult left );

    ParseResult parseRelationalExpr ( ParseResult left );

    // Helper function used by the parser.
    void match (tokenType tt);
    bool attemptMatch (tokenType tt);
    bool nextIs (tokenType tt);
    void nextToken ();

    std::string terminalDescription ( tokenType terminal );
    std::string makeErrorMsg ( tokenType terminal );
    std::string makeErrorMsgExpected ( tokenType terminal );
    std::string makeErrorMsg ( const char *msg );

    ExtToken *tokens;
    ExtToken *currToken;
    ExtToken *prevToken;

};

template <class J>
void node_expected(Node *node, std::string error) {
    if ( false == is_node_type<J>(node) ) {
        std::cout << "error: " << error << std::endl;
        throw ( (std::string) error );
    }
}
template <class K>
void node_expected(ParseResult & pr, std::string error) {
    //node_expected<K>(pr.ast, error);
}

#endif /* PARSER_H */
