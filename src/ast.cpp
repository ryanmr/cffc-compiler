
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <typeinfo>
#include "ast.h"
#include "translator.h"


/*
	----
	Much of the implementation here is repetitive and trivial.
	For those portions, extensive comments will be left out.
	Non-trivial portions of code will be commented.

	Most portions of this code were developed in order, top-down.
	For instance, Expr was created first, then Number, then Constant (to unite all other types).
	
	One assumes that any readers will be familiar with C++ and some degree of the grammar language.
	----
*/

std::string Node::cppCode_h() {
	// nothing
	return "";
}
std::string Node::cppCode_cpp() {
	// nothing
	return "";
}

/*
	----
	Expr implementation.
	----
*/

Expr::Expr() {}
Expr::~Expr() {}

Constant::Constant(std::string lexeme) {
	this->value = lexeme;
}
Constant::~Constant() {};

std::string Constant::get_value() {
	return this->value;
}

std::string Constant::cppCode_cpp() {
	return this->value;
}

/*
	----
	Constant implementation.
	These constructors all do the same things, setup their respective lexeme value.
	----
*/

Number::Number(std::string lexeme): Constant(lexeme) {}
Integer::Integer(std::string lexeme): Number(lexeme) {}
Float::Float(std::string lexeme): Number(lexeme) {}
String::String(std::string lexeme): Constant(lexeme) {}
Char::Char(std::string lexeme): Constant(lexeme) {}
Bool::Bool(std::string lexeme): Constant(lexeme) {}


/*
	----
	Comparison implementation.

	Comparison relational operators have left and right Expr branches.
	These are accessible by get_left and get_right.
	----
*/

Comparison::Comparison(Expr *l, Expr *r) {
	this->left = l;
	this->right = r;
	this->op = "";
}
Comparison::~Comparison() {};
Expr* Comparison::get_left() {
	return this->left;
}
Expr* Comparison::get_right() {
	return this->right;
}
std::string Comparison::cppCode_cpp() {
	std::string output("");
	output = output + " " + this->get_left()->cppCode_cpp() + " " + this->get_operator() + " " + this->get_right()->cppCode_cpp() + " "; 
	return output;
}

/*
	----
	Comparison implementation.

	Setup the subclasses of Comparison, they all do the same thing.
	----
*/

Equals::Equals(Expr *l, Expr *r): Comparison(l, r) {
	this->op = "==";
}

LessThan::LessThan(Expr *l, Expr *r): Comparison(l, r) {
	this->op = "<";
}

LessEqualsThan::LessEqualsThan(Expr *l, Expr *r): Comparison(l, r) {
	this->op = "<=";
}

GreaterThan::GreaterThan(Expr *l, Expr *r): Comparison(l, r) {
	this->op = ">";
}

GreaterEqualsThan::GreaterEqualsThan(Expr *l, Expr *r): Comparison(l, r) {
	this->op = ">=";
}

NotEquals::NotEquals(Expr *l, Expr *r): Comparison(l, r) {
	this->op = "!=";
}


/*
	----
	Operator implementation.

	Mathematical operators have left and right Expr branches like Comparison operators.
	These are accessible by get_left and get_right.
	----
*/

Operator::Operator(Expr *l, Expr *r) {
	this->left = l;
	this->right = r;
}
Operator::~Operator() {};
Expr* Operator::get_left() {
	return this->left;
}
Expr* Operator::get_right() {
	return this->right;
}
std::string Operator::cppCode_cpp() {
	std::string output("");

	output = output + " " + this->get_left()->cppCode_cpp() + " " + this->get_operator() + " " + this->get_right()->cppCode_cpp() + " "; 
	return output;
}

/*
	----
	Operator implementation.

	Setup the subclasses of Operator, they all do the same thing.
	----
*/

Add::Add(Expr *l, Expr *r): Operator(l, r) {
	this->op = "+";
}
Multiply::Multiply(Expr *l, Expr *r): Operator(l, r) {
	this->op = "*";
}
Divide::Divide(Expr *l, Expr *r): Operator(l, r) {
	this->op = "/";
}
Subtract::Subtract(Expr *l, Expr *r): Operator(l, r) {
	this->op = "-";
}

/*
	----
	Variable implementation.

	Variable only contains a name.
	----
*/

