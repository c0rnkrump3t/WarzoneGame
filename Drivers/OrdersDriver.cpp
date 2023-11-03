#include <iostream>
using namespace std;
#include "../HeaderFiles/Player.h"
#include "../HeaderFiles/Cards.h"
#include"../HeaderFiles/Orders.h"
#include "../HeaderFiles/Territory.h"
#include "../HeaderFiles/MapLoader.h"

void testOrdersLists() {
   //create objects of order and its subclasses
    
    bomb* b1 = new bomb();
    deploy * d1 = new deploy();
    airlift *a1 = new airlift();
    negotiate *n = new negotiate();
    blockade* bk = new blockade();
    advancee* adv = new advancee();
    //create order list
    orderlist list;
   //use push method
    list.addOrder(a1);
    list.addOrder(b1);
    list.addOrder(d1);
    list.addOrder(n);
    list.addOrder(bk);
    list.addOrder(adv);
    cout << "--use stream insertion operator of list--\n";
    cout << list;
    cout << "-------------------\n";
    cout << "use validate and execut with all objects";
    list.showmethods();
    cout << "-------------------\n";

    list.move(4, 1);
    cout << "--move position 4 to 1--\n";

    cout << list;
    cout << "-------------------\n";
    cout << "--remove--order in position 4\n";
    list.remove(4);
    cout << list;
    cout << "-------------------\n";
    
    orderlist list2 = list;
    cout << "---using copy constructor---\n";
  
    cout << list2;
    cout << "-------------------\n";
    orderlist list3;
    list3 = list2;
    cout << "---using assignement operator---\n";
    cout << list2;
}
//void testOrderExecution() {
//    MapLoader loader("./test_maps/3D.map");
//    shared_ptr<Map> map = loader.load();// load the map first
//    if (map->validate()) {
//        // creating players object using parameterized constructor 
//        Player* player = new Player("Ali");
//        Player* player2 = new Player("Weza");
//        Player* player3 = new Player("Moe");
//        Player* player4 = player;
//        Card* c3 = new Card(Card::Diplomacy);
//        Card* c2 = new Card(Card::Airlift);
//        Card* c1 = new Card(Card::Blockade);
//        Card* c4 = new Card(Card::Bomb);
//        player->getHandcard()->putBackCard(*c3);
//        Territory* t = map->getTerritory("1c");
//        Territory* t2 = map->getTerritory("2c");
//        Territory* t3 = map->getTerritory("3c");
//        Territory* t4 = map->getTerritory("4c");
//        Territory* t5 = map->getTerritory("5c");
//        Territory* t6 = map->getTerritory("6c");
//        //adding territories to the players
//        player->addTerriorty(t);
//        player->addTerriorty(t5);
//        player->addTerriorty(t6);
//        player2->addTerriorty(t2);
//        player2->addTerriorty(t4);
//        player2->addTerriorty(t3);
//        deploy* d = new deploy(100, *t, player);
//        deploy* d1 = new deploy(20, *t3, player);
//        deploy* d2 = new deploy(150, *t2, player2);
//        d->execute();
//        cout << endl;
//        d1->execute();
//        cout << endl;
//        d2->execute();
//        advancee* a = new advancee(50, *t,*t2, player);
//        a->execute();
//        cout << t2->getOwner()->getName();
//        cout << t->getOwner()->getName();
//       //jdnod
//       // cout << t2->getOwner()->getName();
    