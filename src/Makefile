CXX_DIR = ../lib/cxxtest
CXXTEST = $(CXX_DIR)/cxxtestgen.pl
CXXFLAGS = --error-printer --abort-on-fail --have-eh

FLAGS = -Wall -g

# general files for the application, in literally no specific order.
readInput.o:	readInput.cpp readInput.h
	g++ $(FLAGS) -c readInput.cpp 

regex.o:	regex.cpp regex.h
	g++ $(FLAGS) -c regex.cpp 

scanner.o:	scanner.cpp scanner.h regex.h
	g++ $(FLAGS) -c scanner.cpp 

parseResult.o:	parseResult.cpp parseResult.h
	g++ $(FLAGS) -c parseResult.cpp

extToken.o: extToken.cpp parser.h
	g++ $(FLAGS) -c extToken.cpp

parser.o:	parser.cpp parser.h scanner.h parseResult.h scanner.h extToken.h ast.h translator.h
	g++ $(FLAGS) -c parser.cpp

ast.o:	ast.cpp ast.h translator.h
	g++ $(FLAGS) -c ast.cpp

translator.o:	translator.cpp translator.h ast.h
	g++ $(FLAGS) -c translator.cpp

# Testing files and targets.
run-tests:	regex_tests scanner_tests parser_tests ast_tests
	./regex_tests
	./scanner_tests
	./parser_tests
	./ast_tests

run-ast:	ast_tests
	./ast_tests

run-parser:	parser_tests
	./parser_tests

# regex tests
regex_tests.cpp:	regex.h regex_tests.h
	$(CXXTEST) $(CXXFLAGS) -o regex_tests.cpp regex_tests.h

regex_tests:	regex_tests.cpp regex.o
	g++ $(FLAGS) -I$(CXX_DIR) -o regex_tests regex.o regex_tests.cpp
# end regex tests

# scanner tests
scanner_tests.cpp:	scanner.o scanner_tests.h readInput.h
	$(CXXTEST) $(CXXFLAGS) -o scanner_tests.cpp scanner_tests.h

scanner_tests:	scanner_tests.cpp scanner.o regex.o readInput.o
	g++ $(FLAGS) -I$(CXX_DIR)  -o scanner_tests \
		scanner.o regex.o readInput.o scanner_tests.cpp
# end scanner tests

# parser tests
parser_tests.cpp:	scanner.o parser.o translator.o ast.o parser_tests.h extToken.o extToken.h ast.h
	$(CXXTEST) $(CXXFLAGS) -o parser_tests.cpp parser_tests.h

parser_tests:	parser_tests.cpp scanner.o parser.o parseResult.o translator.o ast.o ast.h extToken.o readInput.o regex.o parser.h
	g++ $(FLAGS) -I$(CXX_DIR)  -o parser_tests \
		scanner.o parser.o extToken.o regex.o readInput.o parseResult.o translator.o ast.o parser_tests.cpp
# end parser tests

# ast tests
ast_tests.cpp:	ast_tests.h ast.o scanner.o parser.o readInput.o extToken.o extToken.h regex.o parseResult.o translator.o
	$(CXXTEST) $(CXXFLAGS) -o ast_tests.cpp ast_tests.h

ast_tests:	ast_tests.h ast_tests.cpp scanner.o parser.o readInput.o extToken.o extToken.h regex.o parseResult.o translator.o
	g++ $(FLAGS) -I$(CXX_DIR)  -o ast_tests \
		ast_tests.cpp ast.o scanner.o parser.o extToken.o readInput.o regex.o parseResult.o translator.o
# end ast tests

# cffc
cffc:	cffc.cpp parser.o readInput.o ast.o extToken.o scanner.o regex.o parseResult.o translator.o
	g++ $(FLAGS) parser.o readInput.o ast.o scanner.o regex.o parseResult.o extToken.o cffc.cpp -o cffc translator.o
	cp cffc ../cffc/

cx:	cffc
	./cffc box.cff
	@echo "\n --- MACHINE H\n"
	@cat ../cffc/Machine.h
	@echo "\n --- MACHINE CPP\n"
	@cat ../cffc/Machine.cpp

clean:
	rm -Rf *.o \
	rm -Rf *.gch \
	regex_tests regex_tests.cpp \
	scanner_tests scanner_tests.cpp \
	parser_tests parser_tests.cpp \
	ast_tests ast_tests.cpp \
	cffc
