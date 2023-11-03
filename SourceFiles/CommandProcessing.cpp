#include "../HeaderFiles/CommandProcessing.h"
#include "../HeaderFiles/LogManager.h"
#include "../HeaderFiles/LogObserver.h"
#include <iostream>
#include <list>
#include <string>
#include <map>
#include <fstream>
#include <cctype>

using namespace std;

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
///////////// CommandProcessor class implementation  /////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

// CommandProcessor class implementation
// 
// Constructors and destructor
// Default constructor
CommandProcessor::CommandProcessor() {
	this->CommandCollection = {};
	this->CurrentState = Config::State::start;
	this->setAvailableCommands();
}

// Copy constructor
CommandProcessor::CommandProcessor(CommandProcessor& cp) {
	this->CommandCollection = cp.CommandCollection;
	this->CurrentState = cp.CurrentState;
	this->setAvailableCommands();
	cout << endl << *this;
}

// Constructor with state selection
CommandProcessor::CommandProcessor(Config::State& state) {
	this->CommandCollection = {};
	this->CurrentState = state;
	this->setAvailableCommands();
	readCommand();
	cout << endl << *this;
}

CommandProcessor::~CommandProcessor() {
	for (auto element : this->CommandCollection) {
		delete element;
	}
	this->CommandCollection.clear();
};

// public Command getter
Command* CommandProcessor::getCurrentCommand() {
	return this->CommandCollection.back();
}

void CommandProcessor::setCurrentState(Config::State& state) {
	this->CurrentState = state;
	setAvailableCommands();
	if (getCurrentState() == Config::State::END) {
		return;
	}
	this->getAvailableCommands();
}

Config::State& CommandProcessor::getCurrentState() {
	return this->CurrentState;
}

list<Command*> CommandProcessor::getCommandCollection() {
	return this->CommandCollection;
}

string CommandProcessor::toString(Config::State state) {
	switch (state) {
	case Config::State::start:
		return "start";
	case Config::State::mapLoaded:
		return "maploaded";
	case Config::State::mapValidated:
		return "mapvalidated";
	case Config::State::playersAdded:
		return "playersadded";
	case Config::State::assignReinforcment:
		return "assignreinforcement";
	case Config::State::issueOrders:
		return "issueorders";
	case Config::State::executeOrders:
		return "executeorders";
	case Config::State::winState:
		return "win";
	case Config::State::END:
		return "end";
	}
}

string CommandProcessor::toString(Config::CommandName command) {
	switch (command) {
	case Config::CommandName::NONE:
		return "none";
	case Config::CommandName::loadMap:
		return "loadmap";
	case Config::CommandName::validateMap:
		return "validatemap";
	case Config::CommandName::addPlayer:
		return "addplayer";
	case Config::CommandName::gameStart:
		return "gamestart";
	case Config::CommandName::issueOrder:
		return "issueorder";
	case Config::CommandName::endIssueOrders:
		return "endissueorders";
	case Config::CommandName::execOrder:
		return "execorder";
	case Config::CommandName::endExecOrders:
		return "endexecorders";
	case Config::CommandName::win:
		return "win";
	case Config::CommandName::replay:
		return "replay";
	case Config::CommandName::quit:
		return "quit";
	}
}

string CommandProcessor::getArgument(Config::CommandName command) {
	switch (command) {
	case Config::CommandName::loadMap:
		return "filename";
	case Config::CommandName::addPlayer:
		return "playername";
	case Config::CommandName::NONE:
	case Config::CommandName::validateMap:
	case Config::CommandName::gameStart:
	case Config::CommandName::issueOrder:
	case Config::CommandName::endIssueOrders:
	case Config::CommandName::execOrder:
	case Config::CommandName::endExecOrders:
	case Config::CommandName::win:
	case Config::CommandName::replay:
	case Config::CommandName::quit:
		return "";
	}
}

// private readCommand to get commands from the console as strings
// cin operator directly checks any typos in the input 
// and converts it into a Command object
Command* CommandProcessor::readCommand() {
	Command* command = new Command();
	cin >> command;
	return command;
}

