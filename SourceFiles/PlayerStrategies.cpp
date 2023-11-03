#include "../HeaderFiles/Player.h"
#include "../HeaderFiles/PlayerStrategies.h"
#include "../HeaderFiles/Orders.h"
#include <string.h>
// PlayerStrategy class
PlayerStrategy::PlayerStrategy() {
}

PlayerStrategy::PlayerStrategy(string type): type(type) {
}

PlayerStrategy::PlayerStrategy(const PlayerStrategy& ps): p(nullptr), type(ps.type) {
	p = new Player(*ps.p);
}

PlayerStrategy& PlayerStrategy::operator=(PlayerStrategy& ps)

{
	this->p = ps.p;
	this->type = ps.type;
	return *this;
}

PlayerStrategy::~PlayerStrategy() {
}

string PlayerStrategy::getType() {
	return type;
}

void PlayerStrategy::setPlayer(Player* inputPlayer) {
	p = inputPlayer;
}
NeutralPlayerStrategy& NeutralPlayerStrategy::operator&=(NeutralPlayerStrategy& p)
{
	PlayerStrategy :: operator=(p);
	return*this;
}
//Neutral Player class
NeutralPlayerStrategy::NeutralPlayerStrategy() : PlayerStrategy("Neutral") {}

NeutralPlayerStrategy::NeutralPlayerStrategy(const NeutralPlayerStrategy& nPlayer) : PlayerStrategy(nPlayer) {}

NeutralPlayerStrategy::~NeutralPlayerStrategy() {}

bool NeutralPlayerStrategy::issueOrder(vector<Player*> AddedPlayerList) {
	// Never issue an order
	cout << "---------------Neutral Player is issuing orders---------------"<< endl;
	cout << "[Neutral] Finish issuing issue an order" << endl;
	return false;
}
vector<Territory*> NeutralPlayerStrategy::toAttack() {
	// Neutral player never attacks
	return vector<Territory*>();
}
vector<Territory*> NeutralPlayerStrategy::toDefend() {
	// Neutral player never defends
	return vector<Territory*>();
}

CheaterPlayerStrategy& CheaterPlayerStrategy::operator=(CheaterPlayerStrategy& cheater)
{
	PlayerStrategy:: operator=(cheater);
		return *this;
}

// Cheater Player class
CheaterPlayerStrategy::CheaterPlayerStrategy() : PlayerStrategy("Cheater") {}
CheaterPlayerStrategy::CheaterPlayerStrategy(const CheaterPlayerStrategy& cheater) : PlayerStrategy(cheater) {}
CheaterPlayerStrategy::~CheaterPlayerStrategy() {}

bool CheaterPlayerStrategy::issueOrder(vector<Player*> AddedPlayerList) {
	cout << "---------------Cheater Player is issuing orders---------------" << endl;
	// This val will determine if Cheater wants to issue an order 
	int randVal = rand() % 10;
	if (randVal < 4) {
		cout << "[Cheater Player] Finish issuing an order\n";
		return false;
	}
	vector<Territory*> toAttackList = toAttack();
	if (toAttackList.size() == 0) {
		cout << "[Cheater Player] No more territory to conquer ---> Cannot issue order to conquer" << endl;
		return false;
	}
	for (Territory* t : toAttackList) {
		p->getOrderList()->addOrder(new conquer(t, p));
		cout << "[Cheater Player] Issue a conquer order successfully" << endl;
		return true;
	}
	cout << "[Cheater Player] Finish issuing an order" << endl;
	return false;
}

vector<Territory*> CheaterPlayerStrategy::toAttack() {
	vector<Territory*> toAttackList;
	vector<Territory*> ownedTerritories = p->getterriortiesOwned();
	for (Territory* t : ownedTerritories) {
		unordered_map<string, Territory*> neighbMap = t->getNeighbors();
		for (pair<string, Territory*> neighbor : neighbMap) {
			if (neighbor.second->getOwner() != p && find(toAttackList.begin(), toAttackList.end(), neighbor.second) == toAttackList.end()) {
				toAttackList.push_back(neighbor.second);
			}
		}
	}
	return toAttackList;
}
vector<Territory*> CheaterPlayerStrategy::toDefend() {
	return p->getterriortiesOwned();
}