Variable::Variable(std::string name) {
	this->name = name;
};
std::string Variable::get_name() {
	return this->name;
}
void Variable::set_on_platform(bool b) {
	this->on_platform = b;
}
bool Variable::is_on_platform() {
	return on_platform;
}
std::string Variable::cppCode_cpp() {
	if ( this->is_on_platform() ) {
		return "platform->get_" + this->get_name() + "()";
	} else {
		return "this->" + this->get_name();
	}
}


/*
	----
	Stmt implementation.

	Stmt requires either Variable, Expr or nothing. If it is the latter, it is an "empty Stmt",
	which means it has no values on its own, and it has no subsequent values in its list.
	Stmt(Variable, Expr): normal statement
	Stmt(): empty stmt

	Stmt is a linked list structure. The list is accessible by get_next.
	----
*/

Stmt::Stmt(Variable *var, Expr *expr) {
	this->var = var;
	this->expr = expr;
	this->empty = false;
}
Stmt::Stmt() {
	this->var = NULL;
	this->expr = NULL;
	this->next = NULL;
	this->empty = true;
}
Variable* Stmt::get_variable() {
	return this->var;
}
Expr* Stmt::get_expr() {
	return this->expr;
}
/*
	----
	set_next
	This method will set the private member variable "next" to the next Stmt in the list.
	----
*/
void Stmt::set_next(Stmt *n) {
	this->next = n;
}
/*
	----
	get_next
	This method will get the private member variable "next".
	----
*/
Stmt* Stmt::get_next() {
	return this->next;
}
bool Stmt::is_empty() {
	return this->empty;
}
/*
	----
	has_next
	This method returns a bool based on if the "next" property is non-null and points to another Stmt.
	----
*/
bool Stmt::has_next() {
	return ( this->next != NULL );
}

/*
	----
	Transition implementation.

	Transition requires one of three series of arguments:
		Transition(Variable, Expr, Stmt): normal Transition
		Transition(Expr, Stmt): Exit Transition
		Transition(): empty Transition

	Transition is a linked list structure. The list is accessible by get_next.
	----
*/

Transition::Transition(Variable *v, Expr *e, Stmt *s) {
	this->var = v;
	this->expr = e;
	this->stmt = s;
	this->exitKwd = false;
	this->next = NULL;
	this->empty = false;
}
Transition::Transition(Expr *e, Stmt *s) {
	this->var = NULL;
	this->expr = e;
	this->stmt = s;
	this->exitKwd = true;
	this->next = NULL;
	this->empty = false;
}
Transition::Transition() {
	this->var = NULL;
	this->expr = NULL;
	this->exitKwd = false;
	this->next = NULL;
	this->empty = true;
}
Variable* Transition::get_variable() {
	return this->var;
}
Expr* Transition::get_expr() {
	return this->expr;
}
Stmt* Transition::get_stmt() {
	return this->stmt;
}

void Transition::set_next(Transition *n) {
	this->next = n;
}

Transition* Transition::get_next() {
	return this->next;
}

bool Transition::is_empty() {
	return this->empty;
}

bool Transition::is_exit() {
	return this->exitKwd;
}
bool Transition::has_next() {
	return ( this->next != NULL );
}

/*
	----
	State implementation.

	State requires one of two series of arguments:
		State(Variable, Transition, bool): normal state
		State(): empty state
	If no arguments are given, the State is empty, and there is no list.
	Otherwise, the bool "i" will configure the "initial" state of this State.

	State is a linked list structure. The list is accessible by get_next.
	----
*/

State::State(Variable *v, Transition *t, bool i) {
	this->var = v;
	this->transition = t;
	this->next = NULL;
	this->initial = i;
	this->empty = false;
}
State::State() {
	this->var = NULL;
	this->transition = NULL;
	this->next = NULL;
	this->initial = false;
	this->empty = true;
}
State::~State() {}

Variable* State::get_variable() {
	return this->var;
}
Transition* State::get_transition() {
	return this->transition;
}
State* State::get_next() {
	return this->next;
}
bool State::has_next() {
	return ( this->next != NULL );
}
bool State::is_initial() {
	return this->initial;
}
bool State::is_empty() {
	return this->empty;
}
void State::set_next(State *s) {
	this->next = s;
}

/*
	----
	Type implementation.

	Type just requires a string to infer the type it represents.
	For example, "int", "float" and so on. No subclass fancies.
	----
*/

