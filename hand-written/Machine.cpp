#include "RunTime.h"
#include "Machine.h"

#include <iostream>

Machine::Machine(IntegerComputer *ic) {
	r = ic;
}
Machine::~Machine() {}

void Machine::Start() {
	if ( true ) {
		i = 0;
		s = 0;
		Compute();
	}
}

void Machine::Compute() {

	if (i <= r->get_input()) {
		s = s + i * i;
		i = i + 1;
		Compute();
	} else if (true) {
		r->set_output(s);
	}

}

int main() {

	IntegerComputer *r = new IntegerComputer(); // RunTime;
	r->set_input(10);
	
	Machine *m = new Machine(r);
	m->Start();

	return 0;
}
