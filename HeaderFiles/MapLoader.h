#ifndef COMP345_MAPLOADER_H
#define COMP345_MAPLOADER_H

#include <string>

#include "Map.h"

using namespace std;
// Declaration for MapLoader
class MapLoader {
public:
	// Default construtor
	MapLoader();
	// User defined constructor
	MapLoader(string path);
	// Copy Constructor
	MapLoader(const MapLoader& map);
	// Destructor
	~MapLoader();
	// Read and process a map file
	shared_ptr<Map> load();
	// Override ostream to display MapLoader object
	friend ostream& operator<<(ostream& outs, const MapLoader& mapLoader);
	//  Assignment Operator
	MapLoader& operator=(const MapLoader& m);

private:
	string path_;
};

#endif // COMP345_MAPLOADER_H