#pragma once
#include "Observer.h"

class Subject;
class LogObserver : public Observer {
public:
	// Default Constructor
	LogObserver();
	// Use-defined constructor
	LogObserver(Subject* s);
	// Copy constructor
	LogObserver(const LogObserver& other);
	// Destructor
	~LogObserver() override;
	// Update views
	void Update(ILoggable* i);
	// Override insertion operator
	friend ostream& operator<<(ostream& outs, const LogObserver& logObserver);
	// Override assignment operator
	LogObserver& operator=(const LogObserver& c);

private:
	// Subject that attaches this observer
	Subject* iLoggableSubject;
};
void testLoggingObserver();