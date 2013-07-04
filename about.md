The Project
===========


So far in the project, we have made the following things directly or indirectly:

1. Scanner: scans tokens, ignores whitespace and comments, and prepares them for the parser
2. Parser: while given to us by the professor, this is a simple recursive parser that parses the tokens and creates the Abstract Syntax Tree (AST)
3. The AST functions as an intermediary between the previous syntax and the C++ syntax we will output

The Scanner
-----------

The scanner uses the given enumeration of token types to parse a text file full. The scanner has some minor logic in it that determine if there is an invalid token.

The Parser
----------

The parser is more interesting. The parser contains links to [Crockford's parser](http://javascript.crockford.com/tdop/tdop.html) which is really quite extraordinary. Since the parser is recursive, it is a travesty, but it is actually quite elegant despite that. Essentially, the parser looks for a sequence of tokens and builds the AST if those tokens are found in a correct sequence. If there is an unexpected token, an exception is thrown and we call it a day.

The Abstract Syntax Tree
------------------------

The AST was based on the UML diagram drawing assignment. The AST a set of classes that manages the data found from the parser. It creates links between related objects. For instance, `4 < 9`, would create: Integer, LessThanOperator and Integer class objects. All these objects are derivatives of the Node superclass that are placed in the tree.

There are many classes in the AST. There are Constant classes for unchanging values, a Variable class that contains the name of the variable and a flag to indicate if its origin is in the platform or not, two sets of operator classes, those for comparison and those for basic math functions, and higher level structures such as States, Transitions and Declarations. All of these reside in the highest level structure, the Program.

The Translator
--------------

This is the final step of the project. That is to take the AST and generate standard c++ compliant code and save it to a file.