Type::Type(std::string s) {
	this->type = s;
}
std::string Type::get_type() {
	return this->type;
}

/*
	----
	DeclList implementation.

	DecList is an abstract class that does nothing, but the subclasses are used.
	----
*/

DeclList::DeclList() {}
DeclList::~DeclList() {}

/*
	----
	SeqDecl implementation.

	SeqDecl implements a head-tail structure. A Decl is the current head and a SeqDecl acts as the tail.
	The parser will recursively construct a DecList, a series of SeqDecl objects and at the end, it will add a
	NoDecl object. Looping that structure is easy, it involves simply for checking for null.
	----
*/

SeqDecl::SeqDecl(Decl *d, DeclList *tail) {
	this->decl = d;
	this->tail = tail;
}
DeclList* SeqDecl::get_tail() {
	return this->tail;
}
Decl* SeqDecl::get_decl() {
	return this->decl;
}
void SeqDecl::irrelevant() {}

std::string SeqDecl::cppCode_h() {
	return this->decl->cppCode_h();
}

/*
	----
	NoDecl implementation.
	
	NoDecl implies the list is empty and there isn't even one. This is similar to the way State works, but with NULLs.
	NoDecl is also the terminating object in a SeqDecl sequence.
	----
*/

NoDecl::NoDecl() {}
NoDecl::~NoDecl() {}
void NoDecl::irrelevant() {}

/*
	----
	Decl implementation.
	
	Decl requires a Type and Variable. This is just a data class. It is embeded in SeqDecl usually.
	----
*/

Decl::Decl(Type *t, Variable *v) {
	this->type = t;
	this->var = v;
}
Decl::~Decl() {}
Type* Decl::get_type() {
	return this->type;
}
Variable* Decl::get_variable() {
	return this->var;
}
std::string Decl::cppCode_h() {
	return "\t\t" + this->type->get_type() + " " + this->get_variable()->get_name() + ";\n";
}

/*
	----
	Platform implementation.
	
	Platform only requires a Variable.
	----
*/

Platform::Platform(Variable* v) {
	this->var = v;
}
Platform::~Platform() {}
Variable* Platform::get_variable() {
	return this->var;
}


/*
	----
	Program implementation.
	----
*/
Program::Program(Variable *v, Platform *p, DeclList *d, State *s) {
	this->var = v;
	this->platform = p;
	this->decls = d;
	this->states = s;
	
	/*
		The counters that the Program has via the common interface are set here.
	*/
	this->decls_count = this->get_decl_count();
	this->variable_uses = this->get_variable_uses();
	this->states_count = this->get_state_count();
}


void Program::set_variable_origins(Variable *v) {
	std::string name(v->get_name());

	if ( this->symbol_table.find(name) == this->symbol_table.end() ) {
		v->set_on_platform(true);
	} else {
		v->set_on_platform(false);
	}
}

void Program::set_decl_origins(Decl *d) {
	this->symbol_table[d->get_variable()->get_name()] = true;
}


Program::Program() {}
Program::~Program() {}


Variable* Program::get_variable() {return this->var;}
DeclList* Program::get_decls() {return this->decls;}
State* Program::get_states() {return this->states;}
Platform* Program::get_platform() {return this->platform;}

std::string Program::getName() {
	return this->var->get_name();
}

std::string Program::cppCode_cpp() {
	std::string output("");

	output = output + _cpp_includes();
	output = output + _cpp_constructor_deconstructor(this);

	// states
	output = output + _cpp_states(this);

	// main
	output = output + _cpp_main(this);

	output = output + "\n\n\n";

	return output;
}

/*
	These _header_ methods are temporary development locations
	for generating the header file code.
*/


std::string Program::cppCode_h() {
	
	std::string output("");

	output = output + _header_ifndef_open();
	output = output + _header_machine_class_open(this);

	output = output + _header_machine_public();
	output = output + _header_machine_constructor_deconstructor(this);

	// decls
	output = output + _header_machine_decls(this);

	// states
	output = output + _header_machine_states(this);

	// end stuff
	output = output + _header_machine_private(this);
	output = output + _header_machine_class_close();

	output = output + _header_machine_main();

	output = output + _header_ifndef_close();

	return output;

}

