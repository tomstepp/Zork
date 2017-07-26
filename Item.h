#ifndef Item_H_
#define Item_H_

#include <string>
#include <stdio.h>
#include "Trigger.h"
#include <vector>

#include "rapidxml-1.13/rapidxml.hpp"

using namespace std;
using namespace rapidxml;

class Item{
public:
	Item(xml_node<> *);
	virtual ~Item();

	string name;
	string status;
	string description;
	string writing;
	//string turnon;
	vector<string> actions;
	string print;
	set<Trigger*> triggers;
private:
};

#endif /*Item_H_*/
