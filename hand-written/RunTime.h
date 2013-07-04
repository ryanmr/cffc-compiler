
#ifndef RUNTIME_H
#define RUNTIME_H

#include <string>

class RunTime {
	public:
		RunTime();
		~RunTime();

		void println(std::string s);
};

class IntegerComputer : public RunTime {
	public:
		IntegerComputer();
		~IntegerComputer();


		int get_output();
		int get_input();

		void set_output(int i);
		void set_input(int i);

	private:
		int output;
		int input;
};

class PositionalRobot : public RunTime {
  public:
    PositionalRobot();
    ~PositionalRobot();

    float get_yPos();
    float get_xPos();
    void set_yPos(float y);
    void set_xPos(float x);

  private:
    float yPos;
    float xPos;
};

#endif
