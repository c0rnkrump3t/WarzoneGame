#ifndef COMP345_CARDS_H
#define COMP345_CARDS_H
#include <iostream>
#include <vector>
#include <string>
#include "../HeaderFiles/Orders.h"

using namespace std;
class Deck;
class Hand;

class Card
{
public:
    //Card type given
       /**Diplomacy - 0
        * Blockade - 1
        * Airlift - 2
        * Reinforcement - 3
        * Bomb - 4
        **/
    enum WarzoneCard { Diplomacy, Blockade, Airlift, Reinforcement, Bomb, end };

    Card(); //constructor
    Card(WarzoneCard wc); //constructor
    ~Card(); //deconstructor of the Card class
    Card(const Card&); //copy constructor initialied used to copy exisiting object
    Card& operator=(const Card&); //assignment operator to assign new value to already initialized object
    void setWarzoneCard(WarzoneCard& c);
    WarzoneCard getWarzoneCard() const;
    friend ostream& operator<<(ostream& display, const Card&); //stream insertion operator
    void play(int cardToRemove, Hand& h, Deck& d); //removes card from hand and returns it to deck

private:
    WarzoneCard CurrentCard; //variable of the type of card given
};

class Deck
{
public:
    Deck(); //constructor that initializes a 20 sized deck
    ~Deck(); //deconstructor of the Card class
    Deck(const Deck& d); //copy constructor
    Deck& operator=(const Deck& d); //assignment operator to assign new value to already initialized object
    friend ostream& operator<<(ostream& display, const Deck& d); //stream insertion operator
    vector<Card*>getCard();
    Card* draw();//randomly draws a card and removes from deck while playing it in hand
    void putCardBackInDeck(Card& c);//returns card to deck
    void showDeckContents();//for testing that the hand is random and applying functions on it works
    Card* removeCard(int i);//removes card from hand. used in the draw and play methods
    void setCard(Card* c);
private:
    vector<Card*> vd;//vector of cards for the deck
};

class Hand
{
public:
    Hand();//constructor for empty hand 
    ~Hand();//deconstructor of the Hand class
    Hand& operator=(const Hand&);//assignment operator to assign new value to already initialized object
    Hand(const Hand& h);//copy constructor initialied used to copy exisiting object
    friend ostream& operator<<(ostream& display, const Hand& h); //stream insertion operator
    vector<Card*>getCard();
    void putBackCard(Card& c);//returns card to the hand vector
    void showHandContents(); //for testing that the hand is random and applying functions on it works
    Card* removeCardFromHand(int i); //removes card from hand. used in the draw and play methods
    void setCardHand(Card* c);

private:
    vector<Card*> vh;//vector of cards for the hand
};

void testCards();
#endif //COMP345_CARDS_H
