
#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <map>


/*
	----
	Node is the top level superclass. It is used exclusively in ParseResult.ast
	----
*/

class Node {
	public:
		virtual ~Node() {};
		virtual std::string cppCode_h();
		virtual std::string cppCode_cpp();
};

/*
	----
	Expr is a core superclass that is "abstract" in nature.
	(It's not abstract here, that will be added in Iteration 4 when there different implementations for subclasses.)
	----
*/

class Expr: public Node {
	public:
		Expr();
		virtual ~Expr();
};

/*
	----
	Constant is a subclass of Expression (Expr).
	Constants include a lexeme string, and a method to get that value.

	Constants: Number (Float or Integer), String, Char and Bool.
	----
*/

class Constant: public Expr {
	public:
		Constant(std::string lexeme);
		virtual ~Constant();
		virtual std::string get_value();

		virtual std::string cppCode_cpp();

	protected:
		std::string value;
};

/*
	----
	Number is a superclass of both Float and Integer. It is unused, but is avaialable for convenience.
	----
*/

class Number : public Constant {
	public:
		Number(std::string lexeme);
};

class Integer: public Number {
	public:
		Integer(std::string lexeme);
};

class Float: public Number {
	public:
		Float(std::string lexeme);
};

class String: public Constant {
	public:
		String(std::string lexeme);
};

class Char: public Constant {
	public:
		Char(std::string lexeme);
};

class Bool: public Constant {
	public:
		Bool(std::string lexeme);
};

/*
	----
	Comparison is a core class of Expr. It contains relational operators.
	Each Comparison operator has a left and right Expr and methods to fetch those.

	Comparison operators include: Equals, LessThan, LessEqualsThan, GreaterThan, GreaterEqualsThan and NotEquals.
	----
*/

class Comparison: public Expr {
	public:
		Comparison(Expr *left, Expr *right);
		virtual ~Comparison();
		virtual Expr* get_left();
		virtual Expr* get_right();

		virtual std::string get_operator() {
			return this->op;
		}

		virtual std::string cppCode_cpp();

	private:
		Expr* left;
		Expr* right;

	protected:
		std::string op;
};

class Equals: public Comparison {
	public:
		Equals(Expr *left, Expr *right);
};

class LessThan: public Comparison {
	public:
		LessThan(Expr *left, Expr *right);
};

class LessEqualsThan: public Comparison {
	public:
		LessEqualsThan(Expr *left, Expr *right);
};

class GreaterThan: public Comparison {
	public:
		GreaterThan(Expr *left, Expr *right);
};

class GreaterEqualsThan: public Comparison {
	public:
		GreaterEqualsThan(Expr *left, Expr *right);
};

class NotEquals: public Comparison {
	public: 
		NotEquals(Expr *left, Expr *right);
};

/*
	----
	Operator is a core class of Expr. It contains left and right Expr fields.
	It supports: Add, Multiply, Divide and Subtract.
	----
*/

class Operator: public Expr {
	public:
		Operator(Expr *left, Expr *right);
		virtual ~Operator();
		virtual Expr* get_left();
		virtual Expr* get_right();

		virtual std::string get_operator() {
			return this->op;
		}

		virtual std::string cppCode_cpp();

	private:
		Expr* left;
		Expr* right;

	protected:
		std::string op;
};

class Add: public Operator {
	public:
		Add(Expr *left, Expr *right);
};
class Multiply: public Operator {
	public:
		Multiply(Expr *left, Expr *right);
};
class Divide: public Operator {
	public:
		Divide(Expr *left, Expr *right);
};
class Subtract: public Operator {
	public:
		Subtract(Expr *left, Expr *right);
};

/*
	----
	Variable is a unique Expression (Expr).
	Variable only has a name and no internal value.
	----
*/

class Variable: public Expr {
	public:
		Variable(std::string name);
		virtual std::string get_name();

		virtual bool is_on_platform();
		virtual void set_on_platform(bool b);

		virtual std::string cppCode_cpp();

	private:
		std::string name;
		bool on_platform;
};

/*
	----
	Stmt is a subclass of Node.
	Stmt contains a left-hand-side Variable and a right-hand-side Expr.
		Var := Expr
	Stmt can also contain a pointer to another Stmt in the form of a linked list.
	----
*/

class Stmt: public Node {
	public:
		Stmt(Variable *var, Expr *expr);
		Stmt();
		virtual Variable* get_variable();
		virtual Expr* get_expr();

		virtual Stmt* get_next();
		virtual bool has_next();
		virtual void set_next(Stmt *n);

		virtual bool is_empty();
	private:
		Variable *var;
		Expr *expr;
		Stmt *next;
		bool empty;
};


/*
	----
	Transition is a subclass of Node.
	Transition contains a series of Stmt objects and optionally an Variable and Expr.

	If the Variable and Expr are not present, this Transition is marked as "exit".
	----
*/

