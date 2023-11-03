#include "../HeaderFiles/ILoggable.h"

// Default constructor 
ILoggable::ILoggable() {}
// Copy constructor 
ILoggable::ILoggable(const ILoggable& other) {}
// Destructor
ILoggable::~ILoggable() {}
//Override insert operator
ostream& operator<<(ostream& outs, const ILoggable& ilog) {
	outs << "An abstract ILoggable object";
	return outs;
}
// Override assignment operator
ILoggable& ILoggable::operator=(const ILoggable& c) {
	return *this;
}