/*
	----
	getNumStates and get_state_count:
	getNumState is the public interface and is user-callable,
	get_state_count is private and is called by the Program constructor.
	----
*/

int Program::getNumStates() {
	return this->states_count;
}

int Program::get_state_count() {
	State *s = this->get_states();

	if ( s->is_empty() ) return 0;
	int count = 0;
	while (s) {
		count = count + 1;
		s = s->get_next();
	}

	return count;
}

/*
	----
	getNumVarDecls and get_state_count:
	getNumVarDecls is the public interface and is user-callable,
	get_decl_count is private and is called by the Program constructor.
	----
*/

int Program::getNumVarDecls() {
	return this->decls_count;
}

int Program::get_decl_count() {
	DeclList *k = this->get_decls();

	int count = 0;
	while ( is_node_type<SeqDecl>(k) ) {

		this->set_decl_origins(((SeqDecl *)k)->get_decl());

		count = count + 1;
		k = ((SeqDecl *)k)->get_tail();
	}

	return count;
}

/*
	----
	getNumVarUses and get_state_count:
	getNumVarUses is the public interface and is user-callable,
	get_variable_uses is private and is called by the Program constructor.

	But get_variable_uses is more interesting than that.
	Flow:
		1) checks to see if the state is empty (no states at all)
		2) loops through all states
		3) in each state, check to see if each the Transition is empty or contains more
		4) in each Transition, loop through Stmt series
		5) recurse through each Expr from each Stmt (left and right branches)
		6) count variables in those Expr object
	Much of this logic is contained in the private _gvu methods that are overloaded
	to handle various linked list structures.
	All _gvu methods return integers.
	----
*/

int Program::getNumVarUses() {
	return this->variable_uses;
}

int Program::get_variable_uses() {

	State *state = this->get_states();
	if ( state->is_empty() ) return 0;

	int count = 0;

	while ( state ) {
		count = count + this->_gvu( state->get_transition() );
		state = state->get_next();
	}
	return count;
}

/*
	----
	_gvu(Expr*)
	This private method will recurse down the given AST Expr to count the number of Variables found.
	----
*/
int Program::_gvu(Expr *n) {

	int count = 0;
	if ( is_node_type<Variable>(n) ) {

		count = count + 1;
		Variable *v = dynamic_cast<Variable *>(n);

		this->set_variable_origins(v);

	} else if ( is_node_type<Operator>(n) ) {
		count = count + this->_gvu( ((Operator *)n)->get_left() );
		count = count + this->_gvu( ((Operator *)n)->get_right() );
	} else if ( is_node_type<Comparison>(n) ) {
		count = count + this->_gvu( ((Comparison *)n)->get_left()  );
		count = count + this->_gvu( ((Comparison *)n)->get_right() );
	} else if ( is_node_type<Constant>(n) ) {
		/*
			A Constant branch used primarily for debugging or for future use.
		*/
	} else {
		/*
			A terminal that is not: a variable, operator, comparison or constant.
		*/
	}

	return count;

}

/*
	----
	_gvu(Stmt*)
	Check to see if the Stmt is empty, otherwise, loop through each Stmt and count variables in its expression.
	----
*/
int Program::_gvu(Stmt *s) {
	if ( s->is_empty() ) return 0;
	int count = 0;

	while ( s ) {
		/*
			Important note:
				Originally, this function automatically counted an extra
				variable for the left hand side of the Stmt.
				Because we're now using the existing code for counting
				variables for also putting them in the symbol table,
				the revised version will count the variable in the LHS and
				also symbol map it.
		*/
		count = count + this->_gvu( s->get_variable() ) + this->_gvu( s->get_expr() );
		s = s->get_next();
	}

	return count;

}

/*
	----
	_gvu(Transition*)
	Check to see if the Transition is empty, otherwise, loop through each Transition, call _gvu on its Stmt and call _gvu on its Expression.
	----
*/
int Program::_gvu(Transition *t) {

	if ( t->is_empty() ) return 0;
	int count = 0;

	while ( t ) {
		/*
			Important note:
			Transitions have two sources of Exprs; Stmt series and its conditional Expr.
		*/
		count = count + this->_gvu( t->get_stmt() ); // Stmt list
		count = count + this->_gvu( t->get_expr() ); // Expr
		t = t->get_next();
	}

	return count;

}
