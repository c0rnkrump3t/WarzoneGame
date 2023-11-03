#pragma once
#include "Observer.h"
#include "Iloggable.h"
#include <list>
using namespace std;
class Subject {
public:
	// Default constructor
	Subject();
	// Copy constructor
	Subject(const Subject& s);
	// Destructor
	virtual ~Subject();
	// Attach an observer to the subject
	void Attach(Observer* o);
	// Detach an observer to the subject
	void Detach(Observer* o);
	// Notify observer if an ILoggable the subject changes
	virtual void Notify(ILoggable* o);
	// Override insertion operator
	friend ostream& operator<<(ostream& outs, const Subject& subject);
private:
	list<Observer*> observers; 
	// Override assignment operator
	Subject& operator=(const Subject& c);
};