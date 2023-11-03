#include <iostream>
#include <map>
#include <fstream>
#include <exception>
#include "../HeaderFiles/GameEngine.h"
#include "../HeaderFiles/MapLoader.h"
#include "../HeaderFiles/Orders.h"
#include "../HeaderFiles/globalVariables.h"
#include "../HeaderFiles/PlayerStrategies.h"
#include "../HeaderFiles/LogManager.h"

//Constructor
GameEngine::GameEngine() {
	this->currentMap = nullptr;
	this->AddedPlayerList = vector<Player*>();
	this->CommandProcess = nullptr;
	lm = new LogManager("tournament_log.txt");
	lm->openNewFile();
};

//Copy constructor
GameEngine::GameEngine(string& input) {
	if (input != "tournament") {
		input == "console" ? this->CommandProcess = new CommandProcessor() : this->CommandProcess = new CommandProcessorAdapter(input);
	}
	else {
		this->CommandProcess = new CommandProcessorTournament(input);
	}
	lm = new LogManager("tournament_log.txt");
	this->currentMap = nullptr;
	this->AddedPlayerList = vector<Player*>();
	lm->openNewFile();
}

//Destructor
GameEngine::~GameEngine() {
	lm->closeFile();
	delete lm;
	delete this->CommandProcess;
}

CommandProcessor* GameEngine::getCommandProcessor() {
	return this->CommandProcess;
}

void GameEngine::executeTransition() {
	// Checks if the command leads to a proper state
	// If so, process the transition
	// If not, save the error effect
	Config::State state = getCommandProcessor()->getCurrentState();
	Config::CommandName command = getCommandProcessor()->getCurrentCommand()->getCommand();

	if (getCommandProcessor()->validate()) {
		map<pair <Config::State, Config::CommandName>, Config::State> transitions
			= getTransitions<map<pair <Config::State, Config::CommandName>, Config::State>>();

		Config::State transitionState = transitions.find({ this->CommandProcess->getCurrentState(), command })->second;

		getCommandProcessor()->setCurrentState(transitionState);
		getCommandProcessor()->getCurrentCommand()->saveEffect(); // Save the Effect once the command gets executed
	}
	else {
		string commandString = getCommandProcessor()->toString(command);
		string stateString = getCommandProcessor()->toString(state);
		string error = "The command " + commandString;
		error += " does not lead to a tansition from the state " + stateString + ". Current state not changed\n";

		getCommandProcessor()->getCurrentCommand()->saveEffect(error);
		getCommandProcessor()->setCurrentState(state);
	}
	Notify(this);
	cout << endl << *getCommandProcessor();
}

string GameEngine::stringToLog() {
	return "Game Engine: new state is " + this->getCommandProcessor()->toString(this->getCommandProcessor()->getCurrentState());
}

