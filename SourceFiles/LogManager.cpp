#include "../HeaderFiles//LogManager.h"
#include <string>
#include <iostream>
#include <fstream>


using namespace std;

// Default constructor
LogManager::LogManager() : path("gamelog.txt") {
}
// User-defined constructor. Receive name of file that the user want to create
LogManager::LogManager(string p) : path(p) {};

// Copy constructor 
LogManager::LogManager(const LogManager& other) : path(other.path) {}
// Destructor
LogManager::~LogManager() {}
// Open file to append/read
void LogManager::openFile() {
	fs.open(path, ios::out | ios::app);
}
void LogManager::openNewFile() {
	fs.open(path, ios::out);
}
// Close file
void LogManager::closeFile() {
	fs.close();
}
// Write a string to file
void LogManager::writeToFile(string content) {
	fs << content;
	fs.flush();
}
// Print new line
void LogManager::println() {
	fs << endl;
}
// Override insertion operator
ostream& operator<<(ostream& outs, const LogManager& lm) {
	outs << "Log Manager Object. Path: " << lm.path << ", file status: File is " << (lm.fs.is_open() ? "opened" : "closed");
	return outs;
}
// Override assignment operator
LogManager& LogManager::operator=(const LogManager& c) {
	path = c.path;
	return *this;
}
