#include "../HeaderFiles/PlayerStrategies.h"
#include "../HeaderFiles/Player.h"
#include "../HeaderFiles/GameEngine.h"
#include "../HeaderFiles/MapLoader.h"
#include <vector>
using namespace std;

void initializeState(shared_ptr<Map> currentMap, vector<Player*>& AddedPlayerList) {
	unordered_map<string, Territory*> territories = currentMap->getTerritories();
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
	// Assign the rest of territories to a neutral player
	Player* neutral = nullptr;
	for (Player* p : AddedPlayerList) {
		if (p->getPlayerStrategy()->getType() == "Neutral") {
			neutral = p;
			break;
		}
	}
	for (int i = 0; i < territoryList.size(); i++) {
		neutral->addTerriorty(territoryList.at(i));
	}
	Deck deck;
	//let each player draw 2 initial cards from the deck using the deck’s draw() method
	for (Player* p : AddedPlayerList) {
		int counter = 0;
		while (counter < 2) {
			Card* c = deck.draw();
			p->getHandcard()->setCardHand(c);
			cout << p->getName() << " drew " << *c << endl;
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
}

void testPlayerStrategies() {
	//Fifferent players can be assigned different strategies 
	//that lead to different behavior using the Strategy design pattern
	cout << "executing player strategies " << endl;
	HumanPlayerStrategy* human = new HumanPlayerStrategy();
	Player* p1 = new Player("HumanTest", human);
	NeutralPlayerStrategy* neutral = new NeutralPlayerStrategy();
	Player* p2 = new Player("NeutralTest", neutral);
	AggressivePlayerStrategy* aggr = new AggressivePlayerStrategy();
	Player* p3 = new Player("AggressiveTest", aggr);
	CheaterPlayerStrategy* cheater = new CheaterPlayerStrategy();
	Player* p4 = new Player("CheaterTest", cheater);
	BenevolentPlayerStrategy* benevolent = new BenevolentPlayerStrategy();
	Player* p5 = new Player("BenevolentTest", benevolent);
	vector<Player*> listOfPlayers;
	listOfPlayers.push_back(p1);
	listOfPlayers.push_back(p2);
	listOfPlayers.push_back(p3);
	listOfPlayers.push_back(p4);
	listOfPlayers.push_back(p5);

	// Load a Map
	MapLoader loader("3D.map");
	shared_ptr<Map> currentMap;
	currentMap = loader.load();
	initializeState(currentMap, listOfPlayers);
	vector<Player*> players = listOfPlayers;
	while (!players.empty()) {
		for (vector<Player*>::iterator itr = players.begin(); itr != players.end();) {
			cout << endl;
			Player* p = *itr;
			cout << "***Player " << p->getName() << "***\n";
			if (!p->issueOrder(listOfPlayers)) {
				itr = players.erase(itr);
			}
			else {
				itr++;
			}
			cout << endl;
		}
	}
	cout << "\n------------Finish one turn------------\n";
	cout << "\n===============================================================\n";
	cout << "\n----------Change strategy dynamically during play----------\n";
	cout << "\n===============================================================\n";
	cout << "\n****Human Player****" << endl;
	p1->issueOrder(listOfPlayers);
	cout << "\n****Change to neutral player****" << endl;
	NeutralPlayerStrategy* n1 = new NeutralPlayerStrategy();
	p1->setPlayerStrategy(n1);
	p1->issueOrder(listOfPlayers);
	cout << "\n****Change to agrressive player****" << endl;
	AggressivePlayerStrategy* a1 = new AggressivePlayerStrategy();
	p1->setPlayerStrategy(a1);
	
	p1->issueOrder(listOfPlayers);
	cout << "\n****Change to cheater player\n" << endl;
	CheaterPlayerStrategy* c1 = new CheaterPlayerStrategy();
	p1->setPlayerStrategy(c1);
	p1->issueOrder(listOfPlayers);
	cout << "\n****Change to benevolent player\n" << endl;
	BenevolentPlayerStrategy* b1 = new BenevolentPlayerStrategy();
	p1->setPlayerStrategy(b1);
	p1->issueOrder(listOfPlayers);
	delete p1;
	delete p2;
	delete p3;
	delete p4;
	delete p5;
}