void CommandProcessor::saveCommand(Command* command) {
	this->CommandCollection.push_back(command);
	Notify(this);
}

void CommandProcessor::setAvailableCommands() {
	this->AvailableCommands.clear();
	auto transitions = getTransitions<map<pair <Config::State, Config::CommandName>, Config::State>>();
	for (map<pair<Config::State, Config::CommandName>, Config::State>::iterator index = transitions.begin(); index != transitions.end(); index++)
	{
		if (index->first.first == getCurrentState())
			this->AvailableCommands.push_back(index->first.second);
	}
}

// Only used for the testDriver to skip the gameplay part
// Only to make the command processor test shorter ! Won't be kept in the future
void CommandProcessor::addCommandToCollection(Config::CommandName commandName) {
	Command* commandToAdd = new Command(commandName);
	this->CommandCollection.push_back(commandToAdd);
}

list<Config::CommandName> CommandProcessor::getAvailableCommands() {
	return this->AvailableCommands;
}

bool CommandProcessor::validate() {
	for (Config::CommandName element : getAvailableCommands())
	{
		if (getCurrentCommand()->getCommand() == element)
			return true;
	}
	return false;
};

string CommandProcessor::stringToLog() {
	Command* m = this->CommandCollection.back();
	string nameOfNewestCommand = "";
	switch (m->getCommand()) {
	case Config::CommandName::loadMap:
		nameOfNewestCommand = "loadMap";
		break;
	case Config::CommandName::validateMap:
		nameOfNewestCommand = "validateMap";
		break;
	case Config::CommandName::addPlayer:
		nameOfNewestCommand = "addPlayer";
		break;
	case Config::CommandName::gameStart:
		nameOfNewestCommand = "addPlayer";
		break;
	case Config::CommandName::replay:
		nameOfNewestCommand = "replay";
		break;
	case Config::CommandName::quit:
		nameOfNewestCommand = "quit";
		break;
	default:
		nameOfNewestCommand = "";
	}
string str = "";
// This may not need cuz each command will be validated
if (nameOfNewestCommand == "") {
	str += "Command: [Error] Invalid command";

}
else {
	str += "Command: " + nameOfNewestCommand + " is saved";

}

return str;
}

void CommandProcessor::getCommand() {
	Command* command = readCommand();
	saveCommand(command);
}

