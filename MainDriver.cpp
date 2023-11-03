#include <iostream>
#include "HeaderFiles/Cards.h"
#include "HeaderFiles/GameEngine.h"
#include "HeaderFiles/Orders.h"
#include "HeaderFiles/Player.h"
#include "HeaderFiles/MapLoader.h"
#include "HeaderFiles/MapDriver.h"
#include "HeaderFiles/LogObserver.h"


int main() {
	srand(time(NULL)); //Comment if you wanna have the same output to test specific cases.
	//testPlayerStrategies();
	testTournament();
	return 0;
}
