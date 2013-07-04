
#include "RunTime.h"
#include "Machine.h"
#include "Machine_Box.h"
#include <string>
#include <iostream>

RunTime::RunTime() {};
RunTime::~RunTime() {};

void RunTime::println(std::string s) {
	std::cout << s << std::endl;
}

// ---

IntegerComputer::IntegerComputer() {}
IntegerComputer::~IntegerComputer() {}
int IntegerComputer::get_output() {
	std::cout << "get output: " << output << std::endl;
	return output;
}
void IntegerComputer::set_output(int i) {
	this->output = i;
	std::cout << "set output: " << output << std::endl;
}

int IntegerComputer::get_input() {
	std::cout << "get input: " << input << std::endl;
	return input;
}
void IntegerComputer::set_input(int i) {
	this->input = i;
	std::cout << "set input: " << input << std::endl;
}

PositionalRobot::PositionalRobot() {}
PositionalRobot::~PositionalRobot() {}

void PositionalRobot::set_yPos(float y) {
 	yPos = y;
  	std::cout << "set yPos: " << yPos << std::endl;
}
void PositionalRobot::set_xPos(float x) {
 	xPos = x;
  	std::cout << "set xPos: " << xPos << std::endl;
}

float PositionalRobot::get_yPos() {
	std::cout << "get yPos: " << yPos << std::endl;
  	return yPos;
}
float PositionalRobot::get_xPos() {
	std::cout << "get xPos: " << xPos << std::endl;
  	return xPos;
}





