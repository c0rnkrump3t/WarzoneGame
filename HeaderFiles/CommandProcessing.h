#ifndef COMP345_COMMANDPROCESSING_H
#define COMP345_COMMANDPROCESSING_H

#include "Command.h"
#include "ILoggable.h"
#include "Subject.h"

#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <map>
#include <fstream>

using namespace std;

// The command processor handles the states and the commands
// It is used by various other classes
// It is aware of the current state, available transitions and the commands history
// It also processes the transition from a certain state to another with the correpsonding command
class CommandProcessor : public ILoggable, public Subject {
protected:
	void saveCommand(Command*);

private:
	// History of previous commands
	list<Command*> CommandCollection;

	Config::State CurrentState;
	list<Config::CommandName> AvailableCommands;

	virtual Command* readCommand(); // only for handling input-tiggered commands
	void setAvailableCommands();

public:

	//constructors and destructors
	CommandProcessor();
	CommandProcessor(CommandProcessor&);
	CommandProcessor(Config::State&);
	virtual ~CommandProcessor();

	list<Command*> getCommandCollection();
	Command* getCurrentCommand();
	void setCurrentState(Config::State&);

	virtual bool validate();

	Config::State& getCurrentState();
	list<Config::CommandName> getAvailableCommands();
	void addCommandToCollection(Config::CommandName);
	void getCommand();
	void getCommand(Command*);

	static string toString(Config::CommandName);
	static string getArgument(Config::CommandName);
	static string toString(Config::State);
	string stringToLog();

};

class FileLineReader {
private:
	string currentLine;
public:
	FileLineReader();
	~FileLineReader();

	void setCurrentLine(string&);
	string getCurrentLine();

	string readLineFromFile(ifstream&);
};


class CommandProcessorAdapter : public CommandProcessor {
private:
	FileLineReader* flr;
	string FileName;
	ifstream CurrentFile;

public:
	CommandProcessorAdapter();
	CommandProcessorAdapter(string&);
	~CommandProcessorAdapter();

	virtual Command* readCommand();
	virtual bool validate();

	void setFileName(string& name);
	string getFileName();

	FileLineReader* getFlr();
	ifstream& getCurrentFile();
};

class CommandProcessorTournament : public CommandProcessor {
private:
	vector<string> listOfMaps;
	vector<string> listOfPlayers;
	int numberOfGames;
	int numberOfTurns;

	string parseMaps(string&);
	string parsePlayers(string&);
	string parseNumGames(string&);
	string parseNumTurns(string&);
	bool validateTournament(string&);

public:
	CommandProcessorTournament();
	CommandProcessorTournament(string&);
	~CommandProcessorTournament();
	
	void setListOfMaps(vector<string>);
	void addToListOfMaps(string);
	vector<string> getListOfMaps();
	void setListOfPlayers(vector<string>);
	void addToListOfPalyers(string);
	vector<string> getListOfPlayers();
	void setNumberOfGames(int);
	int getNumberOfGames();
	void setNumberOfTurns(int);
	int getNumberOfTurns();
	bool validatePlayerType(string);
};

//istream overload operator to take the input as a command object
//ostream overload operators to output either the command or state enum types as string, or also the command processor object with available commands from that state 
ostream& operator<<(ostream&, const Config::CommandName&);
ostream& operator<<(ostream&, const Config::State&);
ostream& operator<<(ostream&, CommandProcessor&);
istream& operator>>(istream&, Command*);


void testCommandProcessor(); // Free function for testing

#endif //COMP345_COMMANDPROCESSING_H