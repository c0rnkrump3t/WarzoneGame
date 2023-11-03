#pragma once
#include "ILoggable.h"

class Observer {
public:
	// Destructor
	virtual ~Observer();
	// Update view whose observer observes
	virtual void Update(ILoggable* i) = 0;
	friend ostream& operator<<(ostream& out, Observer& o);
protected:
	// Default Constructor
	Observer();
	// Copy Constructor
	Observer(const Observer& other);
	// Override assignment operator
	Observer& operator=(const Observer& c);
};