bool GameEngine::isOver() {
	return getCommandProcessor()->getCurrentState() == Config::State::END;
}
//command to select a map from a list of map files
void GameEngine::loadmap(string mapName)
{
	MapLoader loader(mapName);
	currentMap = loader.load();
	if (currentMap == nullptr) {
		cout << "Please try again!" << endl;
		getCommandProcessor()->getCurrentCommand()->saveEffect("Invalid input file");
	}
	else {
		executeTransition();
	}
}
// command to validate the map, based on assignment 1
void GameEngine::validatemap()
{
	if(currentMap->validate()) {
		cout << "Map validated" << endl;
		executeTransition();
		return;
	}
	string error = "Map invalid, please select another map";
	getCommandProcessor()->getCurrentCommand()->saveEffect(error);
	cout << error << endl;
}
// command to enter players in the game 
void GameEngine::addPlayer(string playerName)
{
	vector <string> validTypes = { "Aggressive","Benevolent","Neutral","Cheater" }; //Name for player strategies
	bool is_duplicate = false;

	if (playerName.empty()) {//ensure a name is added
		getCommandProcessor()->getCurrentCommand()->saveEffect("Your name cannot be empty");
		cout << getCommandProcessor()->getCurrentCommand()->getEffect() << endl;
		return;
	}
	if (AddedPlayerList.size() >= 6) { //check if more than 6 players are added
		getCommandProcessor()->getCurrentCommand()->saveEffect("Number of player cannot exceed 6. You cannot add more players");
		cout << getCommandProcessor()->getCurrentCommand()->getEffect() << endl;
		return;
	}
	for (Player* p : AddedPlayerList) {
		if (p->getName() == playerName) { //check if 2 names are the same
			//Two names can be the same if the name is either "Aggressive","Benevolent","Neutral","Cheater"
			if (std::find(validTypes.begin(), validTypes.end(), playerName) != validTypes.end())
			{
				is_duplicate = true;
				break;
			}
			getCommandProcessor()->getCurrentCommand()->saveEffect("The player's name is already taken");
			cout << getCommandProcessor()->getCurrentCommand()->getEffect() << endl;
			return;
		}
	}
	//otherwise, add player to vector and continue

	if (playerName == "Aggressive") {
		PlayerStrategy* aggPlayer = new AggressivePlayerStrategy();
		if (is_duplicate) playerName = playerName + std::to_string(AddedPlayerList.size()); //if same strategy called twice, add size of addedplayerList to name.
		Player* p = new Player(playerName, aggPlayer);
		AddedPlayerList.push_back(p);
	}
	else if (playerName == "Benevolent") {
		PlayerStrategy* benPlayer = new BenevolentPlayerStrategy();
		if (is_duplicate) playerName = playerName + std::to_string(AddedPlayerList.size());
		Player* p = new Player(playerName, benPlayer);
		AddedPlayerList.push_back(p);
	}
	else if (playerName == "Neutral") {
		PlayerStrategy* neutralPlayer = new NeutralPlayerStrategy();
		if (is_duplicate) playerName = playerName + std::to_string(AddedPlayerList.size());
		Player* p = new Player(playerName, neutralPlayer);
		AddedPlayerList.push_back(p);
	}
	else if (playerName == "Cheater") {
		PlayerStrategy* cheaterPlayer = new CheaterPlayerStrategy();
		if (is_duplicate) playerName = playerName + std::to_string(AddedPlayerList.size());
		Player* p = new Player(playerName, cheaterPlayer);
		AddedPlayerList.push_back(p);
	}
	else {
		PlayerStrategy* hummanPlayer = new HumanPlayerStrategy();
		Player* p = new Player(playerName, hummanPlayer);
		AddedPlayerList.push_back(p);

	}

	executeTransition();
}


void GameEngine::gameStart()
{//ensure there are enough players in the game
	if (AddedPlayerList.size() < 2) {
		getCommandProcessor()->getCurrentCommand()->saveEffect("Cannot start the game with less than 2 players");
		cout << getCommandProcessor()->getCurrentCommand()->getEffect() << endl;
		return;
	}
	cout << "Part a: Distributing territories " << endl;
	unordered_map<string, Territory*> territories = this->currentMap->getTerritories();
	vector<Territory*> territoryList;
	for (pair<string, Territory*> territory : territories) { // add territories  to the territory list
		territoryList.push_back(territory.second);
	}
	
	int numOfTerritoryPerPlayer = territoryList.size() / AddedPlayerList.size();
	for (Player* p : AddedPlayerList) {
		int counter = 0;
		while (counter < numOfTerritoryPerPlayer) { //set the number of territories to each player
			int randIndex = rand() % territoryList.size();
			Territory* current = territoryList[randIndex];
			p->addTerriorty(current);
			territoryList.erase(territoryList.begin() + randIndex);
			counter++;
		}
		p->setReinforcementPool(50); //add 50 initial army units to the players, which are placed in their respective reinforcement pool
	}
	// Check if a neutral player is added in the player list
	Player* neutral = nullptr;
	for (Player* p : AddedPlayerList) {
		if (p->getPlayerStrategy()->getType() == "Neutral") {
			neutral = p;
		}
	}
	if (neutral == nullptr) {
		neutral = new Player("Neutral", new NeutralPlayerStrategy());
		AddedPlayerList.push_back(neutral);
	}
	for (int i = 0; i < territoryList.size(); i++) {
		neutral->addTerriorty(territoryList.at(i));
	}
	//let each player draw 2 initial cards from the deck using the deck’s draw() method
	for (Player* p : AddedPlayerList) {
		int counter = 0;
		while (counter < 2) {
			Card* c = deck.draw();
			p->getHandcard()->setCardHand(c);
			cout << p->getName() << " drew " << *c <<endl;
			counter++;
		}
		
	}

	for (Player* p : AddedPlayerList) {
		cout << p->getName() << " has the following territories " << endl;

		vector <Territory*> t = p->getterriortiesOwned();

		for (auto terr : t) {
			cout << terr->getName() << " , ";
		}
		cout << endl;

	}

	executeTransition();
}

