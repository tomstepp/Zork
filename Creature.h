#ifndef Creature_H_
#define Creature_H_

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <map>
#include "Trigger.h"
#include "rapidxml-1.13/rapidxml.hpp"

using namespace rapidxml;
using namespace std;
#include <vector>

//typedef struct _attack{
//	string object;
//	string status;
//	//string print;
//	vector<string> actions;
//}Attack;

class Creature{
public:
	Creature(xml_node<>*);
	void buildAttack(xml_node<>*);
	virtual ~Creature();

	string name;
	string status;
	string description;
	string print;
	set<string> vulnerability;

	//map<string, Attack *> attacks;
	//map<set<string>, Attack*> attacks;
	//Attack * attack;
	string object;
	string object_status;
	vector<string>actions;

	vector<Trigger*> triggers;
private:
};

#endif /*Creature_H_*/
