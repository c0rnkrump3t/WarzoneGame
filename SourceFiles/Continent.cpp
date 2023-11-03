#include "../HeaderFiles/Continent.h"

#include <iostream>

#include "../HeaderFiles/Territory.h"

// Default constructor
Continent::Continent() : name_("Default"), bonus_(0) {}

// User defined constructor
Continent::Continent(string name, int bonus) : name_(name), bonus_(bonus) {}

// Copy constructor
Continent::Continent(const Continent& c) : name_(c.name_), bonus_(c.bonus_), territories_(c.territories_) {}

//Destructor
Continent::~Continent() {
	// delete all territories that belong to this continent
	for (pair<string, Territory*> territory: territories_) {
		delete territory.second;
	}
};

// Return the name of the continent
string Continent::getName() const {
	return name_;
}

// Return the bonus of the continent
int Continent::getBonus() const {
	return bonus_;
}

// Return the list of territories that the continent contains
unordered_map<string, Territory*> Continent::getTerritories() const {
	return territories_;
}

// Set a name for the continent
void Continent::setName(string name) {
	name_ = name;
}

// Set a bonus point that the continent controls
void Continent::setBonus(int bonus) {
	bonus_ = bonus;
}

// Set a new list of territories that is contained by the continent
void Continent::setTerritories(unordered_map<string, Territory*> territories) {
	territories_ = territories;
}

// Add a new territory to the continent's territories
void Continent::addTerritory(Territory* t) {
	territories_.insert({ t->getName(), t });
}

// Get a territory that the continent contains (by name)
Territory* Continent::getTerritory(string name) {
	unordered_map<string, Territory*>::iterator t = territories_.find(name);
	if (t == territories_.end()) {
		return nullptr;
	}
	return t->second;
}

// Override ostream to display the Continent object
ostream& operator<<(ostream& outs, const Continent& continent) {
	outs << "Continent " << continent.name_ << ", bonus " << continent.bonus_ << ": {" << endl;
	for (pair<string, Territory*> territory : continent.territories_) {
		outs << *(territory.second) << endl;
	}
	outs << "}";
	return outs;
}

// Assignment operator
Continent& Continent::operator=(const Continent& c) {
	this->name_ = c.name_;
	this->bonus_ = c.bonus_;
	this->territories_ = c.territories_;
	return *this;
}