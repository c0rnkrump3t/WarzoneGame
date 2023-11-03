#include "../HeaderFiles/MapDriver.h"
#include "../HeaderFiles/MapLoader.h"
#include "../HeaderFiles/Territory.h"

#include <iostream>
#include <string>

using namespace std;
// Test MapLoader
void testMapLists() {
	cout << "----------------------------------Testing MapLoader and validate()----------------------------------" << endl;
	// Set of map files
	string filePath[] = { 
		"valid5.map", "valid4.map", "valid3.map", "valid2.map", "valid1.map",
		"Tangrams.map", "Roman Empire.map", "Medieval Europe.map", "Japan.map", "invalid_no_territories.map", 
		"invalid_no_continents.map", "invalid_maploader_territory_non_existent_continent.map", "invalid_maploader_territory_no_continent_name.map", "invalid_maploader_no_territory_name.map", "invalid_maploader_no_continent_name.map", 
		"invalid_maploader_invalid_territory.map", "invalid_maploader_invalid_position.map", "invalid_maploader_invalid_neighbor.map", "invalid_maploader_invalid_continent_bonus.map", "invalid_maploader_invalid_continent.map", 
		"invalid_maploader_duplicate_territories.map", "invalid_maploader_duplicate_continents.map", "invalid_map_not_connected.map", "invalid_empty_file.map", "invalid_empty_continent.map", 
		"invalid_continent_not_connected.map","Hong Kong.map", "Great Lakes.map", "Grand Montreal.map", "Forest Battle.map", 
		"Flower C.map","Europe 4B.map", "Earth.map", "Discworld Mappe.map", "Dice.map", 
		"D-Day.map", "Colorado.map", "Canada.map", "AuIsometric.map", "AuCircles.map", 
		"Asia.map", "Annys Piratenwelt.map", "Alabama.map", "Aden.map", "ABC_Map.map", 
		"99 Mens Morris.map", "3D.map", "001_I72_Ghtroc 720.map", "_64_ BIG BLUE.map", "_47_ BattleField.map"
	};
	// Read all map files
	// Validate map objects that are created by MapLoader
	for (int i = 0; i < 50; i++) {
		cout << "-----------------File: " << filePath[i] << "-----------------" << endl;
		MapLoader loader("./test_maps/" + filePath[i]);
		shared_ptr<Map> map = loader.load();
		if (map == nullptr) {
			continue;
		}
		bool valid = map->validate();
		if (valid) {
			cout << "******Map is valid.******" << endl;
		}
		else {
			cout << "******Map is invalid.******" << endl;
		}
	}
	cout << "----------------------------------Printing a map----------------------------------" << endl;
	MapLoader loader("./test_maps/3D.map");
	shared_ptr<Map> map = loader.load();
	cout << *map << endl;

}