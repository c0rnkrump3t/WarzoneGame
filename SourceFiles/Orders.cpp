#include <iostream>
#include "../HeaderFiles/Orders.h"
#include"../HeaderFiles/Cards.h"
#include "../HeaderFiles/LogManager.h"
#include "../HeaderFiles/Player.h"
#include "../HeaderFiles/PlayerStrategies.h"
#include <algorithm>
#include "../HeaderFiles/globalVariables.h"
using namespace std;


//order
//validate from order class
bool order::validate()
{
	cout << "validate from super class";
	return true;
}
//execute from order
//void order::execute()
//{
//    cout << "execute from order class";
//}

order* order::clone()
{
	//use the copy constructor
	//this method is used for inheritance AND create objects of each class
	//return order(*this);
	return this;
}
order::order() : type("")
{
	//default constructor

}
//copy constructor
order::order(const order& ord)
{
	this->type = ord.type;
}
//wrie method used strreeam insertion operator to prit  the write object based on inheritance
void order::write(std::ostream&)
{
	cout << "order from superclass";
}
//assignemnt operator
order& order::operator=(const order& ord)
{
	this->type = ord.type;
	return *this;//return ponter to object
}
Player* order::getOwner()
{
	return owner;
}
string order::getType()
{
	return type;
}
ostream& operator<<(ostream& cout, order& b)
{
	b.write(cout);
	return cout;
}
order::~order() {}
//deploy
deploy::deploy(int numbOfarmies, Territory& target, Player* p) : numbOfArmies(numbOfarmies)
{
	this->numbOfArmies = numbOfarmies;
	this->target = &target;
	this->type = "deploy";
	this->owner = p;
}
//destructor
deploy::~deploy()
{
	target = NULL;
}
//validate method
bool deploy::validate()
{
	Player *p = getOwner();
	Territory *target = getTerrority();
	for (int i = 0; i < p->getterriortiesOwned().size(); i++) {
		if (p->getterriortiesOwned()[i] == target)
		{
			return true;
		}
	}
	return false;
}
//return a ostream cout for printing the objecr
ostream& operator<<(ostream& cout, deploy& d)
{
	//use write from subclass by polymorphism
	d.write(cout);
	return cout;
}
//copy constructor
deploy::deploy(deploy* dep) {
	this->numbOfArmies = dep->numbOfArmies;
	this->target = dep->target;
	this->type = dep->type;
}
//execute method
void deploy::execute()
{
	Territory *target = getTerrority();
	if (validate()) {//first check if the terrority belongs to the player or not 
		target->setNumOfArmies(target->getNumOfArmies() + numbOfArmies);// if it does, then it add numbofarmies to the target
		cout << "The deploy order is executed" << endl;
		cout << "The number of armies are moved to the " << target->getName() << " and now the number of this territory is " << target->getNumOfArmies() <<" armies." << endl;
	}
	else
	{
		cout << "Can't execute order since the territory doesn't belong to the player";
	}
    Notify(this);
}
//clone used to create a copy of the object when copying the list to know whic object the class belongs to
deploy* deploy::clone()
{
	return new deploy(this);
}
//default constructor
deploy::deploy()
{
	this->numbOfArmies = 0;
	this->target = NULL;
	this->type = "deploy";
}
//write method same as before

void deploy::write(std::ostream&)
{
	cout << "put " << this->numbOfArmies << " on a target territory\n";
}
int deploy::getNumOfArmies()
{
	return numbOfArmies;
}

Territory* deploy::getTerrority()// returns the target terrority 
{
	return target;
}

Player* deploy::getOwner()// returns the player issuing the order
{
	return owner;
}
//assignemnt operator
deploy& deploy::operator=(const deploy& d)
{
	order ::operator =(d);
this->numbOfArmies = d.numbOfArmies;
this->target = d.target;
return *this;

}
string deploy::stringToLog() {
    string str = "Order Executed: " + this->type;
    if (!validate()) {
        str += "------[Error] Can't execute order since the territory doesn't belong to the player";
    }
    else {
        str += " from Player " + to_string(this->owner->getPlayerID()) + "------Put " + to_string(this->numbOfArmies) + " army units on territory " + 
                        this->target->getName() + ",\n which now contains " + to_string(this->target->getNumOfArmies()) + " army units";
    }
    return str;
}


//advance
//parameterized constructor
advancee::advancee(int numbOfarmies, Territory& source, Territory& to, Player* p)
{
	this->type = "advance";
	this->source = &source;
	this->target = &to;
	this->numbOfarmies = numbOfarmies;
	this->owner = p;
}
//destructor
advancee::~advancee()
{
	target = NULL;
	source = NULL;
}
//default constructor
advancee::advancee()
{
	this->source = NULL;
	this->target = NULL;
	this->numbOfarmies = 0;
	this->type = "advancee";
}

