/*
    A recursive descent parser with operator precedence.
    Original author: Professor EVW
    Modified: Ryan Rampersad
*/

#include "parser.h"
#include "scanner.h"
#include "extToken.h"

#include "ast.h"

#include <assert.h>
#include <iostream>

using namespace std;

Parser::Parser ( ) { } 

ParseResult Parser::parse (const char *text) {
    assert (text != NULL);

    ParseResult pr;
    try {
        Scanner *s = new Scanner();
        tokens = extendTokenList ( this, s->scan (text) );

        assert(tokens != NULL);
        currToken = tokens;
        pr = parseProgram( );
        delete s;
    }
    catch (string errMsg) {
        pr.ok = false;
        pr.errors = errMsg;
        pr.ast = NULL;
    }
    return pr;
}

/*
    prank
    Prank is a testing function. It is based on the given parse function.
    Prank takes a char *text and string type.
    text is a scanner-able string
    type is which type of node should be parsed from it
    This was used for incremental testing. As each additional portion of the grammar
    was created, the large if-else was added to handle that portion.
*/

ParseResult Parser::prank(const char *text, std::string type) {
    assert(text != NULL);

    ParseResult pr;
    try {
        Scanner *s = new Scanner();
        tokens = extendTokenList ( this, s->scan (text) );

        assert (tokens != NULL);
        currToken = tokens;
        
        if ( type == "stmt" ) {
            pr = parseStmt();
        } else if ( type == "stmts" ) {
            pr = parseStmts();
        } else if ( type == "transition" ) {
            pr = parseTransition();
        } else if ( type == "transitions" ) {
            pr = parseTransitions();
        } else if ( type == "state" ) {
            pr = parseState();
        } else if ( type == "platform" ) {
            pr = parsePlatform();
        } else if ( type == "decl" ) {
            pr = parseDecl();
        } else if ( type == "program" ) {
            pr = parseProgram();
        } else {
            pr = parseExpr(0);
        }


    }
    catch (string errMsg) {
        pr.ok = false;
        pr.errors = errMsg;
        pr.ast = NULL;
    }
    return pr;

} 

/* 
 * parse methods for non-terminal symbols
 * --------------------------------------
 */


// Program
ParseResult Parser::parseProgram () {
    ParseResult program, variable, platform, decls, states;

    match(nameKwd);
    match(colon);

    variable = parseVariableName();
    
    node_expected<Variable>(variable, "Bad cast of Variable");

    match(semiColon);

    platform = parsePlatform();

    node_expected<Platform>(platform, "Bad cast of Platform");

    decls = parseDecls();

    node_expected<DeclList>(decls, "Bad cast of Decls");

    states = parseStates();

    node_expected<State>(states, "Bad cast of State");

    match(endOfFile);

    program.ast = new Program( (Variable *)variable.ast, (Platform *)platform.ast, (DeclList *)decls.ast, (State *)states.ast );

    return program;
}

// Platform
ParseResult Parser::parsePlatform () {
    ParseResult platform, variable;

    match(platformKwd);
    match(colon);

    variable = parseVariableName();

    node_expected<Variable>(variable, "Bad cast of Variable");

    match(semiColon);

    platform.ast = new Platform((Variable *)variable.ast);

    return platform;
}

// Decls
ParseResult Parser::parseDecls () {
    ParseResult list, next;

    if ( ! nextIs(initialKwd) && ! nextIs(stateKwd) ) {

        ParseResult d = parseDecl();

        node_expected<Decl>(d, "Bad cast of Decl");

        next = parseDecls();

        list.ast = new SeqDecl( (Decl *)d.ast, (DeclList *)next.ast );
        

    }
    else {

        list.ast = new NoDecl();

    }

    return list;
}

// Decl
ParseResult Parser::parseDecl () {
    ParseResult decl, type, variable;

    type = parseType();
        
    node_expected<Type>(type, "Bad cast of Type");

    variable = parseVariableName();

    node_expected<Variable>(variable, "Bad cast of Variable");

    match(semiColon);

    decl.ast = new Decl((Type *)type.ast, (Variable *)variable.ast);

    return decl;
}

// Type
ParseResult Parser::parseType () {
    ParseResult type;

    if ( attemptMatch(intKwd) ) {

        type.ast = new Type("int");

    } 
    else if ( attemptMatch(floatKwd) ) {

        type.ast = new Type("float");

    }
    else if ( attemptMatch(booleanKwd) ) {

        type.ast = new Type("boolean");

    }
    else if ( attemptMatch(stringKwd) ) {

        type.ast = new Type("string");

    }
    else {

        match(charKwd);

        type.ast = new Type("char");

    }
    return type;
}

ParseResult Parser::parseStates () {
    ParseResult state;

    if ( ! nextIs(endOfFile) ) {

        state = parseState();

        node_expected<State>(state, "Bad cast of State");

        parseStates( (State *)state.ast );

    } else {

        state.ast = new State();

    }
    return state;
}