// Display Territory names in Territory list
void displayTerritory(vector<Territory*> list) {
	bool first = true;
	for (int i = 0; i < list.size(); i++) {
		if (first) {
			first = false;
		}
		else {
			cout << "\t ";
		}
		cout << (i + 1) << ") " << list.at(i)->getName() << ": Holds " << list.at(i)->getNumOfArmies() << " armies";
	}
	cout << endl;
}
// Request input user 
int getInput(string message, string section, int max) {
	cout << message <<endl;
	int input;
	cin >> input;
	while (input < 1 || input > max) {
		cout << "[" << section << "]" << "You must select a number between " << 1 << " to " << max << ".Please try again!" << endl;
		cin >> input;
	}
	return input;
}
HumanPlayerStrategy& HumanPlayerStrategy::operator=(HumanPlayerStrategy& human)
{
	PlayerStrategy :: operator=(human);
	return *this;
}
// Human Player class
HumanPlayerStrategy::HumanPlayerStrategy() : PlayerStrategy("Human") {}
HumanPlayerStrategy::HumanPlayerStrategy (const HumanPlayerStrategy& human) : PlayerStrategy(human) {}
HumanPlayerStrategy::~HumanPlayerStrategy() {}
bool HumanPlayerStrategy::issueOrder(vector<Player*> AddedPlayerList) {
	cout << "----------------Human Player is issuing orders----------------" << endl;
	if (toDefend().size() == 0) {
		cout << "[Human Player] No territory to defend" << endl;
		return false;
	}
	// If reinforcement pool still have armies-- > Deploy
	if (p->getReinforcementPool() > 0) {
		int index = getInput("[Human] Do you want to issue an order?\n1)Yes\n2)No", "Issue An Order", 2);
		if (index == 2) {
			cout << "[Human Player] Finish issuing an order";
			return false;
		}
		vector<Territory*> defenceList = toDefend();
		cout << "[----------Issue Deploy Order----------" << endl;
		cout << "[Human Player] Your ToDefend List: \n";
		displayTerritory(defenceList);
		index = getInput("Which territory you want to issue deploy orders ?", "Issue Deploy Order", defenceList.size()) - 1;
		int numOfArmie = getInput("How many army units you want to deploy?", "Issue Deploy Order", p->getReinforcementPool());
		p->setReinforcementPool(p->getReinforcementPool() - numOfArmie);
		p->getOrderList()->addOrder(new deploy(numOfArmie, *defenceList.at(index), p));
		cout << "[Human Player]Your reinforcement pool currently is " << p->getReinforcementPool() << " armies" << endl;
		cout << "[Human Player] Issue a deploy order successfully" << endl;
		return true;
	}
	else {
		while (true) {
			string msg = "----------What you would like to do?----------\n1) Issue advance order\n2) Play a card\n3) Finish issuing order";
			int choice = getInput(msg, "Issuing Other Orders", 3);
			if (choice == 1) {
				msg = "----------How do you want to issue advance orders?----------\n1) To defend\n2) To attack";
				choice = getInput(msg, "Issuing Other Orderss", 2);
				if (choice == 1) {
					vector<Territory*> defendList = toDefend();
					if (defendList.size() == 0) {
						cout << "[Issuing Advance Order] No territory to defend" << endl;
						continue;
					}
					cout << "Your ToDefend List: \n";
					displayTerritory(defendList);
					choice = getInput("Which territory you want to move army units from?", "Issuing Advance Order To Defend", defendList.size()) - 1;
					Territory* sourceTerritory = defendList.at(choice);
					choice = getInput("Which territory you want to defend ?", "Issuing Advance Order To Defend", defendList.size()) - 1;
					Territory* targetTerritory = defendList.at(choice);
					int numOfArmie;
					cout << "How many army units you want to move from " << sourceTerritory->getName() << " to " + targetTerritory->getName() << endl;
					cin >> numOfArmie;
					p->getOrderList()->addOrder(new advancee(numOfArmie, *sourceTerritory, *targetTerritory, p));
					cout << "[Human Player] Issue an advance order successfully" << endl;
					return true;
				}
				else {
					vector<Territory*> ownedTerritory = p->getterriortiesOwned();
					if (ownedTerritory.size() == 0) {
						cout << "[Issuing Advance Order] You owned no territory" << endl;
						continue;
					}
					cout << "Your Territories: \n";
					displayTerritory(ownedTerritory);
					vector<Territory*> attackList = toAttack();
					choice = getInput("Which territory you want to move army units from?", "Issuing Advance Order To Attack", ownedTerritory.size()) - 1;
					Territory* sourceTerritory = ownedTerritory.at(choice);
					if (attackList.size() == 0) {
						cout << "[Issuing Advance Order] No territory to attack" << endl;
						continue;
					}
					
					choice = getInput("Which territory you want to attack ?", "Issuing Advance Order To Defend", attackList.size()) - 1;
					Territory* targetTerritory = attackList.at(choice);
					int numOfArmie;
					cout << "How many army units you want to move from " << sourceTerritory->getName() << " to " + targetTerritory->getName() << endl;
					cin >> numOfArmie;
					p->getOrderList()->addOrder(new advancee(numOfArmie, *sourceTerritory, *targetTerritory, p));
					cout << "[Human Player] Issue an advance order successfully" << endl;
					return true;
				}

			}
			else if (choice == 2) {
				vector<Card*> cards = p->getHandcard()->getCard();
				cout << "[Human Player] Play a card..." << endl;
				cout << "--------Your Card--------" << endl;
				for (int i = 0; i < cards.size(); i++) {
					cout << i + 1 << ") " << *cards.at(i) << "\t";
				}
				cout << endl;
				int index = getInput("Please select a card that you want to use!", "Play A Card", cards.size()) - 1;;
				//Get card from index
				Card* c = cards.at(index);
				if (c->getWarzoneCard() == Card::WarzoneCard::Diplomacy) {
					cout << "--------Issuing Negotiate Order Using Deplomacy Card--------" << endl;
					cout << "Players in Game" << endl;
					for (int i = 0; i < AddedPlayerList.size(); i++) {
						cout << i + 1 << ". " << AddedPlayerList.at(i)->getName() << endl;
					}
					index = getInput("Which player you want to negotiate?", "Play A Card", AddedPlayerList.size()) - 1;;
					Player* target = AddedPlayerList.at(index - 1);
					p->getOrderList()->addOrder(new negotiate(target, p));
					cout << "[Human Player] Issue an negotiate order successfully" << endl;
					return true;
				}
				if (c->getWarzoneCard() == Card::WarzoneCard::Blockade) {
					cout << "--------Issue Blockade Order Usisng Blockdade Card--------" << endl;
					vector<Territory*> ownedTerritories = toDefend();
					cout << "Your Territories: \n";
					displayTerritory(ownedTerritories);
					int index = getInput("Which territory you want to blockade", "Play A Card", ownedTerritories.size()) - 1;;
					Player* neutralPlayer = nullptr;
					for (Player* player: AddedPlayerList) {
						if (player->getPlayerStrategy()->getType() == "Neutral") {
							neutralPlayer = player;
							break;
						}
					}
					p->getOrderList()->addOrder(new blockade(*ownedTerritories.at(index), p, neutralPlayer)); // fix
					cout << "[Human Player] Issue an blockade order successfully" << endl;
					return true;
				}
				if (c->getWarzoneCard() == Card::WarzoneCard::Airlift) {
					cout << "--------Issue Airlift Order Usisng Airlift Card--------" << endl;
					vector<Territory*> ownedTerritories = toDefend();
					cout << "Your Territories: \n";
					displayTerritory(ownedTerritories);
					int index = getInput("Which territory you want to airlift armies from?", "Play A Card", ownedTerritories.size()) - 1;
					Territory* sourceTerritory = ownedTerritories.at(index);
					index = getInput("Which territory you want to airlift armies to?", "Play A Card", ownedTerritories.size()) - 1;
					Territory* targetTerritory = ownedTerritories.at(index);
					int numOfArmie;
					cout << "How many armies you want to airlift from " << sourceTerritory->getName() << " to " + targetTerritory->getName() << endl;
					cin >> numOfArmie;
					p->getOrderList()->addOrder(new airlift(numOfArmie, *sourceTerritory, *targetTerritory, p));
					cout << "[Human Player] Issue an airlift order successfully" << endl;
					return true;
				}
				if (c->getWarzoneCard() == Card::WarzoneCard::Reinforcement) {
					p->setReinforcementPool(p->getReinforcementPool() + 5);
					cout << "[Human Player] Added five cards in your reinforcement pool. Your reinforcement pool now have " << p->getReinforcementPool() << " armies" << endl;
					return true;
				}
				if (c->getWarzoneCard() == Card::WarzoneCard::Bomb) {
					cout << "--------Issue Bomb Order Usisng Bomb Card--------" << endl;
					vector<Territory*> toAttackTerritories = toAttack();
					if (toAttackTerritories.size() == 0) {
						cout << "[Issuing Advance Order] No territory to bomb" << endl;
						continue;
					}
					int index = getInput("Which territory you want to bomb?", "Play A Card", toAttackTerritories.size()) - 1;
					p->getOrderList()->addOrder(new bomb(*(toAttackTerritories.at(index)), p));
					cout << "[Human Player] Issue a bomb order successfully" << endl;
					return true;
				}
			}
			else {
				break;
			}
		}
	}
	cout << "[Human Player] Finish issuing an order" << endl;
	return false;
}