Territory* advancee::getSourceTerrority()
{
	return source;
}

Territory* advancee::getTargetTerrority()
{
	return target;
}

void advancee::setSourceTerritory(Territory* sourceTerritory) {
	this->source = sourceTerritory;
}

void advancee::setTargetTerritory(Territory* targetTerritory) {
	this->target = targetTerritory;
}

void advancee::setNumOfArmy(int n) {
	this->numbOfarmies = n;
}
// clone method same as other subclasses
advancee* advancee::clone()
{
	return new advancee(this);
}
//write method 
void advancee::write(std::ostream&)
{
	cout << " move " << this->numbOfarmies << " from  a source territory to a target territory\n";
}
//validate method to check tthe order is valid
bool advancee::validate()
{

	Territory* source = this->source;
	Territory* target = getTargetTerrority();
	Player* p = getOwner();
	vector <Territory*> temp = p->getterriortiesOwned();
	vector<Player*> negotiationlist = this->getOwner()->getnegotiateList();

	if (source->getNumOfArmies() < 0) {
		return false;
	}

	for (int x = 0; x < negotiationlist.size(); x++) {
		if (negotiationlist[x]->getPlayerID() == target->getOwner()->getPlayerID())
		{
			return false;
		}
	}
	int old = source->getNumOfArmies();
	Territory* adjacentTerritory = nullptr;
	for (int i = 0; i < temp.size(); i++) {
		adjacentTerritory = temp[i]->getNeighbor(target->getName());
		if (adjacentTerritory != nullptr) {
			break;
		}
	}
	if (adjacentTerritory == nullptr) {
		return false;
	}

	bool founds = false;
	bool foundt = false;
	for (auto ter : p->getterriortiesOwned()) {
		if (ter == source) {
			founds = true;
		}
		if (ter == target) {
			foundt = true;
		}
	}
	if ((founds == true) && (foundt == true)) {// if both source and target terriorty belongs to the player then it moves numbofarmies from source and add them to target
		if (old - numbOfarmies >= 0) {
			source->setNumOfArmies(old - numbOfarmies);
			target->setNumOfArmies(numbOfarmies + target->getNumOfArmies());
			return true;
		}
		else {
			cout << "Cant move armies from source to target because num of armies advanced exceedes the number of armies in the source territory"<< endl;
			return false;
		}
	}
	if (founds == false) {// if source doesnt belong to the player it returns false
		return false;
	}
	if ((founds == true) && (foundt == false)) {// here it will return true and in the execute fuction will check if it did the move armies then it will run this in the execute
			return true;
	}
		return true;
}
void advancee::execute()
{
	Territory* conquered = nullptr;
	Territory* target = getTargetTerrority();
	Territory* source = this->source;
	int TargetArmies = target->getNumOfArmies();
	int old = target->getNumOfArmies();
	if (validate()) {
		// If the number of armies, which the player wants to move is more than the number of armies on the source territory, do nothing
		if (this->numbOfarmies > source->getNumOfArmies()) {
			cout << source->getOwner()->getName() << " advanced armies greater than the number of armies in the source territory\n";
			cout << "Nothing chamges and the order was not executed \n";
			return;
		}
		int armies = target->getNumOfArmies();
		if (armies == (numbOfarmies + old)) {
			cout << "Both source and target belong to the player so the army units are moved from the source to the target terriorty" << endl;
			cout << "The number of armies in the source terriorty is " << source->getNumOfArmies() << " armies" << endl;
			Notify(this);
			return;
		}
		else {// here it execute if the source belongs to the player but the target doesn't
			if (this->numbOfarmies > source->getNumOfArmies()) {
				cout << source->getOwner()->getName() << " advanced armies greater than the number of armies in the source territory\n";
				cout << "Nothing chamges and the order was not executed \n";
				return;
			}
			else if (0.6 * this->numbOfarmies >= target->getNumOfArmies()) {
				int advanced = this->numbOfarmies;
				cout << "INFO OF ADVANCE" << endl;
				cout << "before execution: source " << source->getName() << "  number of armies: " << source->getNumOfArmies() << endl;
				cout << "before execution: target " << target->getName() << "  number of armies: " << target->getNumOfArmies() << endl;
				cout << "-------------  first step --------------" << endl;
				cout << "During execution : source will armires decreased by the number of armies advanced: ";
				//If a Neutral player is attacked, it becomes an Aggressive player
				PlayerStrategy* strategy = target->getOwner()->getPlayerStrategy();
				if (strategy->getType() == "Neutral") {
					target->getOwner()->setPlayerStrategy(new AggressivePlayerStrategy());
				}
				source->setNumOfArmies(source->getNumOfArmies() - this->numbOfarmies);
				cout << source->getNumOfArmies() << endl;
				cout << "-------------  war --------------" << endl;

				cout << "The " << target->getName() << " will lose 60% of the armies advanced" << endl;
				this->numbOfarmies = this->numbOfarmies - (0.7 * target->getNumOfArmies());
				if (target->getNumOfArmies() - (0.6 * advanced) < 0) {
					cout << " the armies will go to negative but we will set it to zero" << endl;
					target->setNumOfArmies(0);
				}
				else {
					target->setNumOfArmies(target->getNumOfArmies() - (0.6 * advanced));
				}

				cout << "The advanceld armies will lose 70% of the armies in the target" << endl;
				cout << "numb of armies in target after loosing 60% of the armies advanced " << target->getNumOfArmies() << endl;
				cout << "the Number of armies advanced after the war" << this->numbOfarmies << endl;
				cout << "-------------  Result of the war  --------------" << endl;
				for (int x = 0; x < target->getOwner()->getterriortiesOwned().size(); x++) {
					cout << target->getOwner()->getterriortiesOwned()[x]->getName() << endl;
				}

				for (int x = 0; x < target->getOwner()->getterriortiesOwned().size(); x++) {
					if (target->getOwner()->getterriortiesOwned()[x] == target) {
						cout << "----------------------   " << target->getName() << "-------" << x << "--------" << endl;
						target->getOwner()->removeTerritory(x);// here we change the ownership when the player successfully conquer the terriorty
						this->getOwner()->addTerriorty(target);
						break;
					}
				}

				cout << "The advanced number of armies survived from the war will conquer the territory" << endl;

				target->setNumOfArmies(this->numbOfarmies);

				cout << target->getName() << "will have " << target->getNumOfArmies() << " armies after the war" << endl;
				cout << source->getName() << "will have " << source->getNumOfArmies() << " armies after the war" << endl;
				return;

			}
			else if (0.6 * this->numbOfarmies < target->getNumOfArmies()) {
				cout << "INFO OF ADVANCE" << endl;
				cout << "before execution: source " << source->getName() << " number of armies : " << source->getNumOfArmies() << endl;
				cout << "before execution: target " << target->getName() << " number of armies: " << target->getNumOfArmies() << endl;
				cout << "-------------  first step --------------" << endl;

				cout << "During execution : source will armires decreased by the number of armies advanced: ";
				//If a Neutral player is attacked, it becomes an Aggressive player
				PlayerStrategy* strategy = target->getOwner()->getPlayerStrategy();
				if (strategy->getType() == "Neutral") {
					target->getOwner()->setPlayerStrategy(new AggressivePlayerStrategy());
				}
				int advanced = this->numbOfarmies;
				int armyNotKilled = this->numbOfarmies - (0.7 * target->getNumOfArmies());
				source->setNumOfArmies(source->getNumOfArmies() - this->numbOfarmies);
				cout << source->getNumOfArmies() << endl;
				cout << "-------------  war --------------" << endl;
				if (this->numbOfarmies - (0.7 * target->getNumOfArmies()) < 0) {
					cout << " advanced armies loose 70% of the armies in the target territory their armies go to negative but we set it as zero" << endl;
					this->numbOfarmies = 0;
				}
				else {
					cout << "a advanced armies loose 70% of the armies in the target territory their armies go to negative but we set it as zero" << endl;
					this->numbOfarmies = this->numbOfarmies - (0.7 * target->getNumOfArmies());
				}
				cout << target->getName() << "  Target  will lose 60% of the armies advanced" << endl;
				target->setNumOfArmies(target->getNumOfArmies() - (0.6 * advanced));
				//cout //<< "The advanceld armies will lose 70% of the armies in the target" << endl;
				cout << "numb of armies in target after loosing 60% of the armies advanced " << target->getNumOfArmies() << endl;
				cout << "the number of armies advanced after the war" << this->numbOfarmies << endl;
				cout << "-------------  Result of the war  --------------" << endl;
				cout << "The souce territory " << source->getName() << " failed to conquer the target sad" << endl;
				cout << "If there exist advances armies after conquering then they will return bacl" << endl;
				this->source->setNumOfArmies(source->getNumOfArmies() + this->numbOfarmies);
				cout << target->getName() << "will have " << target->getNumOfArmies() << " armies after the war" << endl;
				cout << source->getName() << "will have " << source->getNumOfArmies() << " armies after the war" << endl;

			}
		}
						
	conquered = NULL;

				
	//conquered = NULL;
	target = NULL;
	source = NULL;
	}



				
				

				
				
				//int armiesKilledTarget = 0.60 * this->numbOfarmies;
				//int armiesKilledSource = 0.70 * TargetArmies;
				//source->setNumOfArmies(source->getNumOfArmies() - armiesKilledSource);// calcuating the source num of armies
				//target->setNumOfArmies(target->getNumOfArmies() - armiesKilledTarget);// calcuating the target num of armies
				//cout << "The advance order is executed, Player "
				//	<< owner->getName() << " which has number of armies is : "<< source->getNumOfArmies() << " advanced on Terrority "
				//	<< target->getName() << " which is owned by " << target->getOwner()->getName() <<" and has the number of armies is : "
				//	<< target->getNumOfArmies() << endl; 
				//if (target->getNumOfArmies() <= 0) {// if the target armies are 0 or less then the player can conquer the terriorty and take ownership
				//	for (int x = 0; x < target->getOwner()->getterriortiesOwned().size(); x++) {
				//		if (target->getOwner()->getterriortiesOwned()[x]->getName() == target->getName()) {
				//			 conquered = target->getOwner()->getterriortiesOwned()[x];
				//			if (target->getOwner()->getterriortiesOwned().size() == 1) {// only if he has one terriorty
				//				target->getOwner()->removeTerritory();// here we change the ownership when the player successfully conquer the terriorty
				//				target->setOwner(source->getOwner());
				//				this->getOwner()->addTerriorty(conquered);
				//			}
				//			else {
				//				int size = target->getOwner()->getterriortiesOwned().size() - 1;
				//				target->getOwner()->swap(x);
				//				target->getOwner()->removeTerritory();
				//				target->setOwner(this->getOwner());
				//				owner->addTerriorty(conquered);
				//			}
				//		}
				//	}
				//	target->setNumOfArmies(this->numbOfarmies - armiesKilledSource);
				//	cout << "Conquered has occur and now the target terriorty owner is " << conquered->getOwner()->getName() <<
				//		" and the number of armies is : " << target->getNumOfArmies() << endl;
				//	owner->conquer = true;// change the boolean of player conqure to true 
				//}
				//else {
				//	cout << "Cant conquer the terriorty";
				//}
		
		
		else {
			cout << "Cant execute advance order";
		}
		Notify(this);
}

 
//assignemnt operator 
advancee& advancee::operator=(const advancee& adv)
{
	order :: operator=(adv);
	this->source = adv.source;
	this->target = adv.target;
	this->numbOfarmies = adv.numbOfarmies;
	return *this;//return pointetr to the object
}
ostream& operator<<(ostream& cout, advancee& ss)
{
	ss.write(cout);
	return cout;
}
advancee::advancee(advancee* adv) {
	this->source = adv->source;
	this->target = adv->target;
	this->type = adv->type;
	this->numbOfarmies = adv->numbOfarmies;

}
//fix when the code completes
string advancee::stringToLog() {
	
    string str = "Order Executed: " + this->type;
    if (!validate()) {
        str += "------[Error] Can't execute advance order since the source territory does not belong to the player that issued the order\n or the target territory is not adjacent to the source territory.";
    }
    else {
        str += " from Player " + to_string(this->owner->getPlayerID()) + "------Move " + to_string(this->numbOfarmies) + " army units from territory " +
            this->source->getName() + " to territory " + this->target->getName() + ", \n which belongs to Player " + to_string(this->target->getOwner()->getPlayerID());
    }
    return str;
}

