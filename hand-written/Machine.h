#ifndef MACHINE_H
#define MACHINE_H

class Machine {

	public:
		Machine(IntegerComputer *ic);
		~Machine();
		int i;
		int s;

		void Start();
		void Compute();
	private:
		IntegerComputer *r;

};

int main();

#endif