vector<Territory*> HumanPlayerStrategy::toAttack() {
	vector<Territory*> toAttackList;
	vector<Territory*> ownedTerritories = p->getterriortiesOwned();
	for (Territory* owned : ownedTerritories) {
		unordered_map<string, Territory*> neighbMap = owned->getNeighbors();
		for (pair<string, Territory*> neighbor : neighbMap) {
			if (neighbor.second->getOwner() != p && find(toAttackList.begin(), toAttackList.end(), neighbor.second) == toAttackList.end()) {
				toAttackList.push_back(neighbor.second);
			}
		}
	}
	cout << "Your Attack List: \n";
	displayTerritory(toAttackList);
	return toAttackList;
}
vector<Territory*> HumanPlayerStrategy::toDefend() {
	return p->getterriortiesOwned();
	
}



// Aggressive Player class

AggressivePlayerStrategy& AggressivePlayerStrategy::operator=(AggressivePlayerStrategy& agg)
{
	this->currentIndexToAttack = agg.currentIndexToAttack;
	PlayerStrategy::  operator=(agg);
	return *this;

}

AggressivePlayerStrategy::AggressivePlayerStrategy() : PlayerStrategy("Aggressive"), currentIndexToAttack(0) {}

AggressivePlayerStrategy::AggressivePlayerStrategy(const AggressivePlayerStrategy& aggressive) : PlayerStrategy(aggressive), currentIndexToAttack(aggressive.currentIndexToAttack) {}

