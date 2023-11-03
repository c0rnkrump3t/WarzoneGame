#include <iostream>
#include "../HeaderFiles/Player.h"
#include "../HeaderFiles/Cards.h"
#include "../HeaderFiles/Orders.h"
#include "../HeaderFiles/Territory.h"
#include "../HeaderFiles/PlayerStrategies.h"

using namespace std; 

int Player:: playerId = 1;
void Player::swap(int x)
{
	Territory* element = this->terriortiesOwned[x];
								//cout << "siuuuuu"<< target->getOwner()->getterriortiesOwned()[target->getOwner()->getterriortiesOwned().size() - 1]->getName() <<"\n";
	this->terriortiesOwned[x] = this->terriortiesOwned[terriortiesOwned.size() - 1];
	this->terriortiesOwned[terriortiesOwned.size() - 1] = element;
}
void Player::removeTerritory(int index)
{
	cout << " this the index "<< index<< " that will be removed "<<endl;
	if (index == 0) {
		terriortiesOwned.erase(terriortiesOwned.begin());
	}
	else {
		terriortiesOwned.erase(terriortiesOwned.begin() + index);
		cout << this->getName() << " got territories erased" << endl;
		for (int x = 0; x < terriortiesOwned.size();x++) {
			cout << terriortiesOwned[x]->getName()<<endl;
		}
	}
	//this->terriortiesOwned.pop_back();
}
void Player::addnegotiationlist(Player *p)
{
	negotiateList.push_back(p);
}
void Player::clearnegotiationlist()
{
	for (int i = 0; i < negotiateList.size(); i++) {
		negotiateList.pop_back();
	}
}
// static variable
Player::Player() {// initializing the default constructor
	id = playerId++;// increasing the player id
	cout << "id " << id << endl;
	terriortiesOwned = {};// empty vector 
	reinforcementPool = 0;
}
Player::Player(const Player& p) {// initializing the copy constructor
	this->playerId = p.playerId;
	this->terriortiesOwned = p.terriortiesOwned;
	for (int i = 0; i < terriortiesOwned.size(); i++) {// for loop to copy all terriorties owned that player has to the other play terriorties
		terriortiesOwned[i] = p.terriortiesOwned[i];
	}
	this->orderList = p.orderList;
	this->handCard = p.handCard;
	this->ps = p.ps;
	reinforcementPool = 0;
}

Player::Player(string name, PlayerStrategy* inputPS) : id(playerId++), name(name), reinforcementPool(0), ps(inputPS) {// initializing the parameterized constructor 
	inputPS->setPlayer(this);
}

Player::~Player() {// deconstructor method
	if (ps) {
		delete ps;
	}
	ps = NULL;
	playerId = NULL;
	terriortiesOwned.clear();// clearing the terriorties vector 
}
vector <Territory*> Player:: toAttack() {// toAttack method returns a list of territories that the player wish to attack
	return ps->toAttack();
}
vector < Territory*>  Player:: toDefend() {// toDefend method returns a list of territories that the player should defend
	/*
	cout << "The method toDefend from player class is being called" <<endl;
	
	for (int i = 0; i< terriortiesOwned.size(); i++) {
		cout << i+1 << " = " << terriortiesOwned[i]->getName() << endl;
	}
	*/
	return ps->toDefend();
}
bool Player::issueOrder(vector<Player*> AddedPlayerList) {// add a order that the player gives to the orderlist that the player owns
	return ps->issueOrder(AddedPlayerList);
}
int Player:: getPlayerID() {// getter for playerID
	return id;
}
Hand* Player:: getHandcard() {// getter for hand
	return &handCard;
}
vector <Territory*> Player:: getterriortiesOwned() {// returns the terriorties owned 
	return terriortiesOwned;
}
orderlist* Player::getOrderList() {// returns the orderlist that the player owns
	return &orderList;
}
int Player::getReinforcementPool() {
	return reinforcementPool;
}

void Player::setReinforcementPool(int n) {
	this->reinforcementPool = n;
}
void Player::setTerriorty(vector <Territory*> terriorties) {// setter for terriorties owned
	this->terriortiesOwned= terriorties;
}
ostream& operator <<(std::ostream& display, const Player& p) {// prints the player information and overrivdes the << operator 
	display << "Player " << p.id << " own the following terriorties" << endl;
	for (Territory* terriorty : p.terriortiesOwned) {
		display << terriorty->getName();
	}
;
	display << "\nPlayer " << p.id << " has the following cards in their hands " << p.handCard << endl;
	display << "Player " << p.id << " can order from this list shown: " << p.orderList << endl;
	cout << endl;
	
	return display;

}
void Player::addTerriorty(Territory* t) {//adds a terriorty to the player terriortiesowned
	terriortiesOwned.push_back(t);
	t->setOwner(this);
}

Player& Player::operator=(const Player& p1) {// overrides the assignment operator
	playerId = p1.playerId;
	terriortiesOwned = p1.terriortiesOwned;
	handCard = p1.handCard;
	orderList = p1.orderList;
	cout << "Using the assignment operator method from the player class";
	return *this;
}

vector<Player*> Player::getnegotiateList() //getter for negotiate list
{
	return this->negotiateList;
}

string Player::getName()
{
	return name;
}

void Player::setName(string name)
{
	this->name = name;
}

PlayerStrategy* Player::getPlayerStrategy() const {
	return ps;

}
void Player::setPlayerStrategy(PlayerStrategy* newPS) {
	if (ps != nullptr) {
		delete ps;
	}
	ps = newPS;
	ps->setPlayer(this);
}