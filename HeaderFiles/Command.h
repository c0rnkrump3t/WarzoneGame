#ifndef COMP345_COMMAND_H
#define COMP345_COMMAND_H

#include "Config.h"
#include "ILoggable.h"
#include "Subject.h"
#include "LogObserver.h"

#include <iostream>
#include <string>

using namespace std;

// Command class handled with inheritance: each subclass corresponds to a command
// First the input-triggered commands, second the game-triggered events
class Command : public ILoggable, public Subject {
private:
	Config::CommandName command;
	string argument;
	string effect;
	LogObserver* lo;

public:
	Command();
	Command(string&);
	Command(Config::CommandName);
	~Command();

	void setCommand(Config::CommandName);
	Config::CommandName getCommand();
	void setArgument(string argument);
	string getArgument();
	void saveEffect();
	void saveEffect(const string&);
	string getEffect();
	string stringToLog();

	friend istream& operator>>(istream&, Command*);

};

#endif //COMP345_COMMAND_H