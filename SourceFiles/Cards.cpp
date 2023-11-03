#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../HeaderFiles/Cards.h"
#include "../HeaderFiles/Orders.h"

/**********Cards*********/

Card::Card()
{//Constructor for Card class
}

Card::Card(WarzoneCard wc)
{//Constructor for Card class
	this->CurrentCard = wc;
}

Card::~Card()
{//deconstructor of the Card class
}

Card::Card(const Card& c)
{//copy constructor initialied used to copy exisiting object
	this->CurrentCard = c.CurrentCard;
}

Card& Card::operator=(const Card& c)
{//assignment operator to assign new value to already initialized object
	CurrentCard = c.CurrentCard;
	return *this;
}

void Card::setWarzoneCard(WarzoneCard& c)
{
	this->CurrentCard = c;
}

Card ::WarzoneCard Card::getWarzoneCard() const
{
		return this->CurrentCard;
}

void Card::play(int cardToPlay, Hand& h, Deck& d)
{//removes card from hand and returns it to deck
	Card* removeCard = h.removeCardFromHand(cardToPlay);
	cout << "\nYou played card: " << removeCard->getWarzoneCard() << endl;
	d.putCardBackInDeck(*removeCard);
}

ostream& operator<<(ostream& display, const Card& c)
{//display the type of card
	switch (c.getWarzoneCard()) {
	case Card::WarzoneCard::Diplomacy:
		return display << "Diplomacy";
	case Card::WarzoneCard::Blockade:
		return display << "Blockade";
	case Card::WarzoneCard::Airlift:
		return display << "Airlift";
	case Card::WarzoneCard::Reinforcement:
		return display << "Reinforcement";
	case Card::WarzoneCard::Bomb:
		return display << "Bomb";
	}
}

/**********Deck*********/

Deck::~Deck()
{//destructor for Deck class	delete this;
}

Deck::Deck(const Deck& d)
{//copy constructor initialied used to copy exisiting object
	for (size_t c = 0; c < d.vd.size(); c++)
	{
		this->vd.push_back(new Card(*d.vd[c]));
	}
	cout << this << endl;
}

Deck& Deck::operator=(const Deck& d)
{	//asignment operator for Deck class
	vd = d.vd;
	return *this;
}
ostream& operator<<(ostream& display, const Deck& d)
{//display the amount of cards in the deck
	return display << "\nThe deck contains... " << d.vd.size() << " amount of cards!";
}

vector<Card*> Deck::getCard()
{
	return this->vd;
}

Card* Deck::draw()
{//randomly draws a card and removes from deck while playing it in hand
	cout << "\nExecuting draw method" << endl;
	if (vd.empty())
	{
		printf("The deck is empty");
		return 0;
	}
	else
	{
		int currentDeckSize, randValue;
		currentDeckSize = vd.size();
		randValue = rand() % currentDeckSize;
		Card* drawCard = vd.at(randValue);
		vd.erase(vd.begin() + randValue);
		//cout << "\nYou drew: " << drawCard->getWarzoneCard() << endl;
		return drawCard;
	}
}

void Deck::putCardBackInDeck(Card& c)
{//function that puts the card back to the deck
	this->vd.push_back(&c);
}

void Deck::showDeckContents()
{//for testing that the deck is random and applying functions on it works
	cout << "\nHere are the contents of the deck: " << endl;
	for (size_t i = 0; i < vd.size(); i++)
	{
		if (i == vd.size() - 1) {
			cout << vd.at(i)->getWarzoneCard() << " ";
		}
		else {
			cout << vd.at(i)->getWarzoneCard() << ", ";
		}
	}
	cout << " " << endl;
}

Card* Deck::removeCard(int index)
{//remove a card at a certain index
	Card* removedCard = vd.at(index);
	for (size_t i = 0; i < vd.size(); i++)
	{
		if (i == index)
		{
			vd.erase(vd.begin() + index);
		}
	}
	return removedCard;
}


void Deck::setCard(Card* c)
{
	vd.push_back(c);
	}

Deck::Deck()
{//function goes through the pack, then push back the card type into the deck. starts with 20 cards in da deck
	for (size_t x = 0; x < 4; x++) {
		vd.push_back(new Card(Card::WarzoneCard::Diplomacy));
		vd.push_back(new Card(Card::WarzoneCard::Blockade));
		vd.push_back(new Card(Card::WarzoneCard::Airlift));
		vd.push_back(new Card(Card::WarzoneCard::Reinforcement));
		vd.push_back(new Card(Card::WarzoneCard::Bomb));
	}
}

/**********Hand*********/

Hand::Hand()
{//constructor for empty hand 
}

Hand::~Hand()
{//deconstructor of the Hand class
	for (int i = 0; i < vh.size(); i++) {
		delete vh.at(i);
	}
}

ostream& operator <<(ostream& display, const Hand& h)
{//display the amount of cards in the hand
	display << "\nThe hand contains... " << h.vh.size() << " amount of cards!";
	return display;
}


Hand& Hand::operator=(const Hand& h)
{//assignment operator to assign new value to already initialized object
	this->vh = h.vh;
	return *this;
}


Hand::Hand(const Hand& h)
{//copy constructor for hand class
	for (size_t c = 0; c < h.vh.size(); c++)
	{
		this->vh.push_back(new Card(*h.vh[c]));
	}
	cout << this << endl;
}

vector<Card*> Hand::getCard()
{
	return this->vh;
}

void Hand::putBackCard(Card& c)
{
	this->vh.push_back(&c);
}

void Hand::showHandContents()
{//for testing that the hand is random and applying functions on it works
	if (vh.empty()) {
		cout << "There are no cards in the hand!" << endl;
	}
	else
	{
		cout << "Here are the contents of your Hand:" << endl;
		for (size_t i = 0; i < vh.size(); i++)
		{
			if (i == vh.size() - 1) {
				cout << vh.at(i)->getWarzoneCard() << " ";
			}
			else {
				cout << vh.at(i)->getWarzoneCard() << "< ";
			}
		}
	}
	
}

Card* Hand::removeCardFromHand(int index)
{//removes card from hand. used in the draw and play methods
	Card* removedCard = vh.at(index);
	for (size_t i = 0; i < vh.size(); i++)
	{
		if (i == index)
		{
			vh.erase(vh.begin() + index);
		}
	}
	return removedCard;
}

void Hand::setCardHand(Card* c)
{
	vh.push_back(c);
}