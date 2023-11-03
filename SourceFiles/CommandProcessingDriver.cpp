#include <iostream>
#include "../HeaderFiles/GameEngine.h"

void testCommandProcessor() {
	GameEngine* gameEngine;
	string gameplayChoice;
	bool isValid = false;

	do {
		std::cout << "\nPlease choose your gameplay style, either by command or file.\n*console\n*file <filename>\n-";
		getline(cin, gameplayChoice);
		if (gameplayChoice == "console" || gameplayChoice.find("file") == 0) {
			isValid = true;
		};
	} while (!isValid);
	
	// Use the copy constructor to either have a commandProcessor or its adapter
	gameEngine = new GameEngine(gameplayChoice);

	do {
		gameEngine->getCommandProcessor()->getCommand();
		Config::CommandName command = gameEngine->getCommandProcessor()->getCurrentCommand()->getCommand();
		gameEngine->executeTransition();

		// Check if we enter the game-trigger events
		// If so, directly transition into the win state to continue the input tests
		if (gameEngine->getCommandProcessor()->getCurrentState() == Config::State::assignReinforcment) {
			std::cout << "\nWe just skipped the gameplay part, for showing the CommandProcessor inputs.\n";
			gameEngine->getCommandProcessor()->addCommandToCollection(Config::CommandName::win);
			Config::State endGameState = Config::State::executeOrders;
			gameEngine->getCommandProcessor()->setCurrentState(endGameState);
			gameEngine->executeTransition();
		}
	} while (!gameEngine->isOver());
	std::cout << "\nThank you for playing our Risk-like game ! See you soon ! :D\n";
	//}
}