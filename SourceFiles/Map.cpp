#include "../HeaderFiles/Map.h"

#include <iostream>

#include "../HeaderFiles/Territory.h"
#include "../HeaderFiles/Continent.h"
#include <unordered_map>
#include <unordered_set>

//Create directed map
void createDirectedMap(Map* copyMap, const unordered_map<string, Territory*>& originTeritories) {
	//Connect all copied territories
	for (pair<string, Territory*> territory : originTeritories) {
		Territory* current = territory.second;
		Territory* cloneCurrent = copyMap->getTerritory(current->getName());
		for (pair<string, Territory*> n : current->getNeighbors()) {
			Territory* neighbor = n.second;
			Territory* cloneNeighbor = copyMap->getTerritory(neighbor->getName());
			cloneCurrent->addNeighbor(cloneNeighbor);
		}
	}
}

// Create an undirected map
void createUndirectedGraph(Map* copyMap, const unordered_map<string, Territory*>& originTeritories) {
	//Connect all copied territories
	for (pair<string, Territory*> territory : originTeritories) {
		Territory* current = territory.second;
		Territory* cloneCurrent = copyMap->getTerritory(current->getName());
		for (pair<string, Territory*> n : current->getNeighbors()) {
			Territory* neighbor = n.second;
			Territory* cloneNeighbor = copyMap->getTerritory(neighbor->getName());
			cloneCurrent->addNeighbor(cloneNeighbor);
			cloneNeighbor->addNeighbor(cloneCurrent);
		}
	}
}


// Copy all continents and territories
void copyNodes(Map* map, const Map& other) {
	//Get the continents and territories of the input map
	unordered_map<string, Continent*> continents = other.getContinents();
	unordered_map<string, Territory*> territories = other.getTerritories();

	unordered_map<string, Continent*> cloneContinents;;
	unordered_map<string, Territory*> cloneTerritories;
	
	//Make a deep copy of the continents. These new continent will not contain any territories
	for (pair<string, Continent*> continent : continents) {
		Continent* c = continent.second;
		Continent* cloneContinent = new Continent(c->getName(), c->getBonus());
		cloneContinents.insert({ cloneContinent->getName(), cloneContinent });
		map->addContinent(cloneContinent);
	}

	//Make a deep copy of the territories. These new terrioties will not have neighbors	
	for (pair<string, Territory*> territory : territories) {
		Territory* t = territory.second;
		Continent* cloneContinent = cloneContinents.find(t->getContinent()->getName())->second;
		Territory* cloneTerritory = new Territory(t->getName(), t->getNumOfArmies(), t->getOwner(), cloneContinent);
		cloneTerritories.insert({ cloneTerritory->getName(), cloneTerritory });
		map->addTerritory(cloneTerritory);
		cloneContinent->addTerritory(cloneTerritory);
	}
}

// Return the number of node we have visited
int getTotalVisitedNodes(Territory* node, Continent* continent, unordered_set<Territory*>& seen) {
	if (node == nullptr) {
		return 0;
	}
	if (continent != nullptr && node->getContinent() != continent) {
		return 0;
	}
	unordered_set<Territory*>::iterator currentNode = seen.find(node);
	if (currentNode != seen.end()) {
		return 0;
	}
	seen.insert(node);
	int sumNeighbors = 0;
	for (pair<string, Territory*> n : node->getNeighbors()) {
		sumNeighbors += getTotalVisitedNodes(n.second, continent, seen);
	}
	return 1 + sumNeighbors;
}

// Default Constructor
Map::Map() {}

// Copy constructer
Map::Map(const Map& other) {
	copyNodes(this, other);
	createDirectedMap(this, other.territories_);
}

// Destructor
Map::~Map() {
	// delete all continents
	// In the destructor of the continent,
	// we delete all their territories, so they will also be deleted.
	for (pair<string, Continent*> continent : continents_) {
		delete continent.second;
	}
}

// Return list of territories that the map contains
unordered_map<string, Territory*> Map::getTerritories() const {
	return territories_;
}

// Return contienents that belongs to the map
unordered_map<string, Continent*> Map::getContinents() const {
	return continents_;
}

// Set a new list of territories to the map
void Map::setTerritories(unordered_map<string, Territory*> territories) {
	territories_ = territories;
}

// Set a new list of continents to the map
void Map::setContinents(unordered_map<string, Continent*> continents) {
	continents_ = continents;
}

