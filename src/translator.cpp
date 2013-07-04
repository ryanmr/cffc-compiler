#include "translator.h"

/*
  Adds the RunTime and Machine to the CPP file.
*/
std::string _cpp_includes() {
  std::string output("");
  output = output + "#include \"RunTime.h\"\n";
  output = output + "#include \"Machine.h\"\n";
  return output;
}

/*
  Adds the basic constructor and destructor.
  Also adds the platform runtime to `this->platform` in the Machine.
*/
std::string _cpp_constructor_deconstructor(Program *p) {
  std::string output("");
  std::string name(p->get_variable()->get_name());
  output = output + name + "::" + name + "(" + p->get_platform()->get_variable()->get_name() + " *p) {\n";
  output = output + "\tthis->platform = p;\n";
  output = output + "};\n";
  output = output + name + "::~" + name + "() {}\n\n";
  return output;
}

/*
  Adds the generated Expr.
*/
std::string _cpp_expr(Expr *e) {
  std::string output("");

  output = e->cppCode_cpp();

  return output;
}

/*
  Adds statements.
  This will switch between three cases:
    1. No statements.
    2. A statement that LHS is on the platform and thus uses `platform->set_*` where * is the variable name.
    3. A statement that LHS is on the Machine and thus uses `this->*` where * is the variable name.
*/
std::string _cpp_stmts(Stmt *s) {
  std::string output("");

  if ( s->is_empty() ) {
    return "\t\t// No statements\n";
  }

  while ( s ) {
    
    Variable *lhs = s->get_variable();
    if ( lhs->is_on_platform() ) {
      output = output + "\t\tplatform->set_" + lhs->get_name() + "(" + _cpp_expr(s->get_expr()) + ");\n";
    } else {
      output = output + "\t\tthis->" + lhs->get_name() + " = " + _cpp_expr(s->get_expr()) + ";\n";  
    }
    

    s = s->get_next();
  
  }

  return output;

}

/*
  Adds a control structure to handle transition cases.
  This will switch between multiple cases:
    1. No transitions and thus no if/else-if branches.
    2. The first case will be in an if-branch.
    3. The remaining cases will be in successive else-if-branches.

    In 2. and 3. there is a subcase:
      A. If the transition case is an "exit", no Fn() is added.
      B. Otherwise, the next State Fn() is added.
*/
std::string _cpp_transitions(Transition *t) {
  std::string output("");

  if ( t->is_empty() ) {
    return "\n\t\t// No transitions\n";
  }

  bool first = true;

  while ( t ) {

    std::string ask("");
    if (first) ask = "\tif ";
      else ask = "else if "; 

    output = output + ask + "(" + _cpp_expr(t->get_expr()) + ") {\n";

    if ( t->is_exit() ) {

      // stmts
      output = output + _cpp_stmts( t->get_stmt() );
      // no method call to elsewhere, hence exit
      output = output + "\t\tplatform->next_state();\n";
      //but call next_state anyway because it sucks
      // which would make one consider an alternative name
    } else {

      // stmts
      output = output + _cpp_stmts( t->get_stmt() );

      // call next method
      output = output + "\t\tplatform->next_state();\n";
      output = output + "\t\t" + t->get_variable()->get_name() + "();\n";
    }
    
    output = output + "\t} "; 

    t = t->get_next();
    if (first) first = false;
  }

  output = output + "\n"; 
  return output;

}

/*
  Adds a State.
  There are multiple cases:
    1. No states.
    2. Otherwise, states will be added with their transition structures.
*/
std::string _cpp_states(Program *p) {
  std::string output("");

  State *s = p->get_states();

  if ( s->is_empty() ) {
    output = "\n\t\t// No states\n";
    return output;
  }

  while ( s ) {   
    output = output + "void " + p->get_variable()->get_name() + "::" + s->get_variable()->get_name() + "() {\n";

    output = output + "\tplatform->enter_state();\n\n";
    Transition *t = s->get_transition();
    output = output + _cpp_transitions(t);


    output = output + "}\n\n";
    s = s->get_next();
  }

  return output;

}

/*
  Adds the initial state call.
  This will find a State that has the "initial" property and add it.
*/
std::string _cpp_initial_state_call(Program *p) {
  State *s = p->get_states();
  if (s->is_empty()) return "// No initial state (empty)\n";

  while ( s && !s->is_initial() ) {
    s = s->get_next();
  }

  if ( s->is_initial() ) {
    return "machine->" + s->get_variable()->get_name() + "();";
  } else {
    return "// No initial state (not declared)";
  }

}

/*
  Adds the main() with proper calls to create the RunTime platform and Machine.  
*/
std::string _cpp_main(Program *p) {
  std::string output("");

  output = output + "int main(int argc, char **argv) {\n\n";
  output = output + "\t" + p->get_platform()->get_variable()->get_name() + " *platform = new " + p->get_platform()->get_variable()->get_name() + "(argc, argv);\n\n";

  output = output + "\t" + p->get_variable()->get_name() + " *machine = new " + p->get_variable()->get_name() + "(platform);\n";
  output = output + "\t" + _cpp_initial_state_call(p) + "\n\n";

  output = output + "\treturn 0;\n";
  output = output + "}\n";

  return output;
}

/*
  Below are generic header generate functions.
*/

std::string _header_ifndef_open() {
  return "#ifndef MACHINE_H\n#define MACHINE_H\n";
}
std::string _header_ifndef_close() {
  return "#endif\n";
}
std::string _header_machine_class_open(Program *p) {
  return "class " + p->getName() + " { \n";
}
std::string _header_machine_public() {
  return "\tpublic: \n";
}
std::string _header_machine_constructor_deconstructor(Program *p) {
  std::string output("");
  output = output + "\t\t" + p->get_variable()->get_name() + "(" + p->get_platform()->get_variable()->get_name() + " *platform);\n";
  output = output + "\t\t~" + p->get_variable()->get_name() + "();\n";
  return output;
}
std::string _header_machine_private(Program *p) {
  std::string output("");
  output = output + "\tprivate:\n";
  output = output + "\t\t" + p->get_platform()->get_variable()->get_name() + " *platform;\n";
  return output;
}
std::string _header_machine_class_close() {
  return "};\n\n";
}
std::string _header_machine_main() {
  return "int main(int argc, char **argv);\n\n";
}

std::string _header_machine_decls(Program *p) {
  std::string output("");

  DeclList *d = p->get_decls();

  while ( is_node_type<SeqDecl>(d) ) {
    output = output + ((SeqDecl *)d)->cppCode_h();
    d = ((SeqDecl *)d)->get_tail();
  }

  return output;
}
std::string _header_machine_states(Program *p) {

  std::string output("");

  State *s = p->get_states();

  if ( s->is_empty() ) {
    output = "\n\t\t// No states\n";
    return output;
  }

  while ( s ) {
    output = output + "\t\tvoid " + s->get_variable()->get_name() + "();\n";
    s = s->get_next();
  }

  return output;

}
