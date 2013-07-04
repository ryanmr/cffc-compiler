/*
	ParseResult: a simple object for holding the result of a parse.
	Author: Professor EVK
*/

#include "parseResult.h"

using namespace std ;

ParseResult::ParseResult() {
    errors = "" ;
    ok = true ;
    this->ast = NULL; 
}