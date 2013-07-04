#include <cxxtest/TestSuite.h>

#include "readInput.h"
#include "scanner.h"
#include "extToken.h"
#include "parser.h"
#include "parseResult.h"

#include <sstream>

using namespace std ;

class ParserTestSuite : public CxxTest::TestSuite 
{
public:
    Scanner *s ;
    Parser *p ;
    void test_setup_code ( ) {
        s = new Scanner() ;
        p = new Parser() ;
    }

    void test_parse_bad_syntax ( ) {
        const char *text 
          = readInputFromFile ( "../samples/bad_syntax_good_tokens.cff" )  ;
        TS_ASSERT ( text ) ;
        ParseResult pr = p->parse ( text ) ;

        TS_ASSERT ( ! pr.ok ) ; // it runs through the parser but this checks for a false
        // and thus it is clear Program is expected to be never made

        // Please look at comments in extToken.cpp:38 to read a note about this.
    }

    void test_parse_exprs ( ) {
        ostringstream prog ;
        prog << "name: N ; " << endl 
             << "platform: P ; " << endl 
             << "state: S { " << endl 
             << "  goto S when b <= 10 performing { " << endl
             << "    x := 1 + 2 * 3 ; " << endl
             << "    y := x / 4 - 2 ; " << endl
             << "    b := 1 < y ; " << endl
             << "    c := y > 1 ; " << endl
             << "    d := 1 >= z ; " << endl
             << "    e := y <= z ; " << endl
             << "    f := x == 1 ; " << endl
             << "    g := y != 10 ; " << endl
             << "  } ;" << endl
             << "} " << endl ;
        ParseResult pr = p->parse (prog.str().c_str()) ;
        TSM_ASSERT ( pr.errors, pr.ok ) ;
    }

    void test_parse_abstar ( ) {
        const char *filename = "../samples/abstar.cff" ;
        const char *text = readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;
        ParseResult pr = p->parse ( text ) ;
        string msg (filename) ;
        msg += "\n" + pr.errors + "\n" + pr.pp ;
        TSM_ASSERT ( msg , pr.ok );
    }
    void test_parse_box ( ) {
        const char *filename = "../samples/box.cff" ;
        const char *text = readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;
        ParseResult pr = p->parse ( text ) ;
        string msg (filename) ;
        msg += "\n" + pr.errors + "\n" + pr.pp ;
        TSM_ASSERT ( msg , pr.ok );
    }

    void test_parse_sumOfSquares ( ) {
        const char *filename = "../samples/sumOfSquares.cff" ;
        const char *text = readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;
        ParseResult pr = p->parse ( text ) ;
        string msg (filename) ;
        msg += "\n" + pr.errors + "\n" + pr.pp ;
        TSM_ASSERT ( msg , pr.ok );
    }

} ;
