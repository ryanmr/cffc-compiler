
#include "RunTime.h"
#include <iostream>
#include <string>
#include <stdlib.h>

RunTime::RunTime(int argc, char **argv) {
	// nothing
}
RunTime::~RunTime() {}
void RunTime::enter_state() {}
void RunTime::next_state() {}

/*
	IntegerComputer
*/
IntegerComputer::IntegerComputer(int argc, char **argv) : RunTime(argc, argv) {
	sscanf( argv[1], "%d", &input);
	output = 0;
}
IntegerComputer::~IntegerComputer() {}

void IntegerComputer::enter_state() {}
void IntegerComputer::next_state() {std::cout << this->output << std::endl;}

int IntegerComputer::get_output() {return this->output;}
void IntegerComputer::set_output(int n) {this->output = n;}
int IntegerComputer::get_input() {return this->input;}
void IntegerComputer::set_input(int n) {this->input = n;}

/*
	RegexRecognizer
*/
RegexRecognizer::RegexRecognizer(int argc, char **argv) : RunTime(argc, argv) {
	ibuffer = argv[1];
	index = 0;
	obuffer = "";
}
RegexRecognizer::~RegexRecognizer() {}

void RegexRecognizer::enter_state() {}

void RegexRecognizer::next_state() {
	this->index++;
	if ( !this->obuffer.empty() ) {
		std::cout << this->obuffer << std::endl;
	}
	this->obuffer = "";
}

void RegexRecognizer::set_outputBuffer(std::string s) {this->obuffer = s;}
char RegexRecognizer::get_nextChar() {return this->ibuffer[ index ];}

/*
	IntegerStreamComputer
*/
IntegerStreamComputer::IntegerStreamComputer(int argc, char **argv) : RunTime(argc, argv) {
	index = 1;
	numInputs = argc - 1;
	inputStrings = argv;
}
IntegerStreamComputer::~IntegerStreamComputer() {}

void IntegerStreamComputer::enter_state() {
	sscanf( this->inputStrings[this->index], "%d", &this->input );
}
void IntegerStreamComputer::next_state() {
	std::cout << this->output << std::endl;
	if (this->index == this->numInputs) exit(0);
	this->index++;
}

void IntegerStreamComputer::set_output(int n) {this->output = n;}
int IntegerStreamComputer::get_input() {return this->input;}

/*
	PositionalRobot
*/

PositionalRobot::PositionalRobot(int argc, char **argv) : RunTime(argc, argv) {
	// nothing
}
PositionalRobot::~PositionalRobot() {}

void PositionalRobot::enter_state() {
	
}
void PositionalRobot::next_state() {
	std::cout << "  XPos: " << this->xPos << "  YPos: " << this->yPos << std::endl ;
}

void PositionalRobot::set_xPos(float f) {this->xPos = f;}
void PositionalRobot::set_yPos(float f) {this->yPos = f;}

float PositionalRobot::get_xPos() {return this->xPos;}
float PositionalRobot::get_yPos() {return this->yPos;}

