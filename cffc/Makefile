compile:	
	make --no-print-directory -f Makefile_Robot machine

test:
	make --no-print-directory -f Makefile_Tests all

clean:
	make --no-print-directory -f Makefile_Robot clean
	rm Machine.h Machine.cpp cffc *.out

save:
	./cffc ../samples/abstar.cff
	cat Machine.h > ../generated/abstar.cff.h
	cat Machine.cpp > ../generated/abstar.cff.cpp
	./cffc ../samples/sumOfSquares.cff
	cat Machine.h > ../generated/sumOfSquares.cff.h
	cat Machine.cpp > ../generated/sumOfSquares.cff.cpp
	./cffc ../samples/squareMapper.cff
	cat Machine.h > ../generated/squareMapper.cff.h
	cat Machine.cpp > ../generated/squareMapper.cff.cpp
	./cffc ../samples/box.cff	
	cat Machine.h > ../generated/box.cff.h
	cat Machine.cpp > ../generated/box.cff.cpp

valgrind:
	valgrind -v --leak-check=full ./cffc sumOfSquares.cff > ../cffc-valgrind.log 2>&1