class Transition: public Node {
	public:
		Transition(Variable *var, Expr *expr, Stmt* stmt);
		Transition(Expr *expr, Stmt* stmt);
		Transition();
		virtual Variable* get_variable();
		virtual Expr* get_expr();
		virtual Stmt* get_stmt();

		virtual Transition* get_next();
		virtual bool has_next();
		virtual void set_next(Transition *t);

		virtual bool is_exit();
		virtual bool is_empty();
	private:
		bool exitKwd;
		Variable* var;
		Expr* expr;
		Stmt* stmt;
		Transition* next;
		bool empty;
};

/*
	----
	State is a subclass of Node.
	State contains a series of Transition objects and a Variable.

	State can optionally have an initial marker.
	----
*/

class State: public Node {
	public:
		State(Variable *var, Transition *transition, bool initial);
		State();
		~State();
		virtual Variable* get_variable();
		virtual Transition* get_transition();

		virtual State* get_next();
		virtual bool has_next();
		virtual void set_next(State *s);

		virtual bool is_initial();
		virtual bool is_empty();
	private:
		bool initial;
		Variable* var;
		Transition* transition;
		State* next;
		bool empty;
};


/*
	----
	Type is a subclass of Node. It is only used in Decl to define the type.
	They are simply string based.	
	----
*/

class Type: public Node {
	public:
		Type(std::string t);
		std::string get_type();
	private:
		std::string type;
};

/*
	----
	Decl is a subclass of Node. Decl needs a Type and a Variable.
	The "linked list" magic happens in DeclList, NoDecl and SeqDecl.
	----
*/

class Decl: public Node {

	public:
		Decl(Type *t, Variable *v);
		~Decl();
		Variable* get_variable();
		Type* get_type();

		virtual std::string cppCode_h();

	protected:
		Type* type;
		Variable* var;

};

/*
	----
	DeclList is a subclass of Node. DeclList is an "abstract" class.
	----
*/

class DeclList: public Node {

	public:
		DeclList();
		~DeclList();
		virtual void irrelevant() = 0;

};

/*
	----
	SeqDecl is a subclass of DeclList.
	SeqDecl implements a linked list and contains a decl inside.
	----
*/

class SeqDecl: public DeclList {
	public:
		SeqDecl(Decl* decl, DeclList* tail);

		Decl* get_decl();

		DeclList* get_tail();

		virtual void irrelevant();

		virtual std::string cppCode_h();
		// virtual std::string cppCode_cpp();

  private:
    DeclList* tail;
    SeqDecl* next;
    Decl* decl;
};

/*
	----
	NoDecl is a subclass of DeclList. It literally is nothing.
	NoDecl infers there are no decls in the list, within or otherwise.
	----
*/

class NoDecl: public DeclList {
	public:
		NoDecl();
		~NoDecl();
		virtual void irrelevant();
};

/*
	----
	Platform is a subclass of Node. Platform needs a variable name.
	----
*/

class Platform: public Node {
	public:
		Platform(Variable* v);
		~Platform();
		Variable* get_variable();
	private:
		Variable* var;
};


/*
	----
	Program is a subclass of Node. It is also the class that everything stems from aside from Node.
	Program contains methods to get all of its properties, Platform, DeclList and States, along with its name.

	In addition, it has the common interface, counts of various object lists.
	----
*/

class Program: public Node {
	public:
		Program(Variable *v, Platform *p, DeclList *d, State *s);
		Program(); // just for now
		~Program();

		Variable* get_variable();
		Platform* get_platform();
		DeclList* get_decls();
		State* get_states();
		
		int getNumStates();
		int getNumVarDecls();
		int getNumVarUses();

		std::string getName();

		void set_variable_origins(Variable *v);
		void set_decl_origins(Decl *d);

		// --- Code Generation ---
		virtual std::string cppCode_cpp();
		virtual std::string cppCode_h();
		// --- Code Generation ---

	private:
		std::map<std::string, bool> symbol_table;
		Variable* var;
		Platform* platform;
		DeclList* decls;
		State* states;

		// these private member variables are set in the 
		// program constructor
		int variable_uses;
		int decls_count;
		int states_count;
		
		// these private methods are called in the constructor to
		// populate the variables above
		int get_variable_uses();
		int get_decl_count();
		int get_state_count();

		/*
			_gvu methods are private.
			These calculate internally the number of variables used.
			gvu stands for get_variable_uses
		*/
		int _gvu(Expr *n);
		int _gvu(Stmt *n);
		int _gvu(Transition *n);


};

/*
	----
	is_node_type is a wonderful helper function that returns true if the object is of the template type.
	----
*/

template <class X>
bool is_node_type(Node *node) {
    X *result = dynamic_cast<X *>(node);
    return ( result != NULL );
}

#endif