void Parser::parseStates(State *last) {

    if ( ! nextIs(endOfFile) ) {

        ParseResult state;
        state = parseState();
        last->set_next( (State *)state.ast );
        parseStates( (State *)state.ast );

    }

}


// State
ParseResult Parser::parseState () {
    ParseResult state, transition, var;

    bool isInitial;

    isInitial = false;
    if ( attemptMatch(initialKwd) ) {
        isInitial = true;
    }

    match(stateKwd);
    match(colon);

    var = parseVariableName();

    node_expected<Variable>(var, "Bad cast of Variable");

    match(leftCurly);

    transition = parseTransitions();

    node_expected<Transition>(transition, "Bad cast of Transition");

    match(rightCurly);

    state.ast = new State((Variable *)var.ast, (Transition *)transition.ast, isInitial);

    return state;

}



// Transitions
void Parser::parseTransitions (Transition *last) {
    

    if ( ! nextIs(rightCurly) ) {

        ParseResult transition;
        transition = parseTransition();
        last->set_next((Transition *)transition.ast);
        parseTransitions( (Transition *)transition.ast );

    }

}

ParseResult Parser::parseTransitions() {
    ParseResult transition;

    if ( !nextIs(rightCurly) ) {

        transition = parseTransition();

        node_expected<Transition>(transition, "Bad cast of Transition");

        parseTransitions((Transition *)transition.ast);

    } else {

        transition.ast = new Transition();

    }

    return transition;
}


// Transition
ParseResult Parser::parseTransition () {
    ParseResult t, var, expr, stmt;

    if ( nextIs(gotoKwd) ) {

        match(gotoKwd);
        var = parseVariableName();
        match(whenKwd);
        expr = parseExpr(0);
        match(performingKwd);
        match(leftCurly);
        

        stmt = parseStmts();
        t.ast = new Transition((Variable *)var.ast, (Expr *)expr.ast, (Stmt *)stmt.ast);
        
        match(rightCurly);
        match(semiColon);

    } else {

        match(exitKwd);
        match(whenKwd);

        expr = parseExpr(0);
        
        match(performingKwd);
        match(leftCurly);
 
        stmt = parseStmts();       

        t.ast = new Transition((Expr *)expr.ast, (Stmt *)stmt.ast);

        match(rightCurly);
        match(semiColon);
    }

    return t;
}

// Stmts
void Parser::parseStmts (Stmt *last) {

    if ( ! nextIs(rightCurly) ) {

        ParseResult stmt;
        stmt = parseStmt();

        last->set_next( (Stmt *)stmt.ast );
        
        parseStmts( (Stmt *)stmt.ast );

    }    
}

ParseResult Parser::parseStmts() {
    ParseResult stmt;

    if ( ! nextIs(rightCurly) ) {

        stmt = parseStmt();
        parseStmts((Stmt *)stmt.ast);

    }
    else {

        stmt.ast = new Stmt();

    }
    return stmt;

}


// Stmt
ParseResult Parser::parseStmt () {
    ParseResult right, variable, stmt;

    variable = parseVariableName();

    match(assign);
    
    right = parseExpr(0);
    
    match(semiColon);

    stmt.ast = new Stmt((Variable *)variable.ast, (Expr *)right.ast);

    return stmt;

}

// Expr
ParseResult Parser::parseExpr (int rbp) {

    ParseResult left = currToken->nud();
   
    while (rbp < currToken->lbp() ) {
        left = currToken->led(left);
    }

    return left;

}


/*
 * parse methods for Expr productions
 * ----------------------------------
 */

// Expr ::= trueKwd
ParseResult Parser::parseTrueKwd ( ) {

    ParseResult pr;
    match ( trueKwd );
    pr.ast = new Bool(prevToken->lexeme);
    return pr;
    
}

// Expr ::= falseKwd
ParseResult Parser::parseFalseKwd ( ) {
    ParseResult pr;
    match ( falseKwd );
    pr.ast = new Bool(prevToken->lexeme);
    return pr;
}

// Expr ::= intConst
ParseResult Parser::parseIntConst ( ) {
    ParseResult pr;
    match ( intConst );
    pr.ast = new Integer(prevToken->lexeme);
    return pr;
}

// Expr ::= floatConst
ParseResult Parser::parseFloatConst ( ) {
    ParseResult pr;
    match ( floatConst );
    pr.ast = new Float(prevToken->lexeme);
    return pr;
}

// Expr ::= stringConst
ParseResult Parser::parseStringConst ( ) {
    ParseResult pr;
    match ( stringConst );
    pr.ast = new String(prevToken->lexeme);
    return pr;
}

// Expr ::= charConst
ParseResult Parser::parseCharConst ( ) {
    ParseResult pr;
    match ( charConst );
    pr.ast = new Char(prevToken->lexeme);
    return pr;
}

// Expr ::= variableName
ParseResult Parser::parseVariableName ( ) {
    ParseResult pr;
    match ( variableName );
    pr.ast = new Variable(prevToken->lexeme);
    return pr;
}


// Expr ::= leftParen Expr rightParen
ParseResult Parser::parseNestedExpr ( ) {
    ParseResult pr;
    match (leftParen);
    pr = parseExpr(0);
    match(rightParen);
    return pr;
}