/////the comments are exactly the same as the other subclasses
//blockade
blockade* blockade::clone()
{
	return new blockade(this);
}
blockade::blockade(Territory& target, Player* p, Player* neutralPlayer)
{
	this->type = "blockade";
	this->target = &target;
	this->owner = p;
	this->neutralPlayer = neutralPlayer;
}

blockade::~blockade()
{
	this->target = NULL;
}

blockade& blockade::operator=(const blockade& block)
{
	order :: operator =(block);
	this->target = block.target;
	return *this;
}

void blockade::execute()
{
	Territory *t = getTerritory();
	if (validateCard()) {// checks if the player has blockade card in hands or not
		if (validate()) {// checks if the territory belongs to the player or not
			t->setNumOfArmies(t->getNumOfArmies() * 2);
			
			for (int x = 0; x < target->getOwner()->getterriortiesOwned().size(); x++) {
				if (target->getOwner()->getterriortiesOwned()[x] == t) {
					this->target->getOwner()->removeTerritory(x);
					cout << "removing territory " << t->getName();
					break;
				}
				
			}
			//neutralPlayer->addTerriorty(t);
			cout << "Blockade order is executed " << endl;
			cout << "Now the territory has " << t->getNumOfArmies() << " armies and the ownership of "<< t->getName() <<  " is transfered to this player " << t->getOwner()->getName() << endl;
		}
		else {
			cout << "Can't Execute from blockade order";
		}
	}
	else {
		cout << "The player doesn't own have the blockade card in hand";
	}
	Notify(this);
}

