#include "../HeaderFiles/Subject.h"
#include "../HeaderFiles/Observer.h"
#include "../HeaderFiles/Iloggable.h"
#include "../HeaderFiles/LogObserver.h"

#include <iostream>

// Default constructor
Subject::Subject() {
}
// Destructor
Subject::~Subject() {
}
// Copy constructor
Subject::Subject(const Subject& s) {}
// Attach an observer to the subject
void Subject::Attach(Observer* o) {
	observers.push_back(o);
}
// Detach an observer to the subject
void Subject::Detach(Observer* o) {
	observers.remove(o);
}
// Notify observer if an ILoggable subject changes
void Subject::Notify(ILoggable* log) {
	for (Observer* observer : observers) {
		observer->Update(log);
	}
}
// Override insertion operator
ostream& operator<<(ostream& outs, const Subject& subject) {
	list<Observer*> list = subject.observers;
	outs << "This is a subject which is observed by ";
	for (Observer* observer : list) {
		outs << observer;
	}
	return outs;
}

// Override assignment operator
Subject& Subject::operator=(const Subject& c) {
	return *this;
}