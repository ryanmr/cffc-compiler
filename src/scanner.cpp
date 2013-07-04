#include <regex.h>
#include <string>
#include <iostream>

#include <map>
#include <vector>

#include "regex.h"
#include "scanner.h"

Token::Token() {
	// nothing, yet.
}

Token::~Token() {
	// nothing, yet.
}

Token::Token(const char* lexeme, tokenType t, Token* next) {
	this->lexeme = lexeme;
	this->terminal = t;
	this->next = next;
}

Scanner::Scanner() {
	
	this->expressions[nameKwd] = makeRegex("^(name)");
	this->expressions[platformKwd] = makeRegex("^(platform)");
	this->expressions[initialKwd] = makeRegex("^(initial)");
	this->expressions[stateKwd] = makeRegex("^(state)");
	this->expressions[gotoKwd] = makeRegex("^(goto)");
	this->expressions[whenKwd] = makeRegex("^(when)");
	this->expressions[performingKwd] = makeRegex("^(performing)");
	this->expressions[exitKwd] = makeRegex("^(exit)"); // 8

	this->expressions[intKwd] = makeRegex("^(int)");
	this->expressions[floatKwd] = makeRegex("^(float)");
	this->expressions[booleanKwd] = makeRegex("^(boolean)");
	this->expressions[stringKwd] = makeRegex("^(string)");
	this->expressions[charKwd]  = makeRegex("^(char)"); // 5

	this->expressions[trueKwd] = makeRegex("^(true)");
	this->expressions[falseKwd] = makeRegex("^(false)"); // 2

	this->expressions[intConst] = makeRegex("^[0-9]+");
	this->expressions[floatConst] = makeRegex("^[0-9]*\\.[0-9]+");
	this->expressions[stringConst] = makeRegex("^\"[^\"]*\"");
	this->expressions[charConst] = makeRegex("^'[^\']*'");
	// 4

	this->expressions[variableName] = makeRegex("^[a-zA-Z_][a-zA-Z0-9_]*");
	// 1

	this->expressions[leftParen] = makeRegex("^\\(");
	this->expressions[rightParen] = makeRegex("^\\)");
	this->expressions[leftCurly] = makeRegex("^\\{");
	this->expressions[rightCurly] = makeRegex("^\\}");
	// 4

	this->expressions[leftAngle] = makeRegex("^<");
	this->expressions[rightAngle] = makeRegex("^>");
	this->expressions[colon] = makeRegex("^\\:");
	this->expressions[semiColon] = makeRegex("^\\;");
	this->expressions[comma] = makeRegex("^\\,");
	this->expressions[assign] = makeRegex("^(\\:\\=)");
	// 6

	this->expressions[plusSign] = makeRegex("^\\+");
	this->expressions[star] = makeRegex("^\\*");
	this->expressions[dash] = makeRegex("^\\-");
	this->expressions[forwardSlash] = makeRegex("^/");

	this->expressions[equalsEquals] = makeRegex("^(==)");
	this->expressions[lessThanEquals] = makeRegex("^(<=)");
	this->expressions[greaterThanEquals] = makeRegex("^(>=)");
	this->expressions[notEquals] = makeRegex("^(\\!=)");
	// 8

	// there are 38 total expressions; currently

	tokenType o[] = {
		// parsables

		floatConst,
		intConst,
		stringConst,
		charConst,

		// keywords
		nameKwd, platformKwd, initialKwd, stateKwd,
		gotoKwd, whenKwd, performingKwd, exitKwd,

		intKwd, floatKwd, booleanKwd, stringKwd, charKwd,

		trueKwd, falseKwd,

		variableName,

		// punctuation
		leftParen, rightParen,
		leftCurly, rightCurly,

		lessThanEquals, greaterThanEquals,

		leftAngle, rightAngle,

		comma, colon, semiColon,
		assign,

		plusSign, star, dash, forwardSlash,
		equalsEquals, notEquals
	};


	this->order = std::vector<tokenType>(o, o + sizeof(o) / sizeof(o[0]));

	/*
		These are regex for whitespace and both comment types.
		These are setup here rather than _consume (below) so that
		they're only allocated once.
	*/
	this->ignore_expressions.push_back(makeRegex("^[\n\t\r ]+"));
	this->ignore_expressions.push_back(makeRegex("^\\/\\*([^\\*]|\\*+[^\\*\\/])*\\*+\\/"));
	this->ignore_expressions.push_back(makeRegex("^(//)[^\n\r]*[\n\r]"));

}
Scanner::~Scanner() {
	// probably nothing
}

Token *Scanner::scan(const char* text) {

	int tokensCreated = 0;
	int matched_characters = 0;
	int best_match = 0;

	Token *first = NULL;
	Token *previous = NULL;
	bool type_set = false;

	tokenType typeFound;

	matched_characters = _consume(text);
	text = text + matched_characters;

	while ( text[0] != '\0' ) {
		best_match = 0;
		type_set = false;

		for (std::vector<tokenType>::size_type i = 0; i != this->order.size(); i++) {			
			tokenType key = this->order[i];
			regex_t *regex = this->expressions.at(key);

			matched_characters = matchRegex(regex, text);

			if ( matched_characters > best_match ) {
				best_match = matched_characters;
				type_set = true;
				typeFound = key;
			}

		}


		Token *temp = new Token();
		tokensCreated++;
		if ( type_set == false ) {
			
			// aritifcally capture whatever the first character we didn't find is
			matched_characters = 1;

			// obviously, no type was set so this is a lexical error
			temp->terminal = lexicalError;
			std::string lexeme (text, matched_characters);

			temp->lexeme = lexeme;

			// set the text to the next character after processing
			text = text + 1;

		} else { // in this branch, save the token and lexeme found
			temp->terminal = (tokenType)typeFound;

			std::string lexeme (text, best_match);
			temp->lexeme = lexeme;

			text = text + best_match;
		}

		if ( previous != NULL ) {
			previous->next = temp;
		} else {
			first = temp;
		}
		previous = temp;


		matched_characters = this->_consume(text);
		text = text + matched_characters;

	}

	Token *eof = new Token();
	tokensCreated++;
	eof->terminal = endOfFile;

	eof->lexeme = "";
	if ( previous != NULL ) {
		previous->next = eof;
	} else {
		first = eof;
	}
	eof->next = NULL;

	return first;
}

/*
	While this is based on WordCount, it is now
	built to iterate over ignore_expressions which contains
	regex for whitespace and comments.
*/
int Scanner::_consume(const char *text) {

    int numMatchedChars = 0;
    int totalNumMatchedChars = 0;
    int stillConsumingWhiteSpace;

    do {
        stillConsumingWhiteSpace = 0;  // exit loop if not reset by a match

        for(int i = ignore_expressions.size(); i > 0; --i) {
        	numMatchedChars = matchRegex(ignore_expressions[i-1], text);
        	totalNumMatchedChars += numMatchedChars;
        	if (numMatchedChars > 0) {
            text = text + numMatchedChars;
            stillConsumingWhiteSpace =1;
        	}
        }
    }
    while ( stillConsumingWhiteSpace );    

    return totalNumMatchedChars;
}