AggressivePlayerStrategy::~AggressivePlayerStrategy() {}

bool AggressivePlayerStrategy::issueOrder(vector<Player*> AddedPlayerList) {
	cout << "----------------Aggressive Player Issues Order----------------\n";
	vector<Territory*> defendList = toDefend();
	if (defendList.size() == 0) {
		cout << "[Aggressive Player]: No territory to deploy!----Finish issuing order\n";
		currentIndexToAttack = 0;
		return false;
	}
	// If reinforcement pool still have armies --> Deploy
	if (p->getReinforcementPool() > 0) {
		p->getOrderList()->addOrder(new deploy(p->getReinforcementPool(), *(defendList.at(0)), p));
		p->setReinforcementPool(0);
		cout << "[Aggressive Player] Issue a deploy order successfully\n";
		return true;
	}
	else {
		int randVal = rand() % 10;
		if (randVal < 4) {
			cout << "[Aggressive Player] Finish issuing an order\n";
			return false;
		}
		Territory* strongestTeritory = defendList.at(0);
		vector<Territory*> attackList = toAttack();
		vector<Territory*> enemies;
		for (Territory* t : attackList) {
			if (strongestTeritory->getNeighbor(t->getName()) != nullptr) {
				enemies.push_back(t);
			}
		}
		if (enemies.size() == 0) {
			for (int i = 1; i < defendList.size(); i++) {
				if (strongestTeritory->getNeighbor(defendList[i]->getName()) != nullptr) {
					p->getOrderList()->addOrder(new advancee(strongestTeritory->getNumOfArmies(), *strongestTeritory, *defendList.at(i), p));
					cout << "[Aggressive Player] Issue an advance order successfully\n";
					return true;
				}
			}
		}
		randVal = rand() % 2;
		if (randVal == 0) {
			cout << "[Aggressive Player] Issuing Advance Order..." << endl;
			int randIndex = rand() % enemies.size();
			int counter = 0;
			// Check the next enemy if the player cannot attack the current enemy anymore;
			while (counter < 10 && enemies.at(randIndex)->getNumOfArmies() > strongestTeritory->getNumOfArmies()) {
				randIndex = rand() % enemies.size();
				counter++;
			}
			if (enemies.at(randIndex)->getNumOfArmies() > strongestTeritory->getNumOfArmies()) {
				cout << "[Aggressive Player] Finish issuing an order\n";
				return false;
			}
			p->getOrderList()->addOrder(new advancee(strongestTeritory->getNumOfArmies(), *strongestTeritory, *(enemies.at(randIndex)), p));
			cout << "[Aggressive Player] Issue an advance order successfully\n";
			return true;
		}
		else {
			cout << "[Aggressive Player] Play a card..." << endl;
			vector<Card*> usableCard;
			vector<Card*> cards = p->getHandcard()->getCard();
			for (Card* c : cards) {
				if (c->getWarzoneCard() == Card::WarzoneCard::Blockade || c->getWarzoneCard() == Card::WarzoneCard::Diplomacy) {
					continue;
				}
				usableCard.push_back(c);
			}
			if (usableCard.size() == 0) {
				cout << "[Aggressive Player] Finish issuing an order\n";
				return false;
			}
			int indexCard = rand() % usableCard.size();
			Card* c = usableCard.at(indexCard);
			if (c->getWarzoneCard() == Card::WarzoneCard::Airlift) {
				vector<Territory*> airliftTerritories;
				for (int i = 1; i < defendList.size(); i++) {
					airliftTerritories.push_back(defendList.at(i));
				}
				if (airliftTerritories.size() == 0) {
					cout << "[Aggressive Player] Finish issuing an order\n";
					return false;
				}
				int randIndex = rand() % airliftTerritories.size();
				Territory* sourceTerritory = airliftTerritories.at(randIndex);
				p->getOrderList()->addOrder(new airlift(sourceTerritory->getNumOfArmies(), *sourceTerritory, *strongestTeritory, p));
				cout << "[Aggressive Player] Issue an airlift order successfully\n";
				return true;
			}
			if (c->getWarzoneCard() == Card::WarzoneCard::Bomb) {
				int randIndex = rand() % enemies.size();
				p->getOrderList()->addOrder(new bomb(*(enemies.at(randIndex)), p));
				cout << "[Aggressive Player] Issue a bomb order successfully\n";
				return true;
			}
			if (c->getWarzoneCard() == Card::WarzoneCard::Reinforcement) {
				p->setReinforcementPool(p->getReinforcementPool() + 5);
				cout << "[Aggressive Player] Added five cards in your reinforcement pool. Your reinforcement pool now have " << p->getReinforcementPool() << " armies" << endl;
				return true;
			}
		}
		
	}				
	currentIndexToAttack = 0;
	cout << "[Aggressive Player] Finish issuing an order\n";
	return false;
}

