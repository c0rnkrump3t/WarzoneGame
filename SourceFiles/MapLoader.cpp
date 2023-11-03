#include "../HeaderFiles/MapLoader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

#include "../HeaderFiles/Continent.h"
#include "../HeaderFiles/Territory.h"

// Trim a string
string trim(string str, char delimiter) {
	int start = 0;
	int end = str.length() - 1;
	while (start < str.length() && str[start] == delimiter) {
		start++;
	}
	while (end >= 0 && str[end] == delimiter) {
		end--;
	}
	return (end < start) ? "" : str.substr(start, end - start + 1);
}
// To Lowercase
string toLowercase(string str) {
	string newString = "";
	for (int i = 0; i < str.length(); i++) {
		newString += static_cast<char>(static_cast<unsigned char>(tolower(str[i])));
	}
	return newString;
}

// Split a lines to an array
vector<string> splitString(string str, char delimiter) {
	vector<string> result;
	stringstream stream(str);
	string segment;
	while (getline(stream, segment, delimiter)) {
		result.push_back(trim(segment, ' '));
	}
	return result;
}

// Converts "str" to an int and writes the result to "output".
// Returns false if the string could not be converted.
bool toInt(string str, int* output) {
	try {
		*output = stoi(str);
	}
	catch (invalid_argument&) {
		return false;
	}
	// Makes sure that converting the output back to a string gives back the original str.
	// This is because stoi will convert strings like "3,2" to "3".
	return (str == to_string(*output));
}

// Display errors and exit the program
void displayError(const string& error, ifstream& input) {
	input.close();
	cout << error << endl;
}

// Default construtor
MapLoader::MapLoader() : path_("./test_maps/3D.map") {}

// User defined constructor
MapLoader::MapLoader(string path) : path_(path) {}

// Copy Constructor
MapLoader::MapLoader(const MapLoader& map) : path_(map.path_) {}

// Destructor
MapLoader::~MapLoader() {}

// Read and process a map file
shared_ptr<Map> MapLoader::load() {
	shared_ptr<Map> map = shared_ptr<Map>(new Map());

	ifstream input;
	input.open("./test_maps/" + path_);
	// Check if file exist
	if (input.fail()) {
		displayError("[Error] Failed to open file " + path_, input);
		return nullptr;
	}

	string line;
	string currentlyProcessing = "";

	unordered_map<string, Continent*> continentMap;
	unordered_map<string, Territory*> territoryMap;
	unordered_map<string, vector<string>> territoryNeighborsMap;
	// Start read a file
	while (getline(input, line)) {
		if (line.empty()) continue;

		if (line[0] == '[' && line[line.length() - 1] == ']') {
			currentlyProcessing = line.substr(1, line.length() - 2);
			continue;
		}
		// Check if the file is empty or missing one of the sections
		if (currentlyProcessing == "Continents") {
			vector<string> continentInfo = splitString(line, '=');
			if (continentInfo.size() != 2) {
				displayError("[Error] Failed to process continent: " + line, input);
				return nullptr;
			}
			if (continentInfo[0].empty()) {
				displayError("[Error] No continent name: " + line, input);
				return nullptr;
			}
			int bonus;
			if (!toInt(continentInfo[1], &bonus)) {
				displayError("[Error] Invalid continent bonus: " + line, input);
				return nullptr;
			}

			unordered_map<string, Continent*>::iterator it = continentMap.find(toLowercase(continentInfo[0]));
			if (it != continentMap.end()) {
				displayError("[Error] Duplicate continent name: " + line, input);
				return nullptr;
			}

			Continent* continent = new Continent(continentInfo[0], bonus);
			map->addContinent(continent);
			
			continentMap.insert({ toLowercase(continent->getName()), continent });
		} else if (currentlyProcessing == "Territories") {
			vector<string> territoryInfo = splitString(line, ',');
			// The territory should have at least 4 info: name, x, y and continent.
			if (territoryInfo.size() < 4) {
				displayError("[Error] Failed to process territory: " + line, input);
				return nullptr;
			}
			if (territoryInfo[0].empty()) {
				displayError("[Error] No territory name: " + line, input);
				return nullptr;
			}
			int x, y;
			if (!toInt(territoryInfo[1], &x) || !toInt(territoryInfo[2], &y)) {
				displayError("[Error] Invalid territory x/y position: " + line, input);
				return nullptr;
			}
			if (territoryInfo[3].empty()) {
				displayError("[Error] Territory's continent name is empty: " + line, input);
				return nullptr;
			}
			unordered_map<string, Continent*>::iterator it = continentMap.find(toLowercase(territoryInfo[3]));
			if (it == continentMap.end()) {
				displayError("[Error] Non-existent continent name: " + line, input);
				return nullptr;
			}
			Continent* continent = it->second;
			unordered_map<string, Territory*>::iterator entry = territoryMap.find(toLowercase(territoryInfo[0]));
			if (entry != territoryMap.end()) {
				displayError("[Error] Duplicate territory name: " + line, input);
				return nullptr;
			}
			// Create territories, continents from file
			Territory* t = new Territory(territoryInfo[0], 0, nullptr, continent);
			territoryMap.insert({ toLowercase(t->getName()), t});
			continent->addTerritory(t);
			map->addTerritory(t);
			vector<string> list;
			for (int i = 4; i < territoryInfo.size(); i++) {
				list.push_back(territoryInfo[i]);
			}
			territoryNeighborsMap.insert({ t->getName(), list });
		} else {
			// we are neither in the Continents nor Territories section.
			// do nothing.
			continue;
		}
	}
	// Connect territories to each other
	for (pair<const string, vector<string>>& s : territoryNeighborsMap) {
		const string& territoryName = s.first;
		vector<string>& neighbors = s.second;
		Territory* currentTerritory = territoryMap.find(toLowercase(territoryName))->second;
		for (int i = 0; i < neighbors.size(); i++) {
			unordered_map<string, Territory*>::iterator entry = territoryMap.find(toLowercase(neighbors.at(i)));
			if (entry == territoryMap.end()) {
				displayError("[Error] Territory name does not exist: " + neighbors.at(i), input);
				return nullptr;
			}
			currentTerritory->addNeighbor(entry->second);
		}
	}

	input.close();

	return map;
}
// Override ostream to display MapLoader object
ostream& operator<<(ostream& outs, const MapLoader& mapLoader) {
	outs << "Map Loader - Reading file from: " << mapLoader.path_ << endl;
	return outs;
}

//  Assignment Operator
MapLoader& MapLoader::operator=(const MapLoader& m) {
	this->path_ = m.path_;
	return *this;
}