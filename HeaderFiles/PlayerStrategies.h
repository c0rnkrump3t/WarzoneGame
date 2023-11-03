#pragma once
#include <vector>
#include <iostream>
#include <string>

using namespace std;

class Territory;
class Player;

class PlayerStrategy {

protected:
	Player* p;
	string type;
	PlayerStrategy();
	PlayerStrategy(string type);
	PlayerStrategy(const PlayerStrategy& p);
	PlayerStrategy& operator=(PlayerStrategy& ps);

public:
	// Set a player to the strategy
	void setPlayer(Player* inputPlayer);
	// Destructor
	virtual ~PlayerStrategy();
	// Issue order
	virtual bool issueOrder(vector<Player*> AddedPlayerList) = 0;
	virtual vector<Territory*> toAttack() = 0;
	virtual vector<Territory*> toDefend() = 0;
	string getType();
	friend ostream& operator<<(ostream& os, PlayerStrategy& player);
};

class NeutralPlayerStrategy : public PlayerStrategy {
public:
	NeutralPlayerStrategy& operator&=(NeutralPlayerStrategy& p);
	NeutralPlayerStrategy();
	NeutralPlayerStrategy(const NeutralPlayerStrategy& p);
	~NeutralPlayerStrategy();
	bool issueOrder(vector<Player*> AddedPlayerList);
	vector<Territory*> toAttack();
	vector<Territory*> toDefend();
	friend ostream& operator<<(ostream& os, NeutralPlayerStrategy& player);
};

class CheaterPlayerStrategy : public PlayerStrategy {
public:
	CheaterPlayerStrategy& operator=(CheaterPlayerStrategy& cheater);
	CheaterPlayerStrategy();
	CheaterPlayerStrategy(const CheaterPlayerStrategy& p);
	~CheaterPlayerStrategy();
	bool issueOrder(vector<Player*> AddedPlayerList);
	vector<Territory*> toAttack();
	vector<Territory*> toDefend();
	friend ostream& operator<<(ostream& os, CheaterPlayerStrategy& player);
};

class HumanPlayerStrategy : public PlayerStrategy {
public:
	HumanPlayerStrategy& operator=(HumanPlayerStrategy& human);
	HumanPlayerStrategy();
	HumanPlayerStrategy(const HumanPlayerStrategy& p);
	~HumanPlayerStrategy();
	bool issueOrder(vector<Player*> AddedPlayerList);
	vector<Territory*> toAttack();
	vector<Territory*> toDefend();
	friend ostream& operator<<(ostream& os, HumanPlayerStrategy& player);
};

class AggressivePlayerStrategy : public PlayerStrategy {
public:
	AggressivePlayerStrategy& operator=(AggressivePlayerStrategy& agg);
	AggressivePlayerStrategy();
	AggressivePlayerStrategy(const AggressivePlayerStrategy& p);
	~AggressivePlayerStrategy();
	bool issueOrder(vector<Player*> AddedPlayerList);
	vector<Territory*> toAttack();
	vector<Territory*> toDefend();
	friend ostream& operator<<(ostream& os, AggressivePlayerStrategy& player);
private:
	int currentIndexToAttack;
};

class BenevolentPlayerStrategy : public PlayerStrategy {
public:
	BenevolentPlayerStrategy& operator=(BenevolentPlayerStrategy& b);
	BenevolentPlayerStrategy();
	BenevolentPlayerStrategy(const BenevolentPlayerStrategy& p);
	~BenevolentPlayerStrategy();
	bool issueOrder(vector<Player*> AddedPlayerList);
	vector<Territory*> toAttack();
	vector<Territory*> toDefend();
	friend ostream& operator<<(ostream& os, BenevolentPlayerStrategy& player);
};
void testPlayerStrategies();