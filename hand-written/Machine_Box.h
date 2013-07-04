
#ifndef MACHINE_BOX_H
#define MACHINE_BOX_H

class Machine_Box {

	public:
	Machine_Box(PositionalRobot *ic);
	~Machine_Box();
	int timesAround;

    void Init();
    void MoveNorth();
    void MoveEast();
    void MoveSouth();
    void MoveWest();

	private:
    PositionalRobot *r;

};

int main();

#endif