// Expr ::= Expr plusSign Expr
ParseResult Parser::parseAddition ( ParseResult left ) {
    // parser has already matched left expression 
    ParseResult pr;
    ParseResult op;

    match ( plusSign );
    pr = parseExpr( prevToken->lbp() ); 

    Expr *l = (Expr *)left.ast;
    Expr *r = (Expr *)pr.ast;

    op.ast = new Add(l, r);

    return op;
}

// Expr ::= Expr star Expr
ParseResult Parser::parseMultiplication ( ParseResult left ) {
    // parser has already matched left expression 
    ParseResult pr;
    ParseResult op;

    match ( star );
    pr = parseExpr( prevToken->lbp() ); 

    Expr *l = (Expr *)left.ast;
    Expr *r = (Expr *)pr.ast;

    op.ast = new Multiply(l, r);

    return op;
}

// Expr ::= Expr dash Expr
ParseResult Parser::parseSubtraction ( ParseResult left ) {
    // parser has already matched left expression 
    ParseResult pr;
    ParseResult op;

    match ( dash );
    pr = parseExpr( prevToken->lbp() ); 

    Expr *l = (Expr *)left.ast;
    Expr *r = (Expr *)pr.ast;

    op.ast = new Subtract(l, r);

    return op;
}

// Expr ::= Expr forwardSlash Expr
ParseResult Parser::parseDivision ( ParseResult left ) {
    // parser has already matched left expression 
    ParseResult pr;
    ParseResult op;

    match ( forwardSlash );
    pr = parseExpr( prevToken->lbp() );

    Expr *l = (Expr *)left.ast;
    Expr *r = (Expr *)pr.ast;

    op.ast = new Divide(l, r);

    return op;
}


// Expr ::= Expr equalEquals Expr
// Expr ::= Expr lessThanEquals Expr
// Expr ::= Expr greaterThanEquals Expr
// Expr ::= Expr notEquals Expr
// Expr ::= Expr leftAngle Expr
// Expr ::= Expr rightAngle Expr
/*
    Two ParseResults are made, one for the right hand expression,
    and another for the actual return value, the Comparison (which is returned).
    By examining the type of the terminal the curren token is,
    the type of Comparison-subclass is determined.
*/
ParseResult Parser::parseRelationalExpr ( ParseResult left ) {
    // parser has already matched left expression 
    ParseResult pr;
    ParseResult comparison;
    
    tokenType type = currToken->terminal;
    nextToken( );

    // just advance token, since examining it in parseExpr caused
    // this method being called.
    pr = parseExpr( prevToken->lbp() );
    
    Expr *l = (Expr *)left.ast;
    Expr *r = (Expr *)pr.ast;

    if ( leftAngle == type ) {
        comparison.ast = new LessThan(l, r);
    } else if ( rightAngle == type ) {
        comparison.ast = new GreaterThan(l, r);
    } else if ( lessThanEquals == type ) {
        comparison.ast = new LessEqualsThan(l, r);
    } else if ( greaterThanEquals == type ) {
        comparison.ast = new GreaterEqualsThan(l, r);
    } else if ( equalsEquals == type ) {
        comparison.ast = new Equals(l, r);
    } else {
        comparison.ast = new NotEquals(l, r);
    }

    node_expected<Comparison>(comparison, "Bad cast of Comparison");

    return comparison;
}


// Helper function used by the parser.

void Parser::match (tokenType tt) {
    if (! attemptMatch(tt)) {
        throw ( makeErrorMsgExpected ( tt ) );
    }
}

bool Parser::attemptMatch (tokenType tt) {
    if (currToken->terminal == tt) { 
        nextToken();
        return true;
    }
    return false;
}

bool Parser::nextIs (tokenType tt) {
    return currToken->terminal == tt;
}

void Parser::nextToken () {
    if (currToken->terminal == endOfFile && currToken->next == NULL) {
        prevToken = currToken;
    } else if (currToken->terminal != endOfFile && currToken->next == NULL) {
        throw ( makeErrorMsg ( "Error: tokens end with endOfFile" ) );
    } else {
        prevToken = currToken;
        currToken = currToken->next;
    }
}

string Parser::terminalDescription ( tokenType terminal ) {
    Token *dummyToken = new Token ("",terminal, NULL);
    ExtToken *dummyExtToken = extendToken (this, dummyToken);
    string s = dummyExtToken->description();
    delete dummyToken;
    delete dummyExtToken;
    return s;
}

string Parser::makeErrorMsgExpected ( tokenType terminal ) {
    string s = (string) "Expected " + terminalDescription (terminal) +
        " but found " + currToken->description();
    //std::cout << std::endl << "\t" << s << std::endl;
    return s;
}

string Parser::makeErrorMsg ( tokenType terminal ) {
    string s = "Unexpected symbol " + terminalDescription (terminal);
    //std::cout << std::endl << "\t" << s << std::endl;
    return s;
}

string Parser::makeErrorMsg ( const char *msg ) {
    return msg;
}

