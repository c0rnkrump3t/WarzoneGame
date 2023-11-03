#ifndef COMP345_CONTINENT_H
#define COMP345_CONTINENT_H

#include <string>
#include <unordered_map>

using namespace std;

// Forward declare class Territory
class Territory;
// Declaration for Continent
class Continent {

public:
	// Default constructor
	Continent();
	// User defined constructor
	Continent(string name, int bonus);
	// Copy constructor
	Continent(const Continent& c);
	//Destructor
	~Continent();
	// Return the name of the continent
	string getName() const;
	// Return the bonus of the continent
	int getBonus() const;
	// Return the list of territories that the continent contains
	unordered_map<string, Territory*> getTerritories() const;
	// Set a name for the continent
	void setName(string name);
	// Set a bonus point that the continent controls
	void setBonus(int bonus);
	// Set a new list of territories that is contained by the continent
	void setTerritories(unordered_map<string, Territory*> territories);
	// Add a new territory to the continent's territories
	void addTerritory(Territory* t);
	// Get a territory that the continent contains (by name)
	Territory* getTerritory(string name);
	// Override ostream to display the Continent object
	friend ostream& operator<<(ostream& outs, const Continent& continent);
	// Assignment operator
	Continent& operator=(const Continent& c);

// Private attibutes
private:
	string name_;
	int bonus_;
	unordered_map<string, Territory*> territories_;
};

#endif // COMP345_CONTINENT_H