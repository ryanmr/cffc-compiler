#include <cxxtest/TestSuite.h>

#include "readInput.h"
#include "scanner.h"
#include "extToken.h"
#include "parser.h"
#include "parseResult.h"
#include "ast.h"

using namespace std ;


/*
    test_helper
    Test Helper tests to see if the node returned is the type we're looking for. A function template is used here so that the expected type is passed through the template and then it can be checked correctly.
    string fragment the test value for the scanner
    string type the type of non-terminal
    string error the error message
    <class T> the class of node
*/
template <class T>
T* test_helper(std::string fragment, std::string type, std::string error, Parser *p) {
    ParseResult pr = p->prank(fragment.c_str(), type);
    T *node = dynamic_cast<T *>(pr.ast);
    TSM_ASSERT(error, node);
    return node;
}

class AstTestSuite : public CxxTest::TestSuite 
{
public:
    int DEBUG ;

    Scanner *s ;
    Parser *p ;

    void test_setup_code ( ) {
        s = new Scanner() ;
        p = new Parser() ;
        DEBUG = 0 ;
    }


    /*
        The test cases here were written in chronological incremental order.
        For instance, Number is the first test. It is also the first Expr terminal created.
        After it was tested, Integer, Float and other terminal Exprs were made and tested.
    */


    /*
        Testing the AST for the Number nodes.
        There are two Number nodes: Float and Integer, but this tests the super type of each, Number.
    */
    void test_Number() {
        
        // Integer
        test_helper<Number>("19", "expr", "Not a Number expression", this->p);

        // Float
        test_helper<Number>("20.13", "expr", "Not a Number expression", this->p);  
    }

    /*
        Testing the AST for the Integer node.
    */
    void test_Integer() {
        test_helper<Integer>("19", "expr", "Not a Integer expression", this->p);
    }

    /*
        Testing the AST for the Float node.
    */
    void test_Float() {
        test_helper<Float>("20.13", "expr", "Not a Float expression", this->p);      
    }

    /*
        Testing the AST for the String node.
    */
    void test_String() {
        test_helper<String>("\"string\"", "expr", "Not a String expression", this->p);
    }

    /*
        Testing the AST for the Char node.
    */
    void test_Char() {
        test_helper<Char>("\'a\'", "expr", "Not a Char expression", this->p);
    }

    /*
        Testing the AST for the Bool node.
    */
    void test_Bool() {
        test_helper<Bool>("true", "expr", "Not a Bool expression", this->p);
    }

    /*
        Numbers, Strings, Chars and Bools are under the Super class Constant. 
        This tests the Constant node.
    */
    void test_Constant() {
        test_helper<Constant>("19", "expr", "Not a Constant:Integer expression", this->p);   
        test_helper<Constant>("20.13", "expr", "Not a Constant:Float expression", this->p);   
        test_helper<Constant>("\"big week\"", "expr", "Not a Constant:String expression", this->p);   
        test_helper<Constant>("\'a\'", "expr", "Not a Constant:Char expression", this->p);   
        test_helper<Constant>("false", "expr", "Not a Constant:Bool expression", this->p);
    }

    /*
        Testing the AST for the LessThan node.
    */
    void test_LessThan() {
        test_helper<LessThan>("9 < 10", "expr", "Not a LessThan expression", this->p);
    }

    /*
        Testing the AST for the LessEqualsThan node.
    */
    void test_LessEqualsThan() {
        test_helper<LessEqualsThan>("9 <= 10", "expr", "Not a LessEqualsThan expression", this->p);
    }

    /*
        Testing the AST for the GreaterThan node.
    */
    void test_GreaterThan() {
        test_helper<GreaterThan>("100 > 9", "expr", "Not a GreaterThan expression", this->p);
    }

    /*
        Testing the AST for the GreaterEqualsThan node.
    */
    void test_GreaterEqualsThan() {
        test_helper<GreaterEqualsThan>("100 >= 9", "expr", "Not a GreaterEqualsThan expression", this->p);
    }

    /*
        Testing the AST for the Equals node.
    */
    void test_Equals() {
        test_helper<Equals>("100 == 100", "expr", "Not a Equals expression", this->p);
    }

    /*
        Testing the AST for the NotEquals node.
    */
    void test_NotEquals() {
        test_helper<NotEquals>("100 != 9", "expr", "Not a NotEquals expression", this->p);
    }

