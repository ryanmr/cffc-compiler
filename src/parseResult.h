/* 
	ParseResult: a simple object for holding the result of a parse.
  Author: Professor EVW
*/

#ifndef PARSER_RESULT_H
#define PARSER_RESULT_H

// #include "AST.h"
#include "ast.h"

#include <string>


class ParseResult {
public:
    ParseResult() ;
    std::string errors ;
    std::string pp ;
    Node* ast;
    bool ok;
} ;

#endif /* PARSER_RESULT_H */
