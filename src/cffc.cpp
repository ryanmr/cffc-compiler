/*

   _____  ______  ______ 
  / ____ |  ____ |  ____|
 | |     | |__   | |__   
 | |     |  __|  |  __|  
 | |____ | |     | |     
  \_____ |_|     |_| 

  Compile your CFF Program.

*/

#include "parser.h"
#include "readInput.h"
#include "ast.h"

#include <iostream>
#include <fstream>

using namespace std;

int main ( int argc, char **argv ) {

    if ( argc < 2 ) {
        cout << "Usage: cffc <filename>" << endl;
        return 1;
    }

    char *filename = argv[1];
    string filepath = "../samples/" + string(filename);
    char *text = readInputFromFile ( filepath.c_str() ) ;
    if ( ! text ) {
        cout << "File \"" << filepath << "\" not found." << endl;
        return 2;
    }

    Parser *p = new Parser();
    ParseResult pr = p->parse(text);
    delete p;

    if ( ! pr.ok ) {
        cout << "Syntax errors in CFishFish program: " << endl
             << pr.errors << endl;
        return 3;
    }


    Program *program = dynamic_cast<Program *>(pr.ast);

    if ( ! program ) {
        cout << "Internal compiler error, failed to create AST." << endl;
        return 4;
    }

    ofstream machine_h;
    machine_h.open ( "../cffc/Machine.h" );
    machine_h << program->cppCode_h();
    machine_h.close();

    ofstream machine_cpp;
    machine_cpp.open ( "../cffc/Machine.cpp" );
    machine_cpp << program->cppCode_cpp();
    machine_cpp.close();

    delete program;
    return 0;
}
