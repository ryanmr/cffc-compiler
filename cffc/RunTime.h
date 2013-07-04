#ifndef RUNTIME_H
#define RUNTIME_H

#include <cstdio>
#include <string>

class RunTime {
	public:
		RunTime(int argc, char **argv);
		virtual ~RunTime();

		virtual void enter_state();
		virtual void next_state();
};

class IntegerComputer : public RunTime {
	public:
		IntegerComputer(int argc, char **argv);
		~IntegerComputer();

		void enter_state();
		void next_state();		

		int get_output();
		int get_input();

		void set_output(int i);
		void set_input(int i);

	private:
		int output;
		int input;
};

class RegexRecognizer : public RunTime {
	public:
		RegexRecognizer(int argc, char **argv);
		~RegexRecognizer();

		void enter_state();
		void next_state();

		char get_nextChar();
		void set_outputBuffer(std::string s);


	private:
		int index;
		char nextChar;
		std::string ibuffer;
		std::string obuffer;

};


class IntegerStreamComputer : public RunTime {
		public:
		IntegerStreamComputer(int argc, char **argv);
		~IntegerStreamComputer();

		void enter_state();
		void next_state();

		void set_output(int n);
		int get_input();

	private:
		int index;
		int numInputs;
		char **inputStrings;
		int input;
		int output;
};

class PositionalRobot : public RunTime {
  public:
    PositionalRobot(int argc, char **argv);
    ~PositionalRobot();

    void enter_state();
    void next_state();

    float get_yPos();
    float get_xPos();
    void set_yPos(float y);
    void set_xPos(float x);

  private:
    float yPos;
    float xPos;
};


#endif