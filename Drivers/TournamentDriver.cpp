#include "../HeaderFiles/GameEngine.h"
using namespace std;

void testTournament() {
	GameEngine* engine = new GameEngine();
	string typeOfGame;
	cout << "What type of game do you want to play ?\n*normal\n*tournament\n";
	getline(cin, typeOfGame);
	if (typeOfGame == "normal") {
		engine->startupPhase();
	}
	else if (typeOfGame == "tournament") {
		engine->executeTournament();
	}
	else {
		cout << "I don't understand what you want ... We exit the program !\n";
		exit(EXIT_FAILURE);
	}
	//engine->mainGameLoop(dynamic_cast<CommandProcessorTournament*>(engine->getCommandProcessor())->getNumberOfTurns());
	delete engine;
}