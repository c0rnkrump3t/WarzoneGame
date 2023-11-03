#include "../HeaderFiles/Command.h"
#include "../HeaderFiles/LogManager.h"
#include <iostream>
#include <list>
#include <string>
#include <map>

// Command class implementation
//
Command::Command() {
	this->command = Config::CommandName::NONE;
	this->effect = "";
	this->lo = new LogObserver(this);
	this->argument = "";
}

Command::Command(string& name) {
	if (name == "loadmap") {
		this->command = Config::CommandName::loadMap;
	}
	else if (name == "validatemap") {
		this->command = Config::CommandName::validateMap;
	}
	else if (name == "addplayer") {
		this->command = Config::CommandName::addPlayer;
	}
	else if (name == "gamestart") {
		this->command = Config::CommandName::gameStart;
	}
	else if (name == "replay") {
		this->command = Config::CommandName::replay;
	}
	else if (name == "quit") {
		this->command = Config::CommandName::quit;
	}
	else {
		this->command = Config::CommandName::NONE;
	}
	this->effect = "";
	this->argument = "";
}

Command::Command(Config::CommandName name) {
	this->command = name;
	this->effect = "";
	this->lo = new LogObserver(this);
	this->argument = "";
}

Command::~Command() {
};

void Command::setCommand(Config::CommandName command) {
	this->command = command;
}

Config::CommandName Command::getCommand() {
	return this->command;
}

void Command::setArgument(string argument) {
	this->argument = argument;
}

string Command::getArgument() {
	return this->argument;
}

void Command::saveEffect() {
	switch (getCommand()) {
	case Config::CommandName::loadMap:
		this->effect = "Map " + this->argument + " is loaded !";
		break;
	case Config::CommandName::validateMap:
		this->effect = "Map is validated !";
		break;
	case Config::CommandName::addPlayer:
		this->effect = "Player is added !";
		break;
	case Config::CommandName::gameStart:
		this->effect = "The game is started successfully !";
		break;
	case Config::CommandName::replay:
		this->effect = "A new game has begon, let's replay !";
		break;
	case Config::CommandName::quit:
		this->effect = "Exited successfully the game !";
		break;
	}
	Notify(this);
}

void Command::saveEffect(const string& error) {
	this->effect = error;
	Notify(this);
}

string Command::getEffect() {
	return this->effect;
}

string Command::stringToLog() {
	string str = "Command's Effect: " + this->effect;
	return str;
}