// Add a new territory to the map
void Map::addTerritory(Territory* t) {
	territories_.insert({ t->getName(), t });
}

// Add a new continent to the map
void Map::addContinent(Continent* c) {
	continents_.insert({ c->getName(), c});
}

// Get a territory that belongs to the map (using the territory's name)
Territory* Map::getTerritory(string name) {
	unordered_map<string, Territory*>::iterator t = territories_.find(name);
	if (t == territories_.end()) {
		return nullptr;
	}
	return t->second;
}

// Get a continent that belongs to the map (using the continent's name)
Continent* Map::getContinent(string name) {
	unordered_map<string, Continent*>::iterator c = continents_.find(name);
	if (c == continents_.end()) {
		return nullptr;
	}
	return c->second;
}

// Check if the map is valid
bool Map::validate() {
	shared_ptr<Map> undirectedMap = shared_ptr<Map>(new Map());
	copyNodes(undirectedMap.get(), *this);
	createUndirectedGraph(undirectedMap.get(), territories_);
	if (undirectedMap->continents_.size() == 0) {
		cout << "[Map Validate Error] Map contains no continents." << endl;
		return false;
	}
	if (undirectedMap->territories_.size() == 0) {
		cout << "[Map Validate Error] Map contains no territories." << endl;
		return false;
	}
	Territory* startNode = undirectedMap->territories_.begin()->second;
	unordered_set<Territory*> seen;
	int total = getTotalVisitedNodes(startNode, nullptr, seen);
	if (total != undirectedMap->territories_.size()) {
		cout << "[Map Validate Error] Map is not a connected graph." << endl;
		return false;
	}
	for (pair<string, Continent*> n : undirectedMap->continents_) {
		Continent* current = n.second;
		if (current->getTerritories().size() == 0) {
			cout << "[Map Validate Error] Continent " << current->getName() << " contains no territories." << endl;
			return false;
		}
		Territory* startTerritory = current->getTerritories().begin()->second;
		seen.clear();
		int totalTeritories = getTotalVisitedNodes(startTerritory, current, seen);
		if (totalTeritories != current->getTerritories().size()) {
			cout << "[Map Validate Error] Continent " << current->getName() << " is not a connected subgraph." << endl;
			return false;
		}
	}
	seen.clear();
	// Check if the terrioty is in more than one continent
	//Loop through continents, in each continent, insert its territories to the seen map.
	//If a territory is seen in more than 1 continent, the map is invalid
	for (pair<string, Continent*> c : undirectedMap->continents_) {
		for (pair<string, Territory*> t : c.second->getTerritories()) {
			unordered_set<Territory*>::iterator currentTerritory = seen.find(t.second);
			if (currentTerritory != seen.end()) {
				cout << "[Map Validate Error] Territory " << t.second->getName() << " is in multiple continents." << endl;
				return false;
			}
			seen.insert(t.second);
		}
	}
	for (pair<string, Territory*> t : undirectedMap->territories_) {
		Territory* territory = t.second;
		// Check if the territory is owned by a continent
		Continent* continent = territory->getContinent();
		if (continent == nullptr) {
			cout << "[Map Validate Error] Territory " << territory->getName() << " does not belong to any continent." << endl;
			return false;
		}
		// Check if the continent contains the territory
		Territory* temp = continent->getTerritory(territory->getName());
		if (temp == nullptr) {
			cout << "[Map Validate Error] Continent " << continent->getName() << " does not contain " << territory->getName() << "." << endl;
			return false;
		}
		if (temp != territory) {
			cout << "[Map Validate Error] There is a conflict between continent " << continent->getName() << " and territory " << territory->getName() << "." << endl;
			return false;
		}
	}
	return true;
}

// Override ostream to display map object
ostream& operator<<(ostream& outs, const Map& map) {
	for (pair<string, Continent*> continent : map.continents_) {
		outs << *(continent.second) << endl;
	}
	return outs;
}

// Assignment operator
Map& Map::operator=(const Map& c) {
	// It is possible that this map contains territories and continents
	// Before assign it to a new map, we should clear all territories and continents to prevent memory leak
	for (pair<string, Continent*> continent : continents_) {
		delete continent.second;
	}
	this->continents_.clear();
	this->territories_.clear();
	copyNodes(this, c);
	createDirectedMap(this, c.territories_);
	return *this;
}
