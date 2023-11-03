#include <iostream>
#include "../HeaderFiles/Player.h"
#include "../HeaderFiles/Cards.h"
#include"../HeaderFiles/Orders.h"
#include "../HeaderFiles/Territory.h"
#include "../HeaderFiles/MapLoader.h"

using namespace std;

void testPlayers() {
    /*MapLoader loader("./test_maps/3D.map");
    shared_ptr<Map> map = loader.load();// load the map first
    if (map->validate()) {
        // creating hand objects
        Hand* h = new Hand();
        Hand* h1 = new Hand();
        Hand* h2 = new Hand();
        //creating orderlist objects
        orderlist* o = new orderlist();
        orderlist* o1 = new orderlist();
        orderlist* o2 = new orderlist(); 
        // creating players object using parameterized constructor 
        Player* player = new Player("Ali");
        Player* player2 = new Player("Weza");
        Player* player3 = new Player("Moe");
        Player* player4 = player;
        //get the territories from the map
        Territory* t = map->getTerritory("1c");
        Territory* t2 = map->getTerritory("2c");
        Territory* t3 = map->getTerritory("3c");
        //adding territories to the players
        player->addTerriorty(t);
        player2->addTerriorty(t2);
        player3->addTerriorty(t3);
        vector <Territory*> fight = {t2,t};
        vector <Territory*> defend = {t3};
        // using toDefend method
        player->toDefend();
        // using issueorder method
        //player2->issueOrder();
        // using toAttack method
        //player3->toAttack(fight, defend);
        player4->toDefend();
        // printing the players info
        cout <<"Player 1 : " << *player<< endl;
        cout << "Player 2 : " << *player2 << endl;
        cout << "Player 3 : " << *player3 << endl;
        cout << "Player 4 : " << *player4 << endl;
        //deleting the players to show the ~player()
        delete player;
        delete player2;
        delete player3;
    }*/
}