void CommandProcessor::getCommand(Command* command) {
	saveCommand(command);
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
///////// FileLineReader adaptee class implementation  ///////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

FileLineReader::FileLineReader() {
	this->currentLine = "";
}

FileLineReader::~FileLineReader() {}

void FileLineReader::setCurrentLine(string& lineString) {
	this->currentLine = lineString;
}

string FileLineReader::getCurrentLine() {
	return this->currentLine;
}

string FileLineReader::readLineFromFile(ifstream& fileName) {
	string line;
	getline(fileName, line);
	setCurrentLine(line);
	return line;
}



//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
///////// CommandProcessorAdapter class implementation  //////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

CommandProcessorAdapter::CommandProcessorAdapter() {
	this->flr = new FileLineReader();
	this->FileName = "./test_command_processor";
}

CommandProcessorAdapter::CommandProcessorAdapter(string& input) {
	this->flr = new FileLineReader();
	this->FileName = "./test_command_processor/" + input.substr(input.find(" ") + 1);
	if (!getCurrentFile().is_open()) {

		getCurrentFile().open(getFileName());

		if (!getCurrentFile().is_open()) {
			perror("We exit the game");
			exit(EXIT_FAILURE);
		}
	}
}


CommandProcessorAdapter::~CommandProcessorAdapter() {
	free(this->flr);
	delete this->flr;
	this->CurrentFile.close();
}

void CommandProcessorAdapter::setFileName(string& name) {
	this->FileName = name;
}

string CommandProcessorAdapter::getFileName() {
	return this->FileName;
}

FileLineReader* CommandProcessorAdapter::getFlr() {
	return this->flr;
}

ifstream& CommandProcessorAdapter::getCurrentFile() {
	return this->CurrentFile;
}

Command* CommandProcessorAdapter::readCommand() {
	string line = getFlr()->readLineFromFile(getCurrentFile());
	int index = line.find(" ");
	string substring = line.substr(0, index);
	Command* command;
	if (index == -1) {
		command = new Command(line);
	}
	else {
		command = new Command(substring);
		command->setArgument(line.substr(index + 1));
	}

	if (command->getCommand() == Config::CommandName::NONE)
	{
		cout << "\nThe command entered is not a valid command.\nWe exit the game\n";
		exit(1);
	}
	return command;
}

bool CommandProcessorAdapter::validate() {
	for (Config::CommandName element : getAvailableCommands())
	{
		if (getCurrentCommand()->getCommand() == element)
			return true;
	}
	cout << "\nThe command does not lead to a transition.\nWe exit the game\n";
	exit(1);
	return false;
};



//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
///////////////// Free operators and functions  //////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

// Stream operator for converting the enum State into a string whenever << is called
ostream& operator<<(ostream& out, const Config::State& state) {
	static map<Config::State, string> strings;
	if (strings.size() == 0) {
#define INSERT_ELEMENT(p) strings[p] = #p
		INSERT_ELEMENT(Config::State::start);
		INSERT_ELEMENT(Config::State::mapLoaded);
		INSERT_ELEMENT(Config::State::mapValidated);
		INSERT_ELEMENT(Config::State::playersAdded);
		INSERT_ELEMENT(Config::State::assignReinforcment);
		INSERT_ELEMENT(Config::State::issueOrders);
		INSERT_ELEMENT(Config::State::executeOrders);
		INSERT_ELEMENT(Config::State::winState);
		INSERT_ELEMENT(Config::State::END);
#undef INSERT_ELEMENT
	}
	return out << strings[state].substr(15);
};

// Stream operator for converting the enum Command into a string whenever << is called
ostream& operator<<(ostream& out, const Config::CommandName& command) {
	out << CommandProcessor::toString(command);
	std::string argument = CommandProcessor::getArgument(command);
	if (!argument.empty()) {
out << " <" << argument << ">";
	}
	return out;
};

ostream& operator<<(ostream& out, Command& commandInstance) {
	out << "Command: " << CommandProcessor::toString(commandInstance.getCommand());
	if (!commandInstance.getArgument().empty()) {
		out << ' ' << commandInstance.getArgument();
	}
	out << endl;
	out << "Effect: " << commandInstance.getEffect() << endl;
	return out;
};

ostream& operator<<(ostream& out, CommandProcessor& commandProcessorInstance) {
	out << "==================================" << endl;
	out << "Current state: " << commandProcessorInstance.getCurrentState() << endl;
	out << "\nCurrent Command Collection content:\n";
	for (Command* element : commandProcessorInstance.getCommandCollection()) {
		out << "*\n" << *element;
	}
	out << "\nCurrent possible commands: ";
	for (Config::CommandName element : commandProcessorInstance.getAvailableCommands()) {
		out << "\n- " << element;
	};
	return out << endl;
};

string getArgument(istream& in) {
	string argument;
	getline(in, argument);
	if (!argument.empty()) {
		// Skip the space.
		argument = argument.substr(1);
	}
	return argument;
}

// Get input for commands, does not include gameplay commands
// istream overload operator that takes the input
// only handles the specific commands (not gameplay commands handled there)
istream& operator>>(istream& in, Command* command) {
	string entry;
	bool correctCommand;
	cout << "\nPlease enter the command you want to perform:\n";
	do {
		// Ensure the command is valid before proceeding
		correctCommand = true;
		// For commands that take an argument (e.g. loadmap <filename>),
		// the "entry" variable will just be "loadmap".
		in >> entry;
		cout << "\n";
		if (entry == CommandProcessor::toString(Config::CommandName::loadMap)) {
			command->setCommand(Config::CommandName::loadMap);
			command->setArgument(getArgument(in));
		}
		else if (entry == CommandProcessor::toString(Config::CommandName::validateMap))
			command->setCommand(Config::CommandName::validateMap);
		else if (entry == CommandProcessor::toString(Config::CommandName::addPlayer)) {
			command->setCommand(Config::CommandName::addPlayer);
command->setArgument(getArgument(in));
		}
		else if (entry == CommandProcessor::toString(Config::CommandName::gameStart))
		command->setCommand(Config::CommandName::gameStart);
		else if (entry == CommandProcessor::toString(Config::CommandName::replay))
		command->setCommand(Config::CommandName::replay);
		else if (entry == CommandProcessor::toString(Config::CommandName::quit))
		command->setCommand(Config::CommandName::quit);
		else {
		correctCommand = false;
		cout << "The command " << entry << " entered does not exist. Try again!\n";
		}
	} while (!correctCommand);
	return in;
};


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
/////// CommandProcessorTournament class implementation  /////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

CommandProcessorTournament::CommandProcessorTournament() {
	this->listOfMaps = {};
	this->listOfPlayers = {};
	this->numberOfGames = 0;
	this->numberOfTurns = 0;
}

CommandProcessorTournament::CommandProcessorTournament(string& inputString) {
	if (!validateTournament(inputString)) {
		cout << "\nWe exit the program !\n";
		exit(EXIT_FAILURE);
	};
}

CommandProcessorTournament::~CommandProcessorTournament() {}

// CommandProcessorTournament GETTERS & SETTERS begin
void CommandProcessorTournament::setListOfMaps(vector<string> input) {
	this->listOfMaps = input;
}

void CommandProcessorTournament::addToListOfMaps(string input) {
	this->listOfMaps.push_back(input);
}

vector<string> CommandProcessorTournament::getListOfMaps() {
	return this->listOfMaps;
}

void CommandProcessorTournament::setListOfPlayers(vector<string> input) {
	this->listOfPlayers = input;
}

void CommandProcessorTournament::addToListOfPalyers(string input) {
	this->listOfPlayers.push_back(input);
}

vector<string> CommandProcessorTournament::getListOfPlayers() {
	return this->listOfPlayers;
}

void CommandProcessorTournament::setNumberOfGames(int input) {
	this->numberOfGames = input;
}

int CommandProcessorTournament::getNumberOfGames() {
	return this->numberOfGames;
}

void CommandProcessorTournament::setNumberOfTurns(int input) {
	this->numberOfTurns = input;
}

int CommandProcessorTournament::getNumberOfTurns() {
	return this->numberOfTurns;
}

bool CommandProcessorTournament::validatePlayerType(string type) {

	vector <string> validTypes = { "Aggressive","Benevolent","Neutral","Cheater" };
	
	//Check if type is equal to one of the valid types inside the vector.
	if (std::find(validTypes.begin(), validTypes.end(), type) != validTypes.end())
	{
		return true; //valid
	}
	else {
		return false; //invalid
	}

	
}

 // CommandProcessorTournament GETTERS & SETTERS end
string CommandProcessorTournament::parseMaps(string& str) {
	// first map should be written after the -M trigger (+3 to pass through the -M and space)
	int index = str.find("-M ") + 3;

	// find() returns -1 if not found, but here added 3, so should be 2 if there is no -M
	if (index == 2) {
		cout << "\nThe map input trigger \'-M\' is not found.";
		return "";
	}

	str = str.substr(index);

	do {
		if (str.substr(0, 2) == "-P") {
			break;
		}

		index = str.find_first_of(" ");
		if (str.at(0) != ' ') {
			addToListOfMaps(str.substr(0, index));
		}

		if (index == -1) {
			cout << "\nThe input cannot end up with a map file, please follow the proper template.";
			return "";
		}
		str = str.substr(++index);
	} while (getListOfMaps().size() < 5);
	return str;
}

string CommandProcessorTournament::parsePlayers(string& str) {
	// first player should be written after the -P trigger
	int index = str.find("-P ") + 3;
	string playerType = "";
	if (index == 2) {
		cout << "\nThe player input trigger \'-P\' is not found.";
		return "";
	}

	str = str.substr(index);

	do {
		if (str.substr(0, 2) == "-G") {
			break;
		}
		index = str.find_first_of(" ");
		if (str.at(0) != ' ') {
			playerType = str.substr(0, index);
			if (validatePlayerType(playerType)) {
				addToListOfPalyers(playerType);
			}
			else {
				cout << "\'" << playerType <<  "\'" <<" is an invalid strategy.Please choose between Aggressive, Benevolent, Neutral or Cheater  " << endl;
				return "";
			}
			//addToListOfPalyers(str.substr(0, index));
		}

		if (index == -1) {
			cout << "\nThe input cannot end up with a player name, please follow the proper template.";
			return "";
		}
		str = str.substr(++index);
	} while (getListOfPlayers().size() < 4);
	return str;
}

string CommandProcessorTournament::parseNumGames(string& str) {
	// first find the -G trigger to find the number of games wanted
	int index = str.find("-G ");


	if (index == -1) {
		cout << "\nThe number of games input trigger \'-G\' is not found.";
		return "";
	}

	index = index + 3;

	str = str.substr(index);

	if (str.at(0) != ' ') {
		setNumberOfGames(stoi(str.substr(0, 1)));
	}	

	return str;
}

string CommandProcessorTournament::parseNumTurns(string& str) {
	int index = str.find("-D ") + 3;

	if (index == 2) {
		cout << "\nThe number of turns input trigger \'-D\' is not found.";
		return "";
	}

	str = str.substr(index);

	if (str.at(0) != ' ') {
		setNumberOfTurns(stoi(str.substr(0, 2)));
	}

	return str;
}

bool CommandProcessorTournament::validateTournament(string& str) {

	// // PARSE MAPS // //
	str = parseMaps(str);
	if (str.empty()){
		return false;
	}

	if (getListOfMaps().empty()) {
		cout << "\nThere are no maps written.";
		return false;
	}

	if (str.substr(0, 2) != "-P" && str.find("-P") != -1) {
		cout << "\nMore than 5 maps have been written. This is not allowed.";
		return false;
	}


	// // PARSE PLAYERS // //
	str = parsePlayers(str);
	if (str.empty()) {
		return false;
	}

	if (getListOfPlayers().empty()) {
		cout << "\nThere are no players written.";
		return false;
	}

	if (getListOfPlayers().size() < 2) {
		cout << "\nThere are not enough player in the game (between 2 and 4 players are required).";
		return false;
	}

	if (str.substr(0, 2) != "-G" && str.find("-G") != -1) {
		cout << "\nMore than 4 players have been inserted. This is not allowed.";
		return false;
	}



	// // PARSE GAMES NUMBER // //
	str = parseNumGames(str);
	if (str.empty()) {
		return false;
	}

	if (getNumberOfGames() == 0) {
		cout << "\nThe input number is not an integer.";
		return false;
	}

	if (getNumberOfGames() > 5) {
		cout << "\nYou re not allowed to play more than 5 games in tournament mode.";
		return false;
	}

	// Verify number of games matches the number of maps inputted
	if (getNumberOfGames() > getListOfMaps().size()) {
		int num = getNumberOfGames() - getListOfMaps().size();
		cout << "\nThere are less maps than games ... We will have a problem at game ";
		cout << num;
		cout << " ...";
		return false;
	}

	if (getNumberOfGames() < getListOfMaps().size()) {
		int num = getListOfMaps().size() - getNumberOfGames();
		cout << "\nThere are more maps than games... We will then play on the first ";
		cout << num;
		cout << ".\n";
	}
	
	// // PARSE TUNRS NUMBER // //
	str = parseNumTurns(str);
	if (str.empty()) {
		return false;
	}

	if (getNumberOfTurns() < 10) {
		cout << "\nLess than 10 turns are not allowed in a game.";
		return false;
	}

	if (getNumberOfTurns() > 50) {
		cout << "\nYou are not allowed to create games with more than 50 turns.";
		return false;
	}

	if (!str.substr(2).empty()) {
		cout << "\nNothing should be there after the number of turns ... Why do we have: \"" + str.substr(2) + "\" written ?";
		return false;
	}

	return true;
}