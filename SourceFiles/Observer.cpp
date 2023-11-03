#include "../HeaderFiles/Observer.h"

// Default Constructor
Observer::Observer() {}
// Copy constructor
Observer::Observer(const Observer& other) {}
// Destructor
Observer::~Observer() {}
// Override assignment operator
Observer& Observer::operator=(const Observer& c) {
	return *this;
}
// Override insertion operator
ostream& operator<<(ostream& out, Observer& o) {
	out << "This is an abstract observer";
	return out;
}
