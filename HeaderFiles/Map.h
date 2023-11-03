#ifndef COMP345_MAP_H
#define COMP345_MAP_H

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;
//Forward declaration for Continent and Territory
class Territory;
class Continent;

//Declaration for Map
class Map {
public:
	// Default Constructor
	Map();
	// Copy constructer
	Map(const Map& other);
	// Destructor
	~Map();
	// Return list of territories that the map contains
	unordered_map<string, Territory*> getTerritories() const;
	// Return contienents that belongs to the map
	unordered_map<string, Continent*> getContinents() const;
	// Set a new list of territories to the map
	void setTerritories(unordered_map<string, Territory*> territories);
	// Set a new list of continents to the map
	void setContinents(unordered_map<string, Continent*> continents);
	// Add a new territory to the map
	void addTerritory(Territory* t);
	// Add a new continent to the map
	void addContinent(Continent* c);
	// Get a territory that belongs to the map (using the territory's name)
	Territory* getTerritory(string name);
	// Get a continent that belongs to the map (using the continent's name)
	Continent* getContinent(string name);

	// Check if the map is valid
	bool validate();

	// Override ostream to display map object
	friend ostream& operator<<(ostream& outs, const Map& map);
	// Assignment operator
	Map& operator=(const Map& c);

//Private attributes
private:
	unordered_map<string, Territory*> territories_;
	unordered_map<string, Continent*> continents_;
};


#endif // COMP345_MAP_H
