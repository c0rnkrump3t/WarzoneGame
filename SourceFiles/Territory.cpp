#include "../HeaderFiles/Territory.h"

// Default constructor
Territory::Territory() : name_("Default"), numOfArmies_(0), owner_(nullptr), continent_(nullptr) {}

// Copy constructor
Territory::Territory(const Territory& other) : name_(other.name_), neighbors_(other.neighbors_), numOfArmies_(other.numOfArmies_), owner_(other.owner_), continent_(other.continent_) {}

// User defined constructor
Territory::Territory(string name, int armies, Player* owner, Continent* continent) : name_(name), numOfArmies_(armies), owner_(owner), continent_(continent) {}

// Destructor
Territory::~Territory() {}

// Return the name of the territory
string Territory::getName() const {
	return name_;
}

// Return the neighbors of the territory
unordered_map<string, Territory*> Territory::getNeighbors() const {
	return neighbors_;
}

// Return the number of armies that is on the territory
int Territory::getNumOfArmies() const {
	return numOfArmies_;
}

// Return the owner of the territory
Player* Territory::getOwner() const {
	return owner_;
}

//Return the continent to which the teritory belongs
Continent* Territory::getContinent() const {
	return continent_;
}

// Set a name for the territory
void Territory::setName(string newName) {
	name_ = newName;
}

// Set a list of neighbors for the territory
void Territory::setNeighbors(unordered_map<string, Territory*> newNeighbors) {
	neighbors_ = newNeighbors;
}

// Set the number of armies for the territory
void Territory::setNumOfArmies(int newArmies) {
	if (newArmies < 0) {
		newArmies = 0;
	}
	numOfArmies_ = newArmies;
}

// Set an owner for the territory
void Territory::setOwner(Player* newPlayer) {
	owner_ = newPlayer;
}

// Set a continent for the territory
void Territory::setContinent(Continent* newContinent) {
	continent_ = newContinent;
}

// Add a new territory to the territory's neighbors
void Territory::addNeighbor(Territory* n) {
	neighbors_.insert({ n->getName(), n });
}

// Get a neighbor of the territory (by name)
Territory* Territory::getNeighbor(string name) {
	unordered_map<string, Territory*>::iterator t = neighbors_.find(name);
	if (t == neighbors_.end()) {
		return nullptr;
	}
	return t->second;
}

// Override ostream to display Territory object
ostream& operator<<(ostream& outs, const Territory& territory) {
	outs << "Territory:" << territory.name_ << ";Neighbors:[";
	bool first = true;
	for (pair<string, Territory*> neighbor : territory.neighbors_) {
		if (first) {
			first = false;
		} else {
			outs << ",";
		}
		outs << neighbor.second->name_;
	}
	outs << "]";
	outs << ";Number of Armies:" << territory.numOfArmies_ << ";Owner:" << "Player-Name" << ";Continent:" << territory.continent_->getName();
	return outs;
}

// Assignment operator
Territory& Territory::operator=(const Territory& t) {
	this->name_ = t.name_;
	this->neighbors_ = t.neighbors_;
	this->numOfArmies_ = t.numOfArmies_;
	this->owner_ = t.owner_;
	this->continent_ = t.continent_;
	return *this;
}

