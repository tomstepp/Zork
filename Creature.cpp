#include "Creature.h"
#include <iostream>
Creature::Creature(xml_node<>* root){
	for (xml_node<>* node = root->first_node(); node; node=node->next_sibling()){
		string s = string(node->name());
		//cout << s << endl;
		if (s == "name"){
			name = node->value();
		}
		else if (s == "description"){
			description = node->value();
		}
		else if (s == "status"){
			status = node->value();
		}
		else if (s == "attack"){
			//cout << "attack" << endl;
			//buildAttack(node->first_node());
			buildAttack(node);
		}
		else if (s == "vulnerability"){
			vulnerability.insert(node->value());
		}
		else if (s == "trigger"){
			Trigger *t = new Trigger(node);
			//triggers.insert(t);
			triggers.push_back(t);
		}
//		else if (s == "print"){
//			print = node->value();
//		}
	}
}

void Creature::buildAttack(xml_node<>* root){
	//cout << root->name() << endl;
	//Attack *attack = new Attack();
	for (xml_node<>* node = root->first_node(); node; node=node->next_sibling()){
		string s = string(node->name());
		//cout << s << endl;
//		if (s == "object"){
//			attack->object = node->value();
//		}
//		else if (s == "status"){
//			attack->status = node->value();
//		}
//		else if (s == "print"){
//			print = node->value();
//		}
//		else if (s == "action"){
//			attack->actions.insert(node->value());
//		}
		if (s == "condition"){
			//cout << node->first_node()->value() << endl;
			//cout << node->first_node()->next_sibling()->value() << endl;
			object = node->first_node()->value();
			object_status = node->first_node()->next_sibling()->value();
			//cout << attack->object << endl;
			//cout << attack->status << endl;
		}
		else if (s == "print"){
			print = node->value();
		}
		else if (s == "action"){
			//attack->actions.push_back(node->value());
			actions.push_back(node->value());
		}
	}
	//attacks[attack->object] = attack;
	//attacks[vulnerability] = attack;
	//cout << attack->object << endl;
	return;
}

Creature::~Creature(){
}