//input function to start the game and set up the game. takes the loadmap, validatemap, addplayer and gamestart functions
void GameEngine::startupPhase()
{
	string input;
	bool isValid = false;
	do {
		std::cout << "\nPlease choose your gameplay style, either by command or file.\n*console\n*file <filename>\n-";
		getline(cin, input);

		if (input == "console" || input.find("file") == 0) {
			isValid = true;
		};
	} while (!isValid);

	// Use the copy constructor to either have a commandProcessor or its adapter
	input == "console" ? this->CommandProcess = new CommandProcessor() : this->CommandProcess = new CommandProcessorAdapter(input);

	cout << endl << *this->CommandProcess;

	do {
		getCommandProcessor()->getCommand();
		Command* command = getCommandProcessor()->getCurrentCommand();
		if (getCommandProcessor()->validate()) {
			switch (command->getCommand()) {
			case Config::CommandName::loadMap:
				loadmap(command->getArgument());
				break;
			case Config::CommandName::validateMap:
				validatemap();
				break;
			case Config::CommandName::addPlayer:
				addPlayer(command->getArgument());
				break;
			case Config::CommandName::gameStart:
				gameStart();
				break;
			}

		}
		// Check if we enter the game-trigger events
		// If so, directly transition into the win state to continue the input tests
		if (getCommandProcessor()->getCurrentState() == Config::State::assignReinforcment) {
			cout << "\nFinish start up phase\n";
			break;
		}

	} while (!isOver());
}

void GameEngine::mainGameLoop() {
	while (!hasWinner()) {
		mainGameLoopContent();		
	}
}

void GameEngine::mainGameLoop(int numberOfTurns) {
	for (int i = 0; i < numberOfTurns; i++) {
		mainGameLoopContent();
		if (hasWinner()) {
			int totalTerritories = currentMap->getTerritories().size();
			for (Player* p : AddedPlayerList) {
				if (p->getterriortiesOwned().size() == totalTerritories) { //If player has all territories, he won
					lm->writeToFile("\t" + p->getName());
					return;
				}
			}
			break;
		}
	}

	if (!hasWinner()) {
		cout << "Game has finish with no winner - DRAW - " << endl;
		lm->writeToFile("\tDraw");
	}
}

void GameEngine::mainGameLoopContent() {
	reinforcementPhase();

	issueOrdersPhase();

	executeOrdersPhase();

	cout << "\n\nTERRITORIES STATUS " << endl;

	for (Player* p : AddedPlayerList) {
		cout << p->getName() << " has the following territories " << endl;


		if (p->conquer) {
			Card* c = deck.draw();
			p->getHandcard()->setCardHand(c);
		}

		p->conquer = false;


		vector <Territory*> t = p->getterriortiesOwned();

		for (auto terr : t) {
			cout << terr->getName() << " , ";
		}
		cout << endl;

	}
	removeDefeatedPlayers();
}