bool blockade::validate()
{
	Territory* target = getTerritory();
	Player* p = getOwner();
	for (int i = 0; i < p->getterriortiesOwned().size(); i++) {
		if (p->getterriortiesOwned()[i] == target)
		{
			return true;
		}
	}
	return false;
}

blockade::blockade()
{
	this->target = NULL;
	this->type = "blockade";

}
ostream& operator<<(ostream& cout, blockade& b)
{
	b.write(cout);
	return cout;
}

blockade::blockade(blockade* vl) {
	this->target = vl->target;
	this->type = vl->type;
}
Territory* blockade::getTerritory()// returns the territory
{
	return target;
}

void blockade::setTargetTerritory(Territory* targetTerritory) {
	this->target = targetTerritory;
}
void blockade::write(std::ostream&)
{
	cout << "Triple the number of army units on a territory and make it a neutral territory\n";
}
bool blockade::validateCard()
{
	Player* p = getOwner();
	for (int i = 0; i < p->getHandcard()->getCard().size(); i++)
	{
		if (p->getHandcard()->getCard()[i]->getWarzoneCard() == Card::Blockade) {
			return true;
		}
	}
	return false;
}

string blockade::stringToLog() {
	string str = "Order Executed: " + this->type;
	if (!validateCard()) {
		str += "------[Error]The player does not have a blockade card";
	}
	if (!validate()) {
		str += "------[Error] Can't execute blockade order since the target territory does not belong to the player issuing the order";
	}
	else {
		str += " from Player " + to_string(this->owner->getPlayerID()) + "------The number of army units on the territory " + this->target->getName() + " is now doubled to " + to_string(this->target->getNumOfArmies()) + ",\nand the ownership of the territory is transferred to the Neutral player";
	}
	return str;
}
/////the comments are exactly the same as the other subclasses
//negotiate
negotiate& negotiate::operator=(const negotiate& n)
{
	order:: operator=(n);
	this->TargetPlayer = n.TargetPlayer;
	return *this;
}
negotiate::negotiate()
{
	this->TargetPlayer = NULL;
	this->type = "negotiate";
}

