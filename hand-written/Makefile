
Machine.o:	Machine.cpp Machine.h RunTime.h
	g++ -c Machine.cpp

RunTime.o:	RunTime.cpp RunTime.h
	g++ -c RunTime.cpp

machine:	Machine.o RunTime.o
	g++ -g Machine.o RunTime.o -o machine

Machine_Box.o:	Machine_Box.cpp Machine_Box.h RunTime.h
	g++ -c Machine_Box.cpp

machine2:	Machine_Box.o RunTime.o
	g++ -g Machine_Box.o RunTime.o -o machine2

run:	machine
	./machine

run2:	machine2
	./machine2

clean:
	rm -f *.o machine machine2
