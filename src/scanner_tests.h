#include <cxxtest/TestSuite.h>

#include "readInput.h"
#include "scanner.h"

using namespace std ;

class ScannerTestSuite : public CxxTest::TestSuite 
{
public:
    Scanner *s ;
    void test_setup_code ( ) {
        s = new Scanner();
    }

    void test_scanner_properties() {
        int map_size = this->s->expressions.size();
        int order_size = this->s->order.size();
        TS_ASSERT_EQUALS(map_size, order_size);
    }

    bool create_token(tokenType t, const char* text, const char* expected) {
        Token *token = this->s->scan(text);
        std::string expect (expected);
        
        if ( token->terminal == t && 0 == expect.compare(token->lexeme) ) return true;
        return false;
    }

    void test_terminal_nameKwd() {
        TS_ASSERT(create_token(nameKwd, "name ", "name"));
    }

    void test_terminal_platformKwd() {
        TS_ASSERT(create_token(platformKwd, "platform ", "platform"));
    }

    void test_terminal_initialKwd() {
        TS_ASSERT(create_token(initialKwd, "initial ", "initial"));
    }

    void test_terminal_stateKwd() {
        TS_ASSERT(create_token(stateKwd, "state ", "state"));
    }

    void test_terminal_gotoKwd() {
        TS_ASSERT(create_token(gotoKwd, "goto ", "goto"));
    }

    void test_terminal_whenKwd() {
        TS_ASSERT(create_token(whenKwd, "when ", "when"));
    }

    void test_terminal_performingeKwd() {
        TS_ASSERT(create_token(performingKwd, "performing ", "performing"));
    }

    void test_terminal_exitKwd() {
        TS_ASSERT(create_token(exitKwd, "exit ", "exit"));
    }


    // --

    void test_terminal_intKwd() {
        TS_ASSERT(create_token(intKwd, "int ", "int"));
    }

    void test_terminal_floatKwd() {
        TS_ASSERT(create_token(floatKwd, "float ", "float"));
    }

    void test_terminal_booleanKwd() {
        TS_ASSERT(create_token(booleanKwd, "boolean ", "boolean"));
    }

    void test_terminal_stringKwd() {
        TS_ASSERT(create_token(stringKwd, "string ", "string"));
    }

    void test_terminal_charKwd() {
        TS_ASSERT(create_token(charKwd, "char ", "char"));
    }

    // --

    void test_terminal_trueKwd() {
        TS_ASSERT(create_token(trueKwd, "true ", "true"));
    }

    void test_terminal_falseKwd() {
        TS_ASSERT(create_token(falseKwd, "false ", "false"));
    }

    // --

    void test_terminal_intConst() {
        TS_ASSERT(create_token(intConst, "9 ", "9"));   
        TS_ASSERT(create_token(intConst, "009 ", "009"));   
    }

    void test_terminal_floatConst() {
        TS_ASSERT(create_token(floatConst, "243.243 ", "243.243"));           
        TS_ASSERT(create_token(floatConst, ".001 ", ".001"));           
        TS_ASSERT(create_token(floatConst, "0.0 ", "0.0"));      
    }

    void test_terminal_stringConst() {
        TS_ASSERT(create_token(stringConst, "\"big week?\" ", "\"big week?\"")); 
    }

    void test_terminal_charConst() {
        TS_ASSERT(create_token(charConst, "'a' ", "'a'")); 
    }

    // --

    void test_terminal_variableName() {
        TS_ASSERT(create_token(variableName, "abc ", "abc"));   
        TS_ASSERT(create_token(variableName, "_abc ", "_abc"));   
        TS_ASSERT(create_token(variableName, "abc_ ", "abc_"));   
        TS_ASSERT(create_token(variableName, "abc243 ", "abc243"));   
        TS_ASSERT(create_token(variableName, "abc_243 ", "abc_243"));
        TS_ASSERT(false == create_token(variableName, "9abc ", "9abc"));
        // this last test case intentionally fails
        // numbers cannot start a variable name
    }

    // --

    void test_terminal_leftParen() {
        TS_ASSERT(create_token(leftParen, "() ", "("));
    }

    void test_terminal_rightParen() {
        TS_ASSERT(create_token(rightParen, ")( ", ")"));
    }

    void test_terminal_leftCurly() {
        TS_ASSERT(create_token(leftCurly, "{} ", "{"));
    }

    void test_terminal_rightCurly() {
        TS_ASSERT(create_token(rightCurly, "}{ ", "}"));
    }

    void test_terminal_leftAngle() {
        TS_ASSERT(create_token(leftAngle, "< ", "<"));
    }

    void test_terminal_rightAngle() {
        TS_ASSERT(create_token(rightAngle, "> ", ">"));
    }

    void test_terminal_colon() {
        TS_ASSERT(create_token(colon, ": ", ":"));
    }

    void test_terminal_comma() {
        TS_ASSERT(create_token(comma, ", ", ","));
    }

    void test_terminal_semiColon() {
        TS_ASSERT(create_token(semiColon, "; ", ";"));
    }

    // --

    void test_terminal_plusSign() {
        TS_ASSERT(create_token(plusSign, "+ ", "+"));
    }

    void test_terminal_star() {
        TS_ASSERT(create_token(star, "* ", "*"));
    }

    void test_terminal_dash() {
        TS_ASSERT(create_token(dash, "- ", "-"));
    }

    void test_terminal_forwardSlash() {
        TS_ASSERT(create_token(forwardSlash, "/", "/"));
    }

    // --

    void test_terminal_equalsEquals() {
        TS_ASSERT(create_token(equalsEquals, "== ", "=="));
    }

    void test_terminal_lessThanEquals() {
        TS_ASSERT(create_token(lessThanEquals, "<= ", "<="));
    }