negotiate::negotiate(Player* target, Player* source) : TargetPlayer(target)
{
	this->type = "negotiate";
	this->owner = source;
}

negotiate::~negotiate()
{
	this->TargetPlayer = NULL;
}
ostream& operator<<(ostream& cout, negotiate& n)
{
	n.write(cout);
	return cout;
}
negotiate::negotiate(negotiate* neg) {
	this->type = neg->type;
	this->TargetPlayer = neg->TargetPlayer;
}
void negotiate::execute()
{
	if (validateCard()) {// checks if the player has dipolamcy card in their hands or not
		if (validate()) {// check if both players are the same or not
			// negotiatiolist will contain all players that the player issuing order has
			// and we will change the advancee order and bomb order so that of the player 
			// that advance  and bomb( attacking orders )is issued on is in the negotiation list the order will be invalid	
			getOwner()->addnegotiationlist(TargetPlayer);
			cout <<getOwner()->getnegotiateList().size();
			this->getTargetPlayer()->getnegotiateList().push_back(this->getOwner());
			cout << "Players had negotiatied and they will not attack each other for this turn"<<endl;
		}
		else
		{
			cout << "Can't execute negotiate order!\n";
		}
	}
	else
		cout << "The player doesn't own have the negotiate card in hand";
	Notify(this);
}
Player* negotiate::getTargetPlayer()// returns the target player 
{
	return TargetPlayer;
}
void negotiate::setTargetPlayer(Player* P) {
	this->TargetPlayer = P;
}
bool negotiate::validate()
{
	Player *p = getTargetPlayer();
	Player* s = getOwner();
	int pID = p->getPlayerID();
	int sID = s->getPlayerID();
	if (sID == pID) {
		return false;
	}
	return true;
}
bool negotiate::validateCard()
{
	Player* p = getOwner();
	vector <Card*> card = p->getHandcard()->getCard();
	for (int i = 0; i < card.size(); i++)
	{
		if (card[i]->getWarzoneCard() == Card::Diplomacy) {
			return true;
		}
	}
	return false;
}
negotiate* negotiate::clone()
{
	return new negotiate(this);
}
void negotiate::write(std::ostream&)
{
	cout << "prevent attacks between the current player and another target player until the end of the turn.\n";
}

