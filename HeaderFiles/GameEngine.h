#ifndef COMP345_GAMEENGINE_H
#define COMP345_GAMEENGINE_H

#include "CommandProcessing.h"
#include "ILoggable.h"
#include "Subject.h"

#include <iostream>
#include <string>
#include <map>
#include <list>
#include "Map.h"
#include "Player.h"
#include "LogManager.h";


using namespace std;

class GameEngine : public ILoggable, public Subject {

private:
	CommandProcessor* CommandProcess;
	shared_ptr<Map> currentMap; 
	Config::State State;
	vector<Player*> AddedPlayerList;
	Deck deck;
	LogManager* lm;
	void mainGameLoopContent();
	
public:
	//Constructor and destructor
	GameEngine();
	GameEngine(string&);
	~GameEngine();

	CommandProcessor* getCommandProcessor();

	void executeTransition();
	bool isOver();
	void loadmap(string mapName);
	void validatemap();
	void addPlayer(string playerName);
	void gameStart();
	void startupPhase();

	void mainGameLoop();
	void mainGameLoop(int);
	void applyContinentalBonus();
	void reinforcementPhase();
	void issueOrdersPhase();
	void executeOrdersPhase();

	bool hasWinner();
	bool removeDefeatedPlayers();

	bool checkForDuplicates();

	string stringToLog() override;

	void executeTournament();
};

void testGameStates();
void testMainGameLoop();
void testStartupPhase();
void testTournament();

#endif //COMP345_GAMEENGINE_H