
#include "RunTime.h"
#include "Machine_Box.h"

#include <iostream>

Machine_Box::Machine_Box(PositionalRobot *pr) {
	r = pr;
}
Machine_Box::~Machine_Box() {}

void Machine_Box::Init() {
  if (true) {
    timesAround = 0;
    r->set_xPos(0.0);
    r->set_yPos(0.0);
    MoveNorth();
  }
}

void Machine_Box::MoveNorth() {
 if (timesAround == 4) {

 } else if ( r->get_yPos() >= 100 ) {
   MoveEast();
 } else if (true) {
   r->set_yPos( r->get_yPos() + 1.0  );
   MoveNorth();
 }
 
}

void Machine_Box::MoveEast() {

  if ( r->get_xPos() >= 100) {
    MoveSouth();
  } else if (true) {
    r->set_xPos( r->get_xPos() + 1.0  );
    MoveEast();
  }

}


void Machine_Box::MoveSouth() {
  
  if ( r->get_yPos() <= 0.0 ) {
    MoveWest();
  } else if (true) {
    r->set_yPos( r->get_yPos() - 1.0 );
    MoveSouth();
  }

}

void Machine_Box::MoveWest() {

  if ( r->get_xPos() <= 0.0 ) {
    timesAround = timesAround + 1;
    MoveNorth();
  } else if (true) {
    r->set_xPos( r->get_xPos() - 1.0 );
  }

}


int main() {

	PositionalRobot *r = new PositionalRobot(); // RunTime;
	
	Machine_Box *m = new Machine_Box(r);
	m->Init();

	return 0;
}
