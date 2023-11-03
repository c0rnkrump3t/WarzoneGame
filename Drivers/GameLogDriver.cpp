#include "../HeaderFiles/LogObserver.h"
#include "../HeaderFiles/Orders.h"
#include "../HeaderFiles/GameEngine.h"
#include "../HeaderFiles/Observer.h"
#include "../HeaderFiles/MapLoader.h"
#include "../HeaderFiles/Map.h"
#include "../HeaderFiles/Cards.h"


void testLoggingObserver() {
	// Test for CommandProcessor::saveCommand()
	GameEngine* ge = new GameEngine();
    {
        LogObserver observerCP(ge->getCommandProcessor());
        LogObserver obseverGE(ge);
        ge->getCommandProcessor()->getCommand();
        ge->executeTransition();
        ge->getCommandProcessor()->getCommand();
        ge->executeTransition();
    }
    delete ge;
    
    
    MapLoader loader("./test_maps/3D.map");
    shared_ptr<Map> map = loader.load();
    Territory* t1 = map->getTerritory("2c");
    Territory* t2 = map->getTerritory("3c");
    Territory* t3 = map->getTerritory("1c");
    Territory* t4 = map->getTerritory("4c");
    Territory* t5 = map->getTerritory("1t");
    Player p1;
    Player p2;
    //Assign territories to player p1 manually
    t1->setOwner(&p1);
    t2->setOwner(&p1);
    t4->setOwner(&p1);
    p1.addTerriorty(t1);
    p1.addTerriorty(t2);
    p1.addTerriorty(t4);
    // Assign territories to player p2 manually
    t3->setOwner(&p2);
    t5->setOwner(&p2);
    p2.addTerriorty(t3);
    p2.addTerriorty(t5);

    Hand* h = p1.getHandcard();
    orderlist* list = p1.getOrderList();
    orderlist* list2 = p2.getOrderList();
    LogObserver observerList(list);

    // Valid Orders
    order* deploy1 = new deploy(10, *t1, &p1);
    order* deploy2 = new deploy(10, *t3, &p2);
    order* advance = new advancee(5, *t1, *t3, &p1);
   //order* blockadeOrder1 = new blockade(*t1, &p1);
    order* airlift1 = new airlift(2, *t1, *t4, &p1);
    order* b1 = new bomb(*t3, &p1);
    order* n1 = new negotiate(&p2, &p1);
    LogObserver o1(deploy1);
    LogObserver o2(deploy2);
    LogObserver o3(advance);
    //LogObserver o4(blockadeOrder1);
    LogObserver o5(airlift1);
    LogObserver o6(b1);
    LogObserver o7(n1);
    // Add an order to the order list
    list2->addOrder(deploy2);
    list->addOrder(deploy1);
    //list->addOrder(blockadeOrder1);
    list->addOrder(advance);
    list->addOrder(airlift1);
    list->addOrder(b1);
    list->addOrder(n1);
    // Execute valid orders
    Card* c1 = new Card(Card::Airlift);
    Card* c2 = new Card(Card::Bomb);
    Card* c3 = new Card(Card::Blockade);
    Card* c4 = new Card(Card::Diplomacy);
    h->setCardHand(c1);
    h->setCardHand(c2);
    h->setCardHand(c3);
    h->setCardHand(c4);
    
    deploy1->execute();
    deploy2->execute();
    advance->execute();
    airlift1->execute();
    //blockadeOrder1->execute();
    n1->execute();
    b1->execute();
    

    // Invalid orders
    deploy deploy3(5, *t3, &p1);
    //blockade blockadeOrder2(*t3, &p1);
    airlift airlift2(2, *t3, *t1, &p1);
    bomb b2(*t4, &p1);
    bomb b3(*t5, &p1);
    negotiate n2(&p1, &p1);
    LogObserver o8(&deploy3);
    //LogObserver o9(&blockadeOrder2);
    LogObserver o10(&airlift2);
    LogObserver o11(&b2);
    LogObserver o12(&b3);
    LogObserver o13(&n2);
    // Execute invalid orders,  expect error messages in gamelog.txt
    deploy3.execute();
    //blockadeOrder2.execute();
    airlift2.execute();
    b2.execute();
    b3.execute();
    n2.execute();
}