string negotiate::stringToLog() {
    string str = ("Order Executed: " + this->type);
	if (!validateCard()) {
		str += "------[Error]The player does not have a diplomacy card";
	}
    if (!validate()) {
        str += "------[Error] Can't execute blockade order since tthe target is the player issuing the order.";
    }
    else {
        str += "------Successfully negotiate with Player " + to_string(this->TargetPlayer->getPlayerID()) + "\n Any attacks between these players will become invalid"; //Add more when the code completed
    }
    return str;
}


/////the comments are exactly the same as the other subclasses
//airlift
airlift& airlift::operator=(const airlift& air)
{
	order :: operator=(air);
	this->source = air.source;
	this->target = air.target;
	this->numbOfarmies = air.numbOfarmies;
	return *this;
}
airlift::airlift()
{
	this->source = NULL;
	this->target = NULL;
	this->type = "airlift";
	this->numbOfarmies = 0;
}

airlift::airlift(int numbOfarmies, Territory& source, Territory& to, Player* p) : numbOfarmies(numbOfarmies)
, source(&source), target(&to)
{
	this->type = "airlift";
	this->owner = p;
}

airlift::~airlift()
{
	source = NULL;
	target = NULL;

}

airlift::airlift(airlift* air)
{

	this->numbOfarmies = air->numbOfarmies;
	this->source = air->source;
	this->target = air->source;
	this->type = air->type;
}
void airlift::setSourceTerritory(Territory* sourceTerritory) {
	this->source = sourceTerritory;
}

void airlift::setTargetTerritory(Territory* targetTerritory) {
	this->target = targetTerritory;
}

void airlift::execute()
{
	Territory* source = getSourceTerritory();
	Territory* target = getTargetTerritory();
	int olds = source->getNumOfArmies();
	int oldt = target->getNumOfArmies();
	if (validateCard()) {// checks if the player has airlift card in their hands
		if (validate()) {// if the source or target territory does not belong to the player it will not go inside the if statmnemnt 
			
			if (this->numbOfarmies > source->getNumOfArmies()) {
				cout << source->getOwner()->getName() << " airlift armies greater than the number of armies in the source territory\n";
				cout << "Nothing chamges and the order was not executed \n";
				return;
			}
			source->setNumOfArmies(source->getNumOfArmies() - numbOfarmies);
			target->setNumOfArmies(numbOfarmies+target->getNumOfArmies());// moving the armies from source to target
			cout << "The airlift is executed" << endl;
			cout << "Now the source territory had " << olds << " armies but now it has " << source->getNumOfArmies() <<
				" armies. The target territory had " << oldt << " armies and after airlift order execute, now it has "
				<< target->getNumOfArmies() << " armies." << endl;
		}
		else {
			cout << "Cant execute order airlift because either target or source doesnt belong to the player";
		}
	}
	else
		cout << "The player doesn't have airlift card in hand";
	Notify(this);

}
Territory* airlift::getSourceTerritory()// returns the source territory 
{
	return source;
}

Territory* airlift::getTargetTerritory()// returns the target territory
{
	return target;
}

void airlift::setNumOfArmy(int n) {
	this->numbOfarmies = n;
}

bool airlift::validate()
{
	Territory* s = getSourceTerritory();
	Territory* t = getTargetTerritory();
	Player *p = getOwner();
	bool founds = false;
	bool foundt = false;
	for (auto ter : p->getterriortiesOwned()) {
		if (ter == s) {
			founds = true;
		}
		if (ter == t) {
			foundt = true;
		}
	}
	if ((founds == true) && (foundt == false)) {
		return false;
	}
	if ((founds == false) && (foundt == true)) {
		return false;
	}
	// if neither source nor target terriorty belongs to the player it is invalid
	if ((founds == true) && (foundt == true)) {
		return true;
	}
	return false;
}

bool airlift::validateCard()
{
	Player* p = getOwner();
	vector <Card*> card = p->getHandcard()->getCard();
	for (int i = 0; i < card.size(); i++)
	{
		if (card[i]->getWarzoneCard() == Card::Airlift) {
			return true;
		}
	}
	return false;
}

