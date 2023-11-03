#ifndef COMP345_TERRITORY_H
#define COMP345_TERRITORY_H

#include <iostream>
#include <vector>
#include "Continent.h"
#include <unordered_map>

class Player;

using namespace std;
// Declaration for Territory
class Territory
{
public:
	// Default constructor
	Territory();
	// Copy constructor
	Territory(const Territory& other);
	// User defined constructor
	Territory(string name, int armies, Player* owner, Continent* continent);
	// Destructor
	~Territory();
	// Return the name of the territory
	string getName() const;
	// Return the neighbors of the territory
	unordered_map<string, Territory*> getNeighbors() const;
	// Return the number of armies that is on the territory
	int getNumOfArmies() const;
	// Return the owner of the territory
	Player* getOwner() const;
	//Return the continent to which the teritory belongs
	Continent* getContinent() const;
	// Set a name for the territory
	void setName(string newName);
	// Set a list of neighbors for the territory
	void setNeighbors(unordered_map<string, Territory*> newNeighbors);
	// Set the number of armies for the territory
	void setNumOfArmies(int newArmies);
	// Set an owner for the territory
	void setOwner(Player* newOwner);
	// Set a continent for the territory
	void setContinent(Continent* newContinient);
	// Add a new territory to the territory's neighbors
	void addNeighbor(Territory* n);
	// Get a neighbor of the territory (by name)
	Territory* getNeighbor(string name);
	// Override ostream to display Territory object
	friend ostream& operator<<(ostream& outs, const Territory& territory);
	// Assignment operator
	Territory& operator=(const Territory& t);

//private attributes
private:
	string name_;
	unordered_map<string, Territory*> neighbors_;
	int numOfArmies_;
	Player* owner_;
	Continent* continent_;
};

#endif // COMP345_TERRITORY_H