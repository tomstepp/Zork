#ifndef Game_H_
#define Game_H_

#include "Room.h"
#include "Item.h"
#include "Container.h"
#include "Creature.h"
#include "Trigger.h"


#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <set>

#include "rapidxml-1.13/rapidxml.hpp"
//#include "rapidxml-1.13/rapidxml_print.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"

using namespace std;
using namespace rapidxml;

class Game{
public:
	Game(char*);
	virtual ~Game();
	void play();
	void setup(char*);

	// current room, load room objects
	map<string, Room*>::iterator curr;
	map<string, Item*> inv; // my inventory

	// game resource objects
	map<string, Room*> rooms;
	map<string, Item*> items;
	map<string, Container*> containers;
	map<string, Creature*> creatures;
private:
	void take(string);
	void open(string);
	void read(string);
	void drop(string);
	void put(string);
	void turnon(string);
	void attack(string);
	void changeRoom(string);
	bool triggerCheck(string);

	void performAction(string);
	void add(string);
	void del(string);
	void update(string);
	void creatureTrigger(string);

};

#endif /*Game_H_*/
