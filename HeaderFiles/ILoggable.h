#pragma once
#include <string>
#include <iostream>
using namespace std;

class ILoggable {
public:
	// Destructor
	virtual ~ILoggable();
	// Returns a string to be output to the log file
	virtual std::string stringToLog() = 0;
	//Override insertion operator
	friend ostream& operator<<(ostream& outs, const ILoggable& ilog);

protected:
	// Constructor is protected since we want to create its subclass only
	// ILoggable should never be created by itself
	ILoggable();
	//Copy constructor
	ILoggable(const ILoggable& other);
	// Override assignment operator
	ILoggable& operator=(const ILoggable& c);
};