airlift* airlift::clone()
{
	return new airlift(this);
}

void airlift::write(std::ostream&)
{

	cout << " advance " << this->numbOfarmies << " from a source territory to a target territory\n";
}

string airlift::stringToLog() {
    string str = "Order Executed: " + this->type;
    if (!validateCard()) {
        str += "------[Error]The player does not have a airlift card";
    }
    if (!validate()) {
        str += "------[Error] Can't execute airlift order since the source or target territory does not belong to the player that issued the order.";
    }
    else {
        Territory* s = this->source;
        Territory* t = this->target;
        str += " from Player " + to_string(this->owner->getPlayerID()) + "------Move " + to_string(this->numbOfarmies) + " army units from " + s->getName() + " to " + t->getName()
            + ".\nTerritory " + s->getName() + " now has " + to_string(s->getNumOfArmies()) + " army units and territory " + t->getName() + " has " + to_string(t->getNumOfArmies()) + " army units";
    }
    return str;
}


ostream& operator<<(ostream& cout, airlift& ai)
{
	ai.write(cout);
	return cout;
}
/////the comments are exactly the same as the other subclasses
//bomb
bomb& bomb::operator=(const bomb& bom)
{
	order :: operator =(bom);
	this->target = bom.target;
	return *this;
}

bomb::bomb()
{
	this->target = NULL;
	this->type = "bomb";
}

bomb::bomb(Territory& target, Player* p)
{
	this->type = "bomb";
	this->target = &target;
	this->owner = p;
}

bomb::~bomb()
{
	target = NULL;
}

void bomb::execute()
{
	Territory* target = getTerritory();
	int old = target->getNumOfArmies();
	if (validateCard()) {// first checks if the player doesn't have the bomb card or not 
		if (validate()) {// it checks if the target terriorty is adjacnet and not owned by the player then it executes
			target->setNumOfArmies(target->getNumOfArmies() / 2);
			PlayerStrategy* strategyPlayer = target->getOwner()->getPlayerStrategy();
			if (strategyPlayer->getType() == "Neutral") {
				target->getOwner()->setPlayerStrategy(new AggressivePlayerStrategy());
			}
			cout << "The bomb order is executed " << endl;
			cout << "Before the territory got attacked it had " << old <<
				" armies but now after it got attacked, half of its army is gone, it has " << target->getNumOfArmies() << " armies.";
		}
		else {
			cout << "Can't execute the bomb order because the player issuing bomb on his territory ";
		}
	}
	else {
		cout << "The player doesn't have the bomb card in his hand";
	}
	Notify(this);
}

bool bomb::validate()
{
	Territory* target = getTerritory();
	Player* p = getOwner();
	vector <Territory*> temp = p->getterriortiesOwned();
	vector<Player*> negotiationlist = this->getOwner()->getnegotiateList();
	for (int x = 0; x < negotiationlist.size(); x++) {
		if (negotiationlist[x]->getPlayerID() == target->getOwner()->getPlayerID())
			return false;
	}
	for (int i = 0; i < temp.size(); i++) {
		if (temp[i] == target)
		{
			return false;
		}
	}
	Territory* adjacentTerritory = nullptr;
	for (int i = 0; i < temp.size(); i++) {
		adjacentTerritory = temp[i]->getNeighbor(target->getName());
		if (adjacentTerritory != nullptr) {
			break;
		}
	}
	if (adjacentTerritory == nullptr) {
		return false;
	}
	return true;
}
Territory* bomb::getTerritory()
{
	return target;
}

void bomb::setTargetTerritory(Territory* targetTerritory) {
	this->target = targetTerritory;
}

bomb* bomb::clone()
{
	return new bomb(this);
}

void bomb::write(std::ostream&)
{
	cout << "destroy half of the army units located on a targett territory\n";
}

bool bomb::validateCard()
{
	Player* p = getOwner();
	vector <Card*> card = p->getHandcard()->getCard();
	for (int i = 0; i < card.size(); i++)
	{
		if (card[i]->getWarzoneCard() == Card::Bomb) {
			return true;
		}
	}
	return false;
}

