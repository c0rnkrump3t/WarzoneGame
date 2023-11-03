#pragma once
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

// This class handle opening/writing to a txt file
class LogManager {

private:

	string path;
	fstream fs;

public:
	// Default constructor. Create gamelog.txt by default
	LogManager();
	// Copy constructor 
	LogManager(const LogManager& other);
	// User-defined constructor. Receive name of file that the user want to create
	LogManager(string p);
	// Destructor
	~LogManager();
	// Open file to append/read
	void openFile();
	// Open file to read
	void openNewFile();
	// Close file
	void closeFile();
	// Write a string to file
	void writeToFile(string content);
	// Print line
	void println();
	// Override insertion operator
	friend ostream& operator<<(ostream& outs, const LogManager& lm);
	// Override assigment operator
	LogManager& operator=(const LogManager& c);
};