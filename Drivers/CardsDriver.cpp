#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../HeaderFiles/Cards.h"

void testCards() {
    Deck* deck = new Deck();
    Hand* hand = new Hand();

    cout << "--Initial deck--\n" << endl;
    deck->showDeckContents();
    cout << "\n--Initial hand--\n" << endl;
    hand->showHandContents();
    cout << "\n" << endl;
    cout << "\n--Draw method--\n" << endl;

    Card* card1 = deck->draw();
    hand->putBackCard(*card1);
    Card* card2 = deck->draw();
    hand->putBackCard(*card2);
    Card* card3 = deck->draw();
    hand->putBackCard(*card3);
    Card* card4 = deck->draw();
    hand->putBackCard(*card4);
    Card* card5 = deck->draw();
    hand->putBackCard(*card5);
    Card* card6 = deck->draw();
    hand->putBackCard(*card6);
    Card* card7 = deck->draw();
    hand->putBackCard(*card7);
    Card* card8 = deck->draw();
    hand->putBackCard(*card8);
    cout << "\n --Let's see the new hand and deck--\n" << endl;
  // Hand after after drawing 4 times
    hand->showHandContents();
    deck->showDeckContents();

    cout << "\n --Executing play method--\n" << endl;
    int maxHandSize = hand->getCard().size();
    for (size_t i = 0; i < maxHandSize; i++)
    {
        srand((unsigned int)time(NULL));
        int randValueToRemove = rand() % (hand->getCard().size());
        hand->getCard()[randValueToRemove]->play(randValueToRemove, *hand, *deck);
    }

    cout << "\n --Let's see the new deck and hand after playing--\n" << endl;
    deck->showDeckContents();
    hand->showHandContents();

     
    delete hand;
    delete deck;
    delete card1;
    delete card2;
    delete card3;
    delete card4;
    delete card5;
    delete card6;
    delete card7;
    delete card8;

}