#ifndef COMP345_PLAYER_H
#define COMP345_PLAYER_H

//#include "Cards.h"
//#include"Territory.h"
#include "Cards.h"
#include "Orders.h"
#include "PlayerStrategies.h"
#include <vector>

using namespace std;
class Territory;

class Player {
public:
    void swap(int x);
    void removeTerritory(int index);
    void addnegotiationlist(Player *p);
    void clearnegotiationlist();
    Player();// default constructor 
    Player(const Player&); // copy constructor
    Player(string name, PlayerStrategy* inputPS);// parameterized constructor 
    ~Player(); // deconstructor 
    vector <Territory*> toAttack();// toAttack method that takes two territories as parameter one to and one from 
    vector <Territory*> toDefend();// lists all the territories that the player should defend
    bool issueOrder(vector<Player*> AddedPlayerList);// takes an order and add it to the orderlist that the player has
    int getPlayerID();// getter for playerID
    Hand* getHandcard();// getter for hand
    vector <Territory*> getterriortiesOwned();// getter for terriorties owned
    orderlist* getOrderList();// getter for orderlist
    void setTerriorty(vector <Territory*>);// setter for terriorties owned
    friend ostream& operator <<(std::ostream&, const Player& ); // override the << stream operation
    void addTerriorty(Territory*);// method that adds a territory to the terriorties owned list
    Player& operator=(const Player&);// override the assignment operator 
    int getReinforcementPool(); //Return player's reinforcement pool 
    void setReinforcementPool(int n);
    vector< Player*> getnegotiateList();// getter for negotiate list
    string getName();
    PlayerStrategy* getPlayerStrategy() const;
    void setPlayerStrategy(PlayerStrategy* newPS);
    void setName(string name);
    bool conquer = false;

private:
    PlayerStrategy* ps;
    static int playerId;// static player ID
    Hand handCard;
    orderlist orderList;
    vector<Territory*> terriortiesOwned;
    int id;
    int reinforcementPool;
    vector<Player* >negotiateList;
    string name;
};

void testPlayers();
#endif //COMP345_PLAYER_H