bomb::bomb(bomb* bom) {
	this->target = bom->target;
	this->type = bom->type;

}
string bomb::stringToLog() {
    string str = "Order Executed: " + this->type;
    if (!validateCard()) {
        str += "------[Error]The player does not have a bomb card";
    }
    if (!validate()) {
        str += "------[Error] Can't execute bomb order since the target is owned by the player issuing the order\n or the target territory is not adjacent to any territories owned by the player issuing the order";
    }
    else {
        string nameTerritory = this->target->getName();
        str += " from Player " + to_string(this->owner->getPlayerID()) + "------Bomb " +  nameTerritory + ", which belongs to Player " + to_string(this->target->getOwner()->getPlayerID()) + ". Territory " + nameTerritory + " now has " + to_string(this->target->getNumOfArmies()) + " army units";
    }
    return str;
}
ostream& operator<<(ostream& cout, bomb& b)
{
	b.write(cout);
	return cout;
}
// Conquer Order
conquer::conquer() : targetTerritory(nullptr), player(nullptr) {}
conquer::conquer(Territory* target, Player* p) : targetTerritory(target), player(p) {}
conquer::~conquer() {}
void conquer::execute() {
	if (validate()) {
		Player* oldOwner = targetTerritory->getOwner();
		if (oldOwner != nullptr) {
			vector<Territory*> list = oldOwner->getterriortiesOwned();
			for (int i = 0; i < list.size(); i++) {
				if (list.at(i) == targetTerritory) {
					oldOwner->removeTerritory(i);
					break;
				}
			}
		}
		player->addTerriorty(targetTerritory);
	}
	Notify(this);
}
bool conquer::validate() {
	if (player->getPlayerStrategy()->getType() != "Cheater") {
		return false;
	}
	if (targetTerritory->getOwner() == player) {
		return false;
	}
	return true;
}
string conquer::stringToLog() {
	string str = "Order Executed: " + this->type;
	if (!validate()) {
		str += "------[Error] Can't execute bomb order since the target is owned by the player issuing the order\n";
	}
	else {
		string nameTerritory = targetTerritory->getName();
		str += " from Player " + player->getName() + "------Conquered " + nameTerritory + ", which belongs to Player " + this->targetTerritory->getName() + "\n";
	}
	return str;
}
//destructor destroys all the objects inside the list
orderlist::~orderlist()
{

	for (int x = 0; x < list.size(); x++) {
		delete this->list[x];
	}
	this->list.clear();

}
//assigenemnt same function as copy constructor
orderlist& orderlist::operator=(const orderlist& list)
{
	for (int x = 0; x < list.list.size(); x++) {
		this->list.push_back(list.list[x]->clone());
	}
	return *this;
}
//remove method that is given an index and it removes the order from the list
void orderlist::remove(int num)
{
	delete list[num];//delete the order from heap when removed
	this->list.erase(list.begin() + (num));
}
//move method to move an object from position 1 to position 2
void orderlist::move(int position1, int position2)
{
	order* temp = (list[position1]);
	list[position1] = (list[position2]);
	list[position2] = temp;
	temp = NULL;

}
//default constructor
//it initialize all the list to null
orderlist::orderlist() {
	for (int x = 0; x < list.size(); x++) {

		list[x] = NULL;

	}
}
//copy constructor
orderlist::orderlist(const orderlist& ord)
{

	for (int x = 0; x < ord.list.size(); x++) {
		//use copy constructor to create deep copy of the orders
		this->list.push_back(ord.list[x]->clone());

	}


}
void orderlist::reset() {
	for (int i = 0; i < list.size(); i++) {
		delete list[i];
	}
	list.clear();
}
//pushb the order in to the list by order
void orderlist::addOrder(order* ord)
{
	list.push_back(ord);
	rotate(list.rbegin(), list.rbegin() + 1, list.rend());
	Notify(this);
}
int orderlist::getsize()
{
	return list.size();
}
//for visualizaton of execut and validate
void orderlist::showmethods()
{
	for (int x = 0; x < list.size(); x++) {
		//list[x]->execute();
		list[x]->validate();
	}
}
string orderlist::stringToLog() {
	order* o = list.back();
	string str = "Order Issued: Player " + to_string(o->getOwner()->getPlayerID()) + " added " + o->getType() + " order to their order list.\n Player " + to_string(o->getOwner()->getPlayerID()) +
		" now is: ";
	str += ("\n{");
	for (int i = 0; i < list.size(); i++) {
		str += this->list.at(i)->getType();
		if (i != this->getsize() - 1) {
			str += ",";
		}
	}
	str += "}";
	return str;

}
//stream insertion operator
ostream& operator<<(ostream& cout, const orderlist& l)
{
	for (int x = 0; x < l.list.size(); x++) {

		cout << *(l.list[x]);
	}
	return cout;
}

order* orderlist::getFirstOrder() {
	return list.back();
}

vector <order*> orderlist::getList() {
	return list;
}
