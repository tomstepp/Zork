#ifndef Room_H_
#define Room_H_

#include <string>
#include <stdio.h>
#include <iostream>
#include <set>
#include <map>
#include <vector>

#include "Item.h"
#include "Container.h"
#include "Creature.h"
#include "Trigger.h"

#include "rapidxml-1.13/rapidxml.hpp"
//#include "rapidxml-1.13/rapidxml_print.hpp"
//#include "rapidxml-1.13/rapidxml_utils.hpp"

using namespace std;
using namespace rapidxml;

class Room{
public:
	Room(xml_node<> *);
	virtual ~Room();
	
	string name;
	string status;
	string type;
	string description;

	map<string, string> borders;  // map 'north' -> 'MainCavern'
	set<string> items;
	set<string> containers;
	set<string> creatures;
	//set<Trigger*> triggers;
	vector<Trigger*> triggers;
private:
};
#endif /*ROOM_H_*/