bool GameEngine::removeDefeatedPlayers() {
	
	for (Player* p : AddedPlayerList) {
		if (p->getterriortiesOwned().size() == 0) { //Removing player with no territories
			cout  << p->getName() << " got removed from the game since no territories owned were found." << endl;
			this->AddedPlayerList.erase(std::remove(this->AddedPlayerList.begin(), this->AddedPlayerList.end(), p), this->AddedPlayerList.end());
			return true;
		}
	}
	return false;
}

bool GameEngine::hasWinner() {
	shared_ptr<Map> m = this->currentMap;
	int totalTerritories = 0;


	for (auto continent : m->getContinents()) {
		totalTerritories += continent.second->getTerritories().size();
	}

	for (Player* p : AddedPlayerList) {
		if (p->getterriortiesOwned().size() == totalTerritories) { //If player has all territories, he won
			cout << p->getName() << " just won the game with " << totalTerritories << " territories" << endl;;;
			return true;
		}
	}
	if (AddedPlayerList.size() == 1) {
		cout << AddedPlayerList[0]->getName() << " just won the game by eliminating all other players!" << endl;
		return true;
	}
	return false;
}

void GameEngine::applyContinentalBonus() {
	shared_ptr<Map> m = this->currentMap;
	unordered_map<string, Territory*> territoriesMap; //map of each continent in terms of territories.
	unordered_map<string, Continent*> continentMap = m->getContinents(); //all continents
	unordered_map<Player*, int> playerTerritoriesMap; //Will check if a player has same amount of territory of a continent.
	int TerritoriesOfContinent = 0;
	int playerTerritories = 0;
	int bonus = 0;
	int playerReinforcementPool = 0;
	int nbContinent = continentMap.size();
	Player* p = nullptr;
	/*
		Looping through each continent to find the amount of territories (numberOfTerritories)
		Finding the owner of each territory of a continent and then checking if a player has
		a number of territories equal to the number of territories inside a continent.
		if so, retrieve continent bonus and apply to the reinforcement pool of the player

	*/

	for (auto continent : continentMap) { //Looping through each continent
		territoriesMap = continent.second->getTerritories();
		TerritoriesOfContinent = territoriesMap.size();
		for (auto t : territoriesMap) { //Looping through territories of the a continent.
			p = t.second->getOwner();
			if (playerTerritoriesMap.find(p) == playerTerritoriesMap.end()) {
				// Didnt find player -> adding to the map 
				playerTerritoriesMap.insert({ p, 1 });
			}
			else {
				//Player is already in the map -> incrementing its territories owned for that continent.
				playerTerritories = playerTerritoriesMap[p];
				playerTerritoriesMap[p] = playerTerritories + 1;
			}
		}
		//Checking if a player should receive a continent bonus.
		for (auto player : playerTerritoriesMap) {
			if (player.second == TerritoriesOfContinent) { // if player has same number of territory inside a continent

				bonus = continent.second->getBonus();
				cout << player.first->getName() << " just receive a bonus of " << bonus << " units for continent "
					<< continent.second->getName() << endl;
				playerReinforcementPool = player.first->getReinforcementPool();
				player.first->setReinforcementPool(playerReinforcementPool + bonus);




			}
		}
		playerTerritoriesMap.clear(); //Clearing playerMap for next iteration.

	}
}

