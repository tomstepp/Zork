#ifndef Trigger_H_
#define Trigger_H_

#include <string>
#include <set>
#include "rapidxml-1.13/rapidxml.hpp"
#include <vector>

using namespace std;
using namespace rapidxml;

class Trigger{
public:
	Trigger(xml_node<> *);
	void buildCondition(xml_node<> *);
	virtual ~Trigger();

	string type;
	string command;
	string has;
	string object;
	string owner;
	string status;
	string print;
	vector<string> actions;
private:
};

#endif /*Trigger_H_*/
