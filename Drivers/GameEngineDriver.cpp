#include <iostream>
#include "../HeaderFiles/GameEngine.h"
#include "../HeaderFiles/MapLoader.h"
using namespace std;

void testMainGameLoop() {
	// Creates the game engine and spliay the initial state.
	GameEngine* engine = new GameEngine();
	engine->startupPhase();
	//engine->gameStart();
	//engine->gameStart();

	engine->mainGameLoop();

	//engine->getCommandProcessor()->getCommand();
	//engine->startupPhase();
	/*
	engine->addPlayer("player1");
	engine->addPlayer("player2");
	engine->addPlayer("player3");
	engine->addPlayer("player4");
	*/
	/*do {
		cin >> engine;
		engine->getCommandProcessor()->();
	} while (!engine->isOver());

	engine->~GameEngine();

	cout << "\nThank you for playing our Risk-like game! See you soon!\n";
	*/
}

void testStartupPhase() {
	srand(time(NULL));
	GameEngine* gameEngine = new GameEngine();
	gameEngine->startupPhase();
}