vector<Territory*> AggressivePlayerStrategy::toAttack() {
	vector<Territory*> toAttackList;
	for (Territory* owned : p->getterriortiesOwned()) {
		for (pair<string, Territory*> neighbor : owned->getNeighbors()) {
			if (neighbor.second->getOwner() != p &&  find(toAttackList.begin(), toAttackList.end(), neighbor.second) == toAttackList.end()) {
				toAttackList.push_back(neighbor.second);
			}
		}
	}
	return toAttackList;
}

vector<Territory*> AggressivePlayerStrategy::toDefend() {
	vector<Territory*> defendTerritories = p->getterriortiesOwned();
	if (defendTerritories.size() == 0) {
		return defendTerritories;
	}
	int indexOfStrongestTeritory = 0;
	for (int i = 0; i < defendTerritories.size(); i++) {
		if (defendTerritories[i]->getNumOfArmies() >= defendTerritories.at(indexOfStrongestTeritory)->getNumOfArmies()) {
			indexOfStrongestTeritory = i;
		}
	}
	Territory* temp = defendTerritories[indexOfStrongestTeritory];
	defendTerritories[indexOfStrongestTeritory] = defendTerritories[0];
	defendTerritories[0] = temp;
	// Returns a new defend list where the strongest territory is the first element
	return defendTerritories;
}


