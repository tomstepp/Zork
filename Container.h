#ifndef Container_H_
#define Container_H_

#include <string>
#include <set>
#include <vector>
#include "Trigger.h"
#include "rapidxml-1.13/rapidxml.hpp"

using namespace std;
using namespace rapidxml;

class Container{
public:
	Container(xml_node<> *);
	virtual ~Container();

	string name;
	string status;
	string description;
	bool open;

	set<string> items;
	set<string> accept;
	//set<string> trigger;
	//set<Trigger*> triggers;
	vector<Trigger*> triggers;
private:
};

#endif /*Container_H_*/