    /*
        Testing the ASTs for Comparison super class.
    */
    void test_Comparison() {
        test_helper<Comparison>("9 < 10", "expr", "Not a Comparison expression", this->p);
        test_helper<Comparison>("9 <= 10", "expr", "Not a LessEqualsThan expression", this->p);
        test_helper<Comparison>("100 > 9", "expr", "Not a GreaterThan expression", this->p);
        test_helper<Comparison>("100 >= 9", "expr", "Not a GreaterEqualsThan expression", this->p);
        test_helper<Comparison>("100 == 100", "expr", "Not a Equals expression", this->p);
        test_helper<Comparison>("100 != 9", "expr", "Not a NotEquals expression", this->p);
    }

    /*
        Testing the AST for the Add node.
    */
    void test_Add() {
        test_helper<Add>("1 + 1", "expr", "Not a Add expression", this->p);
    }

    /*
        Testing the AST for the Multiply node.
    */
    void test_Multiply() {
        Multiply *m = test_helper<Multiply>("abc * xyz", "expr", "Not a Multiply expression", this->p);
        
        /*
            An assert that the left and right branches exist and are variables.
        */

        TS_ASSERT( is_node_type<Variable>( (Variable *)m->get_left() ) );
        TS_ASSERT( is_node_type<Variable>( (Variable *)m->get_right() ) );
    }

    /*
        Testing the AST for the Divide node.
    */
    void test_Divide() {
        test_helper<Divide>("1 / 1", "expr", "Not a Divide expression", this->p);   
    }

    /*
        Testing the AST for the Subtract node.
    */
    void test_Substract() {
        test_helper<Subtract>("1 - 1", "expr", "Not a Subtract expression", this->p);   
    }

    /*
        Operator is the super class of Add, Subtract, Multiply and Divide. This tests all subclasses.
    */
    void test_Operator() {
        test_helper<Operator>("1 + 1", "expr", "Not a Operator:Add expression", this->p);   
        test_helper<Operator>("1 * 1", "expr", "Not a Operator:Multiply expression", this->p);   
        test_helper<Operator>("1 / 1", "expr", "Not a Operator:Divide expression", this->p);   
        test_helper<Operator>("1 - 1", "expr", "Not a Operator:Subtract expression", this->p);   
    }

    /*
        Test the AST for the Variable node.
    */
    void test_Variable() {
        test_helper<Variable>("varName", "expr", "Not a Variable expression", this->p);
    }

    /*
        Test the AST for the Stmt node.
        Notice the change of "type" to "stmt", this switches the prank-call to parse for a stmt.
    */
    void test_Stmt() {
        test_helper<Stmt>("varName := 1;", "stmt", "Not a Stmt", this->p);   
    }

    /*
        Stmts are special since they're a linked list of Stmt(s).
        Sadly, because the Parser is the way it is, it is not possible to
        test directly a series of stmts (e.g. "varName := 1; varName2 := 1 + varName;")
        So the test for multiple stmts are in Transition.
    */

    /*
        Test the AST for the Transition node.
    */
    void test_Transition() {
        Transition *t = test_helper<Transition>("goto Compute when true performing { i := 0; varName := k + 1; k := k + 1; };", "transition", "Not a Transition", this->p);
        /*
            This tests that sub-elements exist.
        */
        TS_ASSERT( (Variable *)(t->get_variable()) );
        TS_ASSERT( (Expr *)(t->get_expr()) );
        TS_ASSERT( (Stmt *)(t->get_stmt())->get_next()->get_next() );
    }

    /*
        Test the AST for the Transition node, but of the "exit" type.
    */
    void test_Transition_Exit() {
        test_helper<Transition>("exit when true performing {output := 1;};", "transition", "Not a Transition:Exit", this->p);
    }

    /*
        Transitions are special since they are just a linked list of Transition(s).
        See Stmt(s) for the same issue that occurs when testing for multiple transitions.
        A series of Transition(s) can be contained in a State, so the test is there for multiples.
    */

    /*
        Test the AST for the State node.
    */
    void test_State() {
        State* s = test_helper<State>("initial state: Compute { goto Compute when i <= input performing { s := s + 1; }; exit when s > 100 performing { i := i + 1; }; } ", "state", "Not a State", this->p);
        /*
            This tests the sub-elements of the state.
            Variable, two transitions and no other states.
        */
        TS_ASSERT( (Variable *)(s->get_variable()) );
        TS_ASSERT( (Transition *)(s->get_transition()) );
        TS_ASSERT( (Transition *)(s->get_transition()->get_next()) );
        TS_ASSERT( (State *)(s->get_next()) == NULL );
    }


