#include "Trigger.h"
//#include <iostream>
Trigger::Trigger(xml_node<> * root){
	/* string type = "";
	string command = "";
	string has = "";
	string object = "";
	string owner = "";
	string status = "";
	string print = "";*/
	//cout << "build trigger" << endl;
	for (xml_node<>* node = root->first_node(); node; node=node->next_sibling()){
		string s = string(node->name());
		if (s == "type"){
			type = node->value();
		}
		else if (s == "command"){
			//commands.insert(node->value());
			command = node->value();
		}
		else if (s == "condition"){
			buildCondition(node);
		}
		else if (s == "print"){
			print = node->value();
		}
		else if (s == "action"){
			actions.push_back(node->value());
		}
	}
}

void Trigger::buildCondition(xml_node<>* root){
	//Condition *c = new Condition();
	for (xml_node<>* node = root->first_node(); node; node=node->next_sibling()){
		string s = string(node->name());
		if (s == "has"){
			//c->has = node->value();
			has = node->value();
		}
		else if (s == "object"){
			//c->object = node->value();
			object = node->value();
		}
		else if (s == "owner"){
			//c->owner = node->value();
			owner = node->value();
		}
		else if (s == "status"){
			//c->status = node->value();
			status = node->value();
		}
	}
	//conditions.insert(c);
	return;
}

Trigger::~Trigger(){
}