// Benevolent Player class

BenevolentPlayerStrategy& BenevolentPlayerStrategy::operator=(BenevolentPlayerStrategy& b)
{
	PlayerStrategy:: operator=(b);
	return *this;
}

BenevolentPlayerStrategy::BenevolentPlayerStrategy() : PlayerStrategy("Benevolent") {}
BenevolentPlayerStrategy::BenevolentPlayerStrategy(const BenevolentPlayerStrategy& benevolent) : PlayerStrategy(benevolent) {}
BenevolentPlayerStrategy::~BenevolentPlayerStrategy() {}

bool BenevolentPlayerStrategy::issueOrder(vector<Player*> AddedPlayerList) {
	cout << "----------------Benevolent Player Issues Order----------------\n";
	vector<Territory*> defendList = toDefend();
	if (defendList.size() == 0) {
		cout << "[Benevolent Player]: No territory to deploy!----Issue deploy order unsuccessfully\n";
		return false;
	}
	if (p->getReinforcementPool() > 0) {
		p->getOrderList()->addOrder(new deploy(p->getReinforcementPool(), *(defendList.at(0)), p));
		p->setReinforcementPool(0);
		cout << "[Benevolent Player] Issue a deploy order successfully\n";
		return true;
	}
	else {
		int randVal = rand() % 10;
		if (randVal < 4) {
			cout << "[Benevolent Player] Finish issuing an order\n";
			return false;
		}
		cout << "Issuing Advance Order..." << endl;
		Territory* weakestTeritory = defendList.at(0);
		for (int i = 1; i < defendList.size(); i++) {
			if (defendList.at(i)->getNumOfArmies() != 0 && defendList.at(i)->getNeighbor(weakestTeritory->getName()) != nullptr) {
				p->getOrderList()->addOrder(new advancee(defendList.at(i)->getNumOfArmies(), *defendList.at(i), *weakestTeritory, p));
				cout << "[Benevolent Player] Issue a advance order successfully\n";
				return true;
			}
		}
		cout << "[Benevolent Player] Play a card..." << endl;
		vector<Card*> usableCard;
		vector<Card*> cards = p->getHandcard()->getCard();
		for (Card* c : cards) {
			if (c->getWarzoneCard() == Card::WarzoneCard::Bomb) {
				continue;
			}
			usableCard.push_back(c);
		}
		if (usableCard.size() == 0) {
			cout << "[Benevolent Player] Player finishes issuing an order" << endl;
			return false;
		}
		int indexCard = rand() % usableCard.size();
		Card* c = usableCard.at(indexCard);
		if (c->getWarzoneCard() == Card::WarzoneCard::Airlift) {
			vector<Territory*> airliftTerritories;
			for (int i = 1; i < defendList.size(); i++) {
				airliftTerritories.push_back(defendList.at(i));
			}
			int randIndex = rand() % airliftTerritories.size();
			Territory* sourceTerritory = airliftTerritories.at(randIndex);
			p->getOrderList()->addOrder(new airlift(sourceTerritory->getNumOfArmies(), *sourceTerritory, *weakestTeritory, p));
			cout << "[Benevolent Player] Issue an airlift order successfully\n";
			return true;
		}
		if (c->getWarzoneCard() == Card::WarzoneCard::Reinforcement) {
			p->setReinforcementPool(p->getReinforcementPool() + 5);
			cout << "[Benevolent Player] Added five cards in your reinforcement pool. Your reinforcement pool now have " << p->getReinforcementPool() << " armies" << endl;
			return true;
		}
		if (c->getWarzoneCard() == Card::WarzoneCard::Blockade) {
			cout << "--------Issue Blockade Order Usisng Blockdade Card--------" << endl;
			vector<Territory*> ownedTerritories = toDefend();
			cout << "Your Territories: \n";
			displayTerritory(ownedTerritories);
			int randIndex = rand() % ownedTerritories.size();
			Player* neutralPlayer = nullptr;
			for (Player* player : AddedPlayerList) {
				if (player->getPlayerStrategy()->getType() == "Neutral") {
					neutralPlayer = player;
					break;
				}
			}
			p->getOrderList()->addOrder(new blockade(*ownedTerritories.at(randIndex), p, neutralPlayer)); 
			cout << "[Benevolant Player] Issue an blockade order successfully" << endl;
			return true;
		}
		
		if (c->getWarzoneCard() == Card::WarzoneCard::Diplomacy) {
			cout << "--------Issuing Negotiate Order Using Deplomacy Card--------" << endl;
			cout << "Players in Game" << endl;
			for (int i = 0; i < AddedPlayerList.size(); i++) {
				cout << i + 1 << ". " << AddedPlayerList.at(i)->getName() << endl;
			}
			int randIndex = rand() % AddedPlayerList.size();
			Player* target = AddedPlayerList.at(randIndex );
			p->getOrderList()->addOrder(new negotiate(target, p));
			cout << "[Benevolant Player] Issue an negotiate order successfully" << endl;
			return true;
		}
	}
	cout << "[Benevolent Player] Player finishes issuing an order" << endl;
	return false;
}

