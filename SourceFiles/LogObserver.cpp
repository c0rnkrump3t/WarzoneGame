#pragma once
#include "../HeaderFiles/LogObserver.h"
#include "../HeaderFiles/Subject.h"
#include "../HeaderFiles/LogManager.h"
#include <iostream>

// Default Constructor
LogObserver::LogObserver() : iLoggableSubject(nullptr) {}

// Copy Constructor
LogObserver::LogObserver(const LogObserver& other) : iLoggableSubject(other.iLoggableSubject) {
	if (iLoggableSubject != nullptr) {
		iLoggableSubject->Attach(this);
	}
}

// Use-defined constructor
LogObserver::LogObserver(Subject* s) : iLoggableSubject(s) {
	if (iLoggableSubject != nullptr) {
		iLoggableSubject->Attach(this);
	}
}
// Destructor
LogObserver::~LogObserver() {
	if (iLoggableSubject != nullptr) {
		iLoggableSubject->Detach(this);
	}
}
// Update views
void LogObserver::Update(ILoggable* i) {
	string s = i->stringToLog();
	// Write to file
	LogManager lm;
	lm.openFile();
	lm.writeToFile(s);
	lm.println();
	lm.writeToFile("---------------------------------------------------------------------------------------------------------------------------------------------------------");
	lm.println();
	lm.closeFile();
}
// Override insertion operator
ostream& operator<<(ostream& outs, const LogObserver& logObserver) {
	outs << "LogObserver object: Observe " << logObserver.iLoggableSubject;
	return outs;
}
// Override assignment operator
LogObserver& LogObserver::operator=(const LogObserver& c) {
	if (iLoggableSubject != nullptr) {
		iLoggableSubject->Detach(this);
	}
	iLoggableSubject = c.iLoggableSubject;
	if (iLoggableSubject != nullptr) {
		iLoggableSubject->Attach(this);
	}
	return *this;
}