void GameEngine::reinforcementPhase() {
	
	int reinforcementArmies = 0;
	int oldReinforcementPool = 0;

	const int MIN_REINFORCEMENT = 3; //min reinforcement
	
	cout << "\n\n====================" << endl;
	cout << "Reinforcement Phase Started." << endl;
	cout << "====================" << endl;
	applyContinentalBonus();

	for (Player* player : AddedPlayerList) {
		if (!player->getterriortiesOwned().empty()) { //prevents division by zero

			
			player->clearnegotiationlist(); //Clear negotiation list.
			reinforcementArmies = (player->getterriortiesOwned().size() / 3); //gets reinforcement

			
			oldReinforcementPool = player->getReinforcementPool();

			if (reinforcementArmies < MIN_REINFORCEMENT) reinforcementArmies = MIN_REINFORCEMENT;

			cout  << player->getName() << " will receive " << reinforcementArmies << " army units." << endl;

			player->setReinforcementPool(oldReinforcementPool + reinforcementArmies);

			cout << "Updating player " << player->getName() << " reinforcement pool from " << oldReinforcementPool
				<< " to " << (reinforcementArmies + oldReinforcementPool) << endl;



		}
	}

}

void GameEngine::issueOrdersPhase() {

	cout << "\n\n====================" << endl;
	cout << "Issue Orders Phase" << endl;
	cout << "====================" << endl;
	vector<Player*> players = AddedPlayerList;
	while (!players.empty()) {
		for (vector<Player*>::iterator itr = players.begin(); itr != players.end();) {
			cout << endl;
			Player* p = *itr;
			cout << "***Player " << p->getName() << "***\n";
			if (!p->issueOrder(AddedPlayerList)) {
				itr = players.erase(itr);
			}
			else {
				itr++;
			}
			cout << endl;
		}
	}
}

void GameEngine::executeOrdersPhase() {
	int size;
	cout << "\n\n====================" << endl;
	cout << "Execute Order Phase." << endl;
	cout << "====================" << endl;

	//Executing each player deployment order in round robin fashion
	for (Player* player : AddedPlayerList) {

		size = player->getOrderList()->getsize();
		if (size == 0) {
			continue;
		}

		cout << player->getName() <<  " has an orderlist of size "
		<< player->getOrderList()->getsize() << endl;

		vector<order*> currentPlayerOrderList = player->getOrderList()->getList();
		for (order* currentOrder : currentPlayerOrderList) {   //While the order is of deploy type
			if (currentOrder->getType() == "deploy") {
				currentOrder->execute(); //Executing the order 
			}
		}
	}
	
	//Executing all the other orders in the player order list
	for (Player* player : AddedPlayerList) {
		vector<order*> currentPlayerOrderList = player->getOrderList()->getList();
		for (order* currentOrder : currentPlayerOrderList) {   //While the order is of deploy type
			if (currentOrder->getType() != "deploy") {
				currentOrder->execute(); //Executing the order 
			}
		}
		player->getOrderList()->reset(); // Reset each player's order list after finish executing
	}
	cout << "Execution Order Phase done." << endl;
}

bool GameEngine::checkForDuplicates() {
	return false;
}