vector<Territory*> BenevolentPlayerStrategy::toAttack() {
	return vector<Territory*>();
}

vector<Territory*> BenevolentPlayerStrategy::toDefend() {
	vector<Territory*> defendTerritories = p->getterriortiesOwned();
	int indexOfWeeakestTerritory = 0;
	if (defendTerritories.size() == 0) {
		cout << "No territories to defend" << endl;
	}
	for (int i = 0; i < defendTerritories.size(); i++) {
		if (defendTerritories.at(i)->getNumOfArmies() <= defendTerritories.at(indexOfWeeakestTerritory)->getNumOfArmies()) {
			indexOfWeeakestTerritory = i;
		}
	}
	Territory* temp = defendTerritories.at(indexOfWeeakestTerritory);
	defendTerritories.at(indexOfWeeakestTerritory) = defendTerritories.at(0);
	defendTerritories.at(0) = temp;
	// Returns a new defend list where the weakest territory is the first element
	return defendTerritories;
}
ostream& operator<<(ostream& os, PlayerStrategy& player)
{
	os << "strategy player ";
	return os;
	// TODO: insert return statement here
}

ostream& operator<<(ostream& os, NeutralPlayerStrategy& player)
{
	os << "Neutral Player";
	return os;
}

ostream& operator<<(ostream& os, CheaterPlayerStrategy& player)
{
	os << "Cheater Player";
	return os;
}

ostream& operator<<(ostream& os, HumanPlayerStrategy& player)
{
	os << "Human Player";
	return os;
}

ostream& operator<<(ostream& os, AggressivePlayerStrategy& player)
{
	os << "Aggressive Player";
	return os;
}

ostream& operator<<(ostream& os, BenevolentPlayerStrategy& player)
{
	os << "Benevolent Player";
	return os;
}