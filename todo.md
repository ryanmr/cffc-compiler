Todo
====

This is a non-exhaustive list of tasks left to complete.

* ~~move _cpp and _h translator calls into their own file~~ **DONE**
* ~~discouple AST classes and CFF to C++ generator methods as much as possible~~ **DONE**
* ~~move compiled function call into `platform->next_state`~~ **HARD**
* ~~put print out statements into `platform->enter_state` or `platform->next_state`~~ **DONE**
* ~~reduce memory leaks and conditional jumping based on uninitialized values (see valgrind)~~ **NOT OUR FAULT**
* ~~fix `Scanner::_consume` so that it leaks _less_~~ **DONE**
* ~~convert the _Map + Vector_ into just a singular array of regular expressions with `tokenType` keys~~ **HARD**


This a list of tasks that could be completed _for fun_ if time is available.


* create a RunTime method generator
* write a new more interesting (intensive) CFF programs to show off
	* fibonacci
	* infinity Positional Robot (e.g. figure eight pattern)
	* a program with hundreds of States, Transitions and Decls
* troll semi-colon newline