void GameEngine::executeTournament() {

	string input;
	bool isValid = false;
	do {
		std::cout << "\nPlease choose your gameplay style, either by command or file.\n*console\n*file <filename>\n";
		getline(cin, input);

		if (input == "console" || input.find("file") == 0) {
			isValid = true;
		};
	} while (!isValid);

	if (input.find("file") != -1) {
		string fileName = "./test_tournament/" + input.substr(input.find(" ") + 1);
		ifstream file;
		if (!file.is_open()) {

			file.open(fileName);

			if (!file.is_open()) {
				perror("We exit the game");
				exit(EXIT_FAILURE);
			}
		}
		string line;		
		getline(file, line);
		input = line;
	}
	else {
		cout << "\nEnter the tournament command\n*tournament -M <mapname> (1 to 5) -P <playername> (2 to 4) -G <numberofgames> (1 to 5) -D <numberofturns> (10 to 50)\n-";
		getline(cin, input);
	}

	this->CommandProcess = new CommandProcessorTournament(input);
	CommandProcessorTournament* cpt = dynamic_cast<CommandProcessorTournament*>(this->CommandProcess);

	cout << "\nTournament mode: \nM: ";
	cout << (lm == nullptr);
	lm->writeToFile("\nTournament mode: \nM: ");
	bool first = true;
	for (int i = 0; i < cpt->getListOfMaps().size(); i++) {
		int index = cpt->getListOfMaps()[i].find(".");
		string map;
		if (index != -1) {
			map = cpt->getListOfMaps()[i].substr(0, index);
		}
		i == cpt->getListOfMaps().size() - 1 ? cout << map + "\nP: " : cout << map + ", ";
		if (first) {
			first = false;
		}
		else {
			lm->writeToFile(", ");
		}
		lm->writeToFile(map);
	}
	lm->writeToFile("\nP: ");
	first = true;
	for (int i = 0; i < cpt->getListOfPlayers().size(); i++) {
		i == cpt->getListOfPlayers().size() - 1 ? cout << cpt->getListOfPlayers()[i] + "\nG: " : cout << cpt->getListOfPlayers()[i] + ", ";
		if (first) {
			first = false;
		}
		else {
			lm->writeToFile(", ");
		}
		lm->writeToFile(cpt->getListOfPlayers().at(i));
	}
	cout << cpt->getNumberOfGames();
	cout << "\nD: ";
	cout << cpt->getNumberOfTurns();
	cout << "\n";
	lm->writeToFile("\nG: " + to_string(cpt->getNumberOfGames()));
	lm->writeToFile("\nD: " + to_string(cpt->getNumberOfTurns()));
	lm->writeToFile("\n\nResults");
	lm->writeToFile("\n----------------------------------------------------------------------------------------------------------------------");
	for (int i = 0; i < cpt->getListOfMaps().size(); i++) {
		lm->writeToFile("\n===================\n");
		lm->writeToFile("\nMap " + cpt->getListOfMaps().at(i).substr(0, cpt->getListOfMaps()[i].find(".")));
		lm->writeToFile("\n===================\n");
		for (int i = 0; i < cpt->getNumberOfGames(); i++) {
			lm->writeToFile("Game " + to_string(i + 1) + "\n");
			// run the game
			do {
				switch (getCommandProcessor()->getCurrentState()) {
				case Config::State::start: {
					Command* command = new Command(Config::CommandName::loadMap);
					this->CommandProcess->getCommand(command);
					if (getCommandProcessor()->validate()) {
						command->setArgument(cpt->getListOfMaps()[i]);
						command->saveEffect();
						loadmap(command->getArgument());
					}
					break;
				}

				case Config::State::mapLoaded: {
					Command* command = new Command(Config::CommandName::validateMap);
					this->CommandProcess->getCommand(command);
					if (getCommandProcessor()->validate()) {
						validatemap();
					}
					break;
				}

				case Config::State::mapValidated: {
					for (int i = 0; i < cpt->getListOfPlayers().size(); i++) {
						Command* command = new Command(Config::CommandName::addPlayer);
						this->CommandProcess->getCommand(command);
						if (getCommandProcessor()->validate()) {
							command->setArgument(cpt->getListOfPlayers()[i]);
							addPlayer(command->getArgument());
						}
					}
					break;
				}
				case Config::State::playersAdded: {
					Command* command = new Command(Config::CommandName::gameStart);
					this->CommandProcess->getCommand(command);
					if (getCommandProcessor()->validate()) {
						gameStart();
					}
				}
				}
				// Check if we enter the game-trigger events
				// If so, directly transition into the win state to continue the input tests
				if (getCommandProcessor()->getCurrentState() == Config::State::assignReinforcment) {
					cout << "\nFinish start up phase\n";
					break;
				}

			} while (!isOver());
			mainGameLoop(dynamic_cast<CommandProcessorTournament*>(getCommandProcessor())->getNumberOfTurns());
			lm->writeToFile("\n----------------------------------------------------------------------------------------------------------------------\n");;
		}
	}
	// loop through the wanted number of games
	 
}