    void test_terminal_greaterThanEquals() {
        TS_ASSERT(create_token(greaterThanEquals, ">= ", ">="));
    }

    void test_terminal_assign() {
        TS_ASSERT(create_token(assign, ":= ", ":="));
    }

    void test_terminal_notEquals() {
        TS_ASSERT(create_token(notEquals, "!= ", "!="));
    }

    void test_terminal_endOfFile() {
        TS_ASSERT(create_token(endOfFile, " ", ""));
    }

    void test_terminal_lexicalError() {
        TS_ASSERT(create_token(lexicalError, " $$$ ", "$"));
    }

    // --- _consume tests

    void test_whitespace() {
        char string[] = "    \n\r\t    a";
        Token *token = this->s->scan(string);
        TS_ASSERT_EQUALS( token->lexeme, "a" );
        TS_ASSERT_EQUALS( token->next->terminal, endOfFile);
    }

    void test_block_comment() {
        char string[] = "/*big week?*/\r\n/*huge week*/a";
        Token *token = this->s->scan(string);
        TS_ASSERT_EQUALS( token->lexeme, "a" );
    }

    void test_single_comment() {
        char string[] = "\n\r\r// cool story\n a //nice \n\r b";
        Token *token = this->s->scan(string);
        
        TS_ASSERT_EQUALS( token->lexeme, "a" );
        TS_ASSERT_EQUALS( token->next->lexeme, "b");   
    }

    // --

    // Test that a list of tokens has no lexicalError tokens.
    bool noLexicalErrors (Token *tks) {
        Token *currentToken = tks ;
        while (currentToken != NULL) {
            if (currentToken->terminal == lexicalError) {
                return false ;
            }
            else {
                currentToken = currentToken->next ;
            }
        }
        return true ;
    }

    /* A quick, but inaccurate, test for scanning files.  It only
       checks that no lexical errors occurred, not that the right
       tokens were returned. 
    */
    void scanFileNoLexicalErrors ( const char* filename ) {
        char *text =  readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;
        Token *tks = s->scan ( text ) ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT (noLexicalErrors(tks));
    }


    /* This function checks that the terminal fields in the list of
       tokens matches a list of terminals.
    */
    bool sameTerminals (Token *tks, int numTerms, tokenType *ts) {
        Token *currentToken = tks ;
        int termIndex = 0 ;
        while (currentToken != NULL && termIndex < numTerms ) {

            if (currentToken->terminal != ts[termIndex]) {
                return false ;
            }
            else {
                ++ termIndex ;
                currentToken = currentToken->next ;
            }
        }


        return true ;
    }

    /* Below are the provided test files that your code should also
       pass.

       You may initially want to rename these tests to "xtest_..." so
       that the CxxTest framework does not see it as a test and won't
       run it as one.  This way you can focus on the tests that you'll
       write above for the individual terminal types first. Then focus
       on these tests.

    */

    // The "endOfFile" token is always the last one in the list of tokens.
    void test_scan_empty ( ) {
        Token *tks = s->scan ("  ") ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
        TS_ASSERT (tks->next == NULL) ;
    }

    void test_scan_empty_comment ( ) {
        Token *tks = s->scan (" /* a comment */ ") ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
        TS_ASSERT (tks->next == NULL) ;
    }

    // When a lexical error occurs, the scanner creates a token with a 
    // single-character lexeme and lexicalError as the terminal.
    void test_scan_lexicalErrors ( ) {
        Token *tks = s->scan ("$&1  ") ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, lexicalError) ;
        TS_ASSERT_EQUALS (tks->lexeme, "$");
        tks = tks->next ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, lexicalError) ;
        TS_ASSERT_EQUALS (tks->lexeme, "&");
        tks = tks->next ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, intConst) ;
        TS_ASSERT_EQUALS (tks->lexeme, "1");
        tks = tks->next ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
        TS_ASSERT (tks->next == NULL) ;
    }


    // A test for scanning numbers and a variable.
    void test_test_scan_nums_vars ( ) {
        Token *tks = s->scan (" 123 x 12.34 ") ;
        TS_ASSERT (tks != NULL) ;
        tokenType ts[] = { intConst, variableName, floatConst, endOfFile } ;
        TS_ASSERT ( sameTerminals ( tks, 4, ts ) ) ;
    }

    /* This test checks that the scanner returns a list of tokens with
       the correct terminal fields.  It doesn't check that the lexeme
       are correct.
     */

    void x_test_scan_bad_syntax_good_tokens ( ) {
        const char *filename = "../samples/bad_syntax_good_tokens.cff" ;
        char *text =  readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;
        Token *tks = s->scan ( text ) ;

        tokenType ts[] = { 
            intConst, intConst, intConst, intConst, 

            stringConst, stringConst, stringConst,

            floatConst, floatConst, floatConst,

            charConst, charConst, charConst, charConst, 
            charConst, charConst, charConst, 

            trueKwd, falseKwd,


            colon, semiColon, comma, 
            leftCurly, leftParen, rightCurly, rightParen,

            plusSign, star, dash, forwardSlash, 
            rightAngle, leftAngle,

            equalsEquals, lessThanEquals, 
            greaterThanEquals, notEquals, 
            assign,

            variableName, variableName, variableName, variableName, 
            variableName, variableName, variableName,

            nameKwd, platformKwd, initialKwd, stateKwd, 
            gotoKwd, whenKwd, exitKwd, performingKwd,

            intKwd, floatKwd, booleanKwd, stringKwd, charKwd,

            endOfFile
        } ;
        int count = 58; 

        TS_ASSERT ( sameTerminals ( tks, count, ts ) ) ;
    }

    void test_scan_sample_abstar ( ) {
        scanFileNoLexicalErrors ("../samples/abstar.cff") ;
    }


} ;
