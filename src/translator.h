#ifndef __TRANSLATOR_DEFINED__
#define __TRANSLATOR_DEFINED__
#include <string>
#include "ast.h"
std::string _header_machine_states(Program *p);
std::string _header_ifndef_open();
std::string _header_ifndef_close();
std::string _header_machine_class_open(Program *p);
std::string _header_machine_public();
std::string _header_machine_constructor_deconstructor(Program *p);
std::string _header_machine_private(Program *p);
std::string _header_machine_class_close();
std::string _header_machine_main();
std::string _header_machine_decls(Program *p);
std::string _cpp_includes();
std::string _cpp_constructor_deconstructor(Program *p);
std::string _cpp_expr(Expr *e);
std::string _cpp_stmts(Stmt *s);
std::string _cpp_transitions(Transition *t);
std::string _cpp_states(Program *p);
std::string _cpp_initial_state_call(Program *p);
std::string _cpp_main(Program *p);
#endif