    /*
        Test the AST for the Decl node.
        Decls are not exciting as it is the actual data class.

        DeclList, NoDecl and SeqDecl are special. They only exist when there is a "program",
        so like Transitions, Stmts, and States. These are tested in Program.

        (Decls Suck; comments like this are definitely found in professional code...)
    */
    void test_Decl() {
        test_helper<Decl>("int s;", "decl", "Not a Decl", this->p);
    }

    /*
        Test the AST for the Platform node.
    */
    void test_Platform() {
        test_helper<Platform>("platform: BulkBag;", "platform", "Not a Platform", this->p);
    }

    /*
        Program is the big object in this setup.
        Program contains pointers to DeclList, State... and Platform, and a string as its own name.
        Aside from testing Program itself, this function tests:
            variable uses
            number of decls
            number of states
    */
    void test_Program() {
        Program *p = test_helper<Program>("name: BigWeek; platform: BulkBag; int i; char n; boolean b; initial state: Compute { goto Compute when i <= input performing { s := s + 1; }; exit when s > 100 performing { i := i + 1; }; }  ", "program", "Not a Program", this->p);
        TS_ASSERT_EQUALS(p->getNumVarUses(), 7);
        TS_ASSERT_EQUALS(p->getNumVarDecls(), 3);
        TS_ASSERT_EQUALS(p->getNumStates(), 1);
        // Please look at comments in extToken.cpp:38 to read a comment about this.
    }

    /*
        Like the Program test above, but without decls.
    */
    void test_Program_No_Decls() {
        Program *p = test_helper<Program>("name: BigWeek; platform: BulkBag; initial state: Huge { goto Literally when true performing { output := input * input; }; } state: Compute { goto Compute when i <= input performing { s := s + 1; }; exit when s > 100 performing { i := i + 1; }; } ", "program", "Not a Program", this->p);

        TS_ASSERT_EQUALS(p->getNumVarUses(), 10);
        TS_ASSERT_EQUALS(p->getNumVarDecls(), 0);
        TS_ASSERT_EQUALS(p->getNumStates(), 2);

    }

    /*
        The follow is a series of automated tests using the common interface among provided to use in the form of:
            number of variable uses
            number of decls
            number of states
        This is reading in files from ../samples/.
    */

    void test_ast_squareMapper ( ) {
        create_test_bed("../samples/squareMapper.cff", "SquareMapper", 1, 0, 3);
    }

    void test_ast_sumOfSquares() {
        create_test_bed("../samples/sumOfSquares.cff", "SumOfSquares", 2, 2, 12);
    }

    void test_ast_box() {
        create_test_bed("../samples/box.cff", "Box", 5, 1, 18);
    }

    void test_ast_abstar() {
        create_test_bed("../samples/abstar.cff", "ABStar", 3, 0, 16);
    }

    /*
        Below is an abstracted method for calling the file tests listed above.
    */

    void create_test_bed(std::string file, std::string name, int states, int decls, int var_uses) {
        const char *text =  readInputFromFile ( file.c_str() )  ;
        TS_ASSERT ( text ) ;

        // 1. Test that the file can be parsed.
        ParseResult pr = p->parse ( text ) ;
        TSM_ASSERT ( "file \"" + file + "\" failed to parse. " + 
                     "It had errors: " + pr.errors, pr.ok ) ;

        // 2. Verify that the ast field is not null
        TSM_ASSERT ( "\"" + file + "\" failed to generate an AST.",
                     pr.ast != NULL );

        // 3. Verify that the parser AST is an Expr.
        Program *p = dynamic_cast<Program *>(pr.ast) ;
        TSM_ASSERT ( "file \"" + file + 
                     "\" parser failed to return a Program AST", p ) ;

        // 4. Verify that the correct name is reported.
        TSM_ASSERT_EQUALS ( "file \"" + file + 
                            "\" returned wrong name" ,
                            p->getName(), name ) ;

        // 5. Verify that the correct number of states.
        TSM_ASSERT_EQUALS ( "file \"" + file + 
                            "\" returned wrong number of states" ,
                            p->getNumStates(), states ) ;

        // 6. Verify that the correct number variable declarations.
        TSM_ASSERT_EQUALS ( "file \"" + file + 
                            "\" returned wrong number of variable declarations" ,
                            p->getNumVarDecls(), decls ) ;

        // 7. Verify that the correct number variable uses.
        TSM_ASSERT_EQUALS ( "file \"" + file + 
                            "\" returned wrong number of variable uses" ,
                            p->getNumVarUses(), var_uses ) ;
    }

  
} ;
